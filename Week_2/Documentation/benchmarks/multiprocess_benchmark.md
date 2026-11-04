# Multiprocess IPC Benchmark

## Objective
Measure the communication time between two independent processes using POSIX Message Queues.

## Test Setup
- A parent process sends messages to a child process.
- The child receives each message and sends it back.
- Communication uses two POSIX Message Queues.
- The test performs 1,000 round trips.

## Measurement
- Timer: CLOCK_MONOTONIC
- Number of round trips: 1,000
- Total measured time: 0.083971003 seconds
- Average round-trip time: 83.971 microseconds
- Benchmark source: benchmark_ipc.c

## Result
The average time for one parent-child-parent message round trip was approximately 83.971 microseconds.

## Conclusion
The benchmark demonstrates communication between independent processes using POSIX Message Queues. The measured time includes message sending, receiving and process scheduling during the test.
