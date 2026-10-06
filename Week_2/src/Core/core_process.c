#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "../IPC/ipc.h"

int main(void)
{
    CoreState core;
    mqd_t ui_to_core;
    mqd_t core_to_ui;
    mqd_t core_to_logger;
    IPCMessage message;
    IPCMessage response;
    int running = 1;

    printf("Core process starting...\n");

    core_init(&core);

    printf("Core initialized successfully.\n");

    ui_to_core = ipc_open_queue(
        UI_TO_CORE_QUEUE,
        O_RDWR,
        1
    );

    if (ui_to_core == (mqd_t)-1)
    {
        perror("Failed to open UI -> Core queue");
        return EXIT_FAILURE;
    }

    core_to_ui = ipc_open_queue(
        CORE_TO_UI_QUEUE,
        O_RDWR,
        1
    );
   core_to_logger = ipc_open_queue(
    CORE_TO_LOGGER_QUEUE,
    O_WRONLY,
    1
);

if (core_to_logger == (mqd_t)-1)
{
    perror("Failed to open Core -> Logger queue");
    mq_close(ui_to_core);
    mq_close(core_to_ui);
    return EXIT_FAILURE;
}

    if (core_to_ui == (mqd_t)-1)
    {
        perror("Failed to open Core -> UI queue");
        mq_close(ui_to_core);
        return EXIT_FAILURE;
    }

    printf("IPC queues ready.\n");
    printf("Waiting for commands...\n");

    while (running)
    {
        if (!ipc_receive(ui_to_core, &message))
        {
            perror("Failed to receive IPC message");
            break;
        }

        if (message.type != IPC_MSG_COMMAND)
        {
            printf("Core: ignoring non-command message.\n");
            continue;
        }

        response.type = IPC_MSG_RESPONSE;
        response.command = message.command;
        response.status = 0;
        response.text[0] = '\0';

        if (message.command < CORE_CMD_LOAD ||
            message.command > CORE_CMD_EXIT)
        {
            snprintf(
                response.text,
                IPC_TEXT_SIZE,
                "Invalid command: %d",
                message.command
            );

            ipc_send(core_to_ui, &response);
            continue;
        }

        printf(
            "Core received command: %d\n",
            message.command
        );
       IPCMessage log_message;

log_message.type = IPC_MSG_LOG;
log_message.command = message.command;
log_message.status = 0;

snprintf(
    log_message.text,
    IPC_TEXT_SIZE,
    "Core received command: %d",
    message.command
);

ipc_send(core_to_logger, &log_message);

        response.status = core_process_command(
            &core,
            (CoreCommand)message.command
        );

        if (response.status)
        {
            snprintf(
                response.text,
                IPC_TEXT_SIZE,
                "Command %d completed successfully",
                message.command
            );
        }
        else
        {
            snprintf(
                response.text,
                IPC_TEXT_SIZE,
                "Command %d failed",
                message.command
            );
        }

        ipc_send(core_to_ui, &response);

        if (message.command == CORE_CMD_EXIT)
        {
            running = 0;
        }
    }

    mq_close(ui_to_core);
    mq_close(core_to_ui);
    mq_close(core_to_logger);

    ipc_remove_queues();

    printf("Core process stopped.\n");

    return EXIT_SUCCESS;
}
