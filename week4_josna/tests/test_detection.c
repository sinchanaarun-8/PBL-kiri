
#include <stdio.h>
#include "../include/detection.h"

int main(void)
{
    int allocation[MAX_PROCESSES][MAX_RESOURCES] = {0};
    int request[MAX_PROCESSES][MAX_RESOURCES] = {0};
    int available[MAX_RESOURCES] = {0};
    int deadlocked[MAX_PROCESSES] = {0};

    int processes = 2;
    int resources = 2;

    /* P0 holds R0 and requests R1 */
    allocation[0][0] = 1;
    request[0][1] = 1;

    /* P1 holds R1 and requests R0 */
    allocation[1][1] = 1;
    request[1][0] = 1;

    /* Both resource instances are allocated */
    available[0] = 0;
    available[1] = 0;

    int count = detect_deadlock(
        processes, resources,
        allocation, request, available, deadlocked
    );

    printf("Deadlocked processes: %d\n", count);

    for (int i = 0; i < processes; i++) {
        if (deadlocked[i])
            printf("P%d is deadlocked\n", i);
    }

    return 0;
}
