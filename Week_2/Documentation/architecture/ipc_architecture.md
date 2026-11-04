# IPC Architecture

## Overview

The simulator consists of three independent processes:

- **UI Process** — Handles user interaction and sends commands to Core.
- **Core Process** — Executes commands and manages CPU, Memory, Stack and Queue.
- **Logger Process** — Receives log messages from Core and records them.

The implementation uses three IPC mechanisms:

1. POSIX Message Queues
2. POSIX Shared Memory
3. UNIX Domain Sockets

## System Architecture

```text
                         USER
                           |
                           v
                  +-------------------+
                  |    UI PROCESS     |
                  |  User Interface   |
                  |  Command Input    |
                  +-------------------+
                           |
                           | UI -> Core
                           | /pbl_ui_to_core
                           | POSIX Message Queue
                           v
                  +-------------------+
                  |   CORE PROCESS    |
                  |                   |
                  |  CPU              |
                  |  Memory           |
                  |  Stack            |
                  |  Queue            |
                  +-------------------+
                     |           |
                     |           |
          Core -> UI |           | Core -> Logger
          /pbl_core  |           | /tmp/pbl_logger.sock
          _to_ui     |           | UNIX Domain Socket
          POSIX MQ   |           |
                     v           v
              +----------+  +-------------------+
              |    UI    |  |  LOGGER PROCESS   |
              | RESPONSE |  |                   |
              +----------+  |  Log Processing   |
                            +-------------------+
                                     |
                                     v
                            logs/simulator.log

          +--------------------------------------+
          |          POSIX SHARED MEMORY         |
          |       /pbl_core_shared_memory        |
          |                                      |
          |       Shared Core State              |
          +--------------------------------------+
```
## IPC Mechanisms Used
1. POSIX Message Queues
Two active message queues are used for communication between UI and Core.
- /pbl_ui_to_core — Transfers commands from UI to Core.
- /pbl_core_to_ui — Transfers responses from Core to UI.
2. POSIX Shared Memory
- Shared memory name: /pbl_core_shared_memory
- Allows shared Core data to be accessed by participating processes.
- A process-shared mutex is used to protect shared data.
3. UNIX Domain Sockets
- Socket path: /tmp/pbl_logger.sock
- Used by Core to send log messages to Logger.
- Logger records received messages in logs/simulator.log.
## Conclusion
The simulator uses POSIX Message Queues for UI-Core communication, POSIX Shared Memory for shared data and UNIX Domain Sockets for Core-Logger communication.

These mechanisms demonstrate different approaches to inter-process communication in a multi-process system.
