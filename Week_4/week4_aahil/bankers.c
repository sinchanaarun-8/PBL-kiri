#include "bankers.h"

#include <string.h>

static bool valid_dimensions(size_t processes, size_t resources) {
    return processes > 0 && processes <= BANKERS_MAX_PROCESSES &&
           resources > 0 && resources <= BANKERS_MAX_RESOURCES;
}

BankersStatus bankers_calculate_need(BankerState *state) {
    size_t p, r;

    if (state == NULL) {
        return BANKERS_ERR_ARGUMENT;
    }

    if (!valid_dimensions(state->process_count, state->resource_count)) {
        return BANKERS_ERR_DIMENSIONS;
    }

    /* Validate the complete state before modifying Need. */
    for (r = 0; r < state->resource_count; ++r) {
        if (state->available[r] < 0) {
            return BANKERS_ERR_NEGATIVE_VALUE;
        }
    }

    for (p = 0; p < state->process_count; ++p) {
        for (r = 0; r < state->resource_count; ++r) {
            if (state->allocation[p][r] < 0 ||
                state->maximum[p][r] < 0) {
                return BANKERS_ERR_NEGATIVE_VALUE;
            }

            if (state->allocation[p][r] > state->maximum[p][r]) {
                return BANKERS_ERR_ALLOCATION_EXCEEDS_MAXIMUM;
            }
        }
    }

    /* All inputs are valid; now calculate Need. */
    for (p = 0; p < state->process_count; ++p) {
        for (r = 0; r < state->resource_count; ++r) {
            state->need[p][r] =
                state->maximum[p][r] - state->allocation[p][r];
        }
    }

    return BANKERS_OK;
}

BankersStatus bankers_init(
    BankerState *state,
    size_t process_count,
    size_t resource_count,
    int allocation[BANKERS_MAX_PROCESSES][BANKERS_MAX_RESOURCES],
    int maximum[BANKERS_MAX_PROCESSES][BANKERS_MAX_RESOURCES],
    const int available[BANKERS_MAX_RESOURCES]) {

    BankerState temporary = {0};
    BankersStatus status;
    size_t p, r;

    if (state == NULL || allocation == NULL ||
        maximum == NULL || available == NULL) {
        return BANKERS_ERR_ARGUMENT;
    }

    if (!valid_dimensions(process_count, resource_count)) {
        return BANKERS_ERR_DIMENSIONS;
    }

    temporary.process_count = process_count;
    temporary.resource_count = resource_count;

    for (r = 0; r < resource_count; ++r) {
        if (available[r] < 0) {
            return BANKERS_ERR_NEGATIVE_VALUE;
        }

        temporary.available[r] = available[r];
    }

    for (p = 0; p < process_count; ++p) {
        for (r = 0; r < resource_count; ++r) {
            if (allocation[p][r] < 0 || maximum[p][r] < 0) {
                return BANKERS_ERR_NEGATIVE_VALUE;
            }

            if (allocation[p][r] > maximum[p][r]) {
                return BANKERS_ERR_ALLOCATION_EXCEEDS_MAXIMUM;
            }

            temporary.allocation[p][r] = allocation[p][r];
            temporary.maximum[p][r] = maximum[p][r];
        }
    }

    status = bankers_calculate_need(&temporary);
    if (status != BANKERS_OK) {
        return status;
    }

    /* Commit only after the complete state is valid. */
    *state = temporary;

    return BANKERS_OK;
}

BankerSafetyResult bankers_check_safety(const BankerState *state) {
    BankerSafetyResult result = { false, 0, { 0 } };
    int work[BANKERS_MAX_RESOURCES] = { 0 };
    bool finished[BANKERS_MAX_PROCESSES] = { false };
    size_t p, r;

    if (state == NULL ||
        !valid_dimensions(state->process_count, state->resource_count)) {
        return result;
    }

    /* Validate the resource state and initialize Work. */
    for (r = 0; r < state->resource_count; ++r) {
        if (state->available[r] < 0) {
            return result;
        }

        work[r] = state->available[r];
    }

    for (p = 0; p < state->process_count; ++p) {
        for (r = 0; r < state->resource_count; ++r) {
            if (state->allocation[p][r] < 0 ||
                state->maximum[p][r] < 0 ||
                state->allocation[p][r] > state->maximum[p][r]) {
                return result;
            }
        }
    }

    /*
     * Calculate Need directly from Maximum - Allocation.
     * This avoids relying on a potentially stale Need matrix.
     */
    while (result.sequence_length < state->process_count) {
        bool found = false;

        for (p = 0; p < state->process_count; ++p) {
            bool can_finish = true;

            if (finished[p]) {
                continue;
            }

            for (r = 0; r < state->resource_count; ++r) {
                int need = state->maximum[p][r] -
                           state->allocation[p][r];

                if (need > work[r]) {
                    can_finish = false;
                    break;
                }
            }

            if (can_finish) {
                for (r = 0; r < state->resource_count; ++r) {
                    work[r] += state->allocation[p][r];
                }

                finished[p] = true;
                result.safe_sequence[result.sequence_length++] = (int)p;
                found = true;
                break;
            }
        }

        if (!found) {
            /* No complete safe sequence exists. */
            result.sequence_length = 0;
            return result;
        }
    }

    result.is_safe = true;
    return result;
}

