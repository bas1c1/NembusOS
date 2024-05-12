#include "../include/assert.h"

void PANIC(char *message) {
	printf(message);
	newline();
	for(;;) {
		//idk bro
		//asm("hlt");
	}
}