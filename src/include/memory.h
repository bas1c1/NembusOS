#ifndef MEMORY_H 
#define MEMORY_H

#include "stddef.h"

void *memset(void *s, int c, unsigned int len);
void memcpy(void *dest, void *src, size_t n);

#define heap_size 25000

typedef struct memory{
    bool isFree;// memory block is free or not
    int size;//size of the memory block
    struct memory* next;//address of the next memory block
}memory; 

void *malloc(int size);
void m_free(void *ptr);

#endif