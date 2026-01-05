#include "ipc.h"
#include "process.h"

/* Message queues */
static int msg_queue[MAX_PROCS][MAX_IPC_MSG];
static int msg_count[MAX_PROCS];

void ipc_init(void) {
    for (int i = 0; i < MAX_PROCS; i++) {
        msg_count[i] = 0;
    }
}

/* =========================
   Send message
   ========================= */
int ipc_send(int pid, int msg) {
    if (pid < 0 || pid >= MAX_PROCS)
        return -1;

    if (msg_count[pid] >= MAX_IPC_MSG)
        return -1;  /* queue full */

    msg_queue[pid][msg_count[pid]++] = msg;
    return 0;
}

/* =========================
   Receive message
   ========================= */
int ipc_recv(int pid, int* msg) {
    if (pid < 0 || pid >= MAX_PROCS)
        return -1;

    if (msg_count[pid] == 0)
        return -1;  /* no message */

    *msg = msg_queue[pid][0];

    /* shift remaining messages */
    for (int i = 1; i < msg_count[pid]; i++) {
        msg_queue[pid][i - 1] = msg_queue[pid][i];
    }
    msg_count[pid]--;

    return 0;
}
