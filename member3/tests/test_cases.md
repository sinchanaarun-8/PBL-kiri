
# **Member3** Test Cases — Process Scheduling Simulator

## Project Information

- **Developer:** Aahil Sheikh **Member3**
- **Algorithms:** SRTF and Round Robin
- **Language:** C
- **Compiler:** GCC

---

## Test Case 1: SRTF Preemption

### Input

| Process | Arrival Time | Burst Time | Priority |
|---------|--------------|------------|----------|
| P1      |     0        | 8          | 1        |
| P2      |     1        | 4          | 2        |
| P3      |     2        | 2          | 3        |

**Algorithm:** SRTF

### Expected Gantt Chart

0 | P1 | 1 | P2 | 2 | P3 | 4 | P2 | 7 | P1 | 14

### Expected Results

| Process | CT | TAT | WT | RT |
|---------|----|-----|----|----|
| P1      | 14 | 14  | 6  | 0  |
| P2      | 7  | 6   | 2  | 0  |
| P3      | 4  | 2   | 0  | 0  |

- Average Waiting Time: 2.67
- Average Turnaround Time: 7.33

**Status:** PASS

**Observation:** The scheduler preempts the running process when a newly
arrived process has a shorter remaining execution time.

---

## Test Case 2: Round Robin

### Input

| Process | Arrival Time | Burst Time | Priority |
|---------|--------------|------------|----------|
| P1      | 0            | 5          | 2        |
| P2      | 0            | 3          | 2        |
| P3      | 0            | 1          | 3        |

**Algorithm:** Round Robin

**Time Quantum:** 2

### Expected Gantt Chart

0 | P1 | 2 | P2 | 4 | P3 | 5 | P1 | 7 | P2 | 8 | P1 | 9

### Expected Results

| Process | CT | TAT | WT | RT |
|---------|----|-----|----|----|
| P1      | 9  | 9   | 4  | 0  |
| P2      | 8  | 8   | 5  | 2  |
| P3      | 5  | 5   | 4  | 4  |

- Average Waiting Time: 4.33
- Average Turnaround Time: 7.33
- CPU Utilization: 100.00%

**Status:** PASS

**Observation:** Each process receives up to two time units per turn.
Unfinished processes return to the end of the ready queue.

---

## Test Case 3: SRTF with CPU Idle Time

### Input

| Process | Arrival Time | Burst Time | Priority |
|---------|--------------|------------|----------|
| P1      | 3            | 2          | 1        |
| P2      | 8            | 1          | 2        |

**Algorithm:** SRTF

### Expected Gantt Chart

0 | IDLE | 3 | P1 | 5 | IDLE | 8 | P2 | 9

### Expected Results

| Process | CT | TAT | WT | RT |
|---------|----|-----|----|----|
| P1      | 5  | 2   | 0  | 0  |
| P2      | 9  | 1   | 0  | 0  |

- Average Waiting Time: 0.00
- Average Turnaround Time: 1.50
- CPU Utilization: 50.00%

**Status:** PASS

**Observation:** The simulator records idle periods when no process is
available for execution.

---

## Scheduling Metric Formulas

- Turnaround Time (TAT) = Completion Time - Arrival Time
- Waiting Time (WT) = Turnaround Time - Burst Time
- Response Time (RT) = First Start Time - Arrival Time
- Average WT = Sum of Waiting Times / Number of Processes
- Average TAT = Sum of Turnaround Times / Number of Processes
- CPU Utilization = Total CPU Burst Time / Total Elapsed Time * 100

## Testing Summary

| Test |         Algorithm         | Result |
|------|---------------------------|--------|
| 1    | SRTF preemption           | PASS   |
| 2    | Round Robin, quantum 2    | PASS   |
| 3    | SRTF idle time            | PASS   |

These test cases have been executed successfully in the development
environment. Additional edge cases and integration testing remain.


---

## Test Case 4: Round Robin — Time Quantum 1

### Input

Use the same three processes from Test Case 2:

| Process | Arrival Time | Burst Time |
|---------|--------------|------------|
| P1      | 0            | 5          |
| P2      | 0            | 3          |
| P3      | 0            | 1          |

**Time Quantum:** 1

### Results

| Process | CT | TAT | WT | RT |
|---------|----|-----|----|----|
| P1      | 9  | 9   | 4  | 0  |
| P2      | 7  | 7   | 4  | 1  |
| P3      | 3  | 3   | 2  | 2  |

- Average Waiting Time: 3.33
- Average Turnaround Time: 6.33
- CPU Utilization: 100.00%

**Status:** PASS

---

## Test Case 5: Round Robin — Time Quantum 4

### Input

Use the same three processes from Test Case 2.

**Time Quantum:** 4

### Results

| Process | CT | TAT | WT | RT |
|---------|---:|----:|---:|---:|
| P1      | 9  | 9   | 4  | 0  |
| P2      | 7  | 7   | 4  | 4  |
| P3      | 8  | 8   | 7  | 7  |

- Average Waiting Time: 5.00
- Average Turnaround Time: 8.00
- CPU Utilization: 100.00%

**Status:** PASS

**Observation:** Changing the time quantum changes the execution order
and response times. The simulator supports configurable time slices.
