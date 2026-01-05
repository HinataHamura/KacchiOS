#include "process.h"
#include "memory.h"

static process_t process_table[MAX_PROCESSES];
static int current_pid = -1;
static int pid_counter = 0;

/* =========================
   Initialize Process Table
   ========================= */
void process_init(void) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].state = PROC_TERMINATED;
        process_table[i].pid = -1;
        process_table[i].entry = 0;
        process_table[i].stack = 0;
    }
}

/* =========================
   Create a new process
   ========================= */
int process_create(void (*entry)(void)) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].state == PROC_TERMINATED) {

            process_table[i].pid = pid_counter++;
            process_table[i].entry = entry;
            process_table[i].stack = alloc_stack();
            process_table[i].priority = 1; // default
            process_table[i].age = 0;
            process_table[i].state = PROC_READY;

            return process_table[i].pid;
        }
    }
    return -1;  // no slot available
}

/* =========================
   Set process state
   ========================= */
void process_set_state(int pid, proc_state_t state) {
    process_t* p = get_process_by_pid(pid);
    if (p) {
        p->state = state;
        if (state == PROC_CURRENT) {
            current_pid = pid;
        } else if (state == PROC_TERMINATED) {
            if (current_pid == pid) current_pid = -1;
        } else if (current_pid == pid && state != PROC_CURRENT) {
            /* leaving current state */
            current_pid = -1;
        }
    }
}

/* =========================
   Terminate process
   ========================= */
void process_terminate(int pid) {
    process_t* p = get_process_by_pid(pid);
    if (!p) return;

    free_stack(p->stack);
    p->stack = 0;
    p->state = PROC_TERMINATED;
    p->pid = -1; /* free slot for reuse and ensure get_process_by_pid returns NULL */
    if (current_pid == pid) current_pid = -1;
}

/* =========================
   Utility functions
   ========================= */
process_t* get_process_by_pid(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid == pid &&
            process_table[i].state != PROC_TERMINATED) {
            return &process_table[i];
        }
    }
    return 0;
}

process_t* get_current_process(void) {
    return get_process_by_pid(current_pid);
}

process_t* get_ready_process(void) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].state == PROC_READY) {
            return &process_table[i];
        }
    }
    return 0;
}

process_t* get_process_at_index(int index) {
    if (index < 0 || index >= MAX_PROCESSES) return 0;
    return &process_table[index];
}
