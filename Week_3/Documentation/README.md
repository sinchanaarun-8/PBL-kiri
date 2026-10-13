# Week 3 Documentation

This folder contains the architecture, algorithm explanations, test cases, and performance comparison for the C-based Process Scheduling Simulator.

## Documents

- `architecture.md` — Project structure, inputs, execution flow, and system design.
- `algorithm_explanation.md` — Working principles of all six CPU scheduling algorithms.
- `test_cases.md` — Scheduling examples, expected metrics, input validation, and integration tests.
- `performance_comparison.md` — Comparison of recorded scheduling metrics for a common workload.

## Supported Algorithms

1. First Come, First Served (FCFS)
2. SJF Non-Preemptive
3. Priority Non-Preemptive
4. SJF Preemptive (SRTF)
5. Priority Preemptive
6. Round Robin

## Required Performance Metrics

- Completion Time (CT)
- Turnaround Time (TAT) = CT - Arrival Time
- Waiting Time (WT) = TAT - Burst Time
- Response Time (RT) = First CPU Start Time - Arrival Time
- Average Waiting Time
- Average Turnaround Time
- Average Response Time
- CPU Utilization = (CPU Busy Time / Total Elapsed Time) × 100

The elapsed interval is measured from time 0 to the final process completion, including idle periods.

## Build and Run

From the `Week_3/member_3 aahil` directory:

```bash
make
./build/scheduler
