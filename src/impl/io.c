#include "../include/io.h"

void outportb(unsigned short port, uint8_t val) {
    asm volatile("outb %1, %0" :: "dN"(port), "a"(val));
}

uint16_t inports(uint16_t port) {
    uint16_t rv;
    asm volatile ("inw %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}