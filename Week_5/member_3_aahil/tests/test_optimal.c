
#include <assert.h>
#include <stdio.h>
#include "../optimal.h"

static void test_faculty_sample(void) {
    const int pages[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3};
    SimulationResult r = simulate_optimal(pages, 10, 3);

    assert(r.total_references == 10);
    assert(r.total_hits == 4);
    assert(r.total_faults == 6);
    assert(r.steps[0].is_hit == 0);
    assert(r.steps[4].is_hit == 1);
    assert(r.steps[6].is_hit == 1);
    assert(r.steps[8].is_hit == 1);
    assert(r.hit_ratio > 0.399 && r.hit_ratio < 0.401);
    assert(r.fault_ratio > 0.599 && r.fault_ratio < 0.601);

    /* Verify frame contents after every page request. */
    const int expected_frames[10][3] = {
        {7, -1, -1},
        {7,  0, -1},
        {7,  0,  1},
        {2,  0,  1},
        {2,  0,  1},
        {2,  0,  3},
        {2,  0,  3},
        {2,  4,  3},
        {2,  4,  3},
        {2,  4,  3}
    };

    for (int i = 0; i < 10; ++i) {
        for (int f = 0; f < 3; ++f) {
            assert(r.steps[i].frames[f] == expected_frames[i][f]);
        }
    }

    puts("PASS: faculty sample (10 references, 3 frames)");
}

static void test_no_replacement_needed(void) {
    const int pages[] = {4, 4, 4, 4};
    SimulationResult r = simulate_optimal(pages, 4, 2);

    assert(r.total_hits == 3);
    assert(r.total_faults == 1);
    assert(r.steps[0].is_hit == 0);
    assert(r.steps[1].is_hit == 1);

    puts("PASS: repeated page / hit tracking");
}

static void test_invalid_inputs(void) {
    const int pages[] = {1, 2, 3};
    SimulationResult a = simulate_optimal(pages, 0, 2);
    SimulationResult b = simulate_optimal(pages, 3, 0);
    SimulationResult c = simulate_optimal(NULL, 3, 2);

    assert(a.total_references == 0);
    assert(b.total_references == 0);
    assert(c.total_references == 0);

    puts("PASS: invalid input handling");
}

int main(void) {
    test_faculty_sample();
    test_no_replacement_needed();
    test_invalid_inputs();

    puts("All Optimal page replacement tests passed.");
    return 0;
}
