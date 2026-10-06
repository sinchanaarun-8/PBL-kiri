# Week 2 — Team Lead

## Role

Team Lead responsible for integrating the UI, Core, and Logger processes and coordinating communication between them using POSIX Message Queues.

## Responsibilities Completed

* Integrated the UI, Core, and Logger processes.
* Defined the IPC message structure and queue names.
* Implemented POSIX Message Queue communication between processes.
* Connected UI → Core communication.
* Connected Core → UI response communication.
* Connected Core → Logger communication.
* Added process coordination through `main.c`.
* Added queue cleanup when the Core process exits.
* Compiled and tested the processes during integration.

---

## IPC Design

POSIX Message Queues are used for communication between the independent processes.

### Message Flow

```text
UI  ────────>  Core
UI  <────────  Core
Core ───────>  Logger
```

### IPC Queue Names

| Communication | POSIX Message Queue   |
| ------------- | --------------------- |
| UI → Core     | `/pbl_ui_to_core`     |
| Core → UI     | `/pbl_core_to_ui`     |
| Core → Logger | `/pbl_core_to_logger` |

---

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

---

## Testing

The integration was tested by compiling and running the processes and verifying their communication.

The following components were verified:

* Core process compilation and execution.
* Logger process compilation and execution.
* UI process compilation and execution.
* UI → Core command delivery.
* Core → UI response delivery.
* Core → Logger message delivery.
* Logger message reception.
* Log file generation.
* EXIT command and process shutdown.
* POSIX Message Queue cleanup after Core exits.

The processes were also tested together through the Team Lead launcher.

### Verified Commands

The following commands were successfully tested during integrated execution:

| Command | Operation | Status |
| ------- | --------- | ------ |
| `1`     | LOAD      | PASS   |
| `2`     | RUN       | PASS   |
| `3`     | STOP      | PASS   |
| `5`     | EXIT      | PASS   |

---

## Build and Run

The project uses GCC to compile the C source files. POSIX Message Queue support is enabled using the `-lrt` linker option where required.

The main components are:

* `Week_2/src/main.c` — launches and coordinates the processes.
* `Week_2/src/Core/core_process.c` — handles commands received from the UI.
* `Week_2/src/logger/logger_process.c` — receives and records log messages.
* `Week_2/src/ui/ui.c` — provides the user interface.
* `Week_2/src/IPC/ipc.c` — implements IPC communication.
* `Week_2/src/IPC/ipc.h` — contains IPC definitions, queue names, and message structures.

The Team Lead launcher coordinates the startup and shutdown of the processes.

---

## Integration Notes

During integration, the processes had to be started in the correct order because the Core process creates the POSIX Message Queues.

The Core process is started first, followed by the Logger and UI processes.

The integration was tested in WSL Ubuntu using GCC and POSIX Message Queues.

The integrated system successfully processed the LOAD, RUN, STOP, and EXIT commands.

---

## Key Files

* `Week_2/src/main.c` — Team Lead process launcher and coordinator.
* `Week_2/src/IPC/ipc.c` — POSIX Message Queue implementation.
* `Week_2/src/IPC/ipc.h` — IPC definitions, queue names, and message structure.
* `Week_2/src/Core/core_process.c` — Core process and UI command handling.
* `Week_2/src/logger/logger_process.c` — Logger process and Core-to-Logger communication.
* `Week_2/src/ui/ui.c` — User interface and UI-to-Core communication.
* `Week_2/src/IPC/README_TeamLead.md` — documentation for the Team Lead integration work.

