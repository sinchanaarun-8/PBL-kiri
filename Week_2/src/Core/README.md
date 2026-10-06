# Member 2 **Aahil** — Core Process

## Responsibility

The **Core Process** is the main execution component of the simulator. It manages the CPU, memory, stack, queue and program execution.

```text
Core
├── CPU
├── Memory
├── Stack
├── Queue
└── Program
```

The `CoreState` structure combines these components into the overall simulator state.

---

## Core Files

| File       | Purpose                                            |
|------------|----------------------------------------------------|
| `core.c`   | Main Core logic and instruction routing            |
| `core.h`   | Core state, commands and function declarations     |
| `cpu.c`    | CPU operations and instruction execution           |
| `cpu.h`    | CPU structure and interface                        |
| `memory.c` | Memory initialization, read and write              |
| `memory.h` | Memory structure and interface                     |
| `stack.c`  | Stack operations                                   |
| `stack.h`  | Stack structure and interface                      |
| `queue.c`  | Queue operations                                   |
| `queue.h`  | Queue structure and interface                      |

---

## 1. Core Module — `core.c` / `core.h`

`core.c` acts as the **central coordinator** of the Core Process.

It is responsible for:
- Initializing the CPU, Memory, Stack and Queue
- Resetting the simulator state
- Loading the default program
- Identifying and routing instructions
- Running and stopping the program
- Processing Core commands
- Displaying the current Core state

### Supported Core Commands

```text
LOAD
RUN
STOP
RESET
EXIT
```

### Instruction Routing

`core_execute()` identifies the instruction and sends it to the appropriate module:

```text
Instruction
     |
     v
core_execute()
     |
     +---- CPU ------> cpu_execute()
     |
     +---- Memory ---> memory_read/write()
     |
     +---- Stack ----> stack_push/pop/peek()
     |
     +---- Queue ----> queue_enqueue/dequeue/peek()
```

---

## 2. CPU — `cpu.c` / `cpu.h`

The CPU maintains three main values:

- **ACC** — Accumulator
- **PC** — Program Counter
- **Active** — CPU execution status

### Supported Instructions

```text
LOAD
ADD
SUB
MUL
DIV
HALT
```

The CPU updates the accumulator according to the instruction and increments the Program Counter after successful execution.

Division by zero is checked and rejected.

---

## 3. Memory — `memory.c` / `memory.h`

The simulator uses a fixed memory model with **100 locations**.

Memory supports:

```text
Initialize
Reset
Write
Read
```

Memory addresses are validated before read or write operations.

---

## 4. Stack — `stack.c` / `stack.h`

The stack has a capacity of **100 elements** and follows **LIFO (Last In, First Out)**.

Operations:

```text
PUSH
POP
PEEK
```

The stack uses a `top` index to track its current position.

It also checks for stack-full and stack-empty conditions.

---

## 5. Queue — `queue.c` / `queue.h`

The queue has a capacity of **100 elements** and follows **FIFO (First In, First Out)**.

It is implemented as a **circular queue**.

Operations:

```text
ENQUEUE
DEQUEUE
PEEK
```

The queue maintains:

```text
front
rear
count
```

It checks for both full and empty conditions.

---

## 6. Program Execution

The Core can load the following default program:

```text
LOAD 10
ADD 20
PUSH 30
POP
HALT
```

The program is stored inside the Core state and executed sequentially.

`core_run()` sends each instruction to `core_execute()`, which routes it to the required subsystem.

---

## 7. Core State

The Core can display important simulator information:

```text
Accumulator
Program Counter
CPU Active Status
Program Size
Stack Items
Queue Items
Memory[0]
```
The Core Process is responsible for **simulator execution and internal state management**. The UI and Logging processes communicate with it through the project's IPC layer.


