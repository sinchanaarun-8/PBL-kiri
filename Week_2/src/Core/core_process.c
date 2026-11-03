#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "core.h"
#include "shared_memory.h"
#include "../IPC/ipc.h"
#include "../logger/logger.h"

static void send_log(const char *text)
{
    int fd;
    struct sockaddr_un addr;
    size_t len = strlen(text), sent = 0;

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) { perror("logger socket"); return; }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, LOGGER_SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        /* Logger may not yet be ready during startup; logging must not stop Core. */
        close(fd);
        return;
    }
    while (sent < len) {
        ssize_t n = send(fd, text + sent, len - sent, 0);
        if (n <= 0) break;
        sent += (size_t)n;
    }
    shutdown(fd, SHUT_WR);
    close(fd);
}

int main(void)
{
    CoreShmHandle shm;
    mqd_t ui_to_core, core_to_ui;
    IPCMessage message, response;
    int running = 1;
    char log_text[LOGGER_BUFFER_SIZE];

    printf("Core process starting...\n");
    if (!core_shm_create(&shm)) {
        fprintf(stderr, "Could not initialize POSIX shared memory.\\n");
        return EXIT_FAILURE;
    }

    core_init(&shm.ptr->state);
    printf("Core state initialized in POSIX shared memory.\\n");
    printf("Shared memory object: %s\\n", CORE_SHM_NAME);

    ui_to_core = ipc_open_queue(UI_TO_CORE_QUEUE, O_RDWR, 1);
    if (ui_to_core == (mqd_t)-1) {
        perror("Failed to open UI -> Core queue");
        core_shm_close(&shm); core_shm_unlink();
        return EXIT_FAILURE;
    }
    core_to_ui = ipc_open_queue(CORE_TO_UI_QUEUE, O_RDWR, 1);
    if (core_to_ui == (mqd_t)-1) {
        perror("Failed to open Core -> UI queue");
        mq_close(ui_to_core); core_shm_close(&shm); core_shm_unlink();
        return EXIT_FAILURE;
    }

    printf("UI command queues ready. Waiting for commands...\\n");
    while (running) {
        if (!ipc_receive(ui_to_core, &message)) {
            if (errno == EINTR) continue;
            perror("Failed to receive UI command");
            break;
        }
        if (message.type != IPC_MSG_COMMAND) continue;

        response.type = IPC_MSG_RESPONSE;
        response.command = message.command;
        response.status = 0;
        response.text[0] = '\0';

        if (message.command < CORE_CMD_LOAD || message.command > CORE_CMD_EXIT) {
            snprintf(response.text, IPC_TEXT_SIZE, "Invalid command: %d", message.command);
        } else {
            core_shm_lock(shm.ptr);
            response.status = core_process_command(&shm.ptr->state,
                                                   (CoreCommand)message.command);
            shm.ptr->update_count++;
            core_shm_unlock(shm.ptr);

            snprintf(response.text, IPC_TEXT_SIZE, "Command %d %s; shared state updated (version %lu)",
                     message.command, response.status ? "completed" : "failed",
                     shm.ptr->update_count);
            snprintf(log_text, sizeof(log_text), "Core command %d %s",
                     message.command, response.status ? "completed" : "failed");
            send_log(log_text);
        }

        if (!ipc_send(core_to_ui, &response))
            perror("Failed to send response to UI");

        if (message.command == CORE_CMD_EXIT) running = 0;
    }

    mq_close(ui_to_core);
    mq_close(core_to_ui);
    ipc_remove_queues();

    core_shm_lock(shm.ptr);
    shm.ptr->ready = 0;
    core_shm_unlock(shm.ptr);
    core_shm_close(&shm);
    core_shm_unlink();

    send_log("Core process stopped");
    send_log("EXIT");
    printf("Core process stopped; shared memory cleaned up.\\n");
    return EXIT_SUCCESS;
}
