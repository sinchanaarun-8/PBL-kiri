# Standalone Benchmark

## Objective
Measure the execution time of Core operations running directly in a single process without inter-process communication.

## Operations Tested
1. Initialize the Core.
2. Execute LOAD.
3. Execute RUN.
4. Execute STOP.

Core initialization is performed before the timer starts.

## Measurement
- Timer: CLOCK_MONOTONIC
- Measured execution time: 69.70 microseconds
- Benchmark source: benchmark_standalone.c

## Result
The standalone Core completed the measured LOAD, RUN and STOP operations in approximately 69.70 microseconds.

## Conclusion
The standalone benchmark provides a baseline for Core execution without IPC communication overhead.
