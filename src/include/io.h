#ifndef IO_H
#define IO_H

#include "stddef.h"

void outportb(unsigned short port, uint8_t val);
uint16_t inports(uint16_t port);

#define outb_p(value,port) \
__asm__ ("outb %%al,%%dx\n" \
                "\tjmp 1f\n" \
                "1:\tjmp 1f\n" \
                "1:"::"a" (value),"d" (port))

#define inb_p(port) ({ \
unsigned char _v; \
__asm__ volatile ("inb %%dx,%%al\n" \
        "\tjmp 1f\n" \
        "1:\tjmp 1f\n" \
        "1:":"=a" (_v):"d" (port)); \
_v; \
})

#define outb(value,port) \
__asm__ ("outb %%al,%%dx"::"a" (value),"d" (port))


#define inb(port) ({ \
unsigned char _v; \
__asm__ volatile ("inb %%dx,%%al":"=a" (_v):"d" (port)); \
_v; \
})

#endif