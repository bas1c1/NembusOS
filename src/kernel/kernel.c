#include "../include/stddef.h"
#include "../include/std.h"
#include "../include/vga.h"
#include "../include/gdt.h"
#include "../include/isr.h"
#include "../include/keyboard.h"
#include "../include/timer.h"
#include "../include/memory.h"
#include "../include/kheap.h"
#include "../include/paging.h"
#include "../include/multiboot.h"
#include "../include/vfs.h"
#include "../include/initrd.h"
#include "../include/task.h"
#include "../include/syscall.h"

//bruh why i defined it
//#define MEGABYTE(N) 1048576*N

extern uint32_t placement_address;

uint32_t initial_esp;

void proc() {
	for (;;) {
    	printf("\nHello from proc ");
    	printfhex(getpid());
    	newline();
    	switch_task();
	}
}

void _kmain(unsigned long magic, unsigned long addr, uint32_t initial_stack)
{
	MULTIBOOT_INFO *mb = (MULTIBOOT_INFO *)addr;
	uint32_t initrd_location = *(uint32_t*)(mb->modules_addr);
	uint32_t initrd_end = *(uint32_t*)(mb->modules_addr+4);
	placement_address = initrd_end;
	ASSERT(mb->modules_count > 0);

	cls();
	vga_set_cursor_pos(0, 0);
	printf("Booting NembusOS!\n\nInitial stack: ");
	printfhex(initial_stack);
	initial_esp = initial_stack;
	newline();
	newline();

	gdt_install();
    idt_install();
    isrs_install();
    irq_install();

    __asm__ __volatile__ ("sti");

    timer_install();
    
	initialise_paging();

	initialise_tasking();
	
	fs_root = initialise_initrd(initrd_location);

	//not working in user mode :(
	create_proc(&proc);
	create_proc(&proc);

	switch_to_user_mode();
	
	syscall_printf("Hello, syscalls!\n");

	for(;;) {
		//asm ("hlt");
	}
}