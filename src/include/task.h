#ifndef TASK_H
#define TASK_H

#include "io.h"
#include "kheap.h"
#include "paging.h"
#include "memory.h"
#include "tss.h"
#include "gdt.h"

#define KERNEL_STACK_SIZE 2048

typedef struct task
{
    int id;                // Process ID.
    uint32_t esp, ebp;       // Stack and base pointers.
    uint32_t eip;            // Instruction pointer.
    page_directory_t *page_directory; // Page directory.
    uint32_t kernel_stack;   // Kernel stack location.
    struct task *next;     // The next task in a linked list.
} task_t;

void initialise_tasking();
int fork();
void move_stack(void *new_stack_start, uint32_t size);
int getpid();
//void switch_to_user_mode();

#endif