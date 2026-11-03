#include <stdio.h>

#define MAX 100

typedef struct {
    int pid, arrival, burst;
    int start, completion, waiting, turnaround, response;
} Process;

int main(void) {
    Process p[MAX], temp;
    int n, time = 0;
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
    }

    /* Sort by arrival time */
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].arrival > p[j].arrival) {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    /* Calculate scheduling metrics */
    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival)
            time = p[i].arrival;

        p[i].start = time;
        p[i].completion = time + p[i].burst;
        p[i].turnaround = p[i].completion - p[i].arrival;
        p[i].waiting = p[i].turnaround - p[i].burst;
        p[i].response = p[i].start - p[i].arrival;
        time = p[i].completion;

        total_wt += p[i].waiting;
        total_tat += p[i].turnaround;
    }

    printf("\nFCFS Scheduling Results\n");
    printf("PID\tAT\tBT\tST\tCT\tWT\tTAT\tRT\n");

    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].arrival, p[i].burst,
               p[i].start, p[i].completion, p[i].waiting,
               p[i].turnaround, p[i].response);
    }

    /* Gantt chart */
    printf("\nGantt Chart:\n");
    for (int i = 0; i < n; i++)
        printf("+-------");
    printf("+\n");

    for (int i = 0; i < n; i++)
        printf("|  P%-3d ", p[i].pid);
    printf("|\n");

    for (int i = 0; i < n; i++)
        printf("+-------");
    printf("+\n");

    printf("%-8d", p[0].start);
    for (int i = 0; i < n; i++)
        printf("%-8d", p[i].completion);
    printf("\n");

    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);

    return 0;
}
