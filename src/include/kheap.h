#ifndef KHEAP_H
#define KHEAP_H

#include "stddef.h"
#include "ordered_array.h"
#include "assert.h"

uint32_t kmalloc_a(uint32_t sz);  // page aligned.
uint32_t kmalloc_p(uint32_t sz, uint32_t *phys); // returns a physical address.
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys); // page aligned and returns a physical address.
uint32_t kmalloc(uint32_t sz); // vanilla (normal).

#define KHEAP_START         0xC0000000
#define KHEAP_INITIAL_SIZE  0x100000
#define HEAP_INDEX_SIZE   0x20000
#define HEAP_MAGIC        0x123890AB
#define HEAP_MIN_SIZE     0x70000

typedef struct
{
   uint32_t magic;   // Magic number, used for error checking and identification.
   uint8_t is_hole;   // 1 if this is a hole. 0 if this is a block.
   uint32_t size;    // size of the block, including the end footer.
} header_t;

typedef struct
{
   uint32_t magic;     // Magic number, same as in header_t.
   header_t *header; // Pointer to the block header.
} footer_t;

typedef struct
{
   ordered_array_t index;
   uint32_t start_address; // The start of our allocated space.
   uint32_t end_address;   // The end of our allocated space. May be expanded up to max_address.
   uint32_t max_address;   // The maximum address the heap can be expanded to.
   uint8_t supervisor;     // Should extra pages requested by us be mapped as supervisor-only?
   uint8_t readonly;       // Should extra pages requested by us be mapped as read-only?
} heap_t;

heap_t *create_heap(uint32_t start, uint32_t end, uint32_t max, uint8_t supervisor, uint8_t readonly);
void *alloc(uint32_t size, uint8_t page_align, heap_t *heap);
void free(void *p, heap_t *heap);
void kfree(void *p);

#endif