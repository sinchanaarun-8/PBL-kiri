#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

static void wait_for_queue(const char *queue_name)
{
    char path[256];

    snprintf(path, sizeof(path), "/dev/mqueue/%s", queue_name + 1);

    while (access(path, F_OK) != 0)
    {
        usleep(1000);
    }
}

int main(void)
{
    pid_t core_pid;
    pid_t logger_pid;
    pid_t ui_pid;

    core_pid = fork();

    if (core_pid == 0)
    {
        execl("/tmp/core_process",
              "core_process",
              (char *)NULL);

        perror("Failed to start Core");
        exit(1);
    }

    if (core_pid < 0)
    {
        perror("Failed to fork Core");
        return 1;
    }

    /* Wait until Core creates all IPC queues. */
    wait_for_queue("/pbl_ui_to_core");
    wait_for_queue("/pbl_core_to_ui");
    wait_for_queue("/pbl_core_to_logger");

    logger_pid = fork();

    if (logger_pid == 0)
    {
        execl("/tmp/logger_process",
              "logger_process",
              (char *)NULL);

        perror("Failed to start Logger");
        exit(1);
    }

    if (logger_pid < 0)
    {
        perror("Failed to fork Logger");
        return 1;
    }

    /*
     * Give Logger a moment to open the already-created queue
     * before starting UI.
     */
    usleep(100000);

    ui_pid = fork();

    if (ui_pid == 0)
    {
        execl("/tmp/ui_process",
              "ui_process",
              (char *)NULL);

        perror("Failed to start UI");
        exit(1);
    }

    if (ui_pid < 0)
    {
        perror("Failed to fork UI");
        return 1;
    }

    waitpid(ui_pid, NULL, 0);
    waitpid(core_pid, NULL, 0);
    waitpid(logger_pid, NULL, 0);

    return 0;
}
