#include <stdio.h>
#include <time.h>

#include "Week_2/src/Core/core.h"

static double elapsed_seconds(
    struct timespec start,
    struct timespec end)
{
    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) / 1000000000.0;
}

int main(void)
{
    CoreState core;
    struct timespec start;
    struct timespec end;

    core_init(&core);

    clock_gettime(CLOCK_MONOTONIC, &start);

    core_process_command(&core, CORE_CMD_LOAD);
    core_process_command(&core, CORE_CMD_RUN);
    core_process_command(&core, CORE_CMD_STOP);

    clock_gettime(CLOCK_MONOTONIC, &end);

    printf(
        "Standalone execution time: %.9f seconds\n",
        elapsed_seconds(start, end)
    );

    return 0;
}