#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

/* Initialize memory subsystem */
void memory_init(void);

/* Heap allocation */
void* kmalloc(uint32_t size);
void kfree(void* ptr);

/* Stack allocation */
void* alloc_stack(void);
void free_stack(void* stack);

#endif
