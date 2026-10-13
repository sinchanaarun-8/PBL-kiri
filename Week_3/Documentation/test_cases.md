# Process Scheduling Simulator — Test Cases

## 1. Test Environment

- Language: C
- Build system: Makefile
- Test executable: `build/scheduler`
- Algorithms: FCFS, SJF Non-Preemptive, Priority Non-Preemptive, SRTF, Priority Preemptive, and Round Robin.

## 2. Scheduling Test Cases

### Test Case 1: SRTF Preemption

Input:

| PID | Arrival Time | Burst Time | Priority |
|---|---:|---:|---:|
| P1 | 0 | 8 | 2 |
| P2 | 1 | 4 | 1 |
| P3 | 2 | 2 | 3 |

Expected Gantt chart:

`P1 (0-1) | P2 (1-2) | P3 (2-4) | P2 (4-7) | P1 (7-14)`

Expected metrics:

| PID | CT | TAT | WT | RT |
|---|---:|---:|---:|---:|
| P1 | 14 | 14 | 6 | 0 |
| P2 | 7 | 6 | 2 | 0 |
| P3 | 4 | 2 | 0 | 0 |

Average WT: 2.67  
Average TAT: 7.33

Previously documented as PASS.

### Test Case 2: Round Robin, Quantum 2

Input:

| PID | Arrival Time | Burst Time |
|---|---:|---:|
| P1 | 0 | 5 |
| P2 | 0 | 3 |
| P3 | 0 | 1 |

Time quantum: 2

Expected Gantt chart:

`P1 (0-2) | P2 (2-4) | P3 (4-5) | P1 (5-7) | P2 (7-8) | P1 (8-9)`

| PID | CT | TAT | WT | RT |
|---|---:|---:|---:|---:|
| P1 | 9 | 9 | 4 | 0 |
| P2 | 8 | 8 | 5 | 2 |
| P3 | 5 | 5 | 4 | 4 |

Average WT: 4.33  
Average TAT: 7.33

Previously documented as PASS.

### Test Case 3: SRTF with Idle Time

Input:

| PID | Arrival Time | Burst Time |
|---|---:|---:|
| P1 | 3 | 2 |
| P2 | 8 | 1 |

Expected Gantt chart:

`IDLE (0-3) | P1 (3-5) | IDLE (5-8) | P2 (8-9)`

| PID | CT | TAT | WT | RT |
|---|---:|---:|---:|---:|
| P1 | 5 | 2 | 0 | 0 |
| P2 | 9 | 1 | 0 | 0 |

Average WT: 0.00  
Average TAT: 1.50  
CPU busy time: 3  
Total elapsed time: 9  
CPU utilization: 33.33%

Note: The earlier individual test document stated 50% utilization. That value is incorrect when elapsed time is measured from time 0; the corrected value is 3 / 9 × 100 = 33.33%.

### Test Case 4: Round Robin, Quantum 1

Use the processes from Test Case 2 with time quantum 1.

| PID | CT | TAT | WT | RT |
|---|---:|---:|---:|---:|
| P1 | 9 | 9 | 4 | 0 |
| P2 | 7 | 7 | 4 | 1 |
| P3 | 3 | 3 | 2 | 2 |

Average WT: 3.33  
Average TAT: 6.33

Previously documented as PASS.

### Test Case 5: Round Robin, Quantum 4

Use the processes from Test Case 2 with time quantum 4.

| PID | CT | TAT | WT | RT |
|---|---:|---:|---:|---:|
| P1 | 9 | 9 | 4 | 0 |
| P2 | 7 | 7 | 4 | 4 |
| P3 | 8 | 8 | 7 | 7 |

Average WT: 5.00  
Average TAT: 8.00

Previously documented as PASS.

## 3. Integration and Input Validation

The integrated simulator has also been tested for the following cases:

| Test | Expected behaviour | Recorded status |
|---|---|---|
| Run FCFS | Display Gantt chart and metrics | Tested |
| Run SJF Non-Preemptive | Display Gantt chart and metrics | Tested |
| Run Priority Non-Preemptive | Display Gantt chart and metrics | Tested |
| Run SRTF | Display Gantt chart and metrics | Tested |
| Run Priority Preemptive | Display Gantt chart and metrics | Tested |
| Run Round Robin | Display Gantt chart and metrics | Tested |
| Compare All | Run all six algorithms | Tested |
| One process | Produce correct completion and waiting times | Tested |
| Initial/intermediate idle time | Show idle intervals and correct utilization | Tested |
| Process count 0 or above the limit | Reject input | Tested |
| Negative arrival time | Reject input | Tested |
| Zero or negative burst time | Reject input | Tested |
| Zero Round Robin quantum | Reject input | Tested |
| Invalid menu option | Display an error and show the menu again | Tested |

## 4. Metric Formulas

- Turnaround Time (TAT) = Completion Time (CT) - Arrival Time (AT).
- Waiting Time (WT) = TAT - Burst Time (BT).
- Response Time (RT) = First CPU Start Time - AT.
- Average WT = Sum of WT / Number of Processes.
- Average TAT = Sum of TAT / Number of Processes.
- CPU Utilization = (CPU Busy Time / Total Elapsed Time) × 100.

Total elapsed time is measured from time 0 to the final process completion, including idle intervals.

## 5. Notes

The PASS statuses above refer to results previously recorded during development. Re-run the test cases after code changes to confirm the results. Additional boundary and tie-breaking cases can be added as needed.
