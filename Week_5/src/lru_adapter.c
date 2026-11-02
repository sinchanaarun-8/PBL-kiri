#include <stddef.h>
#include "simulator.h"

SimulationResult simulate_lru(
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

    int frames[MAX_FRAMES];
    int last_used[MAX_FRAMES];
    int time = 0;

    result.total_references = reference_count;

    for (int i = 0; i < frame_count; i++) {
        frames[i] = -1;
        last_used[i] = -1;
    }

    for (int i = 0; i < reference_count; i++) {
        int page = pages[i];
        int found = 0;

        for (int j = 0; j < frame_count; j++) {
            if (frames[j] == page) {
                found = 1;
                last_used[j] = time++;
                break;
            }
        }

        result.steps[i].page = page;
        result.steps[i].is_hit = found;

        if (found) {
            result.total_hits++;
        } else {
            result.total_faults++;

            int position = -1;

            for (int j = 0; j < frame_count; j++) {
                if (frames[j] == -1) {
                    position = j;
                    break;
                }
            }

            if (position == -1) {
                position = 0;

                for (int j = 1; j < frame_count; j++) {
                    if (last_used[j] < last_used[position]) {
                        position = j;
                    }
                }
            }

            frames[position] = page;
            last_used[position] = time++;
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
