# IPC Design and Justification

## Overview

The simulator consists of three independent processes: UI, Core and Logger.

The implementation uses three IPC mechanisms:

1. POSIX Message Queues
2. POSIX Shared Memory
3. UNIX Domain Sockets

Each mechanism is selected according to its communication purpose.

## 1. POSIX Message Queues

POSIX Message Queues are used for communication between UI and Core.

Two active message queues are implemented:

| Queue | Direction | Purpose |
|---|---|---|
| /pbl_ui_to_core | UI → Core | Sends commands from UI to Core |
| /pbl_core_to_ui | Core → UI | Sends responses from Core to UI |

### Why POSIX Message Queues?

- Support communication between independent processes.
- Provide structured message-based communication.
- Allow blocking send and receive operations.
- Separate command and response communication.

The IPC message structure contains fields for message type, command, status and text.

## 2. POSIX Shared Memory

The simulator uses POSIX Shared Memory for shared Core state.

- Shared memory name: /pbl_core_shared_memory
- Allows participating processes to access shared data.
- A process-shared mutex protects shared data from simultaneous access.

### Why Shared Memory?

- Provides a shared memory region between processes.
- Avoids repeatedly sending shared data through messages.
- Supports efficient access to shared state.
- Requires synchronization to prevent conflicting access.

## 3. UNIX Domain Sockets

UNIX Domain Sockets are used for communication from Core to Logger.

- Socket path: /tmp/pbl_logger.sock
- Core sends log messages to Logger.
- Logger receives the messages and records them in logs/simulator.log.

### Why UNIX Domain Sockets?

- Support communication between local processes.
- Provide reliable two-way socket communication.
- Are suitable for transferring log messages between Core and Logger.
- Avoid requiring network communication.

## Alternatives Considered

### Pipes
Pipes provide simple communication between processes but are stream-oriented and require additional handling for structured messages.

### FIFOs
FIFOs allow unrelated processes to communicate but do not provide the same message-oriented structure as POSIX Message Queues.

### Signals
Signals are useful for notifications and process control but are unsuitable for transferring structured commands and log messages.

## Communication Flow

```text
                  +------------------+
                  |        UI        |
                  +------------------+
                           |
                           | POSIX Message Queue
                           | /pbl_ui_to_core
                           v
                  +------------------+
                  |       CORE       |
                  +------------------+
                     |            |
                     |            |
       POSIX Message Queue     UNIX Domain Socket
       /pbl_core_to_ui         /tmp/pbl_logger.sock
                     |            |
                     v            v
                  +------+  +------------------+
                  |  UI  |  |      LOGGER      |
                  |Reply |  +------------------+
                  +------+           |
                                     v
                            logs/simulator.log

             POSIX SHARED MEMORY
             /pbl_core_shared_memory
             Shared Core State
```
## Communication Flow Details
1. UI → Core: UI sends commands through /pbl_ui_to_core.
2. Core → UI: Core sends command responses through /pbl_core_to_ui.
3. Core shared state: POSIX Shared Memory provides access to shared Core data, protected by a process-shared mutex.
4. Core → Logger: Core sends log messages through a UNIX Domain Socket. Logger records them in the simulator log.
## Conclusion
The simulator demonstrates three IPC mechanisms for different purposes. POSIX Message Queues handle UI-Core command and response communication, POSIX Shared Memory provides shared data access, and UNIX Domain Sockets transfer log messages from Core to Logger.

Using these mechanisms demonstrates different approaches to communication and data sharing between independent processes.
