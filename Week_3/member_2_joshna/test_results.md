# Member 2 - Josna - Process Scheduling Algorithms

## 1. Priority Non-preemptive Scheduling

Priority Non-preemptive Scheduling selects the highest-priority process among the processes that have arrived. Once execution begins, the process continues until completion.

A smaller priority number indicates higher priority.

### Sample Input

```text
3
0 7 3
2 4 1
4 1 2
```

### Expected Results

| Process | CT | TAT | WT | RT |
|---|---:|---:|---:|---:|
| P1 | 7 | 7 | 0 | 0 |
| P2 | 11 | 9 | 5 | 5 |
| P3 | 12 | 8 | 7 | 7 |

**Average Times**

- Average WT: 4.00
- Average TAT: 8.00
- Average RT: 4.00

## 2. Priority Preemptive Scheduling

Priority Preemptive Scheduling selects the highest-priority process among the processes that have arrived. If a new process arrives with a higher priority, the currently running process is interrupted.

A smaller priority number indicates higher priority.

### Sample Input

```text
3
0 7 3
2 4 1
4 1 2
```

### Expected Results

| Process | CT | TAT | WT | RT |
|---|---:|---:|---:|---:|
| P1 | 12 | 12 | 5 | 0 |
| P2 | 6 | 4 | 0 | 0 |
| P3 | 7 | 3 | 2 | 2 |

**Average Times**

- Average WT: 2.33
- Average TAT: 6.33
- Average RT: 0.67

## 3. Performance Metrics

- **AT:** Arrival Time
- **BT:** Burst Time
- **PR:** Priority
- **CT:** Completion Time
- **TAT:** Turnaround Time = CT − AT
- **WT:** Waiting Time = TAT − BT
- **RT:** Response Time = First Start Time − AT

## Conclusion

Priority Non-preemptive Scheduling allows a process to finish once execution starts, whereas Priority Preemptive Scheduling can interrupt a running process when a higher-priority process arrives. Both algorithms calculate completion time, turnaround time, waiting time, and response time.