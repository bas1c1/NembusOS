#ifndef KEYBOARD_H 
#define KEYBOARD_H

#define CALC_SLEEP 50331647
#include "stddef.h"
#include "io.h"
#include "std.h"
#include "kbdef.h"
#include "vga.h"

char get_input_keycode();
void input(char data[]);
char get_ascii_char(uint8_t key_code);

#endif