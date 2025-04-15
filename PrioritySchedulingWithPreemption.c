// File: CS420/Assignment3/PrioritySchedulingWithPreemption.c

#include <stdio.h>
#include <stdlib.h>
#include "Process.h"

// Function to perform priority scheduling with preemption
void priority_with_preemption(Process processes[], int n, FILE* output) {
    //Initialize variables.
    int current_time = 0;
    int completed = 0;
    int total_waiting_time = 0;

    fprintf(output, "PR withPREMP\n");

    while (completed < n) {
        int selected = -1;

        //Very large number. (>1)
        //Actual priorities will be MUCH lower and will immediately superscede this temporary number.
        int highest_priority = 2147483647;

        // Find process with highest priority AMONG US
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time &&
                !processes[i].completed &&
                processes[i].priority < highest_priority) {
                selected = i;
                highest_priority = processes[i].priority;
            }
        }

        // timeaggregator
        if (selected == -1) {
            current_time++;
            continue;
        }

        // current time and process ID
        fprintf(output, "%d %d\n", current_time, processes[selected].pid);

        // execute process for unit
        processes[selected].remaining_time--;
        current_time++;

        // calculate if the process is completed
        if (processes[selected].remaining_time == 0) {
            processes[selected].completed = 1;
            completed++;
            total_waiting_time += current_time - processes[selected].arrival_time - processes[selected].burst_time;
        }
    }

    // compute time as a float for decimal
    float avg_waiting_time = (float)total_waiting_time / n;
    fprintf(output, "AVG Waiting Time: %.2f\n", avg_waiting_time);
}
