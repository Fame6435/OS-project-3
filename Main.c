#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Process.h"

// runtime error
#define MAX_PROCESSES 100


// Function Prototypes
void round_robin(Process processes[], int n, int time_quantum, FILE* output);
void shortest_job_first(Process processes[], int n, FILE* output);
void priority_no_preemptive(Process processes[], int n, FILE* output);
void priority_with_preemption(Process processes[], int n, FILE* output);

int main(int argc, char** argv) {
    //Make sure the argments are correct and make sense.
    if (argc != 2) {
         printf("Usage: %s <filename>", argv[0]);
         return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    //64 is a reasonable buffer length for each line. Each line should not exceed maybe 5.
    //This in theory could cause a buffer overflow crash, but if usage is within examples provided it is safe.
    char line[64];
    int line_number = 0;

    //A buffer to store the first line indicating the type of algorithm and any special arguments
    //(round robin quantum time).
    char algorithm[64];

    //Reads a line from the file.
    if (fgets(line, sizeof(line), file)) {
        strcpy(algorithm, line);
        //printf("Algorithm: %s\n", algorithm);
    }

     // Read second line: total number of processes
     int total_processes = 0;
     if (fgets(line, sizeof(line), file)) {
         line_number++;
         if (sscanf(line, "%d", &total_processes) != 1) {
             fprintf(stderr, "Could not read total number of processes :(");
             fclose(file);
             return 1;
         }
         printf("Total Processes: %d\n", total_processes);
     } else {
         fprintf(stderr, "Goofy usage, try again with a complete file.");
         fclose(file);
         return 1;
     }

     // Allocate array for processes
     if (total_processes > MAX_PROCESSES) {
         fprintf(stderr, "Too many processes idiot. You are only allowed %d", MAX_PROCESSES);
         fclose(file);
         return 1;
     }
     // incoming ballistic missile
     Process processes[MAX_PROCESSES];
     int process_count = 0;

     while (fgets(line, sizeof(line), file) && process_count < total_processes) {
         line_number++;
         // Remove newline
         line[strcspn(line, "\n")] = '\0';

         Process proc;
         int scanned = sscanf(line, "%d %d %d %d",
                              &proc.pid,
                              &proc.arrival_time,
                              &proc.burst_time,
                              &proc.priority);
         if (scanned != 4) {
             fprintf(stderr, "Parsing process EXPLODED on line %d: '%s'\n", line_number, line);
             continue;
         }

         proc.remaining_time = proc.burst_time;
         proc.completed = 0;

         processes[process_count++] = proc;
     }

     fclose(file);

     FILE* output = fopen("output.txt", "w");
     if (strncmp(algorithm, "RR", 2) == 0) {
         int quantum;
         sscanf(algorithm, "RR %d", &quantum);
         round_robin(processes, total_processes, quantum, output);
     } else if (strncmp(algorithm, "SJF", 3) == 0) {
         shortest_job_first(processes, total_processes, output);
     } else if (strncmp(algorithm, "PR noPREMP", 10) == 0) {
         priority_no_preemptive(processes, total_processes, output);
     } else if (strncmp(algorithm, "PR withPREMP", 12) == 0) {  // Check for new algorithm
              priority_with_preemption(processes, total_processes, output);
        }

     fflush(output);
     fclose(output);


    // int n, time_quantum;

    // // Example Input: Reading process details
    // printf("Enter the number of processes: ");
    // scanf("%d", &n);

    // Process processes[MAX_PROCESSES];
    // for (int i = 0; i < n; i++) {
    //     printf("Enter arrival time, burst time, and priority for Process %d: ", i + 1);
    //     scanf("%d %d %d", &processes[i].arrival_time, &processes[i].burst_time, &processes[i].priority);
    //     processes[i].pid = i + 1;
    //     processes[i].remaining_time = processes[i].burst_time; // Initialize for RR
    //     processes[i].completed = 0;                           // Initialize for SJF and Priority Scheduling
    // }

    // // Round Robin Scheduling
    // printf("Enter time quantum for Round Robin: ");
    // scanf("%d", &time_quantum);
    // printf("\n--- Round Robin Scheduling ---\n");
    // round_robin(processes, n, time_quantum);

    // // Reset Process Data for SJF
    // for (int i = 0; i < n; i++) {
    //     processes[i].remaining_time = processes[i].burst_time;
    //     processes[i].completed = 0;
    // }

    // // Shortest Job First Scheduling
    // printf("\n--- Shortest Job First Scheduling ---\n");
    // shortest_job_first(processes, n);

    // // Reset Process Data for Priority Scheduling
    // for (int i = 0; i < n; i++) {
    //     processes[i].remaining_time = processes[i].burst_time;
    //     processes[i].completed = 0;
    // }

    // // Priority Scheduling
    // printf("\n--- Priority Scheduling (Non-Preemptive) ---\n");
    // priority_no_preemptive(processes, n);

    return 0;
}
