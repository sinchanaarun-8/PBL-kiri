#include "simulator.h"
#include "../../week4_josna/include/detection.h"

#include <limits.h>
#include <string.h>

BankersStatus sim_init(
    Simulator *sim,
    size_t process_count,
    size_t resource_count,
    int allocation[SIM_MAX_PROCESSES][SIM_MAX_RESOURCES],
    int maximum[SIM_MAX_PROCESSES][SIM_MAX_RESOURCES],
    const int available[SIM_MAX_RESOURCES])
{
    if (sim == NULL) {
        return BANKERS_ERR_ARGUMENT;
    }

    Simulator temp = {0};

    BankersStatus status = bankers_init(
        &temp.banker,
        process_count,
        resource_count,
        allocation,
        maximum,
        available);

    if (status != BANKERS_OK) {
        return status;
    }

    for (size_t r = 0; r < resource_count; r++) {
        size_t total = (size_t)temp.banker.available[r];

        for (size_t p = 0; p < process_count; p++) {
            total += (size_t)temp.banker.allocation[p][r];
        }

        temp.resource_instances[r] = total;
    }

    RagStatus rag_status = rag_init(
        &temp.graph,
        process_count,
        resource_count,
        temp.resource_instances);

    if (rag_status != RAG_OK) {
        return BANKERS_ERR_ARGUMENT;
    }

    rag_status = rag_update(
        &temp.graph,
        temp.banker.allocation,
        temp.request);

    if (rag_status != RAG_OK) {
        return BANKERS_ERR_ARGUMENT;
    }

    *sim = temp;
    return BANKERS_OK;
}

BankerSafetyResult sim_check_safety(const Simulator *sim)
{
    BankerSafetyResult result = {0};

    if (sim == NULL) {
        return result;
    }

    return bankers_check_safety(&sim->banker);
}

RagStatus sim_refresh_graph(Simulator *sim)
{
    if (sim == NULL) {
        return RAG_ERR_ARGUMENT;
    }

    return rag_update(
        &sim->graph,
        sim->banker.allocation,
        sim->request);
}

BankersStatus sim_request_resources(
    Simulator *sim,
    size_t process_index,
    const int request[SIM_MAX_RESOURCES],
    BankerSafetyResult *result)
{
    if (sim == NULL || request == NULL ||
        process_index >= sim->banker.process_count) {
        return BANKERS_ERR_ARGUMENT;
    }

    if (sim->terminated[process_index]) {
        return BANKERS_ERR_ARGUMENT;
    }

    Simulator previous = *sim;

    BankersStatus status = bankers_request_resources(
        &sim->banker,
        process_index,
        request,
        result);

    if (status != BANKERS_OK) {
        return status;
    }

    /* A successful Banker request is granted immediately.
       Do not leave it recorded as a pending request. */
    for (size_t r = 0; r < sim->banker.resource_count; r++) {
        sim->request[process_index][r] = 0;
    }

    if (sim_refresh_graph(sim) != RAG_OK) {
        *sim = previous;
        return BANKERS_ERR_ARGUMENT;
    }

    return BANKERS_OK;
}

SimDeadlockResult sim_detect_deadlock(const Simulator *sim)
{
    SimDeadlockResult result = {0};

    if (sim == NULL ||
        sim->banker.process_count == 0 ||
        sim->banker.process_count > SIM_MAX_PROCESSES ||
        sim->banker.resource_count == 0 ||
        sim->banker.resource_count > SIM_MAX_RESOURCES) {
        return result;
    }

    int allocation20[MAX_PROCESSES][MAX_RESOURCES] = {{0}};
    int request20[MAX_PROCESSES][MAX_RESOURCES] = {{0}};
    int available20[MAX_RESOURCES] = {0};
    int deadlocked20[MAX_PROCESSES] = {0};

    size_t processes = sim->banker.process_count;
    size_t resources = sim->banker.resource_count;

    for (size_t p = 0; p < processes; p++) {
        for (size_t r = 0; r < resources; r++) {
            allocation20[p][r] = sim->banker.allocation[p][r];
            request20[p][r] = sim->request[p][r];
        }
    }

    for (size_t r = 0; r < resources; r++) {
        available20[r] = sim->banker.available[r];
    }

    result.count = detect_deadlock(
        (int)processes,
        (int)resources,
        allocation20,
        request20,
        available20,
        deadlocked20);

    if (result.count < 0) {
        result.count = 0;
        return result;
    }

    for (size_t p = 0; p < processes; p++) {
        result.deadlocked[p] = deadlocked20[p];
    }

    return result;
}

int sim_recover_process(Simulator *sim, size_t process_index)
{
    if (sim == NULL ||
        process_index >= sim->banker.process_count ||
        sim->banker.process_count > SIM_MAX_PROCESSES ||
        sim->banker.resource_count == 0 ||
        sim->banker.resource_count > SIM_MAX_RESOURCES) {
        return 0;
    }

    Simulator previous = *sim;

    int allocation20[MAX_PROCESSES][MAX_RESOURCES] = {{0}};
    int available20[MAX_RESOURCES] = {0};

    size_t processes = sim->banker.process_count;
    size_t resources = sim->banker.resource_count;

    for (size_t p = 0; p < processes; p++) {
        for (size_t r = 0; r < resources; r++) {
            allocation20[p][r] = sim->banker.allocation[p][r];
        }
    }

    for (size_t r = 0; r < resources; r++) {
        available20[r] = sim->banker.available[r];
    }

    int recovered = recover_process(
        (int)process_index,
        (int)processes,
        (int)resources,
        allocation20,
        available20);

    if (!recovered) {
        return 0;
    }

    for (size_t p = 0; p < processes; p++) {
        for (size_t r = 0; r < resources; r++) {
            sim->banker.allocation[p][r] = allocation20[p][r];
        }
    }

    for (size_t r = 0; r < resources; r++) {
        sim->banker.available[r] = available20[r];
    }

    /* Terminate the recovered process and cancel its requests. */
    sim->terminated[process_index] = true;

    for (size_t r = 0; r < resources; r++) {
        sim->request[process_index][r] = 0;
        sim->banker.maximum[process_index][r] = 0;
    }

    if (bankers_calculate_need(&sim->banker) != BANKERS_OK ||
        sim_refresh_graph(sim) != RAG_OK) {
        *sim = previous;
        return 0;
    }

    return 1;
}
