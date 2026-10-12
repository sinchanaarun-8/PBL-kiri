# Week 3 — Process Scheduling Simulator

## Objective
Develop a C-based simulator to demonstrate CPU scheduling algorithms and compare their performance.

## Required Algorithms
1. First Come First Served (FCFS)
2. Shortest Job First (SJF) — Non-Preemptive
3. Shortest Remaining Time First (SRTF) — Preemptive SJF
4. Priority Scheduling — Non-Preemptive
5. Priority Scheduling — Preemptive
6. Round Robin (RR)

## Process Inputs
- Process ID (PID)
- Arrival Time
- Burst Time
- Priority
- Time Quantum for Round Robin

## Required Outputs
- Gantt chart
- Completion Time
- Turnaround Time
- Waiting Time
- Response Time
- Average Waiting Time
- Average Turnaround Time
- CPU Utilization

## Project Structure
- `src/` — Main program and shared scheduling code
- `member 2/` — Priority scheduling work and related files
- `member3/` — SRTF and Round Robin work
- `tests/` — Test cases and results
- `Documentation/` — Design, algorithm explanations, and performance comparison

## Team Lead Responsibilities
Integrate the algorithms, implement common performance metrics and Gantt chart support, test the combined program, and document the comparison of algorithms.

## Status
FCFS and non-preemptive SJF will be added when their implementation is completed.
