
---

## Evaluation Quick Reference — Aahil's Module

### 1. Open the project directory

```bash
cd "/mnt/c/Aahil Projects/PBL-kiri/member3"
```

If the terminal is already inside `member3`, skip this command.

### 2. Compile the simulator

```bash
make clean
make
```

A successful build should create:

`build/scheduler`

### 3. Run the simulator

```bash
./build/scheduler
```

### 4. Select an algorithm

- Enter the number of processes.
- Enter each process's arrival time, burst time, and priority.
- Choose `1` for SRTF.
- Choose `2` for Round Robin.
- For Round Robin, enter a positive time quantum.

Priority is collected as part of the common process input, but it is not used by SRTF or Round Robin.

### 5. Run using Make

The following command compiles the program if needed and starts it:

```bash
make run
```

### 6. Clean the executable

```bash
make clean
```

This removes `build/scheduler`. It does not remove the source files.

### 7. Important formulas

- Turnaround Time = Completion Time - Arrival Time
- Waiting Time = Turnaround Time - Burst Time
- Response Time = First Start Time - Arrival Time
- Average Waiting Time = Total Waiting Time / Number of Processes
- Average Turnaround Time = Total Turnaround Time / Number of Processes
- CPU Utilization = Total Burst Time / Elapsed Time * 100

### 8. Test cases

Detailed test inputs and expected results are documented in:

`tests/test_cases.md`

Algorithm explanations are documented in:

`docs/algorithm_explanation.md`

### 9. Quick viva revision

**What is SRTF?**

SRTF is the preemptive version of SJF. It selects the arrived process
with the shortest remaining execution time.

**What is Round Robin?**

Round Robin gives each ready process a turn of up to a configurable
time quantum. An unfinished process returns to the back of the queue.

**What is preemption?**

Preemption occurs when the CPU stops executing a process before it
finishes so that another process can execute.

**Why track remaining time?**

Remaining time records how much CPU execution a process still needs.

**What is response time?**

Response time is the interval between process arrival and its first
CPU execution.

**Why is a Gantt chart useful?**

It shows the execution order, timing, preemption, and idle intervals.
