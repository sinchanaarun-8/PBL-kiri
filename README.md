# Yeregav Kirikiri
# The Problem Based Learning 
..
                         ┌───────────────┐
                         │     USER      │
                         └───────┬───────┘
                                 │
                                 ▼
                    ┌──────────────────────┐
                    │     UI PROCESS       │
                    │        (C)           │
                    │                      │
                    │ • Accept commands    │
                    │ • Display status     │
                    │ • Display results    │
                    └──────────┬───────────┘
                               │
                               │ POSIX
                               │ MESSAGE QUEUE
                               │
                               ▼
                    ┌──────────────────────┐
                    │     CORE PROCESS     │
                    │        (C)           │
                    │                      │
                    │ • CPU Execution      │
                    │ • Memory             │
                    │ • Stack              │
                    │ • Queue              │
                    │ • Process commands   │
                    └──────────┬───────────┘
                               │
                               │ POSIX
                               │ MESSAGE QUEUE
                               │
                               ▼
                    ┌──────────────────────┐
                    │   LOGGING PROCESS    │
                    │        (C)           │
                    │                      │
                    │ • Execution Logs     │
                    │ • Error Logs         │
                    │ • Warnings           │
                    └──────────────────────┘
