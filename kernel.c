/* kernel.c - Main kernel with null process */
#include "types.h"
#include "serial.h"
#include "string.h"
#include "memory.h"
#include "process.h"
#include "scheduler.h"
#include "ipc.h"


#define MAX_INPUT 128

/* forward declaration for integer print helper */
static void serial_putint(int v);

/* simple test process (top-level, not nested) */
static void test_process(void) {
    serial_puts("Hello from test process!\n");
}

/* process for testing scheduler quantum */
static void quantum_process(void) {
    serial_puts("Quantum process executing\n");
}

/* IPC test processes */
static void sender_process(void) {
    serial_puts("Sender: sending messages...\n");
    ipc_send(1, 100);
    ipc_send(1, 200);
    ipc_send(1, 300);
}

static void receiver_process(void) {
    int msg;
    serial_puts("Receiver: receiving messages...\n");

    while (ipc_recv(1, &msg) == 0) {
        serial_puts(" Received msg=");
        serial_putint(msg);
        serial_puts("\n");
    }
}

/* helper: print a non-negative integer to serial */
static void serial_putint(int v) {
    char buf[12];
    int i = 0;
    if (v == 0) {
        buf[i++] = '0';
    } else {
        int n = v;
        char tmp[12];
        int ti = 0;
        while (n > 0 && ti < (int)sizeof(tmp)) {
            tmp[ti++] = '0' + (n % 10);
            n /= 10;
        }
        while (ti > 0) buf[i++] = tmp[--ti];
    }
    buf[i] = '\0';
    serial_puts(buf);
}

