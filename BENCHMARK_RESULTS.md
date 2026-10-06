# IPC Benchmark Results

## Execution Time Comparison

| Implementation | Average Execution Time |
|---|---:|
| Standalone | 75.27 µs |
| Multiprocess IPC | 117 ms |

## Result

The standalone implementation averaged approximately **75.27 µs**, while the multiprocess IPC implementation averaged approximately **117 ms**.

The multiprocess implementation has higher execution time due to process creation, IPC communication, synchronization, and scheduling overhead.

## Conclusion

The standalone implementation is faster for this workload, while the multiprocess implementation demonstrates the additional overhead introduced by IPC and process management.
