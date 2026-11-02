#include <stdio.h>
#include "logger.h"

int main()
{
    printf("Testing Logger Client...\n");

    logger_init();

    logger_send_message("INFO: Program started");
    logger_send_message("WARNING: Low memory");
    logger_send_message("ERROR: Invalid instruction");

    printf("Messages sent to Logger.\n");

    logger_close();

    return 0;
}