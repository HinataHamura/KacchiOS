#include "scheduler.h"
#include "process.h"
#include "serial.h"

static int time_quantum;

/* =========================
   Initialize Scheduler
   ========================= */
void scheduler_init(int quantum) {
    time_quantum = quantum;
}

/* =========================
   Select next process
   Policy: Round Robin + Aging
   ========================= */
static process_t* select_next_process(void) {
    process_t* selected = 0;

    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_t* p = get_process_at_index(i);

        if (p && p->state == PROC_READY) {
            /* Aging: increase priority */
            p->age++;

            if (!selected ||
                (p->priority + p->age) >
                (selected->priority + selected->age)) {
                selected = p;
            }
        }
    }
    return selected;
}

/* =========================
   Scheduler main function
   ========================= */
void schedule(void) {
    process_t* p = select_next_process();

    if (!p)
        return;

    /* Context switch (software-level) */
    process_set_state(p->pid, PROC_CURRENT);

    serial_puts("[Scheduler] Running process ");
    serial_putc('0' + p->pid);
    serial_puts("\n");

    /* Run process for `time_quantum` quanta (cooperative simulation) */
    if (time_quantum <= 0) time_quantum = 1;
    for (int q = 0; q < time_quantum; q++) {
        p->entry();
    }

    /* After execution we consider this run complete and terminate the process.
       (Note: this simple kernel treats a single function invocation as a run
       slice and then cleans up; preemption would require hardware/timer.) */
    p->age = 0;
    process_terminate(p->pid);
}
