
#include <stdio.h>
#include "../include/detection.h"

int recover_process(
    int process_id,
    int processes,
    int resources,
    int allocation[MAX_PROCESSES][MAX_RESOURCES],
    int available[MAX_RESOURCES])
{
    if (process_id < 0 || process_id >= processes) {
        printf("Invalid process ID.\n");
        return 0;
    }

    int has_resources = 0;

    for (int r = 0; r < resources; r++) {
        if (allocation[process_id][r] > 0)
            has_resources = 1;
    }

    if (!has_resources) {
        printf("Process has no allocated resources to release.\n");
        return 0;
    }

    for (int r = 0; r < resources; r++) {
        available[r] += allocation[process_id][r];
        allocation[process_id][r] = 0;
    }

    printf("Resources released from P%d.\n", process_id);
    return 1;
}
