#ifndef IPC_H
#define IPC_H

#include "types.h"

#define MAX_IPC_MSG   8     /* messages per process */
#define MAX_PROCS     MAX_PROCESSES

void ipc_init(void);

/* send a message to pid */
int ipc_send(int pid, int msg);

/* receive a message for pid */
int ipc_recv(int pid, int* msg);

#endif

