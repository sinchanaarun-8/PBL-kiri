# Week 5: Simulation and Comparative Analysis of Page Replacement Algorithms

## 1. Project Overview

This project implements and compares three page replacement algorithms used in operating systems:

- First-In, First-Out (FIFO)
- Least Recently Used (LRU)
- Optimal Page Replacement

The application simulates page references, displays frame contents after each reference, identifies page hits and page faults, calculates performance ratios, and compares the algorithms using a graphical interface.

## 2. Project Objectives

- Understand the working principles of page replacement algorithms.
- Simulate page references using a configurable number of frames.
- Display the frame state after every page reference.
- Calculate total page hits, page faults, hit ratio, and fault ratio.
- Compare FIFO, LRU, and Optimal algorithms.
- Visualize page-fault counts using a comparison graph.
- Integrate individual team members' implementations into one application.

## 3. Team Members and Contributions

### Member 1 — Jishnu: FIFO Algorithm

**Responsibilities:**
- Implemented the FIFO page replacement algorithm.
- Maintained the order in which pages entered memory.
- Replaced the oldest resident page when a page fault occurred and all frames were occupied.
- Recorded page hits, page faults, and frame states.
- Worked on FIFO testing.

**Implementation:** `member_1_jishnu/src/fifo.c`

### Member 2 — Josna: LRU Algorithm

**Responsibilities:**
- Implemented the Least Recently Used page replacement algorithm.
- Tracked the last-used time of pages in memory.
- Replaced the least recently used page when all frames were occupied.
- Displayed page hits, page faults, and frame states in the original implementation.
- Worked on LRU testing.

**Implementation:** `member_2_josna/src/lru.c`

### Member 3 — Aahil: Optimal Page Replacement Algorithm

**Responsibilities:**
- Implemented the Optimal page replacement algorithm.
- Examined future page references to determine which resident page would be used farthest in the future.
- Preferred empty frames before replacing existing pages.
- Recorded page hits, page faults, and frame states.
- Worked on Optimal algorithm testing.

**Implementation:** `member_3_aahil/optimal.c`

### Team Lead — Integration, GUI, and Testing

**Responsibilities:**
- Integrated the three algorithms through a common simulation interface.
- Developed the GTK3 graphical user interface.
- Connected the algorithms to the GUI.
- Added input parsing and validation for page references.
- Displayed frame states, hit/fault status, totals, and performance ratios.
- Implemented the Compare All option and page-fault comparison graph.
- Created an integration test to verify the algorithms together.
- Compiled and tested the integrated application.
- Managed the integration commit and GitHub push.

**Integration files:**
- `gui.c`
- `src/lru_adapter.c`
- `tests/test_integration.c`
- `include/simulator.h`

## 4. Application Features

1. **Algorithm selection:** Choose FIFO, LRU, Optimal, or Compare All.
2. **Custom page references:** Enter page numbers separated by spaces or commas.
3. **Configurable frames:** Select the number of memory frames.
4. **Step-by-step simulation:** View frame contents after every page reference.
5. **Performance statistics:** View total references, hits, page faults, hit ratio, and fault ratio.
6. **Comparison summary:** Compare the hits and page faults of all three algorithms.
7. **Graphical comparison:** View a bar graph of page-fault counts.
8. **Input validation:** Reject empty or invalid page-reference input.

## 5. Project Structure

```text
Week_5/
├── include/
│   └── simulator.h
├── member_1_jishnu/
│   ├── src/
│   │   └── fifo.c
│   └── tests/
│       └── test_fifo.c
├── member_2_josna/
│   ├── include/
│   │   └── lru.h
│   ├── src/
│   │   └── lru.c
│   └── tests/
│       └── test_lru.c
├── member_3_aahil/
│   ├── optimal.c
│   ├── optimal.h
│   └── tests/
├── src/
│   └── lru_adapter.c
├── tests/
│   └── test_integration.c
├── gui.c
├── build/
└── README.md
```

## 6. Technologies Used

- C programming language
- GTK3 for the graphical user interface
- Cairo for graph drawing
- GCC compiler
- Git and GitHub for version control and collaboration
- Linux/WSL development environment

## 7. Build and Run Instructions

Run these commands from the repository root in WSL.

### Install GTK3 development dependencies

```bash
sudo apt update
sudo apt install build-essential pkg-config libgtk-3-dev
```

### Compile the application

```bash
mkdir -p Week_5/build

gcc -Wall -Wextra -Wpedantic -std=c11 \
-IWeek_5/include \
Week_5/gui.c \
Week_5/member_1_jishnu/src/fifo.c \
Week_5/src/lru_adapter.c \
Week_5/member_3_aahil/optimal.c \
$(pkg-config --cflags --libs gtk+-3.0) \
-o Week_5/build/page_replacement_simulator
```

### Run the application

```bash
./Week_5/build/page_replacement_simulator
```

Enter page references, choose the number of frames, select an algorithm, and click **Run Simulation**.

## 8. Integration Testing

The integration test uses the page-reference sequence:

`7 0 1 2 0 3 0 4 2 3`

Number of frames: `3`

| Algorithm | Page Hits | Page Faults |
|---|---:|---:|
| FIFO | 1 | 9 |
| LRU | 2 | 8 |
| Optimal | 4 | 6 |

The integration test produced the expected results and printed:

`INTEGRATION TEST PASSED`

## 9. Additional GUI Testing

A second sequence was tested:

`1 2 3 1 4 5`

Number of frames: `3`

| Algorithm | Page Hits | Page Faults |
|---|---:|---:|
| FIFO | 1 | 5 |
| LRU | 1 | 5 |
| Optimal | 1 | 5 |

The GUI displayed the frame states, hit/fault statistics, and comparison summary correctly for this input.

## 10. Conclusion

The project demonstrates how FIFO, LRU, and Optimal page replacement algorithms manage memory frames. The integrated application provides step-by-step simulation, performance statistics, and a graphical comparison of page faults. The project also demonstrates teamwork, modular C programming, GUI development, testing, and Git-based collaboration.
