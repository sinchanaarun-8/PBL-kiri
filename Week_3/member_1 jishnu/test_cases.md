Test Cases – SJF Scheduling Algorithms

1. SJF Non-Preemptive

Test Case 1: Basic Scheduling

Input

3
1 0 5
2 1 3
3 2 2

Expected Execution Order: P1 → P3 → P2

PID| AT| BT| CT| TAT| WT| RT
P1| 0| 5| 5| 5| 0| 0
P3| 2| 2| 7| 5| 3| 3
P2| 1| 3| 10| 9| 6| 6

Average Waiting Time: 3.00

Average Turnaround Time: 6.33

Test Case 2: Different Arrival Times

Input

3
1 0 3
2 5 2
3 6 1

Expected Execution Order: P1 → P2 → P3

Average Waiting Time: 0.00

Average Turnaround Time: 2.00

Test Case 3: All Processes Arrive Together

Input

3
1 0 6
2 0 2
3 0 4

Expected Execution Order: P2 → P3 → P1

Average Waiting Time: 2.67

Average Turnaround Time: 6.67

---

2. SJF Preemptive (SRTF)

Test Case 1: Preemption Occurs

Input

3
0 7
2 4
4 1

Expected Gantt Chart: P1 (0–2) → P2 (2–4) → P3 (4–5) → P2 (5–7) → P1 (7–12)

PID| AT| BT| CT| TAT| WT| RT
P1| 0| 7| 12| 12| 5| 0
P2| 2| 4| 7| 5| 1| 0
P3| 4| 1| 5| 1| 0| 0

Average Waiting Time: 2.00

Average Turnaround Time: 6.00

Average Response Time: 0.00

Test Case 2: CPU Idle Time

Input

3
2 3
7 2
10 1

Expected Gantt Chart: IDLE (0–2) → P1 (2–5) → IDLE (5–7) → P2 (7–9) → IDLE (9–10) → P3 (10–11)

Average Waiting Time: 0.00

Average Turnaround Time: 2.00

Average Response Time: 0.00

Test Case 3: All Processes Arrive Together

Input

3
0 6
0 2
0 4

Expected Execution Order: P2 → P3 → P1

Average Waiting Time: 2.67

Average Turnaround Time: 6.67

Average Response Time: 2.67

---

3. Invalid Input Tests

Test| Input| Expected Result
Zero processes| "0"| Display invalid number of processes
Negative arrival time| "-1 5"| Display invalid input
Zero burst time| "0 0"| Display invalid input
Negative burst time| "0 -3"| Display invalid input

Conclusion

These test cases check normal scheduling, process arrival differences, preemption, CPU idle time, and invalid inputs for both SJF scheduling algorithms.
