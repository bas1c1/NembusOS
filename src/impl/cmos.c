#include "../include/cmos.h"

int sec, min, hrs, day, month, yr, cent;

void get_time() {
    sec = CMOS_READ(0);
    min = CMOS_READ(2);
    hrs = CMOS_READ(4);
    day = CMOS_READ(7);
    month = CMOS_READ(8);
    yr = CMOS_READ(9);
    cent = CMOS_READ(32);

    if (sec > 60) {
        min += sec / 60;
        sec = sec % 60;
    }
}

int get_sec() { return sec; }
int get_min() { return min; }
int get_hrs() { return hrs; }
int get_day() { return day; }
int get_month() { return month; }
int get_yr() { return yr; }
int get_cent() { return cent; }