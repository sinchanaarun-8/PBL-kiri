#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "logger.h"
#include "../IPC/ipc.h"

int main(void)
{
    mqd_t core_to_logger;
    IPCMessage message;

    printf("Logger process starting...\n");

    if (!logger_init())
    {
        fprintf(stderr, "Failed to initialize logger.\n");
        return EXIT_FAILURE;
    }

    core_to_logger = ipc_open_queue(
        CORE_TO_LOGGER_QUEUE,
        O_RDONLY,
        0
    );

    if (core_to_logger == (mqd_t)-1)
    {
        perror("Failed to open Core -> Logger queue");
        logger_close();
        return EXIT_FAILURE;
    }

    printf("Logger IPC queue ready.\n");
    printf("Logger waiting for messages...\n");

    while (1)
    {
        if (!ipc_receive(core_to_logger, &message))
        {
            perror("Failed to receive logger message");
            break;
        }

        if (message.type != IPC_MSG_LOG)
        {
            continue;
        }

        printf("Logger received: %s\n", message.text);

        log_info(message.text);

        if (message.command == IPC_CMD_EXIT)
        {
            break;
        }
    }

    mq_close(core_to_logger);
    logger_close();

    printf("Logger process stopped.\n");

    return EXIT_SUCCESS;
}
