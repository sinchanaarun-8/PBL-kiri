# Week 2 — Team Lead

## Role

Team Lead responsible for integrating the UI, Core, and Logger processes and coordinating communication between them using POSIX message queues.

## Responsibilities Completed

- Integrated the UI, Core, and Logger processes.
- Defined the IPC message structure and queue names.
- Implemented POSIX message queue communication between processes.
- Connected UI → Core communication.
- Connected Core → UI response communication.
- Connected Core → Logger communication.
- Added process coordination through `main.c`.
- Added queue cleanup when the Core process exits.
- Compiled and tested the individual processes during integration.


## IPC Design

POSIX message queues are used for communication between the independent processes.

### Message Flow

```text
UI  ────────>  Core
UI  <────────  Core
Core ───────>  Logger


## Process Coordination

The Team Lead integration uses `main.c` to coordinate the three independent processes.

The startup order is:

1. Start the Core process.
2. Wait briefly for the Core IPC queues to become available.
3. Start the Logger process.
4. Start the UI process.
5. Wait for the child processes to finish.
6. Display the simulator shutdown message.

The Core process creates the required message queues, while the UI and Logger processes open the queues they need.

The processes are started using `fork()` and `execl()`, and `waitpid()` is used to wait for their completion.


## Testing

The integration was tested by compiling and running the individual processes and then testing their communication.

The following components were verified:

- Core process compilation and execution.
- Logger process compilation and execution.
- UI process compilation and execution.
- UI → Core command delivery.
- Core → UI response delivery.
- Core → Logger message delivery.
- EXIT command and process shutdown.
- POSIX message queue cleanup after Core exits.

The processes were also tested together through the Team Lead launcher.


## Build and Run

The project uses GCC to compile the individual processes. POSIX message queue support is enabled using the `-lrt` linker option.

The main components are:

- `src/main.c` — launches and coordinates the processes.
- `src/Core/core_process.c` — handles commands received from the UI.
- `src/logger/logger_process.c` — receives and records log messages.
- `src/ui/ui.c` — provides the user interface.
- `src/IPC/ipc.c` and `src/IPC/ipc.h` — implement IPC communication.

The processes were compiled individually and tested for communication during integration.

The Team Lead launcher coordinates the startup and shutdown of the processes.



## Integration Notes

During integration, the processes had to be started in the correct order because the Core process creates the POSIX message queues.

The Core process is started first, followed by the Logger and UI processes.

The integration was tested in WSL Ubuntu using GCC and POSIX message queues.

The project was tested with commands including LOAD, RUN, STOP, RESET, and EXIT.



## Key Files

- `src/main.c` — Team Lead process launcher and coordinator.
- `src/IPC/ipc.c` — POSIX message queue implementation.
- `src/IPC/ipc.h` — IPC definitions, queue names, and message structure.
- `src/Core/core_process.c` — Core process and UI command handling.
- `src/logger/logger_process.c` — Logger process and Core-to-Logger communication.
- `README_TeamLead.md` — documentation for the Team Lead integration work.
