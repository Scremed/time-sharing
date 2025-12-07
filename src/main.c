#include "../include/scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#define NUM_PROCESSES 5
#define TIME_QUANTUM_MS 100

// Simple CPU-bound worker function
void worker_process(int id) {
    printf("[Worker %d] Started (PID=%d)\n", id, getpid());
    
    // Do some CPU-bound work
    long counter = 0;
    for (int i = 0; i < 100000000; i++) {
        counter += i;
        
        // Periodically check if we should continue
        if (i % 10000000 == 0) {
            printf("[Worker %d] Working... iteration %d\n", id, i);
        }
    }
    
    printf("[Worker %d] Finished (PID=%d)\n", id, getpid());
    exit(0);
}

int main() {
    pid_t pids[NUM_PROCESSES];
    
    printf("=== Time-Sharing System Simulation ===\n");
    printf("Round-Robin Scheduler with %d ms quantum\n", TIME_QUANTUM_MS);
    printf("Number of processes: %d\n\n", NUM_PROCESSES);
    
    // Initialize the scheduler
    scheduler_init(TIME_QUANTUM_MS);
    
    // Create worker processes
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid_t pid = fork();
        
        if (pid == 0) {
            // Child process
            worker_process(i + 1);
        } else if (pid > 0) {
            // Parent process
            pids[i] = pid;
            scheduler_add_process(pid);
            
            // Stop the process immediately so scheduler can control it
            usleep(1000); // Give child time to start
            kill(pid, SIGSTOP);
        } else {
            perror("fork");
            exit(1);
        }
    }
    
    printf("\n[Main] All processes created, starting scheduler...\n\n");
    
    // Run the scheduler
    scheduler_run();
    
    // Clean up
    scheduler_stop();
    
    printf("\n=== Simulation Complete ===\n");
    
    return 0;
}
