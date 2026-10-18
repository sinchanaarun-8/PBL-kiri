
#include <stdio.h>
#include "../include/detection.h"

int recover_process(
    int process_id,
    int processes,
    int resources,
    int allocation[MAX_PROCESSES][MAX_RESOURCES],
    int available[MAX_RESOURCES]);

int main(void)
{
    int allocation[MAX_PROCESSES][MAX_RESOURCES] = {0};
    int available[MAX_RESOURCES] = {1, 2};

    allocation[0][0] = 2;
    allocation[0][1] = 1;

    printf("Before recovery: %d %d\n",
           available[0], available[1]);

    int result = recover_process(
        0, 2, 2, allocation, available);

    printf("After recovery: %d %d\n",
           available[0], available[1]);

    if (result == 1 &&
        available[0] == 3 &&
        available[1] == 3 &&
        allocation[0][0] == 0 &&
        allocation[0][1] == 0)
        printf("TEST PASSED\n");
    else
        printf("TEST FAILED\n");

    return result == 1 &&
           available[0] == 3 &&
           available[1] == 3 ? 0 : 1;
}
