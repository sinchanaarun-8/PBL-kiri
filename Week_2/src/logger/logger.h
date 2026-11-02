
#ifndef LOGGER_H
#define LOGGER_H

#define LOGGER_SOCKET_PATH "/tmp/pbl_logger.sock"
#define LOGGER_BUFFER_SIZE 256

int logger_init(void);

void log_info(const char *message);
void log_warning(const char *message);
void log_error(const char *message);

void logger_close(void);

int logger_send_message(const char *message);

#endif
