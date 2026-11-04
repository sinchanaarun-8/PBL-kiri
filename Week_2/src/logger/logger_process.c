
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "logger.h"

int main(void)
{
    int server_fd, client_fd;
    struct sockaddr_un address;
    char buffer[LOGGER_BUFFER_SIZE];

    printf("Logger process starting...\n");

    if (!logger_init())
    {
        printf("Logger initialization failed.\n");
        return 1;
    }

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (server_fd == -1)
    {
        perror("Socket creation failed");
        logger_close();
        return 1;
    }

    unlink(LOGGER_SOCKET_PATH);

    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, LOGGER_SOCKET_PATH);

    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) == -1)
    {
        perror("Bind failed");
        close(server_fd);
        logger_close();
        return 1;
    }

    if (listen(server_fd, 5) == -1)
    {
        perror("Listen failed");
        close(server_fd);
        unlink(LOGGER_SOCKET_PATH);
        logger_close();
        return 1;
    }

    printf("Logger waiting for Core process...\n");

    while (1)
    {
        client_fd = accept(server_fd, NULL, NULL);

        if (client_fd == -1)
        {
            perror("Accept failed");
            break;
        }



        while (1)
        {
            int bytes = recv(client_fd, buffer,
                             sizeof(buffer) - 1, 0);

            if (bytes <= 0)
                break;

            buffer[bytes] = '\0';



            if (strcmp(buffer, "EXIT") == 0)
            {
                printf("Logger shutting down.\n");

                close(client_fd);
                close(server_fd);
                unlink(LOGGER_SOCKET_PATH);
                logger_close();

                return 0;
            }

            if (strncmp(buffer, "ERROR:", 6) == 0)
                log_error(buffer + 6);
            else if (strncmp(buffer, "WARNING:", 8) == 0)
                log_warning(buffer + 8);
            else
                log_info(buffer);
            printf("Logger received: %s\n", buffer); fflush(stdout);
        }

        close(client_fd);

    }

    close(server_fd);
    unlink(LOGGER_SOCKET_PATH);
    logger_close();

    return 0;
}
