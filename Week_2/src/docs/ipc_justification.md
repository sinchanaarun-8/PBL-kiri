# IPC Design and Justification

## Selected IPC Mechanism

The simulator uses **POSIX Message Queues** for inter-process communication.

## Why POSIX Message Queues?

POSIX message queues were selected because the simulator needs structured communication between independent processes.

Advantages for this project:

- Supports communication between independent processes.
- Messages are maintained by the operating system.
- Provides structured message-based communication.
- Sender and receiver do not have to execute at exactly the same time.
- Supports clear separation between UI, Core and Logger processes.
- Provides blocking send/receive operations suitable for command processing.

## Alternatives Considered

### Pipes
Simple and efficient, but primarily provide stream-based communication and are less convenient for structured messages.

### FIFOs
Allow unrelated processes to communicate, but provide less structure than message queues.

### Shared Memory
Very fast, but requires additional synchronization and is unnecessary for the small command/response messages used by this simulator.

### Signals
Useful for notifications and process control, but unsuitable for transferring structured command and log data.

### Sockets
Powerful and suitable for network communication, but unnecessary for communication between local simulator processes.

## Architecture

```text
                 ┌──────────────────┐
                 │       UI         │
                 │ User Interface   │
                 └────────┬─────────┘
                          │
                    UI → Core MQ
                          │
                          ▼
                 ┌──────────────────┐
                 │      CORE        │
                 │ CPU              │
                 │ Memory           │
                 │ Stack            │
                 │ Queue            │
                 └───────┬──────────┘
                         │
                   Core → UI MQ
                         │
                         ▼
                        UI

                         CORE
                           │
                    Core → Logger MQ
                           │
                           ▼
                 ┌──────────────────┐
                 │      LOGGER      │
                 │ Execution/Error  │
                 │ Logging          │
                 └────────┬─────────┘
                          │
                          ▼
                   simulator.log