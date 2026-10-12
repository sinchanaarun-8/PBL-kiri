#include <stdio.h>

#define MAX 100
#define SEG 10000

typedef struct {
    int at, bt, pr, ct, first, done;
} Process;

typedef struct {
    int pid, start, end;
} Block;

int main(void)
{
    Process p[MAX];
    Block g[SEG];
    int n, i, time = 0, completed = 0, count = 0;
    double total_wt = 0, total_tat = 0, total_rt = 0;

    printf("PRIORITY NON-PREEMPTIVE SCHEDULING\n");
    printf("Smaller priority number means higher priority.\n");
    printf("Enter number of processes: ");

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        printf("Enter Arrival Time, Burst Time and Priority for P%d: ",
               i + 1);

        if (scanf("%d%d%d", &p[i].at, &p[i].bt,
                  &p[i].pr) != 3 ||
            p[i].at < 0 || p[i].bt <= 0) {
            printf("Invalid input.\n");
            return 1;
        }

        p[i].ct = 0;
        p[i].first = -1;
        p[i].done = 0;
    }

    while (completed < n) {
        int best = -1;

        for (i = 0; i < n; i++) {
            if (!p[i].done && p[i].at <= time) {
                if (best == -1 ||
                    p[i].pr < p[best].pr ||
                    (p[i].pr == p[best].pr &&
                     p[i].at < p[best].at) ||
                    (p[i].pr == p[best].pr &&
                     p[i].at == p[best].at && i < best)) {
                    best = i;
                }
            }
        }

        if (best == -1) {
            int next = -1;

            for (i = 0; i < n; i++) {
                if (!p[i].done &&
                    (next == -1 || p[i].at < next)) {
                    next = p[i].at;
                }
            }

            if (count >= SEG) {
                printf("Too many Gantt chart segments.\n");
                return 1;
            }

            g[count].pid = -1;
            g[count].start = time;
            g[count].end = next;
            count++;

            time = next;
            continue;
        }

        p[best].first = time;

        if (count >= SEG) {
            printf("Too many Gantt chart segments.\n");
            return 1;
        }

        g[count].pid = best + 1;
        g[count].start = time;
        g[count].end = time + p[best].bt;
        count++;

        time += p[best].bt;
        p[best].ct = time;
        p[best].done = 1;
        completed++;
    }

    printf("\nGantt Chart:\n");

    for (i = 0; i < count; i++) {
        if (g[i].pid == -1)
            printf("| IDLE ");
        else
            printf("| P%d ", g[i].pid);
    }

    printf("|\n");

    for (i = 0; i < count; i++)
        printf("%-7d", g[i].start);

    printf("%d\n", g[count - 1].end);

    printf("\nPID\tAT\tBT\tPR\tCT\tTAT\tWT\tRT\n");

    for (i = 0; i < n; i++) {
        int tat = p[i].ct - p[i].at;
        int wt = tat - p[i].bt;
        int rt = p[i].first - p[i].at;

        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               i + 1, p[i].at, p[i].bt, p[i].pr,
               p[i].ct, tat, wt, rt);

        total_wt += wt;
        total_tat += tat;
        total_rt += rt;
    }

    printf("\nAverage WT: %.2f\n", total_wt / n);
    printf("Average TAT: %.2f\n", total_tat / n);
    printf("Average RT: %.2f\n", total_rt / n);

    return 0;
}