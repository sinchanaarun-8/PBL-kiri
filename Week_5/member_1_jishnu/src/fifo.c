#include <stddef.h>
#include "simulator.h"

SimulationResult simulate_fifo(
    const int pages[],
    int reference_count,
    int frame_count
) {
    SimulationResult result = {0};

    if (pages == NULL ||
        reference_count <= 0 ||
        reference_count > MAX_REFERENCES ||
        frame_count <= 0 ||
        frame_count > MAX_FRAMES) {
        return result;
    }

    result.total_references = reference_count;

    int frames[MAX_FRAMES];
    int next_frame = 0;

    for (int i = 0; i < frame_count; i++) {
        frames[i] = -1;
    }

    for (int i = 0; i < reference_count; i++) {
        int page = pages[i];
        int found = 0;

        for (int j = 0; j < frame_count; j++) {
            if (frames[j] == page) {
                found = 1;
                break;
            }
        }

        result.steps[i].page = page;
        result.steps[i].is_hit = found;

        if (found) {
            result.total_hits++;
        } else {
            result.total_faults++;
            frames[next_frame] = page;
            next_frame = (next_frame + 1) % frame_count;
        }

        for (int j = 0; j < frame_count; j++) {
            result.steps[i].frames[j] = frames[j];
        }
    }

    result.hit_ratio =
        (double)result.total_hits / reference_count;

    result.fault_ratio =
        (double)result.total_faults / reference_count;

    return result;
}
