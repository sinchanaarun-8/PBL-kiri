# Member 3 — Josna — Logger Process

## 1. Responsibility

Member 3 is responsible for developing the Logging Process of the PBL Process Simulator.

The Logger is an independent process responsible for receiving log messages and recording them in a log file.

It communicates using **UNIX Domain Sockets** and does not directly access Core variables such as CPU, memory, stack, or queue.

---

## 2. Logger Responsibilities

The Logger is responsible for:

- Initializing the logging system.
- Creating and opening the log file.
- Recording INFO messages.
- Recording WARNING messages.
- Recording ERROR messages.
- Receiving log messages through UNIX Domain Sockets.
- Writing received messages into `simulator.log`.
- Handling client disconnections.
- Closing the log file properly.
- Cleaning up resources when the Logger terminates.

---

## 3. Logger Architecture

The communication flow is:

```text
       Core Process
            |
            | Log Messages
            v
     UNIX Domain Socket
            |
            v
      Logger Process
            |
            v
       simulator.log
```

The Logger operates independently and receives information through the socket connection.

It does not directly access Core memory or Core variables.

---

## 4. Files and Their Purpose

### logger.h

Contains the public Logger function declarations and shared definitions.

Functions include:

- `logger_init()` — Initializes the logging system.
- `log_info()` — Records an INFO message.
- `log_warning()` — Records a WARNING message.
- `log_error()` — Records an ERROR message.
- `logger_close()` — Closes the log file and releases resources.
- `logger_send_message()` — Sends a message to the Logger through a UNIX Domain Socket.

### logger.c

Contains the implementation of the logging functions and log file handling.

Responsibilities:

- Creating and opening the log file.
- Writing INFO messages.
- Writing WARNING messages.
- Writing ERROR messages.
- Flushing log output.
- Closing the log file.

### logger_process.c

Contains the process-level Logger functionality.

Responsibilities:

- Running the Logger as an independent process.
- Creating a UNIX Domain Socket server.
- Binding and listening for client connections.
- Accepting client connections.
- Receiving and processing log messages.
- Writing received messages into the log file.
- Handling client disconnections.
- Cleaning up socket resources.

### logger_client.c

Contains the client-side socket communication functionality.

Responsibilities:

- Creating a UNIX Domain Socket client.
- Connecting to the Logger server.
- Sending log messages.
- Closing the socket connection after sending messages.

### test_logger_client.c

Used to test socket communication independently by sending sample INFO, WARNING, and ERROR messages to the Logger server.

---

## 5. Log Levels

The Logger supports three basic log levels.

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

## 6. Example Log Output

```text
[INFO] Program started
[INFO] Instruction executed: LOAD
[INFO] Instruction executed: ADD
[WARNING] Stack is almost full
[ERROR] Invalid instruction: XYZ
[INFO] Program completed
```

The examples illustrate the supported log levels. Actual entries depend on the messages received by the Logger.

---

## 7. IPC Communication — UNIX Domain Sockets

The Logger uses UNIX Domain Sockets for communication.

The socket path is:

```text
/tmp/pbl_logger.sock
```

Communication process:

1. The Logger creates a UNIX Domain Socket.
2. It binds the socket to the specified path.
3. It listens for incoming client connections.
4. The client connects to the Logger.
5. The client sends log messages.
6. The Logger receives and processes the messages.
7. The Logger records the messages in `simulator.log`.

UNIX Domain Sockets provide communication between processes on the same system.

---

## 8. Testing

The Logger socket communication was tested using a separate test client.

### Test Messages

```text
INFO: Program started
WARNING: Low memory
ERROR: Invalid instruction
```

### Testing Procedure

1. Compile and start the Logger server.
2. Open another terminal.
3. Compile and run the test client.
4. Verify that the Logger receives the messages.
5. Check the generated `simulator.log` file.

### Test Result

The Logger successfully received the test messages through UNIX Domain Sockets and processed them according to their log levels.

---

## 9. Development Status

The following functionalities have been implemented:

- Created the Logger component directory.
- Created `logger.h`.
- Created `logger.c`.
- Created `logger_process.c`.
- Created `logger_client.c`.
- Created `test_logger_client.c`.
- Implemented Logger initialization.
- Implemented log file creation and opening.
- Implemented INFO logging.
- Implemented WARNING logging.
- Implemented ERROR logging.
- Implemented Logger cleanup.
- Implemented the Logger as an independent process.
- Implemented UNIX Domain Socket server functionality.
- Implemented UNIX Domain Socket client functionality.
- Tested socket communication using a separate test client.

---

## 10. Architecture Rule

The Logger must remain independent from the Core.

The following architecture is NOT allowed:

```text
Core Memory → Logger
```

The correct architecture is:

```text
Core Process
     |
     | Log Message
     v
UNIX Domain Socket
     |
     v
Logger Process
     |
     v
simulator.log
```

The Logger receives information only through IPC and records it.

It does not directly access:

- CPU state.
- Memory state.
- Stack state.
- Queue state.
- Core program state.

---

## 11. Conclusion

The Logger Process has been developed as an independent process using UNIX Domain Sockets for communication.

It supports INFO, WARNING, and ERROR messages and records them in `simulator.log`.

The socket communication has been tested independently using a test client. The implementation maintains separation between the Core execution logic and the Logging Process.