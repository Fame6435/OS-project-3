#include <stdio.h>
#include <stdlib.h>
#include "Process.h"

// initiate queue
typedef struct Queue {Process* next;
    Process* front;
    Process* rear;
} Queue;

// ENQUEUES Processes
void enqueue(Queue* queue, Process* process) {
    // if for first, else for every other occurance
    if (queue->rear == NULL) {
        queue->front = queue->rear = process;
    } else {
        queue->rear->next = process;
        queue->rear = process;
    }
    process->next = NULL;
}

// DEQUEUES Proceses
Process* dequeue(Queue* queue) {
    if (queue->front == NULL) return NULL;
    Process* process = queue->front;
    queue->front = queue->front->next;
    if (queue->front == NULL) queue->rear = NULL;
    return process;
}

// MAIN AMAIANA AMAIANAIANAJAAIAN ANANNAANNA
void round_robin(Process processes[], int n, int time_quantum, FILE* output) {
    Queue queue = {NULL, NULL};
    int time = 0, completed = 0;
    int total_waiting_time = 0;

    // print header
    fprintf(output, "RR %d\n", time_quantum);

    // initialize queue w time 0 processes
    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time == 0) {
            enqueue(&queue, &processes[i]);
        }
    }

    while (completed < n) {
        Process* current = dequeue(&queue);

        if (current != NULL) {
            // print current time, process id
            fprintf(output, "%d %d\n", time, current->pid);

            int exec_time = (current->remaining_time > time_quantum) ?
                            time_quantum : current->remaining_time;

            current->remaining_time -= exec_time;
            time += exec_time;

            // ?? check for new arrival
            for (int i = 0; i < n; i++) {
                if (processes[i].arrival_time > time - exec_time &&
                    processes[i].arrival_time <= time) {
                    enqueue(&queue, &processes[i]);
                }
            }

            if (current->remaining_time > 0) {
                enqueue(&queue, current);
            } else {
                completed++;
                // calculate waiting time (if needed)
                total_waiting_time += time - current->arrival_time - current->burst_time;
            }
        } else {
            time++;
        }
    }

    // average waiting time calculator
    fprintf(output, "AVG Waiting Time: %.2f\n",
            (float)total_waiting_time / n);
}
