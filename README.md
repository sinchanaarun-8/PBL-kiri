# Yeregav Kirikiri 
--------------------------------------------------------------------------------------------------------------
## PBL - Multi-Process Simulator

## Overview

 A C-based multi-process simulator developed for Problem Based Learning. The system demonstrates process coordination and Inter-Process Communication (IPC) using **POSIX Message Queues** on a POSIX/Linux environment.

The simulator is divided into three independent processes:

- **UI Process** — accepts user commands and displays responses.
- **Core Process** — performs CPU, memory, stack and queue operations.
- **Logging Process** — receives and stores execution events.

A separate IPC layer connects these processes.

## System Architecture

```text
                         USER
                           |
                           v
                  +-------------------+
                  |    UI PROCESS     |
                  |        (C)        |
                  |  Terminal Input   |
                  +-------------------+
                           |
                           | UI → Core
                           | POSIX MQ
                           v
                  +-------------------+
                  |   CORE PROCESS    |
                  |        (C)        |
                  |                   |
                  |   CPU Execution   |
                  | Memory Management |
                  | Stack Operations  |
                  | Queue Operations  |
                  +-------------------+
                       |           |
                       |           |
              Core → UI|           |Core → Logger
               POSIX MQ|           |POSIX MQ
                       |           |
                       v           v
                  +----------+   +-------------------+
                  |   UI     |   | LOGGING PROCESS   |
                  | RESPONSE |   |        (C)        |
                  +----------+   +-------------------+
                                      |
                                      v
                              +--------------------+
                              | logs/simulator.log |
                              +--------------------+

---------------------------------------------------------------------------------------------------------

## IPC Channels
The project uses three POSIX message queues:

|       Queue           |              Purpose                  |
|-----------------------|---------------------------------------|
|                       | UI sends commands to Core             |
|                       | Core sends responses to UI            |
|                       | Core sends log messages to Logger     |



## Core Simulation

The Core Process integrates:

- CPU execution
- Memory management
- Stack operations
- Queue operations
- Program loading and execution
- Run, stop and reset operations


## Team Work

| Member             | Responsibility             |
|--------------------|----------------------------|
| Member 1 - Jishnu  | UI Process                 |
| Member 2 - Aahil   | Core Process               |
| Member 3 - Joshna  | Logging Process            |
| Member 4 - Sinchana| IPC and system integration | - Team Lead 

## Technology

- **Language:** C
- **Platform:** POSIX/Linux
- **IPC:** POSIX Message Queues
- **Interface:** Terminal
- **Logging:** File-based logging

## Repository Structure

PBL-/
├── README.md
└── week2/
    ├── ui/
    │   
    ├── core/
    │    
    ├── logger/
    │   
    └── ipc/
