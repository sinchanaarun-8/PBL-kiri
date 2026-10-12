
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../include/scheduling.h"

static int compare_pid(const void *a, const void *b) {
    const Process *p1 = (const Process *)a;
    const Process *p2 = (const Process *)b;

    if (p1->pid < p2->pid) return -1;
    if (p1->pid > p2->pid) return 1;
    return 0;
}

void display_results(
    const Process processes[],
    int n,
    const GanttChart *chart
) {
    if (processes == NULL || chart == NULL ||
        n <= 0 || chart->count < 0 ||
        chart->count > MAX_GANTT_SEGMENTS) {
        return;
    }

    /* Display each Gantt segment with its time interval. */
    printf("\n========== GANTT CHART ==========\n");

    for (int i = 0; i < chart->count; i++) {
        const GanttSegment *segment = &chart->segments[i];

        if (segment->pid == -1) {
            printf("| IDLE (%d-%d) ",
                   segment->start_time,
                   segment->end_time);
        } else {
            printf("| P%d (%d-%d) ",
                   segment->pid,
                   segment->start_time,
                   segment->end_time);
        }
    }

    printf("|\n");

    Process sorted[MAX_PROCESSES];

    for (int i = 0; i < n; i++) {
        sorted[i] = processes[i];
    }

    qsort(sorted, n, sizeof(Process), compare_pid);

    printf("\n========== PROCESS METRICS ==========\n");
    printf("%-6s %-5s %-5s %-5s %-5s %-5s %-5s\n",
           "PID", "AT", "BT", "CT", "TAT", "WT", "RT");

    double total_wt = 0.0;
    double total_tat = 0.0;
    double total_burst = 0.0;
    int first_arrival = INT_MAX;
    int last_completion = 0;

    for (int i = 0; i < n; i++) {
        const Process *p = &sorted[i];

        printf("P%-5d %-5d %-5d %-5d %-5d %-5d %-5d\n",
               p->pid,
               p->arrival_time,
               p->burst_time,
               p->completion_time,
               p->turnaround_time,
               p->waiting_time,
               p->response_time);

        total_wt += p->waiting_time;
        total_tat += p->turnaround_time;
        total_burst += p->burst_time;

        if (p->arrival_time < first_arrival) {
            first_arrival = p->arrival_time;
        }

        if (p->completion_time > last_completion) {
            last_completion = p->completion_time;
        }
    }

    printf("\nAverage Waiting Time    : %.2f\n",
           total_wt / n);
    printf("Average Turnaround Time : %.2f\n",
           total_tat / n);

    int elapsed = last_completion - first_arrival;

    if (elapsed > 0) {
        printf("CPU Utilization         : %.2f%%\n",
               (total_burst / elapsed) * 100.0);
    }
}

int main(void) {
    Process processes[MAX_PROCESSES];

    GanttChart *chart = malloc(sizeof(GanttChart));

    if (chart == NULL) {
        fprintf(stderr, "Error: Unable to allocate Gantt chart.\n");
        return EXIT_FAILURE;
    }

    int n;

    printf("====================================\n");
    printf("   PROCESS SCHEDULING SIMULATOR\n");
    printf("====================================\n");

    printf("Enter number of processes (1-%d): ",
           MAX_PROCESSES);

    if (scanf("%d", &n) != 1 ||
        n < 1 || n > MAX_PROCESSES) {
        fprintf(stderr, "Error: Invalid process count.\n");
        free(chart);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++) {
        printf("\nProcess P%d\n", i + 1);
        processes[i].pid = i + 1;

        printf("Arrival Time: ");

        if (scanf("%d", &processes[i].arrival_time) != 1 ||
            processes[i].arrival_time < 0) {
            fprintf(stderr,
                    "Error: Arrival time must be non-negative.\n");
            free(chart);
            return EXIT_FAILURE;
        }

        printf("Burst Time: ");

        if (scanf("%d", &processes[i].burst_time) != 1 ||
            processes[i].burst_time <= 0) {
            fprintf(stderr,
                    "Error: Burst time must be positive.\n");
            free(chart);
            return EXIT_FAILURE;
        }

        printf("Priority (not used by these algorithms): ");

        if (scanf("%d", &processes[i].priority) != 1) {
            fprintf(stderr, "Error: Invalid priority.\n");
            free(chart);
            return EXIT_FAILURE;
        }
    }

    int choice;

    printf("\nChoose Scheduling Algorithm:\n");
    printf("1. SRTF (Preemptive SJF)\n");
    printf("2. Round Robin\n");
    printf("Enter choice: ");

    if (scanf("%d", &choice) != 1 ||
        (choice != 1 && choice != 2)) {
        fprintf(stderr, "Error: Invalid algorithm choice.\n");
        free(chart);
        return EXIT_FAILURE;
    }

    int status;

    if (choice == 1) {
        status = schedule_srtf(processes, n, chart);
    } else {
        int quantum;

        printf("Enter Time Quantum: ");

        if (scanf("%d", &quantum) != 1 || quantum <= 0) {
            fprintf(stderr,
                    "Error: Time quantum must be positive.\n");
            free(chart);
            return EXIT_FAILURE;
        }

        status = schedule_round_robin(
            processes, n, quantum, chart
        );
    }

    if (status != 0) {
        fprintf(stderr, "Error: Scheduling failed.\n");
        free(chart);
        return EXIT_FAILURE;
    }

    display_results(processes, n, chart);

    free(chart);
    return EXIT_SUCCESS;
}
