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
        printf("Enter PID, Arrival Time and Burst Time for process %d: ", i + 1);
        if (scanf("%d %d %d", &p[i].pid, &p[i].arrival,
                  &p[i].burst) != 3 ||
            p[i].arrival < 0 || p[i].burst <= 0) {
            printf("Invalid process details.\n");
            return 1;
        }
        p[i].done = 0;
    }

    printf("\nSJF Non-Preemptive Scheduling Results\n");
    printf("PID\tAT\tBT\tST\tCT\tWT\tTAT\tRT\n");

    while (completed < n) {
        int shortest = -1;

        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].arrival <= time) {
                if (shortest == -1 ||
                    p[i].burst < p[shortest].burst ||
                    (p[i].burst == p[shortest].burst &&
                     p[i].arrival < p[shortest].arrival)) {
                    shortest = i;
                }
            }
        }

        if (shortest == -1) {
            time++;
            continue;
        }

        p[shortest].start = time;
        p[shortest].completion = time + p[shortest].burst;
        p[shortest].turnaround =
            p[shortest].completion - p[shortest].arrival;
        p[shortest].waiting =
            p[shortest].turnaround - p[shortest].burst;
        p[shortest].response =
            p[shortest].start - p[shortest].arrival;

        time = p[shortest].completion;
        p[shortest].done = 1;
        completed++;

        total_wt += p[shortest].waiting;
        total_tat += p[shortest].turnaround;

        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[shortest].pid, p[shortest].arrival,
               p[shortest].burst, p[shortest].start,
               p[shortest].completion, p[shortest].waiting,
               p[shortest].turnaround, p[shortest].response);
    }

    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);

    return 0;
}
