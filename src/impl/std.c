#include "../include/string.h"

void sleep(uint32_t timer_count)
{
	while(true){
		asm volatile("nop");
		timer_count--;
		if(timer_count <= 0)
	  		break;
	}
}