#include "../include/process.h"
#include <stdio.h>
#include <string.h>

void init_queue(ProcessQueue *queue) {
    queue->front = 0;
    queue->rear = -1;
    queue->count = 0;
    memset(queue->processes, 0, sizeof(queue->processes));
}

int enqueue(ProcessQueue *queue, pid_t pid) {
    if (is_full(queue)) {
        return -1;
    }
    
    queue->rear = (queue->rear + 1) % MAX_PROCESSES;
    queue->processes[queue->rear].pid = pid;
    queue->processes[queue->rear].state = READY;
    queue->processes[queue->rear].cpu_time = 0;
    queue->count++;
    
    return 0;
}

pid_t dequeue(ProcessQueue *queue) {
    if (is_empty(queue)) {
        return -1;
    }
    
    pid_t pid = queue->processes[queue->front].pid;
    queue->front = (queue->front + 1) % MAX_PROCESSES;
    queue->count--;
    
    return pid;
}

int is_empty(ProcessQueue *queue) {
    return queue->count == 0;
}

int is_full(ProcessQueue *queue) {
    return queue->count >= MAX_PROCESSES;
}

PCB* find_process(ProcessQueue *queue, pid_t pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (queue->processes[i].pid == pid && queue->processes[i].state != TERMINATED) {
            return &queue->processes[i];
        }
    }
    return NULL;
}

void remove_process(ProcessQueue *queue, pid_t pid) {
    PCB *pcb = find_process(queue, pid);
    if (pcb != NULL) {
        pcb->state = TERMINATED;
        pcb->pid = 0;
    }
}
