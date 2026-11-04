# Benchmark Comparison

## Objective
Document the measured performance of standalone Core execution and inter-process communication.

## Results

| Benchmark | Operations | Measured Time |
|---|---|---:|
| Standalone Core | LOAD, RUN and STOP | 69.70 µs |
| POSIX Message Queue | Average of 1,000 round trips | 83.971 µs |

## Observations

- The standalone benchmark measures Core operations within one process.
- The IPC benchmark measures message exchange between two independent processes.
- POSIX Message Queue communication introduces communication and process scheduling overhead.
- The two benchmarks use different workloads and are not a direct performance comparison.

## Conclusion

Both benchmarks demonstrate the performance of different parts of the simulator. The standalone test provides a baseline for Core execution, while the IPC test measures communication overhead between processes.
