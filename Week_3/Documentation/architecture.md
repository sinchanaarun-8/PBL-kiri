# Process Scheduling Simulator — Architecture

## 1. Overview

The Process Scheduling Simulator is a C-based program that demonstrates six CPU scheduling algorithms. Users enter process information and select an algorithm to view its execution order and performance metrics.

## 2. System Inputs

For each process, the user enters:
- Process ID (PID): unique positive integer.
- Arrival Time (AT): non-negative time when the process becomes available.
- Burst Time (BT): positive CPU execution time.
- Priority: non-negative integer; a smaller number represents a higher priority.

Round Robin additionally requires a positive time quantum.

## 3. Project Structure

- `src/main.c` — Reads and validates inputs, displays the menu, invokes algorithms, and prints results.
- `src/basic_scheduling.c` — Contains FCFS, SJF Non-Preemptive, and Priority Non-Preemptive implementations.
- `src/srtf.c` — Contains SJF Preemptive (SRTF) and Priority Preemptive implementations.
- `src/round_robin.c` — Contains the Round Robin implementation.
- `include/scheduling.h` — Defines shared process data structures, Gantt chart structures, constants, and function declarations.
- `tests/test_cases.md` — Documents scheduling test cases.
- `docs/algorithm_explanation.md` — Contains algorithm explanations.
- `Makefile` — Automates compilation of the simulator.

## 4. Execution Flow

1. The program starts and asks for the number of processes.
2. The user enters each process's PID, arrival time, burst time, and priority.
3. The program validates the inputs and rejects invalid or duplicate process IDs.
4. The menu allows the user to select one algorithm, compare all algorithms, or exit.
5. The selected scheduling function calculates the execution order and process metrics.
6. The program displays the Gantt chart, process metrics, averages, CPU busy time, CPU idle time, and CPU utilization.
7. The menu is displayed again until the user chooses Exit.

## 5. Supported Algorithms

1. First Come, First Served (FCFS)
2. Shortest Job First (SJF), Non-Preemptive
3. Priority Scheduling, Non-Preemptive
4. Shortest Remaining Time First (SRTF), Preemptive
5. Priority Scheduling, Preemptive
6. Round Robin (RR)

The Compare All option runs all six algorithms using the same process inputs. The user supplies the time quantum when Round Robin is run.

## 6. Performance Metrics

- Completion Time (CT): time when a process finishes.
- Turnaround Time (TAT) = CT - AT.
- Waiting Time (WT) = TAT - BT.
- Response Time (RT) = First CPU Start Time - AT.
- Average WT = sum of all waiting times / number of processes.
- Average TAT = sum of all turnaround times / number of processes.
- CPU Utilization = (CPU Busy Time / Total Elapsed Time) × 100.

The elapsed time is measured from time 0 until the final process completes. Idle intervals are included in elapsed time.

## 7. Build and Run

From the `Week_3/member_3 aahil` directory:

```bash
make
./build/scheduler

```
