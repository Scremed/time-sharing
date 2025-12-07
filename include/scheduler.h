#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

// Function declarations
void scheduler_init(int quantum_ms);
void scheduler_add_process(pid_t pid);
void scheduler_run();
void scheduler_stop();
void handle_quantum_expired(int signo);

#endif
