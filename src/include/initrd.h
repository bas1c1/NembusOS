#ifndef INITRD_H
#define INITRD_H

#include "stddef.h"
#include "kheap.h"
#include "vfs.h"

typedef struct
{
   uint32_t nfiles;
} initrd_header_t;

typedef struct
{
   uint8_t magic;     // Magic number, for error checking.
   char name[64];  // Filename.
   uint32_t offset;   // Offset in the initrd that the file starts.
   uint32_t length;   // Length of the file.
} initrd_file_header_t;

fs_node_t *initialise_initrd(uint32_t location);

#endif