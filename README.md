# Yeregav Kirikiri
# The Problem Based Learning 
## System Architecture

The project uses a three-process architecture implemented in C on a POSIX/Linux environment.

The three independent processes are:

- **UI Process** – Handles user input and displays simulator status and results.
- **Core Process** – Performs the main simulator operations, including CPU execution, memory management, stack operations, and queue operations.
- **Logging Process** – Records execution events, warnings, and errors.

### Architecture Diagram

```text
                         USER
                           |
                           v
                  +-------------------+
                  |    UI PROCESS     |
                  |       (C)         |
                  +-------------------+
                           |
                           | Command Messages
                           | RUN, STOP, RESET,
                           | LOAD_PROGRAM
                           v
                  +-------------------+
                  |   POSIX MESSAGE   |
                  |      QUEUE        |
                  +-------------------+
                           |
                           v
                  +-------------------+
                  |   CORE PROCESS    |
                  |       (C)         |
                  |                   |
                  |   CPU Execution   |
                  | Memory Management |
                  | Stack Operations  |
                  | Queue Operations  |
                  +-------------------+
                           |
                           | Event / Log Messages
                           v
                  +-------------------+
                  | LOGGING PROCESS   |
                  |       (C)         |
                  +-------------------+
                           |
                           v
                  Execution and Error Logs
