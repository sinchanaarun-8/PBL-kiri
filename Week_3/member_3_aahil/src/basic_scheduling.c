#include <limits.h>
#include "../include/scheduling.h"

static int add_segment(GanttChart *chart, int pid, int start, int end)
{
    if (start >= end) return 0;

    if (chart->count > 0) {
        GanttSegment *last = &chart->segments[chart->count - 1];
        if (last->pid == pid && last->end_time == start) {
            last->end_time = end;
            return 0;
        }
    }

    if (chart->count >= MAX_GANTT_SEGMENTS) return -1;

    chart->segments[chart->count++] = (GanttSegment){pid, start, end};
    return 0;
}

static int prepare(Process p[], int n, GanttChart *chart)
{
    if (!p || !chart || n < 1 || n > MAX_PROCESSES) return -1;

    for (int i = 0; i < n; i++) {
        if (p[i].arrival_time < 0 || p[i].burst_time <= 0)
            return -1;
        for (int j = 0; j < i; j++)
            if (p[i].pid == p[j].pid) return -1;
    }

    chart->count = 0;
    for (int i = 0; i < n; i++) {
        p[i].remaining_time = p[i].burst_time;
        p[i].completion_time = 0;
        p[i].turnaround_time = 0;
        p[i].waiting_time = 0;
        p[i].response_time = -1;
        p[i].first_start_time = -1;
    }
    return 0;
}

static void finish(Process *p, int time)
{
    p->completion_time = time;
    p->turnaround_time = time - p->arrival_time;
    p->waiting_time = p->turnaround_time - p->burst_time;
}

static int schedule(Process p[], int n, GanttChart *chart, int mode)
{
    if (prepare(p, n, chart) != 0) return -1;

    int time = 0, completed = 0;

    while (completed < n) {
        int best = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].remaining_time <= 0 || p[i].arrival_time > time)
                continue;

            if (best == -1) {
                best = i;
                continue;
            }

            int choose = 0;

            if (mode == 0) {
                choose = p[i].arrival_time < p[best].arrival_time ||
                    (p[i].arrival_time == p[best].arrival_time &&
                     p[i].pid < p[best].pid);
            } else if (mode == 1) {
                choose = p[i].burst_time < p[best].burst_time ||
                    (p[i].burst_time == p[best].burst_time &&
                     p[i].arrival_time < p[best].arrival_time);
            } else if (mode == 2 || mode == 3) {
                choose = p[i].priority < p[best].priority ||
                    (p[i].priority == p[best].priority &&
                     p[i].arrival_time < p[best].arrival_time);
            }

            if (choose) best = i;
        }

        if (best == -1) {
            int next = INT_MAX;
            for (int i = 0; i < n; i++)
                if (p[i].remaining_time > 0 &&
                    p[i].arrival_time > time &&
                    p[i].arrival_time < next)
                    next = p[i].arrival_time;

            if (next == INT_MAX ||
                add_segment(chart, -1, time, next) != 0)
                return -1;

            time = next;
            continue;
        }

        Process *current = &p[best];

        if (current->first_start_time == -1) {
            current->first_start_time = time;
            current->response_time = time - current->arrival_time;
        }

        if (mode == 0 || mode == 1 || mode == 2) {
            int end = time + current->remaining_time;
            if (add_segment(chart, current->pid, time, end) != 0)
                return -1;
            current->remaining_time = 0;
            time = end;
            finish(current, time);
            completed++;
        } else {
            if (add_segment(chart, current->pid, time, time + 1) != 0)
                return -1;

            current->remaining_time--;
            time++;

            if (current->remaining_time == 0) {
                finish(current, time);
                completed++;
            }
        }
    }

    return 0;
}

int schedule_fcfs(Process p[], int n, GanttChart *chart)
{
    return schedule(p, n, chart, 0);
}

int schedule_sjf_non_preemptive(Process p[], int n, GanttChart *chart)
{
    return schedule(p, n, chart, 1);
}

int schedule_priority_non_preemptive(Process p[], int n, GanttChart *chart)
{
    return schedule(p, n, chart, 2);
}

int schedule_priority_preemptive(Process p[], int n, GanttChart *chart)
{
    return schedule(p, n, chart, 3);
}
