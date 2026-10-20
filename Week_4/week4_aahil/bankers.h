#ifndef BANKERS_H
#define BANKERS_H

#include <stdbool.h>
#include <stddef.h>

#define BANKERS_MAX_PROCESSES 10
#define BANKERS_MAX_RESOURCES 10

typedef enum {
    BANKERS_OK = 0,
    BANKERS_ERR_ARGUMENT,
    BANKERS_ERR_DIMENSIONS,
    BANKERS_ERR_NEGATIVE_VALUE,
    BANKERS_ERR_ALLOCATION_EXCEEDS_MAXIMUM,
    BANKERS_ERR_REQUEST_EXCEEDS_NEED,
    BANKERS_ERR_INSUFFICIENT_AVAILABLE,
    BANKERS_ERR_UNSAFE_REQUEST
} BankersStatus;

typedef struct {
    size_t process_count;
    size_t resource_count;
    int allocation[BANKERS_MAX_PROCESSES][BANKERS_MAX_RESOURCES];
    int maximum[BANKERS_MAX_PROCESSES][BANKERS_MAX_RESOURCES];
    int need[BANKERS_MAX_PROCESSES][BANKERS_MAX_RESOURCES];
    int available[BANKERS_MAX_RESOURCES];
} BankerState;

typedef struct {
    bool is_safe;
    size_t sequence_length;
    int safe_sequence[BANKERS_MAX_PROCESSES];
} BankerSafetyResult;

/* Initialize and validate a state. Matrix rows are processes; columns are resource types. */
BankersStatus bankers_init(
    BankerState *state,
    size_t process_count,
    size_t resource_count,
    int allocation[BANKERS_MAX_PROCESSES][BANKERS_MAX_RESOURCES],
    int maximum[BANKERS_MAX_PROCESSES][BANKERS_MAX_RESOURCES],
    const int available[BANKERS_MAX_RESOURCES]);

/* Recalculate Need = Maximum - Allocation. */
BankersStatus bankers_calculate_need(BankerState *state);

/* Run the safety algorithm. Safe sequence entries are zero-based process indices. */
BankerSafetyResult bankers_check_safety(const BankerState *state);

/* Validate, tentatively allocate, and grant only if the resulting state remains safe.
   A rejected request leaves the state unchanged. Request values must be non-negative. */
BankersStatus bankers_request_resources(
    BankerState *state,
    size_t process_index,
    const int request[BANKERS_MAX_RESOURCES],
    BankerSafetyResult *result_after_request);

const char *bankers_status_message(BankersStatus status);

#endif
