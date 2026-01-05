#ifndef PROCESS_H
#define PROCESS_H

#include "types.h"

/* Maximum number of processes */
#define MAX_PROCESSES 8

/* Process states */
typedef enum {
    PROC_NEW,
    PROC_READY,
    PROC_CURRENT,
    PROC_WAITING,
    PROC_TERMINATED
} proc_state_t;

/* Process Control Block (PCB) */
typedef struct process {
    int pid;
    proc_state_t state;
    void (*entry)(void);   // process function
    void* stack;           // stack pointer

    int priority;   // base priority
    int age;        // aging counter
} process_t;

/* Process Manager API */
void process_init(void);
int process_create(void (*entry)(void));
void process_set_state(int pid, proc_state_t state);
void process_terminate(int pid);

/* Utility functions */
process_t* get_current_process(void);
process_t* get_ready_process(void);
process_t* get_process_by_pid(int pid);
process_t* get_process_at_index(int index);

#endif
