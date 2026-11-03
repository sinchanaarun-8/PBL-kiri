# Jishnu's Process Scheduling Algorithms

## Overview
This folder contains two CPU scheduling algorithms implemented in C.

## 1. FCFS
First Come, First Served executes processes in arrival order.

## 2. SJF Non-Preemptive
Selects the available process with the shortest burst time. Once a process starts, it runs until completion.

## Input
Each process requires:
- Process ID (PID)
- Arrival Time (AT)
- Burst Time (BT)

## Metrics
- Turnaround Time = Completion Time - Arrival Time
- Waiting Time = Turnaround Time - Burst Time
- Response Time = Start Time - Arrival Time
- Average Waiting Time = Total Waiting Time / Number of Processes
- Average Turnaround Time = Total Turnaround Time / Number of Processes

## Compile and Run

### FCFS
    gcc -Wall -Wextra -std=c11 fcfs.c -o fcfs
    ./fcfs

### SJF Non-Preemptive
    gcc -Wall -Wextra -std=c11 sjf_non_preemptive.c -o sjf_non_preemptive
    ./sjf_non_preemptive

## Sample Test
    3
    1 0 5
    2 1 3
    3 2 2

## Test Results

FCFS:
- Execution order: P1 -> P2 -> P3
- Average Waiting Time: 3.33
- Average Turnaround Time: 6.67

SJF Non-Preemptive:
- Execution order: P1 -> P3 -> P2
- Average Waiting Time: 3.00
- Average Turnaround Time: 6.33

## Status
Both programs compile successfully with GCC and have been tested with the sample input.
