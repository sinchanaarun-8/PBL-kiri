# Yeregav Kirikiri

## PBL - Multi-Process Simulator

## Overview

A C-based multi-process simulator developed for Problem Based Learning. The system demonstrates process coordination and Inter-Process Communication (IPC) using POSIX Message Queues, POSIX Shared Memory and UNIX Domain Sockets on a POSIX/Linux environment.

The simulator consists of three main processes:

- **UI Process** — accepts user commands and displays responses.
- **Core Process** — performs CPU, memory, stack and queue operations.
- **Logging Process** — receives and stores execution events.

An independent Shared Memory Reader process demonstrates access to the Core's shared simulator state.

## System Architecture

```text
                         USER
                           |
                           v
                  +-------------------+
                  |    UI PROCESS     |
                  |   Terminal Input  |
                  +-------------------+
                           |
                           | POSIX Message Queues (UI <-> Core)
                           v
                  +-------------------+
                  |   CORE PROCESS    |
                  |                   |
                  |   CPU Execution   |
                  | Memory Management |
                  | Stack Operations  |
                  | Queue Operations  |
                  +-------------------+
                     |           |
                     |           |
          POSIX Shared Memory    | UNIX Domain Socket
                     |           |
                     v           v
             +----------------+  +-------------------+
             | SHARED MEMORY  |  | LOGGING PROCESS   |
             | Live CoreState |  +-------------------+
             +----------------+           |
                     |                     v
                     |              simulator.log
                     |
                     | Attach and read
                     v
             +----------------------+
             | SHM READER PROCESS   |
             | Independent test    |
             +----------------------+
```

## IPC Channels

The project demonstrates three IPC mechanisms:

| IPC Mechanism | Purpose |
|---|---|
| POSIX Message Queues | UI-Core command and response communication |
| POSIX Shared Memory | Shares the live Core simulator state with another process |
| UNIX Domain Sockets | Transfers log messages from Core to Logger |

### Shared Memory Reader

An independent reader process (`core_shm_reader.c`) attaches to shared memory, reads the Core state and displays the update count. This demonstrates inter-process data sharing and synchronization.

## Core Simulation

The Core Process integrates:

- CPU execution
- Memory management
- Stack operations
- Queue operations
- Program loading and execution
- Run, stop and reset operations

## Team Work

| Member | Responsibility |
|---|---|
| Member 1 - Jishnu | UI Process |
| Member 2 - Aahil | Core Process |
| Member 3 - Joshna | Logging Process |
| Member 4 - Sinchana | IPC and system integration - Team Lead |

## Technology

- **Language:** C
- **Platform:** POSIX/Linux
- **IPC:** POSIX Message Queues, POSIX Shared Memory, UNIX Domain Sockets
- **Interface:** Terminal
- **Logging:** File-based logging

## Repository Structure

```text
Week_2/
├── .gitignore
├── Documentation/
│   ├── architecture/
│   ├── benchmarks/
│   ├── ipc/
│   ├── performance/
│   ├── BENCHMARK_RESULTS.md
│   └── Makefile
├── build/
├── logs/
└── src/
    ├── Core/
    ├── IPC/
    ├── logger/
    ├── ui/
    ├── logs/
    ├── README.md
    └── main.c
```
