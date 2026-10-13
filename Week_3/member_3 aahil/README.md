# Week 3 — CPU Process Scheduling Simulator

A C-based simulator that implements and compares six CPU scheduling algorithms.

## Features

- Accepts process ID, arrival time, burst time, and priority.
- Supports Round Robin with a configurable time quantum.
- Displays Gantt charts and process scheduling metrics.
- Calculates average waiting time, turnaround time, response time, and CPU utilization.
- Compares all six algorithms.

## Scheduling Algorithms

1. First Come First Served (FCFS)
2. Shortest Job First (SJF) — Non-Preemptive
3. Priority Scheduling — Non-Preemptive
4. Shortest Remaining Time First (SRTF) — Preemptive
5. Priority Scheduling — Preemptive
6. Round Robin (RR)

A smaller priority number means a higher priority.

## Build and Run

Run from the `Week_3/member_3 aahil` directory:

```bash
make
./build/scheduler

```

## Performance Metrics

- TAT = CT - Arrival Time
- WT = TAT - Burst Time
- RT = First CPU Start Time - Arrival Time
- CPU Utilization = (CPU Busy Time / Total Elapsed Time) * 100

## Documentation

See `../Documentation/` for architecture, algorithm explanations, test cases, and performance comparisons.

## Menu

Options 1–6 run individual algorithms, option 7 compares all algorithms, and option 8 exits.
