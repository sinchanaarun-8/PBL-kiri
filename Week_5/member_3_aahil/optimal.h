#ifndef OPTIMAL_H
#define OPTIMAL_H

#include "../include/simulator.h"

/*
 * Simulate Optimal page replacement.
 *
 * pages:          page-reference sequence
 * reference_count: number of page references
 * frame_count:     number of available frames
 *
 * Returns a SimulationResult containing the frame state, hit/fault status,
 * totals, and ratios for each reference.
 *
 * Ratios are fractions in [0.0, 1.0], matching the existing LRU output
 * convention. Multiply by 100 when displaying percentages.
 */
SimulationResult simulate_optimal(
    const int pages[],
    int reference_count,
    int frame_count
);

#endif
