# LRU Page Replacement Algorithm — Test Results

## Algorithm
Least Recently Used (LRU) Page Replacement

## Test Case 1: Basic Test

**Number of frames:** 3

**Page reference string:** 1 2 3 1 4 5

### Expected Results

| Page | Frames | Status |
|---|---|---|
| 1 | 1 - - | Fault |
| 2 | 1 2 - | Fault |
| 3 | 1 2 3 | Fault |
| 1 | 1 2 3 | Hit |
| 4 | 1 4 3 | Fault |
| 5 | 1 4 5 | Fault |

### Final Statistics

- Total page requests: 6
- Total hits: 1
- Total page faults: 5
- Hit ratio: 0.17
- Fault ratio: 0.83

**Result:** Test passed. The LRU algorithm correctly identifies page hits and faults and replaces the least recently used page.

## Files Delivered

- `include/lru.h`
- `src/lru.c`
- `tests/test_lru.c`
- `tests/test_results.md`