# Member 2 - josna- Process Scheduling Algorithms

## 1. SJF Preemptive (SRTF)

SJF Preemptive selects the process with the shortest remaining burst time.
A running process can be interrupted when a process with a shorter
remaining time becomes available.

### Sample Input

    3
    0 7
    2 4
    4 1

### Expected Results

| Process | CT | TAT | WT | RT |
|---------|---:|----:|---:|---:|
| P1 | 12 | 12 | 5 | 0 |
| P2 | 7 | 5 | 1 | 0 |
| P3 | 5 | 1 | 0 | 0 |

Average WT: 2.00
Average TAT: 6.00
Average RT: 0.00

## 2. Priority Non-preemptive

Priority Non-preemptive selects the highest-priority process among
the processes that have arrived. Once execution begins, the process
continues until completion.

A smaller priority number indicates higher priority.

### Sample Input

    3
    0 7 3
    2 4 1
    4 1 2

### Expected Results

| Process | CT | TAT | WT | RT |
|---------|---:|----:|---:|---:|
| P1 | 7 | 7 | 0 | 0 |
| P2 | 11 | 9 | 5 | 5 |
| P3 | 12 | 8 | 7 | 7 |

Average WT: 4.00
Average TAT: 8.00
Average RT: 4.00

## 3. Performance Metrics

- CT: Completion Time
- TAT: Turnaround Time = CT - AT
- WT: Waiting Time = TAT - BT
- RT: Response Time = First Start Time - AT

## 4. Testing Status

Both programs were compiled and tested with the sample inputs.
The reported metrics match the expected calculations.