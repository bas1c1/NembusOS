#ifndef VGA_H
#define VGA_H

#define VGA_LINES 25
#define VGA_COLUMNS_IN_LINE 80
#define VGA_BYTES_FOR_EACH_ELEMENT 2
#define VGA_SCREENSIZE VGA_BYTES_FOR_EACH_ELEMENT * VGA_COLUMNS_IN_LINE * VGA_LINES
#define VGA_DEFAULT_COLOR 0x07

enum vga_colour {
    VGA_COLOUR_BLACK,
    VGA_COLOUR_BLUE,
    VGA_COLOUR_GREEN,
    VGA_COLOUR_CYAN,
    VGA_COLOUR_RED,
    VGA_COLOUR_MAGENTA,
    VGA_COLOUR_BROWN,
    VGA_COLOUR_LIGHT_GREY,
    VGA_COLOUR_DARK_GREY,
    VGA_COLOUR_LIGHT_BLUE,
    VGA_COLOUR_LIGHT_GREEN,
    VGA_COLOUR_LIGHT_CYAN,
    VGA_COLOUR_LIGHT_RED,
    VGA_COLOUR_LIGHT_MAGENTA,
    VGA_COLOUR_LIGHT_BROWN,
    VGA_COLOUR_WHITE,
};

#include "stddef.h"
#include "io.h"

static inline uint8_t vga_entry_colour(enum vga_colour foreground, enum vga_colour background){
    return foreground | (background << 4);
}

static inline uint16_t vga_entry(unsigned char c, uint8_t colour){
    return (uint16_t) c | ((uint16_t) colour << 8);
}

void clear_line(unsigned char *p);
void scroll_up();
void putchar(char ch);
void newline();
void printf(char *message);
void printfhex(uint32_t n);
void cls();
void delsym();
void putcharat(char c, size_t x, size_t y);
void vga_set_cursor_pos(int64_t x, int64_t y);
void vga_disable_cursor();
void scroll();

#endif