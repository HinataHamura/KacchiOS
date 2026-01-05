#include "memory.h"

/* =======================
   CONFIGURATION
   ======================= */

#define HEAP_SIZE  0x20000      // 128 KB heap

/* The linker defines `__kernel_end` at the end of the kernel image.
    Place the heap immediately after the kernel to avoid overlapping kernel sections. */
extern uint32_t __kernel_end;
#define HEAP_START ((uint32_t)&__kernel_end)

#define STACK_SIZE 4096         // 4 KB per stack
#define MAX_STACKS 16           // max processes

/* =======================
   HEAP STRUCTURE
   ======================= */

typedef struct block {
    uint32_t size;               // usable size
    uint8_t  free;               // 1 = free, 0 = used
    struct block* next;
} block_t;

static block_t* free_list;

/* =======================
   STACK STRUCTURE
   ======================= */

static uint8_t stack_pool[MAX_STACKS][STACK_SIZE];
static uint8_t stack_used[MAX_STACKS];

/* =======================
   MEMORY INIT
   ======================= */

void memory_init(void) {
    /* Initialize heap as one big free block */
    free_list = (block_t*)HEAP_START;
    free_list->size = HEAP_SIZE - sizeof(block_t);
    free_list->free = 1;
    free_list->next = 0;

    /* Initialize stack usage table */
    for (int i = 0; i < MAX_STACKS; i++)
        stack_used[i] = 0;
}

/* =======================
   HEAP ALLOCATION
   Optimized First-Fit
   ======================= */

void* kmalloc(uint32_t size) {
    /* Align size to 4 bytes for safety */
    uint32_t asize = (size + 3) & ~3;

    block_t* curr = free_list;
    block_t* prev = 0;

    /* First-fit with splitting */
    while (curr) {
        if (curr->free && curr->size >= asize) {

            /* Split block if large enough (optimization) */
            if (curr->size >= asize + sizeof(block_t) + 4) {
                block_t* new_block =
                    (block_t*)((uint8_t*)(curr + 1) + asize);

                new_block->size = curr->size - asize - sizeof(block_t);
                new_block->free = 1;
                new_block->next = curr->next;

                curr->next = new_block;
                curr->size = asize;
            }

            curr->free = 0;
            return (void*)(curr + 1); // usable memory
        }

        prev = curr;
        curr = curr->next;
    }

    return 0; // allocation failed
}

/* =======================
   HEAP DEALLOCATION
   ======================= */

void kfree(void* ptr) {
    if (!ptr) return;

    block_t* block = ((block_t*)ptr) - 1;
    block->free = 1;

    /* Coalesce with next block if it's free */
    if (block->next && block->next->free) {
        block->size += sizeof(block_t) + block->next->size;
        block->next = block->next->next;
    }

    /* Coalesce with previous block by walking from head */
    block_t* curr = free_list;
    block_t* prev = 0;
    while (curr && curr != block) {
        prev = curr;
        curr = curr->next;
    }

    if (prev && prev->free) {
        prev->size += sizeof(block_t) + block->size;
        prev->next = block->next;
    }
}

/* =======================
   STACK ALLOCATION
   ======================= */

void* alloc_stack(void) {
    for (int i = 0; i < MAX_STACKS; i++) {
        if (!stack_used[i]) {
            stack_used[i] = 1;
            return stack_pool[i] + STACK_SIZE; // top of stack
        }
    }
    return 0; // no stack available
}

/* =======================
   STACK DEALLOCATION
   ======================= */

void free_stack(void* stack) {
    for (int i = 0; i < MAX_STACKS; i++) {
        if (stack == stack_pool[i] + STACK_SIZE) {
            stack_used[i] = 0;
            return;
        }
    }
}
