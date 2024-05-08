#include "../include/assert.h"

void PANIC(char *message) {
	printf(message);
	newline();
	for(;;) {
		asm("hlt");
	}
}