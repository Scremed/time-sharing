# Time-Sharing System Simulation

A userspace Round-Robin scheduler implementation in C that demonstrates OS process scheduling concepts using Linux system calls, signals, and timers.

## Features

- **Round-Robin Scheduling**: Fair time-sharing among multiple processes
- **Real Process Management**: Uses `fork()` to create actual child processes
- **Timer-Based Preemption**: Uses `setitimer()` and `SIGALRM` for time quantum interrupts
- **Signal-Based Control**: Uses `SIGSTOP`/`SIGCONT` to pause/resume processes
- **Context Switching**: Tracks and displays scheduler context switches
- **Process Control Block (PCB)**: Maintains process state and metadata

## System Calls Used

- `fork()` - Create worker processes
- `setitimer()` - Set up periodic timer for time quantum
- `sigaction()` - Install signal handlers for preemption
- `kill()` - Send signals (`SIGSTOP`, `SIGCONT`) to control processes
- `waitpid()` - Detect terminated child processes

## Building

```bash
make
```

## Running

```bash
./timeshare
```

Or use the convenience target:

```bash
make run
```

## Configuration

Edit `src/main.c` to adjust:
- `NUM_PROCESSES`: Number of worker processes (default: 5)
- `TIME_QUANTUM_MS`: Time slice in milliseconds (default: 100ms)

## How It Works

1. **Initialization**: Creates N worker processes and adds them to the ready queue
2. **Scheduling Loop**: 
   - Dequeues the next ready process
   - Resumes it with `SIGCONT`
   - Timer expires after quantum, sends `SIGALRM`
   - Signal handler stops current process and re-enqueues it
   - Repeats until all processes complete
3. **Cleanup**: Waits for all child processes and displays statistics

## Output

The program displays:
- Process creation and initialization
- Context switches with PIDs
- Worker process progress
- Process termination notifications
- Total context switches

## Clean Up

```bash
make clean
```

## Architecture

```
/home/louis/code/time-sharing/
├── src/
│   ├── main.c          # Entry point and worker process logic
│   ├── scheduler.c     # Round-Robin scheduler implementation
│   ├── process.c       # PCB and queue management
│   └── timer.c         # Timer setup and signal handling
├── include/
│   ├── process.h       # Process structures and queue declarations
│   ├── scheduler.h     # Scheduler function declarations
│   └── timer.h         # Timer function declarations
├── Makefile
└── README.md
```

## Requirements

- GCC compiler
- Linux system (tested on Fedora)
- glibc-devel
- kernel-headers

## Educational Value

This project demonstrates:
- Operating system process scheduling
- Inter-process communication using signals
- Timer-based interrupts
- Process state management
- Context switching overhead
- Real-world application of system calls
