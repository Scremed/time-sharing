#define _XOPEN_SOURCE=700
#include "../include/timer.h"
#include "../include/scheduler.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>

static struct itimerval timer;

void setup_timer(int quantum_ms) {
    struct sigaction sa;
    
    // Set up signal handler for SIGALRM
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_quantum_expired;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("sigaction");
        return;
    }
    
    // Configure the timer
    timer.it_value.tv_sec = quantum_ms / 1000;
    timer.it_value.tv_usec = (quantum_ms % 1000) * 1000;
    timer.it_interval.tv_sec = quantum_ms / 1000;
    timer.it_interval.tv_usec = (quantum_ms % 1000) * 1000;
    
    // Start the timer
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("setitimer");
        return;
    }
    
    printf("[Timer] Timer started with %d ms quantum\n", quantum_ms);
}

void stop_timer() {
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    
    setitimer(ITIMER_REAL, &timer, NULL);
    printf("[Timer] Timer stopped\n");
}
