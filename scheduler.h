#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "types.h"

/* Initialize scheduler with time quantum */
void scheduler_init(int quantum);

/* Run scheduler */
void schedule(void);

#endif
