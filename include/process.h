#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

#define MAX_PROCESSES 10

typedef enum {
    READY,
    RUNNING,
    TERMINATED
} ProcessState;

typedef struct {
    pid_t pid;
    ProcessState state;
    int cpu_time;  // CPU time used in milliseconds
} PCB;

typedef struct {
    PCB processes[MAX_PROCESSES];
    int front;
    int rear;
    int count;
} ProcessQueue;

// Function declarations
void init_queue(ProcessQueue *queue);
int enqueue(ProcessQueue *queue, pid_t pid);
pid_t dequeue(ProcessQueue *queue);
int is_empty(ProcessQueue *queue);
int is_full(ProcessQueue *queue);
PCB* find_process(ProcessQueue *queue, pid_t pid);
void remove_process(ProcessQueue *queue, pid_t pid);

#endif
