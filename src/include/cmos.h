#ifndef CMOS_H 
#define CMOS_H

#include "io.h"

#define CMOS_READ(addr) ({ \
outb_p(0x80|addr,0x70); \
inb_p(0x71); \
})

void get_time();
int get_sec();
int get_min();
int get_hrs();
int get_day();
int get_month();
int get_yr();
int get_cent();

#endif