#include "../include/scheduler.h"
#include "../include/timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static ProcessQueue ready_queue;
static pid_t current_pid = -1;
static int running = 1;
static int context_switches = 0;

void scheduler_init(int quantum_ms) {
    init_queue(&ready_queue);
    setup_timer(quantum_ms);
}

void scheduler_add_process(pid_t pid) {
    if (enqueue(&ready_queue, pid) == 0) {
        printf("[Scheduler] Added process PID=%d to ready queue\n", pid);
    } else {
        printf("[Scheduler] Failed to add process PID=%d (queue full)\n", pid);
    }
}

void handle_quantum_expired(int signo) {
    (void)signo; // Suppress unused parameter warning
    
    if (current_pid > 0) {
        printf("[Scheduler] Quantum expired for PID=%d\n", current_pid);
        
        // Stop the current process
        kill(current_pid, SIGSTOP);
        
        // Put it back at the end of the ready queue
        enqueue(&ready_queue, current_pid);
        
        current_pid = -1;
        context_switches++;
    }
}

void scheduler_run() {
    printf("[Scheduler] Starting Round-Robin scheduler\n");
    int active_processes = MAX_PROCESSES;
    
    while (running && active_processes > 0) {
        // Check for terminated children
        int status;
        pid_t finished_pid;
        while ((finished_pid = waitpid(-1, &status, WNOHANG)) > 0) {
            printf("[Scheduler] Process PID=%d terminated\n", finished_pid);
            active_processes--;
            
            // If it was the current process, clear it
            if (finished_pid == current_pid) {
                current_pid = -1;
            }
        }
        
        // If no process is currently running, get the next one
        if (current_pid <= 0 && !is_empty(&ready_queue)) {
            current_pid = dequeue(&ready_queue);
            
            // Verify the process is still alive before resuming
            if (kill(current_pid, 0) == 0) {
                printf("[Scheduler] Switching to PID=%d (Context switches: %d)\n", 
                       current_pid, context_switches);
                
                // Resume the process
                kill(current_pid, SIGCONT);
            } else {
                // Process died, try next one
                current_pid = -1;
            }
        }
        
        // If queue is empty and no current process, we're done
        if (is_empty(&ready_queue) && current_pid <= 0) {
            printf("[Scheduler] All processes completed\n");
            break;
        }
        
        usleep(10000); // Sleep 10ms to avoid busy waiting
    }
    
    printf("[Scheduler] Total context switches: %d\n", context_switches);
}

void scheduler_stop() {
    running = 0;
    stop_timer();
}
