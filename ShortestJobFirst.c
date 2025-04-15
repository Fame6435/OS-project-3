#include <stdio.h>
#include <stdlib.h>
#include "Process.h"


// shortest job first - calculates and returns the arrival time if burst time is equal
int compare_sjf(const void* a, const void* b) {
    Process* p1 = (Process*)a;
    Process* p2 = (Process*)b;
    if (p1->burst_time == p2->burst_time) return p1->arrival_time - p2->arrival_time;
    return p1->burst_time - p2->burst_time;
}


// main sjf

void shortest_job_first(Process processes[], int n, FILE* output) {
    int current_time = 0;
    int completed = 0;
    int total_waiting_time = 0;

    fprintf(output, "SJF\n");

    while (completed < n) {
        int selected = -1;
        int shortest_burst = 2147483647;

        // Find the process with shortest burst time among arrived processes
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time &&
                !processes[i].completed &&
                processes[i].burst_time < shortest_burst) {
                selected = i;
                shortest_burst = processes[i].burst_time;
            }
        }

        // If no process is found, increment time
        if (selected == -1) {
            current_time++;
            continue;
        }

        // Process found, execute it
        fprintf(output, "%d %d\n", current_time, processes[selected].pid);

        // Calculate waiting time for this process
        int waiting_time = current_time - processes[selected].arrival_time;
        total_waiting_time += waiting_time;

        // Update time and mark process as completed
        current_time += processes[selected].burst_time;
        processes[selected].completed = 1;
        completed++;
    }

    float avg_waiting_time = (float)total_waiting_time / n;
    fprintf(output, "AVG Waiting Time: %.2f\n", avg_waiting_time);
}
