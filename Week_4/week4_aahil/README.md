# Week 4 — Aahil: Banker's Algorithm and Resource Management

This module implements the Banker's Algorithm safety check and safe resource-request handling for the Interactive Deadlock Simulation System.

## Files

- `bankers.h` — constants, state/result structures, status codes, public function declarations.
- `bankers.c` — input validation, `Need = Maximum - Allocation`, safety algorithm, safe-sequence generation, and request grant/reject with rollback.
- `tests/test_bankers.c` — executable tests for safe/unsafe states and request validation.
- `Makefile` — reproducible GCC build and test commands.

## Build and run (Linux / WSL)

```bash
make clean
make test
```

Compiler settings use C11 with warnings treated as errors.

## Data conventions

- Rows represent processes; columns represent resource types.
- Process indices and safe-sequence entries are zero-based (`P0`, `P1`, ...).
- The caller provides the current `Available` vector. This module validates that values are non-negative, but does not infer total system resources because total-resource data is not part of this API.
- The API supports up to 10 processes and 10 resource types; adjust the constants if the integrated application requires a different limit.

## Public API

- `bankers_init(...)`: copies and validates the initial Allocation, Maximum, and Available data, then calculates Need.
- `bankers_calculate_need(...)`: recalculates Need and validates allocation/maximum relationships.
- `bankers_check_safety(...)`: returns whether the state is safe and the safe sequence if one exists.
- `bankers_request_resources(...)`: validates a process request, tentatively allocates, checks safety, and either commits the request or rolls back to the prior state.
- `bankers_status_message(...)`: maps status codes to readable messages.

## Test coverage

1. Classic five-process, three-resource safe-state example and Need calculation.
2. Unsafe state where no process can finish with current Available resources.
3. Valid request that preserves safety and is granted.
4. Request exceeding the process's remaining Need.
5. Negative request values.
6. Request rejected when Available resources are insufficient.
7. Rejected invalid requests leave the state unchanged.

## Integration note

The GUI/team lead should call this module through `bankers.h`; do not duplicate Banker's Algorithm logic in the GUI. Deadlock detection and recovery remain separate modules owned by other team members. An unsafe state is not automatically proof that a deadlock currently exists.
