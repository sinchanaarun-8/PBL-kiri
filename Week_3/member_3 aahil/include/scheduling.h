
#ifndef SCHEDULING_H
#define SCHEDULING_H

#define MAX_PROCESSES 100
#define MAX_GANTT_SEGMENTS 10000

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;

    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
    int first_start_time;
} Process;

typedef struct {
    int pid;
    int start_time;
    int end_time;
} GanttSegment;

typedef struct {
    GanttSegment segments[MAX_GANTT_SEGMENTS];
    int count;
} GanttChart;

/* SRTF: Shortest Remaining Time First */
int schedule_srtf(
    Process processes[],
    int n,
    GanttChart *chart
);

/* Round Robin with configurable time quantum */
int schedule_round_robin(
    Process processes[],
    int n,
    int time_quantum,
    GanttChart *chart
);

/* Display scheduling results */
void display_results(
    const Process processes[],
    int n,
    const GanttChart *chart
);

#endif
