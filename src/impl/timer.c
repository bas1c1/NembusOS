#include "../include/timer.h"
#include "../include/isr.h"
#include "../include/vga.h"
#include "../include/gdt.h"
#include "../include/task.h"

int timer_ticks = 0;

void timer_handler(struct regs *r)
{
    timer_ticks++;
    switch_task();
}

void timer_wait(int ticks)
{
    unsigned long eticks;

    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks);
}

void timer_install()
{
    irq_install_handler(0, timer_handler);
}