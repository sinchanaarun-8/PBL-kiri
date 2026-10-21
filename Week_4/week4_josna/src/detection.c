
#include "../include/detection.h"

int detect_deadlock(int processes, int resources,
                    int allocation[MAX_PROCESSES][MAX_RESOURCES],
                    int request[MAX_PROCESSES][MAX_RESOURCES],
                    int available[MAX_RESOURCES],
                    int deadlocked[MAX_PROCESSES])
{
    int work[MAX_RESOURCES];
    int finish[MAX_PROCESSES] = {0};

    for (int r = 0; r < resources; r++)
        work[r] = available[r];

    for (int p = 0; p < processes; p++) {
        int allocated = 0;

        for (int r = 0; r < resources; r++)
            if (allocation[p][r] > 0)
                allocated = 1;

        finish[p] = !allocated;
    }

    int changed;

    do {
        changed = 0;

        for (int p = 0; p < processes; p++) {
            if (finish[p])
                continue;

            int possible = 1;

            for (int r = 0; r < resources; r++)
                if (request[p][r] > work[r])
                    possible = 0;

            if (possible) {
                for (int r = 0; r < resources; r++)
                    work[r] += allocation[p][r];

                finish[p] = 1;
                changed = 1;
            }
        }
    } while (changed);

    int count = 0;

    for (int p = 0; p < processes; p++) {
        deadlocked[p] = !finish[p];

        if (deadlocked[p])
            count++;
    }

    return count;
}
