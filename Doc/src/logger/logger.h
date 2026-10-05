#ifndef LOGGER_H
#define LOGGER_H

int logger_init(void);

void log_info(const char *message);
void log_warning(const char *message);
void log_error(const char *message);

void logger_close(void);

#endif