void kmain(void) {
    char input[MAX_INPUT];
    int pos = 0;
    void* p1;
    void* p2;
    
    /* Initialize hardware */
    serial_init();

    /* Initialize memory manager */
    memory_init();

    /* ===== Process test (temporary until scheduler arrives) ===== */
    process_init();

    int pid = process_create(test_process);
    if (pid >= 0) {
        serial_puts("Process created successfully\n");
    }

    /* ===== Extended process tests ===== */
    /* Create multiple processes up to MAX_PROCESSES, check overflow */
    serial_puts("Creating multiple test processes...\n");
    int created = 0;
    int pids[MAX_PROCESSES];
    for (int i = 0; i < MAX_PROCESSES; i++) {
        int r = process_create(test_process);
        if (r >= 0) {
            serial_puts(" created pid="); serial_putint(r); serial_puts("\n");
            pids[created++] = r;
        } else {
            serial_puts(" failed to create (process table full)\n");
        }
    }

    /* attempt one more - should fail */
    int extra = process_create(test_process);
    if (extra < 0) {
        serial_puts("Expected failure when creating extra process.\n");
    } else {
        serial_puts("Unexpectedly created extra pid="); serial_putint(extra); serial_puts("\n");
    }

    /* Run all ready processes sequentially (temporary scheduler) */
    serial_puts("Running ready processes (manual runner)...\n");
    process_t* p;
    while ((p = get_ready_process()) != 0) {
        serial_puts(" Running pid="); serial_putint(p->pid); serial_puts("\n");
        process_set_state(p->pid, PROC_CURRENT);
        p->entry();
        process_terminate(p->pid);
    }
    serial_puts("Finished running ready processes (manual)\n");

    /* ===== Scheduler tests (Round Robin + Aging) ===== */
    /* Re-create processes to test scheduler if none exist */
    serial_puts("Re-creating processes for scheduler test...\n");
    int sched_pids[MAX_PROCESSES];
    int sched_count = 0;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        int r = process_create(test_process);
        if (r >= 0) {
            sched_pids[sched_count++] = r;
        }
    }

    /* assign varying priorities to demonstrate aging */
    for (int i = 0; i < sched_count; i++) {
        process_t* pp = get_process_by_pid(sched_pids[i]);
        if (pp) pp->priority = i % 3; /* priorities: 0,1,2,... */
    }

    scheduler_init(1); /* quantum=1 */
    serial_puts("Running scheduler (schedule())...\n");
    while (get_ready_process() != 0) {
        schedule();
    }
    serial_puts("Scheduler run complete\n");
    /* ===== End scheduler tests ===== */
    /* ===== End extended tests ===== */

    /* ===== Process state-transition tests ===== */
    serial_puts("Testing process state transitions...\n");
    int s_pid = process_create(test_process);
    if (s_pid < 0) {
        serial_puts("Failed to create process for state-test\n");
    } else {
        serial_puts(" created pid="); serial_putint(s_pid); serial_puts("\n");

        process_set_state(s_pid, PROC_NEW);
        serial_puts(" State -> NEW\n");

        process_set_state(s_pid, PROC_WAITING);
        serial_puts(" State -> WAITING\n");

        process_set_state(s_pid, PROC_READY);
        serial_puts(" State -> READY\n");

        process_t* sp = get_process_by_pid(s_pid);
        if (sp) {
            serial_puts(" get_process_by_pid OK pid="); serial_putint(sp->pid); serial_puts("\n");
        } else {
            serial_puts(" get_process_by_pid returned NULL\n");
        }

        process_terminate(s_pid);
        sp = get_process_by_pid(s_pid);
        if (!sp) serial_puts(" Process terminated successfully\n");
        else serial_puts(" Process still present after terminate\n");
    }
    /* ===== End state-transition tests ===== */

        /* ===== IPC tests ===== */
        ipc_init();
        int recv_pid = process_create(receiver_process);
        int send_pid = process_create(sender_process);
        if (recv_pid >= 0 && send_pid >= 0) {
            serial_puts("IPC processes created (recv="); serial_putint(recv_pid); serial_puts(", send="); serial_putint(send_pid); serial_puts(")\n");
        }

        /* temporary sequential execution for IPC */
        process_t* ipc_p;
        while ((ipc_p = get_ready_process()) != 0) {
            process_set_state(ipc_p->pid, PROC_CURRENT);
            ipc_p->entry();
            process_terminate(ipc_p->pid);
        }
        serial_puts("IPC tests complete\n");
        /* ===== End IPC tests ===== */

    /* ===== Memory Manager Tests ===== */
    serial_puts("Running memory + stack tests...\n");

    /* Simple allocation/deallocation */
    p1 = kmalloc(64);
    p2 = kmalloc(128);
    if (p1 && p2) serial_puts(" Memory allocation successful\n");
    kfree(p1);
    kfree(p2);
    serial_puts(" Basic memory deallocation successful\n");

    /* Coalescing test: allocate three small blocks, free middle and left, then allocate larger block */
    void* a1 = kmalloc(64);
    void* a2 = kmalloc(64);
    void* a3 = kmalloc(64);
    if (a1 && a2 && a3) {
        serial_puts(" Allocated 3 small blocks\n");
        kfree(a2);
        serial_puts(" Freed middle block\n");
        kfree(a1);
        serial_puts(" Freed left block (should coalesce with middle)\n");

        void* big = kmalloc(128);
        if (big) {
            serial_puts(" Coalescing appears to work (large alloc succeeded)\n");
            kfree(big);
        } else {
            serial_puts(" Coalescing failed (large alloc did not succeed)\n");
        }

        kfree(a3);
    } else {
        serial_puts(" Failed to allocate blocks for coalesce test\n");
    }

    /* Stack exhaustion test: allocate stacks until failure */
    void* stacks[32];
    int sc = 0;
    void* s;
    while (sc < 32 && (s = alloc_stack()) != 0) {
        stacks[sc++] = s;
    }
    serial_puts(" Allocated stacks: "); serial_putint(sc); serial_puts("\n");

    for (int i = 0; i < sc; i++) free_stack(stacks[i]);
    serial_puts(" Stack free/reuse test done\n");

    /* Scheduler quantum test: create a process that prints and run scheduler with quantum=2 */
    serial_puts("Scheduler quantum test:\n");
    process_t* qp = 0;
    int qpid = process_create(quantum_process);
    if (qpid >= 0) {
        serial_puts(" Created quantum-test process pid="); serial_putint(qpid); serial_puts("\n");
        scheduler_init(2); /* run entry twice per schedule() */
        /* run one schedule iteration */
        schedule();
        serial_puts(" Scheduler quantum test completed\n");
    } else {
        serial_puts(" Failed to create quantum-test process\n");
    }

    serial_puts("Memory + stack tests complete\n");
    /* ===== End Memory/Stack Tests ===== */


    
    /* Print welcome message */
    serial_puts("\n");
    serial_puts("========================================\n");
    serial_puts("    kacchiOS - Minimal Baremetal OS\n");
    serial_puts("========================================\n");
    serial_puts("Hello from kacchiOS!\n");
    serial_puts("Running null process...\n\n");
    
    /* Main loop - the "null process" */
    while (1) {
        serial_puts("kacchiOS> ");
        pos = 0;
        
        /* Read input line */
        while (1) {
            char c = serial_getc();
            
            /* Handle Enter key */
            if (c == '\r' || c == '\n') {
                input[pos] = '\0';
                serial_puts("\n");
                break;
            }
            /* Handle Backspace */
            else if ((c == '\b' || c == 0x7F) && pos > 0) {
                pos--;
                serial_puts("\b \b");  /* Erase character on screen */
            }
            /* Handle normal characters */
            else if (c >= 32 && c < 127 && pos < MAX_INPUT - 1) {
                input[pos++] = c;
                serial_putc(c);  /* Echo character */
            }
        }
        
        /* Echo back the input */
        if (pos > 0) {
            serial_puts("You typed: ");
            serial_puts(input);
            serial_puts("\n");
        }
    }
    
    /* Should never reach here */
    for (;;) {
        __asm__ volatile ("hlt");
    }
}