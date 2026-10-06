# Member 3 — **Josna** — Logger Process

## Responsibility

Member 3 is responsible for the Logging Process of the PBL Process Simulator.

The Logger is an independent process whose main responsibility is to receive log information and record it into a log file.

The Logger does not directly access Core variables such as CPU, memory, stack, or queue.

---

## Logger Responsibilities

The Logger is responsible for:

- Initializing the logging system.
- Opening/creating the log file.
- Recording INFO messages.
- Recording WARNING messages.
- Recording ERROR messages.
- Receiving log events from the communication mechanism.
- Writing received events into the log file.
- Handling non-log IPC messages.
- Closing the log file properly.
- Cleaning up resources when the Logger terminates.

---

## Logger Architecture

The implemented communication flow is:

```text
Core Process
     |
     | Log Message
     v
POSIX Message Queue
/pbl_core_to_logger
     |
     v
Logger Process
     |
     v
simulator.log
```

The Logger does not directly access Core memory or Core variables.

---

## Files

### logger.h

Contains the public Logger function declarations.

Current functions include:

- `logger_init()` — Initializes the Logger.
- `log_info()` — Records an INFO message.
- `log_warning()` — Records a WARNING message.
- `log_error()` — Records an ERROR message.
- `logger_close()` — Closes the Logger and releases resources.

### logger.c

Contains the implementation of the logging functions and log file handling.

It is responsible for:

- Creating/opening the log file.
- Writing INFO messages.
- Writing WARNING messages.
- Writing ERROR messages.
- Flushing log output.
- Closing the log file.

### logger_process.c

Contains the process-level Logger functionality.

It runs the Logger as an independent process and:

- Initializes the Logger.
- Opens the Core-to-Logger POSIX Message Queue.
- Waits for IPC messages.
- Processes log messages received from Core.
- Handles the Logger exit message.
- Closes the IPC queue.
- Closes the log file.
- Terminates the Logger process cleanly.

---

## Log Levels

The Logger supports three basic log levels:

### INFO

Used for normal program events.

Example:

```text
[INFO] Instruction executed: LOAD
```

### WARNING

Used for events that require attention but do not necessarily stop the simulator.

Example:

```text
[WARNING] Stack is almost full
```

### ERROR

Used when an error occurs.

Example:

```text
[ERROR] Invalid instruction: XYZ
```

---

## Example Log Output

```text
[INFO] Logger started
[INFO] Instruction executed: LOAD
[INFO] Instruction executed: ADD
[WARNING] Stack is almost full
[ERROR] Invalid instruction: XYZ
[INFO] Program completed
```

The examples above illustrate the supported log levels. The actual integrated log output depends on the messages sent by the Core process.

---

## IPC Communication

The Logger communicates with the Core using POSIX Message Queues.

The queue used by the Logger is:

```text
/pbl_core_to_logger
```

The Core sends log messages to this queue, and the Logger receives them.

The Logger processes messages of the appropriate log-message type and records the received information in the log file.

---

## Development Status

### Completed

- Created Logger component directory.
- Created `logger.c`.
- Created `logger.h`.
- Created `logger_process.c`.
- Implemented Logger initialization.
- Implemented log file creation/opening.
- Implemented INFO logging.
- Implemented WARNING logging.
- Implemented ERROR logging.
- Implemented Logger cleanup.
- Implemented Logger as an independent process.
- Implemented POSIX Message Queue communication.
- Implemented Core → Logger communication.
- Implemented receiving log messages from the Core process.
- Implemented handling of non-log IPC messages.
- Implemented clean Logger termination.
- Verified Core → Logger communication during integrated execution.
- Verified log file generation.

### Remaining / Not Separately Verified

- Detailed process failure and recovery handling has not been separately implemented or verified.
- Additional Logger-specific stress testing has not been separately performed.

---

## Verified Integrated Execution

The Logger was verified as part of the integrated three-process simulator.

During the integrated execution, the Logger successfully received Core messages corresponding to the tested commands.

Example:

```text
Core received command: 1
Logger received: Core received command: 1
```

The Logger also received the exit-related message:

```text
Logger received: Core received command: 5
Logger process stopped.
```

The simulator log file contained Core command log entries, confirming that the Core-to-Logger communication and log recording were functioning during the integrated execution.

---

## Architecture Rule

The Logger must remain independent from the Core.

The following is NOT allowed:

```text
Core Memory → Logger
```

The correct architecture is:

```text
Core
  |
  | Message
  v
POSIX Message Queue
  |
  v
Logger
  |
  v
Log File
```

The Logger only receives information through IPC and records it.

It does not directly access:

- CPU state
- Memory state
- Stack state
- Queue state
- Core program state

---

## Conclusion

The Logger Process has been implemented as an independent process and integrated with the Core using POSIX Message Queues.

The Logger successfully receives Core log messages, records them in the simulator log file, and terminates cleanly when the simulator exits.

This maintains the required separation between the Core execution logic and the Logging Process.