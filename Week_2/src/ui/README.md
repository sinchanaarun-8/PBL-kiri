# UI Process

## Overview
The UI Process provides an interactive menu for the Multi-Process Simulator.

## Features
- Load Program
- Run Program
- Stop Program
- Reset Simulator
- Exit Simulator
- Input validation
- Sends commands to Core using POSIX Message Queues.
- Receives and displays responses from Core.

## Communication
- UI to Core: `/pbl_ui_to_core`
- Core to UI: `/pbl_core_to_ui`

## Files
- `ui.c` - UI implementation
- `ui.h` - UI command definitions

## Testing Results

| Command | Result |
|---|---|
| LOAD | PASS |
| RUN | PASS |
| STOP | PASS |
| RESET | PASS |
| EXIT | PASS |