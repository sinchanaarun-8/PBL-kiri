# CPU Scheduling Algorithms — Explanation

## 1. Introduction

CPU scheduling determines which ready process gets CPU time. This simulator implements six scheduling algorithms and reports a Gantt chart and performance metrics for each selected algorithm.

## 2. First Come, First Served (FCFS)

FCFS executes processes in order of arrival. Once a process starts, it continues until its burst time is completed.

- Type: Non-preemptive
- Selection rule: Earliest arrival first
- Advantage: Simple and easy to implement
- Limitation: A long process can make shorter processes wait.

## 3. Shortest Job First (SJF) — Non-Preemptive

SJF selects the arrived process with the smallest burst time. The selected process runs until completion.

- Type: Non-preemptive
- Selection rule: Smallest burst time among arrived processes
- Advantage: Can reduce average waiting time.
- Limitation: Long processes may wait for a long time.

## 4. Priority Scheduling — Non-Preemptive

The scheduler selects the arrived process with the highest priority. In this simulator, a smaller priority number means higher priority. The selected process runs until completion.

- Type: Non-preemptive
- Selection rule: Highest priority among arrived processes
- Advantage: Important processes can be executed earlier.
- Limitation: Low-priority processes may experience starvation.

## 5. Shortest Remaining Time First (SRTF)

SRTF is the preemptive version of SJF. The scheduler selects the arrived process with the shortest remaining execution time. A newly arrived process may preempt the current process if it has a shorter remaining time.

- Type: Preemptive
- Selection rule: Smallest remaining burst time
- Advantage: Can reduce average waiting time.
- Limitation: Frequent preemption adds scheduling overhead.

## 6. Priority Scheduling — Preemptive

The scheduler runs the arrived process with the highest priority. If a higher-priority process arrives, it may preempt the currently running process.

- Type: Preemptive
- Selection rule: Highest priority among arrived processes
- Priority convention: Smaller number means higher priority
- Advantage: Urgent processes can run sooner.
- Limitation: Low-priority processes may experience starvation.

## 7. Round Robin (RR)

Round Robin gives each ready process a time slice called the time quantum. If a process is not finished when its quantum expires, it returns to the ready queue and waits for another turn.

- Type: Preemptive
- Selection rule: Ready-queue order
- Advantage: Gives processes repeated opportunities to use the CPU.
- Limitation: A very small quantum can cause frequent context switching.

The user must enter a positive time quantum.

## 8. Preemptive and Non-Preemptive Scheduling

- Non-preemptive scheduling allows a running process to continue until it finishes.
- Preemptive scheduling allows the operating system's scheduler to interrupt a running process and select another eligible process.

## 9. Performance Metrics

- Completion Time (CT): Time when a process finishes.
- Turnaround Time (TAT) = CT - Arrival Time (AT).
- Waiting Time (WT) = TAT - Burst Time (BT).
- Response Time (RT) = First CPU Start Time - AT.
- Average Waiting Time = Sum of WT / Number of Processes.
- Average Turnaround Time = Sum of TAT / Number of Processes.
- CPU Utilization = (CPU Busy Time / Total Elapsed Time) × 100.

The elapsed time is measured from time 0 to the final process completion, including any initial or intermediate idle periods.

## 10. Comparing Algorithms

The simulator's Compare All option runs all six algorithms for the same process inputs. Round Robin additionally asks for a time quantum. The resulting Gantt charts and metrics allow users to compare scheduling behaviour.

Actual performance results should be recorded from program executions rather than assumed in advance.
