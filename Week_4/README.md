# Week 4 — Resource Allocation Graph (Jishnu)

## Scope
This module builds a Resource Allocation Graph and exposes structured nodes/edges for GUI rendering. It does not replace Banker's Algorithm or the separate deadlock detection/recovery modules.

## Files
- `rag.h`: public types and function declarations.
- `rag.c`: graph initialization, edge updates, process-release updates, cycle detection, rendering data.
- `tests/test_rag.c`: standalone tests.
- `Makefile`: strict C11 build and test commands.

## Graph conventions
- Process nodes: `P0`, `P1`, ...
- Resource nodes: `R0`, `R1`, ...
- Allocation edge: resource → process (`Rj -> Pi`).
- Request edge: process → resource (`Pi -> Rj`).
- Edge `instances` stores the allocation/request quantity.

`rag_update()` replaces allocation and request edges from the supplied matrices. It does not decide whether a request should be granted; the OS/application logic should decide first, then provide the current matrices.

## Cycle interpretation
`rag_has_cycle()` detects a directed cycle. For single-instance resource types, a cycle implies deadlock. With multiple instances, a cycle alone does not necessarily prove deadlock; use the dedicated detection algorithm and current system state.

## Four necessary conditions for deadlock
1. **Mutual exclusion** — at least one resource cannot be shared simultaneously.
2. **Hold and wait** — a process holds resources while waiting for more.
3. **No preemption** — resources cannot be forcibly taken; they are released voluntarily.
4. **Circular wait** — a circular chain exists where each process waits for a resource held by the next process.

These conditions are necessary for deadlock. A graph cycle alone does not establish that all conditions are present.

## Build and test
Run from this folder:
```bash
make clean
make test
```

## GUI integration
Call `rag_init()` once process/resource dimensions and resource instance counts are known. After a committed allocation/request/release, call `rag_update()` with current Allocation and Request matrices. Use `rag_render_data()` to read nodes and edges for the GUI renderer.

The team lead should connect this module to the GUI and shared state during integration.