BankersStatus bankers_request_resources(
    BankerState *state,
    size_t process_index,
    const int request[BANKERS_MAX_RESOURCES],
    BankerSafetyResult *result_after_request) {

    int old_available[BANKERS_MAX_RESOURCES];
    int old_allocation[BANKERS_MAX_RESOURCES];
    BankerSafetyResult safety;
    BankerSafetyResult unsafe_result = { false, 0, { 0 } };
    BankerState validated_state;
    BankersStatus status;
    size_t r;

    if (state == NULL || request == NULL) {
        return BANKERS_ERR_ARGUMENT;
    }

    if (!valid_dimensions(state->process_count, state->resource_count) ||
        process_index >= state->process_count) {
        return BANKERS_ERR_DIMENSIONS;
    }

    /* Validate and calculate Need without partially changing state. */
    validated_state = *state;
    status = bankers_calculate_need(&validated_state);

    if (status != BANKERS_OK) {
        return status;
    }

    /* Validate the entire request before modifying any resources. */
    for (r = 0; r < state->resource_count; ++r) {
        if (request[r] < 0) {
            return BANKERS_ERR_NEGATIVE_VALUE;
        }

        if (request[r] > validated_state.need[process_index][r]) {
            return BANKERS_ERR_REQUEST_EXCEEDS_NEED;
        }

        if (request[r] > validated_state.available[r]) {
            return BANKERS_ERR_INSUFFICIENT_AVAILABLE;
        }
    }

    /* Preserve the original state for rollback. */
    for (r = 0; r < state->resource_count; ++r) {
        old_available[r] = state->available[r];
        old_allocation[r] = state->allocation[process_index][r];
    }

    /*
     * Apply the request tentatively.
     * Need is recalculated before the safety check.
     */
    for (r = 0; r < state->resource_count; ++r) {
        state->available[r] -= request[r];
        state->allocation[process_index][r] += request[r];
    }

    status = bankers_calculate_need(state);

    if (status != BANKERS_OK) {
        for (r = 0; r < state->resource_count; ++r) {
            state->available[r] = old_available[r];
            state->allocation[process_index][r] = old_allocation[r];
        }

        (void)bankers_calculate_need(state);
        return status;
    }

    safety = bankers_check_safety(state);

    if (result_after_request != NULL) {
        *result_after_request = safety;
    }

    if (!safety.is_safe) {
        /* Restore the original state when the request is unsafe. */
        for (r = 0; r < state->resource_count; ++r) {
            state->available[r] = old_available[r];
            state->allocation[process_index][r] = old_allocation[r];
        }

        (void)bankers_calculate_need(state);

        if (result_after_request != NULL) {
            *result_after_request = unsafe_result;
        }

        return BANKERS_ERR_UNSAFE_REQUEST;
    }

    return BANKERS_OK;
}

const char *bankers_status_message(BankersStatus status) {
    switch (status) {
        case BANKERS_OK:
            return "Request accepted / operation successful";

        case BANKERS_ERR_ARGUMENT:
            return "Invalid null argument";

        case BANKERS_ERR_DIMENSIONS:
            return "Invalid process/resource count or process index";

        case BANKERS_ERR_NEGATIVE_VALUE:
            return "Resource values and requests must be non-negative";

        case BANKERS_ERR_ALLOCATION_EXCEEDS_MAXIMUM:
            return "Allocation cannot exceed maximum claim";

        case BANKERS_ERR_REQUEST_EXCEEDS_NEED:
            return "Request exceeds this process's remaining need";

        case BANKERS_ERR_INSUFFICIENT_AVAILABLE:
            return "Insufficient available resources";

        case BANKERS_ERR_UNSAFE_REQUEST:
            return "Request rejected because it would leave the system unsafe";

        default:
            return "Unknown Banker's Algorithm status";
    }
}