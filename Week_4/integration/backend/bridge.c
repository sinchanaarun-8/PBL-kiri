#include "simulator.h"

#include <string.h>

static Simulator state;
static int initialized = 0;

#define FLAT(p, r) ((p) * SIM_MAX_RESOURCES + (r))

int bridge_init(int processes, int resources,
                const int *allocation_flat,
                const int *maximum_flat,
                const int *available_flat)
{
    if (processes < 1 || processes > SIM_MAX_PROCESSES ||
        resources < 1 || resources > SIM_MAX_RESOURCES ||
        allocation_flat == NULL || maximum_flat == NULL ||
        available_flat == NULL) {
        return BANKERS_ERR_ARGUMENT;
    }

    int allocation[SIM_MAX_PROCESSES][SIM_MAX_RESOURCES] = {{0}};
    int maximum[SIM_MAX_PROCESSES][SIM_MAX_RESOURCES] = {{0}};
    int available[SIM_MAX_RESOURCES] = {0};

    for (int p = 0; p < processes; p++) {
        for (int r = 0; r < resources; r++) {
            allocation[p][r] = allocation_flat[FLAT(p, r)];
            maximum[p][r] = maximum_flat[FLAT(p, r)];
        }
    }

    for (int r = 0; r < resources; r++) {
        available[r] = available_flat[r];
    }

    int status = sim_init(&state, (size_t)processes,
                          (size_t)resources, allocation,
                          maximum, available);

    initialized = (status == BANKERS_OK);
    return status;
}

int bridge_check_safety(int *safe, int *sequence, int *length)
{
    if (!initialized || safe == NULL ||
        sequence == NULL || length == NULL) {
        return BANKERS_ERR_ARGUMENT;
    }

    BankerSafetyResult result = sim_check_safety(&state);
    *safe = result.is_safe ? 1 : 0;
    *length = (int)result.sequence_length;

    for (int i = 0; i < *length; i++) {
        sequence[i] = result.safe_sequence[i];
    }

    return BANKERS_OK;
}

int bridge_request(int process, const int *request,
                   int *safe, int *sequence, int *length)
{
    if (!initialized || request == NULL || safe == NULL ||
        sequence == NULL || length == NULL) {
        return BANKERS_ERR_ARGUMENT;
    }

    int req[SIM_MAX_RESOURCES] = {0};
    for (size_t r = 0; r < state.banker.resource_count; r++) {
        req[r] = request[r];
    }

    BankerSafetyResult result = {0};
    int status = sim_request_resources(
        &state, (size_t)process, req, &result);

    *safe = result.is_safe ? 1 : 0;
    *length = (int)result.sequence_length;

    for (int i = 0; i < *length; i++) {
        sequence[i] = result.safe_sequence[i];
    }

    return status;
}

/* Record a waiting request without granting it. */
int bridge_set_waiting_request(int process, const int *request)
{
    if (!initialized || request == NULL || process < 0 ||
        (size_t)process >= state.banker.process_count) {
        return BANKERS_ERR_ARGUMENT;
    }

    int previous[SIM_MAX_RESOURCES] = {0};

    for (size_t r = 0; r < state.banker.resource_count; r++) {
        if (request[r] < 0 ||
            request[r] > state.banker.need[process][r]) {
            return BANKERS_ERR_REQUEST_EXCEEDS_NEED;
        }
        previous[r] = state.request[process][r];
    }

    for (size_t r = 0; r < state.banker.resource_count; r++) {
        state.request[process][r] = request[r];
    }

    if (sim_refresh_graph(&state) != RAG_OK) {
        for (size_t r = 0; r < state.banker.resource_count; r++) {
            state.request[process][r] = previous[r];
        }
        (void)sim_refresh_graph(&state);
        return BANKERS_ERR_ARGUMENT;
    }

    return BANKERS_OK;
}

int bridge_detect_deadlock(int *deadlocked)
{
    if (!initialized || deadlocked == NULL) {
        return -1;
    }

    SimDeadlockResult result = sim_detect_deadlock(&state);

    for (size_t p = 0; p < state.banker.process_count; p++) {
        deadlocked[p] = result.deadlocked[p];
    }

    return result.count;
}

int bridge_recover(int process)
{
    if (!initialized || process < 0 ||
        (size_t)process >= state.banker.process_count) {
        return 0;
    }

    return sim_recover_process(&state, (size_t)process);
}

int bridge_get_state(int *allocation, int *maximum, int *need,
                     int *available, int *requests)
{
    if (!initialized || allocation == NULL || maximum == NULL ||
        need == NULL || available == NULL || requests == NULL) {
        return 0;
    }

    for (int p = 0; p < SIM_MAX_PROCESSES; p++) {
        for (int r = 0; r < SIM_MAX_RESOURCES; r++) {
            int i = FLAT(p, r);
            allocation[i] = state.banker.allocation[p][r];
            maximum[i] = state.banker.maximum[p][r];
            need[i] = state.banker.need[p][r];
            requests[i] = state.request[p][r];
        }
    }

    for (int r = 0; r < SIM_MAX_RESOURCES; r++) {
        available[r] = state.banker.available[r];
    }

    return 1;
}

int bridge_get_graph(int *edge_count, int *types,
                     int *processes, int *resources, int *instances)
{
    if (!initialized || edge_count == NULL || types == NULL ||
        processes == NULL || resources == NULL || instances == NULL) {
        return 0;
    }

    *edge_count = (int)state.graph.edge_count;

    for (int i = 0; i < *edge_count; i++) {
        types[i] = (int)state.graph.edges[i].type;
        processes[i] = (int)state.graph.edges[i].process_index;
        resources[i] = (int)state.graph.edges[i].resource_index;
        instances[i] = (int)state.graph.edges[i].instances;
    }

    return 1;
}

int bridge_graph_has_cycle(void)
{
    return initialized && rag_has_cycle(&state.graph);
}
