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

#define MEGABYTE(N) 1048576*N

extern uint32_t placement_address;

uint32_t initial_esp;

void proc() {
	int ret = fork();
	printf("fork() returned ");
   	printfhex(ret);
   	printf(", and getpid() returned ");
   	printfhex(getpid());
   	newline();
   	newline();

   	asm volatile("cli");

    // list the contents of /
	int i = 0;
	struct dirent *node = 0;
	while ( (node = readdir_fs(fs_root, i)) != 0)
	{
	  printf("Found file ");
	  printf(node->name);
	  
	  fs_node_t *fsnode = finddir_fs(fs_root, node->name);

	  if ((fsnode->flags&0x7) == FS_DIRECTORY)
	    printf("\n     (directory)\n");
	  else
	  {
	    printf("\n     contents: \"");
	    char buf[256];
	    uint32_t sz = read_fs(fsnode, 0, 256, buf);
	    int j;
	    for (j = 0; j < sz; j++)
	      putchar(buf[j]);

	    printf("\"\n");
	  }
	  i++;
	}

	asm volatile("sti");
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
	
	//fs_root = initialise_initrd(initrd_location);
	
	//proc();

	//fork();

	syscall_printf("Hello, syscalls!\n");
    

	for(;;) {
		asm ("hlt");
		//syscall_printf("Hello, user world! - pid = ");
		//syscall_printfhex(getpid());
		//syscall_printf("\n");
	}

    //volatile uint32_t *ptr = (uint32_t*)0xA0000000;
    //volatile uint32_t do_page_fault = *ptr;
	/*for(;;) {
	    //asm("hlt");
	    char *a = kmalloc(128);
	    input(a);
	    printf(a);
	    kfree(a);
	    newline();
	}*/
	//input(a);
	//newline();
	//printf(a);
}