#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "logger.h"

static FILE *log_file = NULL;

int logger_init(void)
{
    mkdir("logs", 0777);

    log_file = fopen("logs/simulator.log", "a");

    if (log_file == NULL)
    {
        perror("Failed to open log file");
        return 0;
    }

    return 1;
}

void log_info(const char *message)
{
    if (log_file != NULL)
    {
        fprintf(log_file, "[INFO] %s\n", message);
        fflush(log_file);
    }
}

void log_warning(const char *message)
{
    if (log_file != NULL)
    {
        fprintf(log_file, "[WARNING] %s\n", message);
        fflush(log_file);
    }
}

void log_error(const char *message)
{
    if (log_file != NULL)
    {
        fprintf(log_file, "[ERROR] %s\n", message);
        fflush(log_file);
    }
}

void logger_close(void)
{
    if (log_file != NULL)
    {
        fclose(log_file);
        log_file = NULL;
    }
}

int main(void)
{
    if (!logger_init())
    {
        return 1;
    }

    log_info("Logger started");
    log_info("Instruction executed: LOAD");
    log_info("Instruction executed: ADD");

    log_warning("Stack is almost full");

    log_error("Invalid instruction: XYZ");

    log_info("Program completed");

    logger_close();

    return 0;
}