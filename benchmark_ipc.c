#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#include "Week_2/src/IPC/ipc.h"

#define TEST_COUNT 1000

static double elapsed_seconds(
    struct timespec start,
    struct timespec end)
{
    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) / 1000000000.0;
}

int main(void)
{
    pid_t child;
    mqd_t send_queue;
    mqd_t receive_queue;
    IPCMessage message;
    struct timespec start;
    struct timespec end;

    ipc_remove_queues();

    child = fork();

    if (child < 0)
    {
        perror("fork");
        return 1;
    }

    if (child == 0)
    {
        send_queue = ipc_open_queue(
            UI_TO_CORE_QUEUE,
            O_RDONLY,
            1
        );

        receive_queue = ipc_open_queue(
            CORE_TO_UI_QUEUE,
            O_WRONLY,
            1
        );

        if (send_queue == (mqd_t)-1 ||
            receive_queue == (mqd_t)-1)
        {
            perror("Child queue");
            exit(1);
        }

        for (int i = 0; i < TEST_COUNT; i++)
        {
            if (!ipc_receive(send_queue, &message))
            {
                perror("Child receive");
                exit(1);
            }

            if (!ipc_send(receive_queue, &message))
            {
                perror("Child send");
                exit(1);
            }
        }

        mq_close(send_queue);
        mq_close(receive_queue);
        exit(0);
    }

    send_queue = ipc_open_queue(
        UI_TO_CORE_QUEUE,
        O_WRONLY,
        1
    );

    receive_queue = ipc_open_queue(
        CORE_TO_UI_QUEUE,
        O_RDONLY,
        1
    );

    if (send_queue == (mqd_t)-1 ||
        receive_queue == (mqd_t)-1)
    {
        perror("Parent queue");
        return 1;
    }

    message.type = IPC_MSG_COMMAND;
    message.command = IPC_CMD_LOAD;
    message.status = 0;

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < TEST_COUNT; i++)
    {
        if (!ipc_send(send_queue, &message))
        {
            perror("Parent send");
            return 1;
        }

        if (!ipc_receive(receive_queue, &message))
        {
            perror("Parent receive");
            return 1;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    waitpid(child, NULL, 0);

    mq_close(send_queue);
    mq_close(receive_queue);

    ipc_remove_queues();

    double total = elapsed_seconds(start, end);

    printf("IPC round trips: %d\n", TEST_COUNT);
    printf("Total IPC time: %.9f seconds\n", total);
    printf("Average IPC round trip: %.9f seconds\n",
           total / TEST_COUNT);

    return 0;
}
