## Member 3 **josna** -  Logger Process


## Responsibility

Member 3 is responsible for the Logging Process of the PBL Process Simulator.

The Logger is an independent process whose main responsibility is to receive
log information and record it into a log file.

The Logger does not directly access Core variables such as CPU, memory, stack,
or queue.

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
- Handling invalid messages.
- Closing the log file properly.
- Cleaning up resources when the Logger terminates.

---

## Logger Architecture

The planned communication flow is:

Core Process
     |
     | Log Message
     v
POSIX Message Queue
     |
     v
Logger Process
     |
     v
Log File

The Logger does not directly access Core memory or Core variables.

---

## Files

### logger.h

Contains the public Logger function declarations.

Current functions include:

- `logger_init()` - Initializes the Logger.
- `log_info()` - Records an INFO message.
- `log_warning()` - Records a WARNING message.
- `log_error()` - Records an ERROR message.
- `logger_close()` - Closes the Logger and releases resources.

### logger.c

Contains the implementation of the logging functions and log file handling.

### logger_process.c

Contains the process-level Logger functionality.

It is intended to run the Logger as an independent process and handle
communication with the rest of the simulator.

---

## Log Levels

The Logger supports three basic log levels:

### INFO

Used for normal program events.

Example:

[INFO] Instruction executed: LOAD

### WARNING

Used for events that require attention but do not necessarily stop the
simulator.

Example:

[WARNING] Stack is almost full

### ERROR

Used when an error occurs.

Example:

[ERROR] Invalid instruction: XYZ

---

## Example Log Output

[INFO] Logger started
[INFO] Instruction executed: LOAD
[INFO] Instruction executed: ADD
[WARNING] Stack is almost full
[ERROR] Invalid instruction: XYZ
[INFO] Program completed

---

## Development Status

### Completed

- Created Logger component directory.
- Created `logger.c`.
- Created `logger.h`.
- Implemented Logger initialization.
- Implemented log file creation/opening.
- Implemented INFO logging.
- Implemented WARNING logging.
- Implemented ERROR logging.
- Implemented Logger cleanup.
- Tested basic log file generation.

### In Progress

- Logger independent process implementation.
- POSIX Message Queue communication.
- Receiving log messages from the Core process.
- Invalid message handling.
- Process failure handling.

### Planned

Core → POSIX Message Queue → Logger integration.

---

## Architecture Rule

The Logger must remain independent from the Core.

The following is NOT allowed:

Core Memory → Logger

The correct architecture is:

Core → Message → POSIX Message Queue → Logger

The Logger only receives information and records it.