# IPC Design and Justification

## Selected IPC Mechanism

The simulator uses **POSIX Message Queues** for inter-process communication.

The IPC layer provides three message queues for communication between the UI, Core and Logging processes.

## Why POSIX Message Queues?

POSIX Message Queues were selected because the simulator requires structured communication between independent processes.

Advantages for this project:

- Supports communication between independent processes.
- Messages are maintained by the operating system.
- Provides structured message-based communication.
- Separates communication between the UI, Core and Logger processes.
- Supports blocking send/receive operations suitable for command processing.
- Allows different message types to be exchanged using a common message structure.

The simulator uses the following message types:

```text
IPC_MSG_COMMAND
IPC_MSG_RESPONSE
IPC_MSG_LOG

Alternatives Considered
Pipes
Pipes provide simple inter-process communication, but they are stream-oriented and are less convenient for the structured command and response messages used by this simulator.
FIFOs
FIFOs allow unrelated processes to communicate, but they provide less message-oriented structure than POSIX Message Queues.
Shared Memory
Shared memory can provide high-speed data sharing, but it requires additional synchronization mechanisms. It is unnecessary for the relatively small command, response and log messages used by this simulator.
Signals
Signals are useful for notifications and process control, but they are not suitable for transferring structured command and log data.
Sockets
Sockets provide powerful local or network communication, but network-style communication is unnecessary for the local simulator processes.
Queue Design
The simulator uses three POSIX Message Queues:
Queue	Direction	Purpose
/pbl_ui_to_core	UI → Core	Sends commands from the UI to the Core
/pbl_core_to_ui	Core → UI	Sends responses from the Core to the UI
/pbl_core_to_logger	Core → Logger	Sends log messages from the Core to the Logger


Message Structure
The IPC system uses a common IPCMessage structure containing:
type
command
status
text

This allows commands, responses and log messages to be transferred through the message queues.
Communication Architecture
                  ┌──────────────────┐
                  │       UI         │
                  │  User Interface  │
                  └────────┬─────────┘
                           │
                           │ /pbl_ui_to_core
                           │ POSIX Message Queue
                           ▼
                  ┌──────────────────┐
                  │      CORE        │
                  │                  │
                  │ CPU              │
                  │ Memory           │
                  │ Stack            │
                  │ Queue            │
                  └───────┬──────────┘
                          │   │
          /pbl_core_to_ui     │ /pbl_core_to_logger
          POSIX MQ            │ POSIX MQ
                      │       │
                      ▼       ▼
               ┌──────────┐  ┌──────────────────┐
               │    UI    │  │     LOGGER       │
               │ Response │  │ Simulator Logs   │
               └──────────┘  └────────┬─────────┘
                                      │
                                      ▼
                              logs/simulator.log

**Communication Flow**

UI → Core
The UI sends a command to the Core through /pbl_ui_to_core.

Core → UI
After processing the command, the Core sends a response through /pbl_core_to_ui.

Core → Logger
The Core sends command information to the Logger through /pbl_core_to_logger.
The Logger receives the message and records it in the simulator log.

**Conclusion**
POSIX Message Queues provide a suitable IPC mechanism for this simulator because they allow the three independent processes to exchange structured command, response and log messages while keeping the process responsibilities separated.