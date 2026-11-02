
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "logger.h"

int logger_send_message(const char *message)
{
    int sock_fd;
    struct sockaddr_un address;
    size_t length = strlen(message);
    size_t sent = 0;

    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sock_fd == -1)
    {
        perror("Socket creation failed");
        return 0;
    }

    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, LOGGER_SOCKET_PATH);

    if (connect(sock_fd, (struct sockaddr *)&address,
                sizeof(address)) == -1)
    {
        perror("Connection failed");
        close(sock_fd);
        return 0;
    }

    while (sent < length)
    {
        ssize_t n = send(sock_fd, message + sent,
                         length - sent, 0);

        if (n <= 0)
        {
            perror("Send failed");
            close(sock_fd);
            return 0;
        }

        sent += (size_t)n;
    }

    shutdown(sock_fd, SHUT_WR);
    close(sock_fd);

    return 1;
}
