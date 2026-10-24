
#ifndef SIMULATOR_H
#define SIMULATOR_H

#define MAX_REFERENCES 1000
#define MAX_FRAMES 100

typedef enum {
    ALGORITHM_FIFO,
    ALGORITHM_LRU,
    ALGORITHM_OPTIMAL
} Algorithm;

typedef struct {
    int page;
    int frames[MAX_FRAMES];
    int is_hit;
} SimulationStep;

typedef struct {
    SimulationStep steps[MAX_REFERENCES];
    int total_references;
    int total_hits;
    int total_faults;
    double hit_ratio;
    double fault_ratio;
} SimulationResult;

SimulationResult simulate_fifo(
    const int pages[],
    int reference_count,
    int frame_count
);

SimulationResult simulate_lru(
    const int pages[],
    int reference_count,
    int frame_count
);

SimulationResult simulate_optimal(
    const int pages[],
    int reference_count,
    int frame_count
);

#endif
