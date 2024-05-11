#ifndef ISR_H
#define ISR_H

#include "io.h"
#include "gdt.h"
#include "vga.h"
#include "memory.h"
#include "string.h"
#include "timer.h"

struct regs
{
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;    
};

void isrs_install();
void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
void irq_install();

#endif