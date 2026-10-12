
# Process Scheduling Algorithms — Aahil Sheikh

## 1. Overview

This module implements two CPU scheduling algorithms in C:

1. Shortest Remaining Time First (SRTF)
2. Round Robin (RR) with a configurable time quantum

Both algorithms use process arrival times and burst times to simulate
CPU execution. The simulator generates a Gantt chart and calculates
completion time, turnaround time, waiting time, and response time.

## 2. Process Attributes

Each process contains the following attributes:

- Process ID (PID): Identifies the process.
- Arrival Time (AT): Time at which the process becomes available.
- Burst Time (BT): Total CPU time required by the process.
- Remaining Time: CPU time still required.
- Completion Time (CT): Time at which the process finishes.
- Turnaround Time (TAT): Total time between arrival and completion.
- Waiting Time (WT): Total time spent waiting for the CPU.
- Response Time (RT): Time between arrival and first CPU execution.

## 3. Shortest Remaining Time First (SRTF)

### Definition

SRTF is the preemptive version of Shortest Job First scheduling.

At each scheduling decision, the algorithm selects the arrived process
with the smallest remaining execution time.

### Working

1. Initialize each process's remaining time to its burst time.
2. Find the processes that have arrived and are not complete.
3. Select the process with the smallest remaining time.
4. Execute the selected process for one time unit.
5. Check the ready processes again at the next time unit.
6. If a shorter remaining process is available, switch to it.
7. When a process finishes, calculate its completion time, turnaround
   time, and waiting time.
8. Repeat until every process has completed.

If no process is ready, the CPU remains idle until the next arrival.

### Advantages

- Short processes can finish quickly.
- It can reduce average waiting time for suitable workloads.

### Limitations

- Long processes may wait for a long time if short processes keep arriving.
- Frequent preemption requires tracking remaining execution time.

## 4. Round Robin Scheduling

### Definition

Round Robin is a preemptive scheduling algorithm that gives each ready
process a fixed maximum CPU time slice called the time quantum.

### Working

1. Arrange processes by arrival time.
2. Add arrived processes to the ready queue.
3. Remove the process at the front of the queue.
4. Execute it for the smaller of its remaining time and the quantum.
5. Add any newly arrived processes to the ready queue.
6. If the process is unfinished, add it to the back of the queue.
7. If it finishes, record its completion time and calculate its metrics.
8. Repeat until every process has completed.

If the ready queue is empty, the CPU waits until the next process arrives.

### Advantages

- Gives ready processes opportunities to execute in turn.
- Supports time-sharing workloads.
- The quantum can be configured for different experiments.

### Limitations

- A very small quantum can create frequent context switches.
- A very large quantum can make execution resemble FCFS.

## 5. Scheduling Metrics

### Turnaround Time

TAT = CT - AT

Turnaround time measures the total time between process arrival and
completion.

### Waiting Time

WT = TAT - BT

Waiting time measures the total time a process spends waiting for CPU
execution.

### Response Time

RT = First Start Time - AT

Response time measures how long a process waits before receiving CPU
time for the first time.

### Average Waiting Time

Average WT = Sum of all waiting times / Number of processes

### Average Turnaround Time

Average TAT = Sum of all turnaround times / Number of processes

### CPU Utilization

CPU Utilization = Total CPU execution time / Total elapsed time * 100

Idle periods are included in the elapsed time.

## 6. Gantt Chart

The Gantt chart represents the order and duration of CPU execution.

Each segment contains a process ID, start time, and end time. Consecutive
segments for the same process may be merged. Idle intervals are shown
as IDLE.

## 7. Implementation Details

- Language: C
- Compiler: GCC
- Build system: Make
- Process data structures: scheduling.h
- SRTF implementation: srtf.c
- Round Robin implementation: round_robin.c
- CLI and output: main.c

## 8. Testing

The implementation has been tested with:

1. SRTF preemption.
2. Round Robin with a time quantum of 2.
3. SRTF with CPU idle periods.

Additional edge-case and integration tests are still required.

## 9. Conclusion

SRTF chooses the available process with the shortest remaining execution
time, while Round Robin shares CPU time among ready processes using a
configurable quantum.

Both algorithms produce scheduling metrics and a Gantt chart, allowing
their execution behaviour to be studied and compared.
