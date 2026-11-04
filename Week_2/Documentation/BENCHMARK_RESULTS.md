# IPC and Performance Benchmark Results

## 1. Standalone Core Benchmark

- Operations: LOAD, RUN and STOP
- Measured execution time: 69.70 µs
- Core initialization is excluded from the timed section.

## 2. POSIX Message Queue Benchmark

- Processes: Parent and child
- Communication: Two POSIX Message Queues
- Number of round trips: 1,000
- Total measured time: 0.083971003 seconds
- Average round-trip time: 83.971 µs

## 3. Complete Simulator

- Measured elapsed time: 15.15 seconds
- Includes process startup delays, interactive execution and shutdown.
- This is not a pure IPC measurement.

## 4. Performance Observations

The standalone benchmark measures Core operations within one process, whereas the POSIX Message Queue benchmark measures communication between two processes.

These tests use different workloads and should not be treated as a direct performance comparison.

The multi-process architecture provides independent UI, Core and Logger processes, but introduces communication and process-management overhead.

## Conclusion

The benchmark results demonstrate standalone Core execution performance, POSIX Message Queue communication performance and complete simulator execution time.
