# Process Scheduling Simulator — Performance Comparison

## 1. Objective

Compare six CPU scheduling algorithms using the same set of processes and their average waiting, turnaround, and response times.

## 2. Test Workload

| Process | Arrival Time | Burst Time | Priority |
|---|---:|---:|---:|
| P1 | 0 | 7 | 2 |
| P2 | 2 | 4 | 1 |
| P3 | 4 | 1 | 3 |
| P4 | 5 | 2 | 2 |

Priority convention: a smaller number indicates a higher priority.

## 3. Recorded Results

| Algorithm | Average WT | Average TAT | Average RT | CPU Utilization |
|---|---:|---:|---:|---:|
| FCFS | 4.75 | 8.25 | 4.75 | 100% |
| SJF Non-Preemptive | 3.50 | 7.00 | 3.50 | 100% |
| Priority Non-Preemptive | 5.00 | 8.50 | 5.00 | 100% |
| SRTF | 2.50 | 6.00 | 0.50 | 100% |
| Priority Preemptive | 4.75 | 8.25 | 3.75 | 100% |
| Round Robin, quantum 2 | 4.00 | 7.50 | 1.50 | 100% |

These values were recorded from previous simulator test runs. Re-run the integrated program to confirm them if the scheduling code changes.

## 4. Analysis

- SRTF has the lowest average waiting time in this workload.
- SRTF also has the lowest average turnaround and response times in this workload.
- SJF Non-Preemptive performs better than FCFS on average waiting and turnaround time for this input.
- Priority scheduling favours higher-priority processes, but lower-priority processes may wait longer.
- Round Robin gives processes turns to execute. Its results depend on the selected time quantum.
- CPU utilization is 100% for this workload because the CPU has a process ready to run throughout the interval from time 0 to time 14.

## 5. Metric Definitions

- Average Waiting Time = Sum of all waiting times / Number of processes.
- Average Turnaround Time = Sum of all turnaround times / Number of processes.
- Average Response Time = Sum of all response times / Number of processes.
- CPU Utilization = (CPU Busy Time / Total Elapsed Time) × 100.

## 6. Conclusion

No single scheduling algorithm is best for every workload. SRTF performs well for this particular set of processes, while Round Robin provides time-sharing behaviour through a configurable quantum. Results should be compared across multiple workloads before drawing broader conclusions.

This report compares simulated scheduling metrics; it does not claim measured wall-clock execution speed or operating-system scheduling overhead.
