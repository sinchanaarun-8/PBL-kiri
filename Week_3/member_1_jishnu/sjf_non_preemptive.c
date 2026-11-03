#include <stdio.h>

#define MAX 100

typedef struct {
    int pid, arrival, burst;
    int start, completion, waiting, turnaround, response;
    int done;
} Process;

int main(void) {
    Process p[MAX];
    int n, time = 0, completed = 0;
    float total_wt = 0, total_tat = 0;

    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("Enter PID, Arrival Time and Burst Time for process %d: ",
               i + 1);
        if (scanf("%d %d %d", &p[i].pid, &p[i].arrival,
                  &p[i].burst) != 3 ||
            p[i].arrival < 0 || p[i].burst <= 0) {
            printf("Invalid process details.\n");
            return 1;
        }
        p[i].done = 0;
    }

    /* Store execution order for the Gantt chart */
    int order[MAX], count = 0, start_time[MAX], end_time[MAX];

    while (completed < n) {
        int idx = -1;

        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].arrival <= time) {
                if (idx == -1 || p[i].burst < p[idx].burst ||
                    (p[i].burst == p[idx].burst &&
                     p[i].arrival < p[idx].arrival)) {
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        p[idx].start = time;
        p[idx].completion = time + p[idx].burst;
        p[idx].turnaround = p[idx].completion - p[idx].arrival;
        p[idx].waiting = p[idx].turnaround - p[idx].burst;
        p[idx].response = p[idx].start - p[idx].arrival;

        order[count] = idx;
        start_time[count] = time;
        end_time[count] = p[idx].completion;
        count++;

        total_wt += p[idx].waiting;
        total_tat += p[idx].turnaround;

        time = p[idx].completion;
        p[idx].done = 1;
        completed++;
    }

    printf("\nSJF Non-Preemptive Results\n");
    printf("PID\tAT\tBT\tST\tCT\tWT\tTAT\tRT\n");

    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].arrival, p[i].burst,
               p[i].start, p[i].completion, p[i].waiting,
               p[i].turnaround, p[i].response);
    }

    printf("\nGantt Chart:\n");
    for (int i = 0; i < count; i++)
        printf("+-------");
    printf("+\n");

    for (int i = 0; i < count; i++)
        printf("|  P%-3d ", p[order[i]].pid);
    printf("|\n");

    for (int i = 0; i < count; i++)
        printf("+-------");
    printf("+\n");

    printf("%-8d", start_time[0]);
    for (int i = 0; i < count; i++)
        printf("%-8d", end_time[i]);
    printf("\n");

    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);

    return 0;
}
