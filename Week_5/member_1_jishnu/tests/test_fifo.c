#include <stdio.h>
#include "simulator.h"

int main(void) {
    int pages[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3};
    int count = (int)(sizeof(pages) / sizeof(pages[0]));
    int frame_count = 3;

    SimulationResult result =
        simulate_fifo(pages, count, frame_count);

    printf("Algorithm: FIFO\n");
    printf("References: %d\n", result.total_references);
    printf("Hits: %d\n", result.total_hits);
    printf("Page Faults: %d\n", result.total_faults);
    printf("Hit Ratio: %.2f\n", result.hit_ratio);
    printf("Fault Ratio: %.2f\n", result.fault_ratio);

    if (result.total_hits == 1 && result.total_faults == 9) {
        printf("TEST PASSED\n");
        return 0;
    }

    printf("TEST FAILED\n");
    return 1;
}
