#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../include/scheduling.h"

typedef int (*Scheduler)(Process[], int, GanttChart *);
typedef struct {
    char name[40];
    double avg_wt;
    double avg_tat;
    double avg_rt;
    double cpu_util;
} ComparisonResult;


static int compare_pid(const void *a, const void *b)
{
    const Process *x = a;
    const Process *y = b;
    return (x->pid > y->pid) - (x->pid < y->pid);
}
static void calculate_metrics(
    const Process p[], int n, const GanttChart *chart,
    ComparisonResult *result
)
{
    double total_wt = 0, total_tat = 0, total_rt = 0;
    double total_burst = 0;
    int last_completion = 0;

    for (int i = 0; i < n; i++) {
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
        total_rt += p[i].response_time;
        total_burst += p[i].burst_time;

        if (p[i].completion_time > last_completion)
            last_completion = p[i].completion_time;
    }

    result->avg_wt = total_wt / n;
    result->avg_tat = total_tat / n;
    result->avg_rt = total_rt / n;

    result->cpu_util = last_completion > 0
        ? total_burst * 100.0 / last_completion
        : 0.0;
}
void display_results(const Process p[], int n, const GanttChart *chart)
{
    printf("\n========== GANTT CHART ==========\n");
    for (int i = 0; i < chart->count; i++) {
        const GanttSegment *s = &chart->segments[i];
        if (s->pid == -1)
            printf("| IDLE (%d-%d) ", s->start_time, s->end_time);
        else
            printf("| P%d (%d-%d) ", s->pid, s->start_time, s->end_time);
    }
    printf("|\n");

    Process sorted[MAX_PROCESSES];
    for (int i = 0; i < n; i++)
        sorted[i] = p[i];
    qsort(sorted, n, sizeof(Process), compare_pid);

    printf("\n%-6s %-5s %-5s %-5s %-5s %-5s %-5s %-5s\n",
           "PID", "AT", "BT", "PR", "CT", "TAT", "WT", "RT");

    double total_wt = 0, total_tat = 0, total_rt = 0, total_burst = 0;
    int first_arrival = INT_MAX, last_completion = 0;

    for (int i = 0; i < n; i++) {
        Process *x = &sorted[i];
        printf("P%-5d %-5d %-5d %-5d %-5d %-5d %-5d %-5d\n",
               x->pid, x->arrival_time, x->burst_time, x->priority,
               x->completion_time, x->turnaround_time,
               x->waiting_time, x->response_time);

        total_wt += x->waiting_time;
        total_tat += x->turnaround_time;
        total_rt += x->response_time;
        total_burst += x->burst_time;

        if (x->arrival_time < first_arrival)
            first_arrival = x->arrival_time;
        if (x->completion_time > last_completion)
            last_completion = x->completion_time;
    }

    printf("\nAverage Waiting Time    : %.2f\n", total_wt / n);
    printf("Average Turnaround Time : %.2f\n", total_tat / n);
    printf("Average Response Time   : %.2f\n", total_rt / n);

    int elapsed = last_completion;
    double busy = total_burst;
    double idle = elapsed - busy;

    printf("CPU Busy Time           : %.0f\n", busy);
    printf("CPU Idle Time           : %.0f\n", idle);

    if (elapsed > 0)
        printf("CPU Utilization         : %.2f%%\n", busy * 100.0 / elapsed);
}

static int run_scheduler(
    const char *name, Scheduler scheduler,
    const Process input[], int n
)
{
    Process copy[MAX_PROCESSES];
    GanttChart *chart = malloc(sizeof(*chart));

    if (!chart) {
        fprintf(stderr, "Unable to allocate Gantt chart.\n");
        return -1;
    }

    for (int i = 0; i < n; i++)
        copy[i] = input[i];

    printf("\n\n========== %s ==========\n", name);
    int status = scheduler(copy, n, chart);

    if (status == 0)
        display_results(copy, n, chart);
    else
        fprintf(stderr, "Scheduling failed for %s.\n", name);

    free(chart);
    return status;
}

static int run_rr(const Process input[], int n, int quantum)
{
    Process copy[MAX_PROCESSES];
    GanttChart *chart = malloc(sizeof(*chart));

    if (!chart) {
        fprintf(stderr, "Unable to allocate Gantt chart.\n");
        return -1;
    }

    for (int i = 0; i < n; i++)
        copy[i] = input[i];

    printf("\n\n========== ROUND ROBIN ==========\n");
    int status = schedule_round_robin(copy, n, quantum, chart);

    if (status == 0)
        display_results(copy, n, chart);
    else
        fprintf(stderr, "Round Robin scheduling failed.\n");

    free(chart);
    return status;
}

