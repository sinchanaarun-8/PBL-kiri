#include "../backend/simulator.h"

#include <assert.h>
#include <stdio.h>

int main(void)
{
    Simulator sim = {0};

    int allocation[SIM_MAX_PROCESSES][SIM_MAX_RESOURCES] = {{0}};
    int maximum[SIM_MAX_PROCESSES][SIM_MAX_RESOURCES] = {{0}};
    int available[SIM_MAX_RESOURCES] = {3, 3, 2};

    int alloc_data[5][3] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    int max_data[5][3] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    for (int p = 0; p < 5; p++) {
        for (int r = 0; r < 3; r++) {
            allocation[p][r] = alloc_data[p][r];
            maximum[p][r] = max_data[p][r];
        }
    }

    assert(sim_init(&sim, 5, 3, allocation, maximum, available)
           == BANKERS_OK);
    puts("PASS: simulator initialization");

    BankerSafetyResult safety = sim_check_safety(&sim);
    assert(safety.is_safe);
    assert(safety.sequence_length == 5);
    puts("PASS: safe-state check");

    assert(sim.graph.node_count == 8);
    assert(sim.graph.edge_count > 0);
    puts("PASS: Resource Allocation Graph initialized");

    int request[SIM_MAX_RESOURCES] = {1, 0, 2};
    BankerSafetyResult after_request = {0};

    assert(sim_request_resources(&sim, 1, request, &after_request)
           == BANKERS_OK);
    assert(after_request.is_safe);
    assert(sim.banker.allocation[1][0] == 3);
    assert(sim.banker.available[0] == 2);
    assert(sim.banker.available[2] == 0);
    puts("PASS: safe resource request");

    assert(sim_refresh_graph(&sim) == RAG_OK);
    puts("PASS: graph refresh");

    assert(sim_recover_process(&sim, 1) == 1);
    assert(sim.banker.allocation[1][0] == 0);
    assert(sim.banker.allocation[1][1] == 0);
    assert(sim.banker.allocation[1][2] == 0);
    assert(sim.banker.available[0] == 5);
    assert(sim.banker.available[1] == 3);
    assert(sim.banker.available[2] == 2);
    puts("PASS: process recovery and resource release");

    safety = sim_check_safety(&sim);
    assert(safety.is_safe);
    puts("PASS: safety check after recovery");

    puts("\nAll integration tests passed.");
    return 0;
}
