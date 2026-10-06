#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t core_pid;
    pid_t logger_pid;
    pid_t ui_pid;
    int status;

    printf("PBL Process Simulator starting...\n");

    core_pid = fork();

    if (core_pid == 0)
    {
       execl("./build/core_process", "core_process", (char *)NULL);
        perror("Failed to start Core");
        exit(EXIT_FAILURE);
    }

    if (core_pid < 0)
    {
        perror("Failed to fork Core");
        return EXIT_FAILURE;
    }

    sleep(1);

    logger_pid = fork();

    if (logger_pid == 0)
    {
        execl("./build/logger_process", "logger_process", (char *)NULL);
        perror("Failed to start Logger");
        exit(EXIT_FAILURE);
    }

    if (logger_pid < 0)
    {
        perror("Failed to fork Logger");
        return EXIT_FAILURE;
    }

    sleep(1);

    ui_pid = fork();

    if (ui_pid == 0)
    {
        execl("./build/ui_process", "ui_process", (char *)NULL);
        perror("Failed to start UI");
        exit(EXIT_FAILURE);
    }

    if (ui_pid < 0)
    {
        perror("Failed to fork UI");
        return EXIT_FAILURE;
    }

    printf("Core PID: %d\n", core_pid);
    printf("Logger PID: %d\n", logger_pid);
    printf("UI PID: %d\n", ui_pid);

    waitpid(ui_pid, &status, 0);
    waitpid(core_pid, &status, 0);
    waitpid(logger_pid, &status, 0);

    printf("PBL Process Simulator stopped.\n");

    return EXIT_SUCCESS;
}