int main(void)
{
    Process processes[MAX_PROCESSES];
    int n;

    printf("====================================\n");
    printf("   PROCESS SCHEDULING SIMULATOR\n");
    printf("====================================\n");
    printf("Enter number of processes (1-%d): ", MAX_PROCESSES);

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_PROCESSES) {
        fprintf(stderr, "Invalid process count.\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++) {
        printf("\nProcess %d\n", i + 1);
        printf("PID: ");
        if (scanf("%d", &processes[i].pid) != 1 || processes[i].pid <= 0) {
            fprintf(stderr, "PID must be a positive integer.\n");
            return EXIT_FAILURE;
        }

        for (int j = 0; j < i; j++) {
            if (processes[i].pid == processes[j].pid) {
                fprintf(stderr, "Duplicate PID is not allowed.\n");
                return EXIT_FAILURE;
            }
        }

        printf("Arrival Time: ");
        if (scanf("%d", &processes[i].arrival_time) != 1 ||
            processes[i].arrival_time < 0) {
            fprintf(stderr, "Arrival time must be non-negative.\n");
            return EXIT_FAILURE;
        }

        printf("Burst Time: ");
        if (scanf("%d", &processes[i].burst_time) != 1 ||
            processes[i].burst_time <= 0) {
            fprintf(stderr, "Burst time must be positive.\n");
            return EXIT_FAILURE;
        }

        printf("Priority (smaller number = higher priority): ");
        if (scanf("%d", &processes[i].priority) != 1 ||
            processes[i].priority < 0) {
            fprintf(stderr, "Priority must be non-negative.\n");
            return EXIT_FAILURE;
        }
    }

    for (;;) {
        int choice;

        printf("\n========== SCHEDULING MENU ==========\n");
        printf("1. FCFS\n");
        printf("2. SJF Non-Preemptive\n");
        printf("3. Priority Non-Preemptive\n");
        printf("4. SJF Preemptive (SRTF)\n");
        printf("5. Priority Preemptive\n");
        printf("6. Round Robin\n");
        printf("7. Compare All Algorithms\n");
        printf("8. Exit\n");
        printf("Choose an option: ");

        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Invalid menu input.\n");
            return EXIT_FAILURE;
        }

        int status = 0;
        switch (choice) {
        case 1:
            status = run_scheduler("FCFS", schedule_fcfs, processes, n);
            break;
        case 2:
            status = run_scheduler("SJF NON-PREEMPTIVE",
                                   schedule_sjf_non_preemptive, processes, n);
            break;
        case 3:
            status = run_scheduler("PRIORITY NON-PREEMPTIVE",
                                   schedule_priority_non_preemptive,
                                   processes, n);
            break;
        case 4:
            status = run_scheduler("SJF PREEMPTIVE (SRTF)",
                                   schedule_srtf, processes, n);
            break;
        case 5:
            status = run_scheduler("PRIORITY PREEMPTIVE",
                                   schedule_priority_preemptive,
                                   processes, n);
            break;
        case 6: {
            int quantum;
            printf("Enter Time Quantum: ");
            if (scanf("%d", &quantum) != 1 || quantum <= 0) {
                fprintf(stderr, "Time quantum must be positive.\n");
                return EXIT_FAILURE;
            }
            status = run_rr(processes, n, quantum);
            break;
        }
        case 7:
            run_scheduler("FCFS", schedule_fcfs, processes, n);
            run_scheduler("SJF NON-PREEMPTIVE",
                          schedule_sjf_non_preemptive, processes, n);
            run_scheduler("PRIORITY NON-PREEMPTIVE",
                          schedule_priority_non_preemptive, processes, n);
            run_scheduler("SJF PREEMPTIVE (SRTF)",
                          schedule_srtf, processes, n);
            run_scheduler("PRIORITY PREEMPTIVE",
                          schedule_priority_preemptive, processes, n);
            {
                int quantum;
                printf("\nEnter Time Quantum for Round Robin: ");
                if (scanf("%d", &quantum) != 1 || quantum <= 0) {
                    fprintf(stderr, "Time quantum must be positive.\n");
                    return EXIT_FAILURE;
                }
                run_rr(processes, n, quantum);
            }
            break;
        case 8:
            printf("Exiting simulator. Goodbye!\n");
            return EXIT_SUCCESS;
        default:
            printf("Invalid choice. Select 1-8.\n");
        }

        if (status != 0)
            fprintf(stderr, "Please check the input and try again.\n");
    }
}
