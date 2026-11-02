#include <stdio.h>
#include "simulator.h"

int main(void) {
    int pages[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3};
    int count = (int)(sizeof(pages) / sizeof(pages[0]));
    int frames = 3;

    SimulationResult fifo = simulate_fifo(pages, count, frames);
    SimulationResult lru = simulate_lru(pages, count, frames);
    SimulationResult optimal = simulate_optimal(pages, count, frames);

    printf("Algorithm   Hits   Faults\n");
    printf("FIFO        %d      %d\n",
           fifo.total_hits, fifo.total_faults);
    printf("LRU         %d      %d\n",
           lru.total_hits, lru.total_faults);
    printf("Optimal     %d      %d\n",
           optimal.total_hits, optimal.total_faults);

    if (fifo.total_hits != 1 || fifo.total_faults != 9 ||
        lru.total_hits != 2 || lru.total_faults != 8 ||
        optimal.total_hits != 4 || optimal.total_faults != 6) {
        printf("INTEGRATION TEST FAILED\n");
        return 1;
    }

    printf("INTEGRATION TEST PASSED\n");
    return 0;
}
