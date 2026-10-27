# Aahil — Optimal Page Replacement

## Files
- `optimal.h`: declaration for the Optimal simulation function.
- `optimal.c`: Optimal page-replacement implementation.
- `tests/test_optimal.c`: automated tests.

## Compile and run tests
From `Week_5/member_3_aahil/`:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic optimal.c tests/test_optimal.c -o tests/test_optimal
./tests/test_optimal
```

## Interface
`simulate_optimal(pages, reference_count, frame_count)` returns the shared
`SimulationResult` type declared in `Week_5/include/simulator.h`. Each step
contains the requested page, a snapshot of all `MAX_FRAMES` frame slots, and
the hit/fault flag. Empty frame slots are represented by `-1`.

`hit_ratio` and `fault_ratio` are fractions (e.g. `0.3` means 30%), consistent
with the existing LRU output convention. Display percentages by multiplying
by 100.

## Algorithm
1. Search frames for the requested page. If found, count a hit.
2. Otherwise count a fault and use an empty frame if available.
3. If memory is full, inspect future references for each resident page.
4. Replace the page used farthest in the future; if it is never used again,
   it can be replaced immediately.
5. Save the frame snapshot for this request.
6. Calculate hit and fault ratios after all requests.

## Complexity
The straightforward implementation uses O(n × f × n) worst-case time,
where `n` is the reference count and `f` is the frame count, and O(n × F)
result storage as specified by the shared `SimulationResult` structure.
