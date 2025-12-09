#include "../include/scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#define NUM_PROCESSES 5
#define TIME_QUANTUM_MS 100
#define TOTAL_ITERATIONS 100000000
#define PROGRESS_CHECK_INTERVAL 10000000

// Global array to track progress of each worker
static long worker_progress[NUM_PROCESSES] = {0};

// Simple CPU-bound worker function
void worker_process(int id) {
    printf("┌─────────────────────────────────────────┐\n");
    printf("│ Worker %d Started (PID=%d)             │\n", id, getpid());
    printf("└─────────────────────────────────────────┘\n");
    
    // Do some CPU-bound work
    long counter = 0;
    for (int i = 0; i < TOTAL_ITERATIONS; i++) {
        counter += i;
        
        // Periodically check if we should continue
        if (i % PROGRESS_CHECK_INTERVAL == 0) {
            int progress_pct = (i * 100) / TOTAL_ITERATIONS;
            int remaining_pct = 100 - progress_pct;
            
            printf("╭─ Worker %d Status ────────────────────╮\n", id);
            printf("│ Progress: [");
            for (int p = 0; p < 20; p++) {
                if (p < progress_pct / 5) printf("█");
                else printf("░");
            }
            printf("] %3d%%│\n", progress_pct);
            printf("│ Iteration: %-9d/%-9d       │\n", i, TOTAL_ITERATIONS);
            printf("│ Remaining: %-3d%%                      │\n", remaining_pct);
            printf("╰──────────────────────────────────────╯\n");
        }
    }
    
    printf("\n┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
    printf("┃ ✓ Worker %d FINISHED (PID=%d)        ┃\n", id, getpid());
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
    exit(0);
}

int main() {
    pid_t pids[NUM_PROCESSES];
    
    printf("\n");
    printf("═══════════════════════════════════════════════════════\n");
    printf("   TIME-SHARING SYSTEM SIMULATION\n");
    printf("═══════════════════════════════════════════════════════\n");
    printf("  Scheduler: Round-Robin\n");
    printf("  Time Quantum: %d ms\n", TIME_QUANTUM_MS);
    printf("  Number of Processes: %d\n", NUM_PROCESSES);
    printf("  Total Work per Process: %d iterations\n", TOTAL_ITERATIONS);
    printf("═══════════════════════════════════════════════════════\n\n");
    
    // Initialize the scheduler
    scheduler_init(TIME_QUANTUM_MS);
    
    printf("Creating worker processes...\n\n");
    
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
            printf("  [✓] Worker %d created (PID=%d)\n", i + 1, pid);
            
            // Stop the process immediately so scheduler can control it
            usleep(1000); // Give child time to start
            kill(pid, SIGSTOP);
        } else {
            perror("fork");
            exit(1);
        }
    }
    
    printf("\n");
    printf("═══════════════════════════════════════════════════════\n");
    printf("   STARTING SCHEDULER\n");
    printf("═══════════════════════════════════════════════════════\n\n");
    
    // Run the scheduler
    scheduler_run();
    
    // Clean up
    scheduler_stop();
    
    printf("\n");
    printf("═══════════════════════════════════════════════════════\n");
    printf("   SIMULATION COMPLETE\n");
    printf("═══════════════════════════════════════════════════════\n");
    printf("  All %d workers have finished their work.\n", NUM_PROCESSES);
    printf("═══════════════════════════════════════════════════════\n\n");
    
    return 0;
}
