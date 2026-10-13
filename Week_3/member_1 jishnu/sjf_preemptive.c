#include <stdio.h>

#define MAX 100
#define MAX_SEGMENTS 100000

typedef struct {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int completion;
    int waiting;
    int turnaround;
    int response;
    int started;
} Process;

int main(void) {
    Process p[MAX];
    int order[MAX_SEGMENTS];
    int seg_start[MAX_SEGMENTS];
    int seg_end[MAX_SEGMENTS];
    int n, time = 0, completed = 0, segments = 0;
    double total_wt = 0;
    double total_tat = 0;
    double total_rt = 0;

    printf("Enter number of processes: ");

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        p[i].started = 0;

        printf("Enter Arrival Time and Burst Time for P%d: ", i + 1);

        if (scanf("%d %d", &p[i].arrival, &p[i].burst) != 2 ||
            p[i].arrival < 0 || p[i].burst <= 0) {
            printf("Invalid process details.\n");
            return 1;
        }

        p[i].remaining = p[i].burst;
    }

    while (completed < n) {
        int idx = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && p[i].remaining > 0) {
                if (idx == -1 ||
                    p[i].remaining < p[idx].remaining) {
                    idx = i;
                }
            }
        }

        if (segments == 0 || order[segments - 1] != idx) {
            if (segments >= MAX_SEGMENTS) {
                printf("Too many Gantt chart segments.\n");
                return 1;
            }

            order[segments] = idx;
            seg_start[segments] = time;
            seg_end[segments] = time + 1;
            segments++;
        } else {
            seg_end[segments - 1] = time + 1;
        }

        if (idx != -1) {
            if (!p[idx].started) {
                p[idx].response = time - p[idx].arrival;
                p[idx].started = 1;
            }

            p[idx].remaining--;

            if (p[idx].remaining == 0) {
                p[idx].completion = time + 1;
                p[idx].turnaround =
                    p[idx].completion - p[idx].arrival;
                p[idx].waiting =
                    p[idx].turnaround - p[idx].burst;

                total_wt += p[idx].waiting;
                total_tat += p[idx].turnaround;
                total_rt += p[idx].response;
                completed++;
            }
        }

        time++;
    }

    printf("\nSJF Preemptive (SRTF) Results\n");
    printf("PID\tAT\tBT\tCT\tWT\tTAT\tRT\n");

    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival,
               p[i].burst,
               p[i].completion,
               p[i].waiting,
               p[i].turnaround,
               p[i].response);
    }

    printf("\nGantt Chart:\n");
    printf("%d", seg_start[0]);

    for (int i = 0; i < segments; i++) {
        if (order[i] == -1) {
            printf(" | Idle | %d", seg_end[i]);
        } else {
            printf(" | P%d | %d",
                   p[order[i]].pid, seg_end[i]);
        }
    }

    printf("\n\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Response Time: %.2f\n", total_rt / n);

    return 0;
}

