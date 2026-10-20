#include "../bankers.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define P BANKERS_MAX_PROCESSES
#define R BANKERS_MAX_RESOURCES

static void test_classic_safe_state(void) {
    int allocation[P][R] = {{0}};
    int maximum[P][R] = {{0}};
    int available[R] = {3, 3, 2};
    BankerState state;
    BankerSafetyResult result;

    int a[5][3] = {{0,1,0},{2,0,0},{3,0,2},{2,1,1},{0,0,2}};
    int m[5][3] = {{7,5,3},{3,2,2},{9,0,2},{2,2,2},{4,3,3}};
    for (int i = 0; i < 5; ++i) for (int j = 0; j < 3; ++j) { allocation[i][j] = a[i][j]; maximum[i][j] = m[i][j]; }
    assert(bankers_init(&state, 5, 3, allocation, maximum, available) == BANKERS_OK);
    assert(state.need[0][0] == 7 && state.need[0][1] == 4 && state.need[0][2] == 3);
    result = bankers_check_safety(&state);
    assert(result.is_safe && result.sequence_length == 5);
    puts("PASS: classic safe state and Need calculation");
}

static void test_unsafe_state(void) {
    int allocation[P][R] = {{0}};
    int maximum[P][R] = {{0}};
    int available[R] = {0, 0};
    BankerState state;
    BankerSafetyResult result;
    allocation[0][0] = 1; maximum[0][0] = 2; maximum[0][1] = 1;
    allocation[1][1] = 1; maximum[1][0] = 1; maximum[1][1] = 2;
    assert(bankers_init(&state, 2, 2, allocation, maximum, available) == BANKERS_OK);
    result = bankers_check_safety(&state);
    assert(!result.is_safe);
    puts("PASS: unsafe state identified");
}

static void test_request_checks_and_grant(void) {
    int allocation[P][R] = {{0}};
    int maximum[P][R] = {{0}};
    int available[R] = {3, 3, 2};
    int request[R] = {1, 0, 2};
    BankerState state;
    BankerSafetyResult result;
    int a[5][3] = {{0,1,0},{2,0,0},{3,0,2},{2,1,1},{0,0,2}};
    int m[5][3] = {{7,5,3},{3,2,2},{9,0,2},{2,2,2},{4,3,3}};
    for (int i = 0; i < 5; ++i) for (int j = 0; j < 3; ++j) { allocation[i][j] = a[i][j]; maximum[i][j] = m[i][j]; }
    assert(bankers_init(&state, 5, 3, allocation, maximum, available) == BANKERS_OK);
    assert(bankers_request_resources(&state, 1, request, &result) == BANKERS_OK);
    assert(result.is_safe);
    assert(state.allocation[1][0] == 3 && state.allocation[1][2] == 2);
    assert(state.available[0] == 2 && state.available[2] == 0);
    puts("PASS: valid request granted when resulting state is safe");
}

static void test_invalid_requests_do_not_change_state(void) {
    int allocation[P][R] = {{0}};
    int maximum[P][R] = {{0}};
    int available[R] = {1};
    int too_much[R] = {2};
    int negative[R] = {-1};
    BankerState state;
    BankerState before;
    assert(bankers_init(&state, 1, 1, allocation, maximum, available) == BANKERS_OK);
    maximum[0][0] = 1;
    assert(bankers_init(&state, 1, 1, allocation, maximum, available) == BANKERS_OK);
    before = state;
    assert(bankers_request_resources(&state, 0, too_much, NULL) == BANKERS_ERR_REQUEST_EXCEEDS_NEED);
    assert(memcmp(&state, &before, sizeof(state)) == 0);
    assert(bankers_request_resources(&state, 0, negative, NULL) == BANKERS_ERR_NEGATIVE_VALUE);
    assert(memcmp(&state, &before, sizeof(state)) == 0);
    puts("PASS: invalid requests rejected without modifying state");
}

static void test_insufficient_resources(void) {
    int allocation[P][R] = {{0}};
    int maximum[P][R] = {{0}};
    int available[R] = {0};
    int request[R] = {1};
    BankerState state;
    assert(bankers_init(&state, 1, 1, allocation, maximum, available) == BANKERS_OK);
    maximum[0][0] = 1;
    assert(bankers_init(&state, 1, 1, allocation, maximum, available) == BANKERS_OK);
    assert(bankers_request_resources(&state, 0, request, NULL) == BANKERS_ERR_INSUFFICIENT_AVAILABLE);
    puts("PASS: request rejected when resources are unavailable");
}


static void test_unsafe_request_is_rolled_back(void) {
    int allocation[P][R] = {{0}};
    int maximum[P][R] = {{0}};
    int available[R] = {0, 1};
    int request[R] = {0, 1};
    BankerState state;
    BankerState before;
    BankerSafetyResult result;
    allocation[1][1] = 1;
    maximum[0][1] = 2;
    maximum[1][1] = 2;
    assert(bankers_init(&state, 2, 2, allocation, maximum, available) == BANKERS_OK);
    assert(bankers_check_safety(&state).is_safe);
    before = state;
    assert(bankers_request_resources(&state, 0, request, &result) == BANKERS_ERR_UNSAFE_REQUEST);
    assert(!result.is_safe);
    assert(memcmp(&state, &before, sizeof(state)) == 0);
    puts("PASS: unsafe request rejected and tentative allocation rolled back");
}

int main(void) {
    test_classic_safe_state();
    test_unsafe_state();
    test_request_checks_and_grant();
    test_invalid_requests_do_not_change_state();
    test_insufficient_resources();
    test_unsafe_request_is_rolled_back();
    puts("\nAll Banker's Algorithm tests passed.");
    return 0;
}
