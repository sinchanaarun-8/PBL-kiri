
#include <stdio.h>

#define MAX 100
#define SEG 10000

typedef struct {
    int at, bt, pr, ct, first, done;
} Process;

int main(void) {
    Process p[MAX];
    int gantt_pid[SEG], gantt_start[SEG], gantt_end[SEG];
    int n, time = 0, completed = 0, segments = 0;
    double wt_sum = 0, tat_sum = 0, rt_sum = 0;

    printf("PRIORITY PREEMPTIVE SCHEDULING\n");
    printf("Smaller priority number = higher priority\n");
    printf("Enter number of processes: ");

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        p[i].first = -1;
        p[i].done = 0;

        printf("Enter Arrival Time, Burst Time and Priority for P%d: ",
               i + 1);

        if (scanf("%d%d%d", &p[i].at, &p[i].bt, &p[i].pr) != 3 ||
            p[i].at < 0 || p[i].bt <= 0 || p[i].pr < 0) {
            printf("Invalid process details.\n");
            return 1;
        }
    }

    while (completed < n) {
        int idx = -1;

        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].at <= time) {
                if (idx == -1 || p[i].pr < p[idx].pr ||
                    (p[i].pr == p[idx].pr && p[i].at < p[idx].at)) {
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        if (p[idx].first == -1)
            p[idx].first = time;

        if (segments >= SEG) {
            printf("Too many Gantt chart segments.\n");
            return 1;
        }

        gantt_pid[segments] = idx;
        gantt_start[segments] = time;

        time++;
        p[idx].bt--;
        
        gantt_end[segments] = time;
        segments++;

        if (p[idx].bt == 0) {
            p[idx].ct = time;
            p[idx].done = 1;
            completed++;
        }
    }

    printf("\nGantt Chart:\n");
    for (int i = 0; i < segments; i++)
        printf("| P%d ", gantt_pid[i] + 1);
    printf("|\n");

    if (segments > 0) {
        printf("%d", gantt_start[0]);
        for (int i = 0; i < segments; i++)
            printf("\t%d", gantt_end[i]);
        printf("\n");
    }

    printf("\n%-6s %-5s %-5s %-5s %-5s %-5s %-5s %-5s\n",
           "PID", "AT", "BT", "PR", "CT", "TAT", "WT", "RT");

    for (int i = 0; i < n; i++) {
        int original_bt = 0;

        /* Recover original burst time from total execution time. */
        for (int j = 0; j < segments; j++)
            if (gantt_pid[j] == i)
                original_bt += gantt_end[j] - gantt_start[j];

        int tat = p[i].ct - p[i].at;
        int wt = tat - original_bt;
        int rt = p[i].first - p[i].at;

        printf("P%-5d %-5d %-5d %-5d %-5d %-5d %-5d %-5d\n",
               i + 1, p[i].at, original_bt, p[i].pr,
               p[i].ct, tat, wt, rt);

        wt_sum += wt;
        tat_sum += tat;
        rt_sum += rt;
    }

    printf("\nAverage WT: %.2f\n", wt_sum / n);
    printf("Average TAT: %.2f\n", tat_sum / n);
    printf("Average RT: %.2f\n", rt_sum / n);

    return 0;
}
