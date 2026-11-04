# Performance Analysis

## Overview
The simulator uses separate UI, Core and Logger processes. Inter-process communication allows these processes to work independently and exchange information.

## Benchmark Results

| Measurement | Result |
|---|---:|
| Standalone Core execution (LOAD, RUN, STOP) | 69.70 µs |
| Average POSIX MQ round trip | 83.971 µs |
| Complete interactive simulator elapsed time | 15.15 seconds |

## Analysis

1. Standalone execution measures Core operations without inter-process communication.
2. POSIX Message Queue testing measures communication between a parent and child process.
3. Separate processes introduce communication, synchronization and scheduling overhead.
4. The complete simulator elapsed time includes startup delays, interactive user input and process shutdown. It is not a pure IPC measurement.
5. The standalone and IPC benchmark results use different workloads and should not be treated as a direct speed comparison.

## Conclusion

The benchmarks demonstrate the execution cost of Core operations and the communication cost of POSIX Message Queues. The multi-process architecture provides modularity and independent process execution, while introducing additional communication and process-management overhead.
