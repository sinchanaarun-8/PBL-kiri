#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdbool.h>
#include <stddef.h>

#include "../../week4_aahil/bankers.h"
#include "../../weeek 4_jisnu/rag.h"

#define SIM_MAX_PROCESSES BANKERS_MAX_PROCESSES
#define SIM_MAX_RESOURCES BANKERS_MAX_RESOURCES

typedef struct {
    BankerState banker;
    int request[SIM_MAX_PROCESSES][SIM_MAX_RESOURCES];
    bool terminated[SIM_MAX_PROCESSES];
    size_t resource_instances[SIM_MAX_RESOURCES];
    RagGraph graph;
} Simulator;

typedef struct {
    int deadlocked[SIM_MAX_PROCESSES];
    int count;
} SimDeadlockResult;

/* Initialize the shared simulator state. */
BankersStatus sim_init(
    Simulator *sim,
    size_t process_count,
    size_t resource_count,
    int allocation[SIM_MAX_PROCESSES][SIM_MAX_RESOURCES],
    int maximum[SIM_MAX_PROCESSES][SIM_MAX_RESOURCES],
    const int available[SIM_MAX_RESOURCES]);

/* Check safety using the existing Banker's Algorithm module. */
BankerSafetyResult sim_check_safety(const Simulator *sim);

/* Submit a resource request through Banker's Algorithm. */
BankersStatus sim_request_resources(
    Simulator *sim,
    size_t process_index,
    const int request[SIM_MAX_RESOURCES],
    BankerSafetyResult *result);

/* Run the separate deadlock detection module. */
SimDeadlockResult sim_detect_deadlock(const Simulator *sim);

/* Recover a process and synchronize the shared state and graph. */
int sim_recover_process(Simulator *sim, size_t process_index);

/* Refresh the Resource Allocation Graph from the shared matrices. */
RagStatus sim_refresh_graph(Simulator *sim);

#endif

