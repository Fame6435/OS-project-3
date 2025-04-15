#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>


// Struct for process obj initialization
typedef struct Process {
    int pid;            // Process ID
    int arrival_time;   // Arrival Time
    int burst_time;     // CPU Burst Time
    int remaining_time; // Remaining Burst Time (for RR)
    int priority;       // Priority (for Priority Scheduling)
    int completed;      // Completion flag (used in SJF & Priority Scheduling)

    struct Process* next; // For round-robin.
    //NULL for other scheduling.
} Process;


#endif // PROCESS_H
