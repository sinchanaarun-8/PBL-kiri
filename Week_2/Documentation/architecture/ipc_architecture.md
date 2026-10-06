# IPC Architecture

## Overview

The simulator is divided into three independent processes:

- **UI Process** — Handles user interaction and sends commands.
- **Core Process** — Executes simulator commands and manages CPU, Memory, Stack and Queue.
- **Logging Process** — Receives log messages from the Core Process and records them.

The processes communicate using **POSIX Message Queues**.

---

## System Architecture

```text
                         USER
                           |
                           v
                  +-------------------+
                  |    UI PROCESS     |
                  |                   |
                  |  User Interface   |
                  |  Command Input    |
                  +-------------------+
                           |
                           | UI → Core
                           | /pbl_ui_to_core
                           | POSIX Message Queue
                           v
                  +-------------------+
                  |   CORE PROCESS    |
                  |                   |
                  |       Core        |
                  |      / | \        |
                  |     /  |  \       |
                  |    v   v   v      |
                  |   CPU Memory      |
                  |       Stack       |
                  |       Queue       |
                  +-------------------+
                     |             |
                     |             |
          Core → UI  |             | Core → Logger
          /pbl_core  |             | /pbl_core_to_logger
          _to_ui     |             | POSIX Message Queue
          POSIX MQ   |             |
                     v             v
              +----------+   +-------------------+
              |    UI    |   | LOGGING PROCESS   |
              | RESPONSE |   |                   |
              +----------+   |  Log Processing   |
                             +-------------------+
                                      |
                                      v
                             logs/simulator.log