
#include <stdio.h>
#include <limits.h>
#include "../include/scheduling.h"

/* Add a Gantt chart segment, merging consecutive segments
   when the same process continues running. */
static int add_segment(
    GanttChart *chart,
    int pid,
    int start,
    int end
) {
    if (start >= end) {
        return 0;
    }

    if (chart->count > 0) {
        GanttSegment *last =
            &chart->segments[chart->count - 1];

        if (last->pid == pid && last->end_time == start) {
            last->end_time = end;
            return 0;
        }
    }

    if (chart->count >= MAX_GANTT_SEGMENTS) {
        return -1;
    }

    chart->segments[chart->count].pid = pid;
    chart->segments[chart->count].start_time = start;
    chart->segments[chart->count].end_time = end;
    chart->count++;

    return 0;
}

/* Select the arrived process with the shortest remaining time.
   Ties are resolved by earlier arrival time, then lower PID. */
static int select_process(const Process processes[], int n, int time) {
    int selected = -1;

    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time > time ||
            processes[i].remaining_time <= 0) {
            continue;
        }

        if (selected == -1 ||
            processes[i].remaining_time <
                processes[selected].remaining_time ||
            (processes[i].remaining_time ==
                processes[selected].remaining_time &&
             processes[i].arrival_time <
                processes[selected].arrival_time) ||
            (processes[i].remaining_time ==
                processes[selected].remaining_time &&
             processes[i].arrival_time ==
                processes[selected].arrival_time &&
             processes[i].pid < processes[selected].pid)) {
            selected = i;
        }
    }

    return selected;
}

int schedule_srtf(
    Process processes[],
    int n,
    GanttChart *chart
) {
    if (processes == NULL || chart == NULL ||
        n <= 0 || n > MAX_PROCESSES) {
        return -1;
    }

    /* Validate inputs before modifying process state. */
    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time < 0 ||
            processes[i].burst_time <= 0) {
            return -1;
        }
    }

    chart->count = 0;

    /* Reset results so the algorithm can be run repeatedly. */
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].completion_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].waiting_time = 0;
        processes[i].response_time = -1;
        processes[i].first_start_time = -1;
    }

    int completed = 0;
    int time = 0;

    while (completed < n) {
        int selected = select_process(processes, n, time);

        /* No process is ready: record CPU idle time and jump
           to the next arrival instead of advancing one unit at a time. */
        if (selected == -1) {
            int next_arrival = INT_MAX;

            for (int i = 0; i < n; i++) {
                if (processes[i].remaining_time > 0 &&
                    processes[i].arrival_time > time &&
                    processes[i].arrival_time < next_arrival) {
                    next_arrival = processes[i].arrival_time;
                }
            }

            if (next_arrival == INT_MAX) {
                return -1;
            }

            if (add_segment(chart, -1, time, next_arrival) != 0) {
                return -1;
            }

            time = next_arrival;
            continue;
        }

        Process *p = &processes[selected];

        if (p->first_start_time == -1) {
            p->first_start_time = time;
            p->response_time = time - p->arrival_time;
        }

        /* Execute for one time unit. A new arrival may cause
           the next scheduling decision to select another process. */
        if (add_segment(chart, p->pid, time, time + 1) != 0) {
            return -1;
        }

        p->remaining_time--;
        time++;

        if (p->remaining_time == 0) {
            p->completion_time = time;
            p->turnaround_time =
                p->completion_time - p->arrival_time;
            p->waiting_time =
                p->turnaround_time - p->burst_time;

            completed++;
        }
    }

    return 0;
}
