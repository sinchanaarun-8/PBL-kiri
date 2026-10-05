# Member 1 — UI Process

## Responsibility

Member 1 is responsible for the User Interface Process of the
PBL Multi-Process Simulator.

## Current Version

UI V1 — Terminal-Based Interface

## Implemented Features

- Simulator title display
- System status display
- Menu display
- User input
- Input validation
- Invalid choice handling
- Logical command mapping
- Repeated menu operation
- Exit handling

## Menu

1. Load Program
2. Run Program
3. Stop Program
4. Reset Simulator
5. Exit

## Command Mapping

| Choice | Command |
|---|---|
| 1 | LOAD |
| 2 | RUN |
| 3 | STOP |
| 4 | RESET |
| 5 | EXIT |

## Current IPC Status

POSIX Message Queue integration is not implemented in UI V1.

The UI currently generates logical commands locally.

IPC integration will be performed later after the
team communication protocol is finalized.

## Technology

- Language: C
- Environment: POSIX/Linux/macOS development environment
- UI: Terminal
- Future IPC: POSIX Message Queue