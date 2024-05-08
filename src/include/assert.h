#ifndef ASSERT_H
#define ASSERT_H

#include "vga.h"

void PANIC(char *message);
#define ASSERT(b) ((b) ? (void)0 : PANIC("ASSERT FAILED"))

#endif