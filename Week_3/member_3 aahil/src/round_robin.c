
#include <stdio.h>
#include "../include/scheduling.h"

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

int schedule_round_robin(
    Process processes[],
    int n,
    int time_quantum,
    GanttChart *chart
) {
    if (processes == NULL || chart == NULL ||
        n <= 0 || n > MAX_PROCESSES ||
        time_quantum <= 0) {
        return -1;
    }

    /* Validate before changing process state. */
    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time < 0 ||
            processes[i].burst_time <= 0) {
            return -1;
        }

        for (int j = 0; j < i; j++) {
            if (processes[i].pid == processes[j].pid) {
                return -1;
            }
        }
    }

    chart->count = 0;

    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].completion_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].waiting_time = 0;
        processes[i].response_time = -1;
        processes[i].first_start_time = -1;
    }

    /* Sort process indices by arrival time, then PID.
       The original input array remains unchanged. */
    int order[MAX_PROCESSES];

    for (int i = 0; i < n; i++) {
        order[i] = i;
    }

    for (int i = 1; i < n; i++) {
        int key = order[i];
        int j = i - 1;

        while (j >= 0 &&
              (processes[order[j]].arrival_time >
                   processes[key].arrival_time ||
              (processes[order[j]].arrival_time ==
                   processes[key].arrival_time &&
               processes[order[j]].pid > processes[key].pid))) {
            order[j + 1] = order[j];
            j--;
        }

        order[j + 1] = key;
    }

    /* Circular ready queue stores process indices. */
    int queue[MAX_PROCESSES];
    int head = 0;
    int tail = 0;
    int count = 0;
    int next = 0;
    int completed = 0;
    int time = 0;

    while (completed < n) {
        /* If the ready queue is empty, jump to the next arrival. */
        if (count == 0) {
            if (next >= n) {
                return -1;
            }

            int arrival = processes[order[next]].arrival_time;

            if (time < arrival) {
                if (add_segment(chart, -1, time, arrival) != 0) {
                    return -1;
                }

                time = arrival;
            }

            while (next < n &&
                   processes[order[next]].arrival_time <= time) {
                queue[tail] = order[next++];
                tail = (tail + 1) % MAX_PROCESSES;
                count++;
            }
        }

        int index = queue[head];
        head = (head + 1) % MAX_PROCESSES;
        count--;

        Process *p = &processes[index];

        if (p->first_start_time == -1) {
            p->first_start_time = time;
            p->response_time = time - p->arrival_time;
        }

        int run_time = p->remaining_time < time_quantum
                     ? p->remaining_time
                     : time_quantum;

        int end_time = time + run_time;

        if (add_segment(chart, p->pid, time, end_time) != 0) {
            return -1;
        }

        time = end_time;
        p->remaining_time -= run_time;

        /* Add processes that arrived during this time slice
           before re-queuing the unfinished process. */
        while (next < n &&
               processes[order[next]].arrival_time <= time) {
            queue[tail] = order[next++];
            tail = (tail + 1) % MAX_PROCESSES;
            count++;
        }

        if (p->remaining_time == 0) {
            p->completion_time = time;
            p->turnaround_time =
                p->completion_time - p->arrival_time;
            p->waiting_time =
                p->turnaround_time - p->burst_time;
            completed++;
        } else {
            queue[tail] = index;
            tail = (tail + 1) % MAX_PROCESSES;
            count++;
        }
    }

    return 0;
}
