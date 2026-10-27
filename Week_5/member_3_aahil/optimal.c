#include <string.h>
#include "optimal.h"

SimulationResult simulate_optimal(
    const int pages[],
    int reference_count,
    int frame_count
) {
    SimulationResult result;
    memset(&result, 0, sizeof(result));

    /* Reject invalid inputs safely; callers can detect no references processed. */
    if (pages == NULL ||
        reference_count <= 0 ||
        reference_count > MAX_REFERENCES ||
        frame_count <= 0 ||
        frame_count > MAX_FRAMES) {
        return result;
    }

    result.total_references = reference_count;

    int frames[MAX_FRAMES];
    for (int f = 0; f < frame_count; ++f) {
        frames[f] = -1; /* -1 represents an empty frame */
    }

    for (int i = 0; i < reference_count; ++i) {
        const int requested_page = pages[i];
        int hit_frame = -1;

        /* First check whether the requested page is already in memory. */
        for (int f = 0; f < frame_count; ++f) {
            if (frames[f] == requested_page) {
                hit_frame = f;
                break;
            }
        }

        SimulationStep *step = &result.steps[i];
        step->page = requested_page;

        if (hit_frame >= 0) {
            step->is_hit = 1;
            result.total_hits++;
        } else {
            step->is_hit = 0;
            result.total_faults++;

            int replacement_frame = -1;

            /* Prefer an empty frame before replacing a resident page. */
            for (int f = 0; f < frame_count; ++f) {
                if (frames[f] == -1) {
                    replacement_frame = f;
                    break;
                }
            }

            if (replacement_frame == -1) {
                int farthest_next_use = -1;

                /*
                 * For every resident page, find its next occurrence.
                 * A page never used again is an optimal replacement choice.
                 */
                for (int f = 0; f < frame_count; ++f) {
                    int next_use = reference_count;

                    for (int future = i + 1; future < reference_count; ++future) {
                        if (pages[future] == frames[f]) {
                            next_use = future;
                            break;
                        }
                    }

                    if (next_use == reference_count) {
                        replacement_frame = f;
                        break;
                    }

                    if (next_use > farthest_next_use) {
                        farthest_next_use = next_use;
                        replacement_frame = f;
                    }
                }
            }

            frames[replacement_frame] = requested_page;
        }

        /* Store the complete memory snapshot for this page request. */
        for (int f = 0; f < frame_count; ++f) {
            step->frames[f] = frames[f];
        }
    }

    result.hit_ratio = (double)result.total_hits / reference_count;
    result.fault_ratio = (double)result.total_faults / reference_count;

    return result;
}
