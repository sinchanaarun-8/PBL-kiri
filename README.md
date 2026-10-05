# Yeregav Kirikiri
# The Problem Based Learning 
..
                         USER
                           |
                           v
                  +------------------+
                  |    UI PROCESS    |
                  |       (C)        |
                  |                  |
                  | - User Commands  |
                  | - Status Display |
                  | - Results Display |
                  +--------+---------+
                           |
                           | POSIX
                           | Message Queue
                           v
                  +------------------+
                  |   CORE PROCESS   |
                  |       (C)        |
                  |                  |
                  | - CPU            |
                  | - Memory         |
                  | - Stack          |
                  | - Queue          |
                  +--------+---------+
                           |
                           | POSIX
                           | Message Queue
                           v
                  +------------------+
                  | LOGGING PROCESS  |
                  |       (C)        |
                  |                  |
                  | - Execution Logs |
                  | - Error Logs     |
                  +------------------+
