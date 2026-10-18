
#ifndef DETECTION_H
#define DETECTION_H

#define MAX_PROCESSES 20
#define MAX_RESOURCES 20

/* Detect deadlocked processes */
int detect_deadlock(
    int processes,
    int resources,
    int allocation[MAX_PROCESSES][MAX_RESOURCES],
    int request[MAX_PROCESSES][MAX_RESOURCES],
    int available[MAX_RESOURCES],
    int deadlocked[MAX_PROCESSES]
);

/* Recover resources by releasing a selected process's allocation */
int recover_process(
    int process_id,
    int processes,
    int resources,
    int allocation[MAX_PROCESSES][MAX_RESOURCES],
    int available[MAX_RESOURCES]
);

#endif
