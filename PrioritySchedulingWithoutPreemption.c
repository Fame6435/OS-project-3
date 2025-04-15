#include <stdio.h>
#include <stdlib.h>
#include "Process.h"

// cmp priority - a/b
int compare_priority(const void* a, const void* b) {
    Process* p1 = (Process*)a;
    Process* p2 = (Process*)b;
    if (p1->priority == p2->priority) return 0;
    return (p1->priority < p2->priority) ? -1 : 1;
}

// proccesses is array of procceses, n is a int. file is hopefully a pointer to a file.
// If OS is evil, file* will not be a file. and pc will erupt in flames:(
void priority_no_preemptive(Process processes[], int n, FILE* output) {
    int time = 0, completed = 0;
    int total_waiting_time = 0;

    //I HATE THIS LINE.
    fprintf(output, "PR noPREMP\n");

    while (completed < n) {
        int selected = -1;

        // Find process with highest priority (lowest priority number) DO NOT ASK FOR PIZZA
        for (int i = 0; i < n; i++) {
            if (!processes[i].completed && processes[i].arrival_time <= time) {
                if (selected == -1 || processes[i].priority < processes[selected].priority) {
                    selected = i;
                }
            }
        }

        if (selected != -1) {
            // Print current time and process ID
            fprintf(output, "%d %d\n", time, processes[selected].pid);

            // Calculate waiting time :D
            total_waiting_time += time - processes[selected].arrival_time;

            time += processes[selected].burst_time;
            processes[selected].completed = 1;
            completed++;
        } else {
            time++;
        }
    }

    // Print average waiting time
    fprintf(output, "AVG Waiting Time: %.2f\n", (float)total_waiting_time / n);
}
