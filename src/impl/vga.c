#include "../include/vga.h"
#include "../include/string.h"

int64_t cursor_x = 0;
int64_t cursor_y = 0;
char *vidmem = (char*)0xb8000;

void cls()
{
	unsigned int i = 0;
	while (i < (VGA_SCREENSIZE))
	{
		vidmem[i] = ' ';
		i++;
		vidmem[i] = VGA_DEFAULT_COLOR;
		i++;
	}
	vidmem[0] = ' ';
	vidmem[1] = VGA_DEFAULT_COLOR;
}

void clear_line(unsigned char *p)
{
	int i;
	for (i = 0; i < VGA_COLUMNS_IN_LINE; i++)
	{
		*p++ = ' ';
		*p++ = VGA_DEFAULT_COLOR;
	}
}

void scroll_up()
{
	memcpy(vidmem, vidmem + (VGA_COLUMNS_IN_LINE * VGA_BYTES_FOR_EACH_ELEMENT), VGA_SCREENSIZE - (VGA_COLUMNS_IN_LINE * VGA_BYTES_FOR_EACH_ELEMENT));
	clear_line((unsigned char *)(vidmem + VGA_SCREENSIZE - (VGA_COLUMNS_IN_LINE * VGA_BYTES_FOR_EACH_ELEMENT)));
}

void putchar(char ch)
{
	unsigned int i = (cursor_y * VGA_COLUMNS_IN_LINE * VGA_BYTES_FOR_EACH_ELEMENT) + cursor_x;
	if (ch == '\n')
	{
		newline();
		return;
	}
	vidmem[i] = ch;
	i++;
	vidmem[i] = VGA_DEFAULT_COLOR;
	//cursor_x += VGA_BYTES_FOR_EACH_ELEMENT;
	//scroll();
	vga_set_cursor_pos(cursor_x + VGA_BYTES_FOR_EACH_ELEMENT, cursor_y);
}

void newline()
{
	if (cursor_y >= VGA_LINES - 1)
	{
		scroll_up();
		vga_set_cursor_pos(0, cursor_y);
	}
	else
	{
		//unsigned int i = (cursor_y * VGA_COLUMNS_IN_LINE * VGA_BYTES_FOR_EACH_ELEMENT) + cursor_x;
		//cursor_y++;
		//cursor_x = 0;
		vga_set_cursor_pos(0, cursor_y + 1);
		//i = (cursor_y * VGA_COLUMNS_IN_LINE * VGA_BYTES_FOR_EACH_ELEMENT);
	}
}

void printf(char *message)
{
	//unsigned int i = (cursor_y * VGA_COLUMNS_IN_LINE * VGA_BYTES_FOR_EACH_ELEMENT);
	/*int j = 0;
	char __temp__char = message[j];

	while (__temp__char != '\0')
	{
		putchar(__temp__char);
		j++;
		__temp__char = message[j];
	}*/
	/*size_t len = strlen(message);
	for (int i = 0; i < len; i++) {
		putchar(message[i]);
	}*/
	int i = 0;
	while (message[i])
	{
	   putchar(message[i++]);
	}
	//free(message);
}

void printfhex(uint32_t n)
{
    int tmp;

    printf("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }
    
        if (tmp >= 0xA)
        {
            noZeroes = 0;
            putchar (tmp-0xA+'a' );
        }
        else
        {
            noZeroes = 0;
            putchar( tmp+'0' );
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        putchar (tmp-0xA+'a');
    }
    else
    {
        putchar (tmp+'0');
    }
}

//this shit has to be called every second
void scroll()
{
   uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
   uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

   // Row 25 is the end, this means we need to scroll up
   if(cursor_y >= 25)
   {
       int i;
       for (i = 0; i < 24*80; i++)
       {
           vidmem[i] = vidmem[i+80];
       }

       for (i = 24*80; i < 25*80; i++)
       {
           vidmem[i] = blank;
       }
       vga_set_cursor_pos(cursor_x, 24);
   }
}

void delete()
{
	unsigned int i = (cursor_y * VGA_COLUMNS_IN_LINE * VGA_BYTES_FOR_EACH_ELEMENT) + cursor_x--;
	vidmem[i] = '\0';

	vga_set_cursor_pos((cursor_x+VGA_BYTES_FOR_EACH_ELEMENT)-2, cursor_y);
}

void delsym()
{
    vga_set_cursor_pos(cursor_x-1, cursor_y);
	delete();
}

void putcharat(char c, size_t x, size_t y)
{
    const size_t index = y * VGA_COLUMNS_IN_LINE + x;
    vidmem[index] = vga_entry(c, VGA_DEFAULT_COLOR);
    vga_set_cursor_pos(x+1, y);
}

void vga_set_cursor_pos(int64_t x, int64_t y) {
	cursor_x = x;
	cursor_y = y;
	// division by 2 is normalizing btw
    uint16_t cursorLocation = (uint16_t)(y * VGA_COLUMNS_IN_LINE + (int64_t)(x / 2));
    outportb(0x3D4, 14);
    outportb(0x3D5, cursorLocation >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, cursorLocation);
}

void vga_disable_cursor() {
    outportb(0x3D4, 10);
    outportb(0x3D5, 32);
}