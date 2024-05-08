#include "../include/paging.h"

uint32_t *frames;
uint32_t nframes;

extern void copy_page_physical(int source, int dest);

//defined in kheap.c
extern uint32_t placement_address;
extern heap_t *kheap;

page_directory_t* kernel_directory;

page_directory_t* current_directory;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

// Static function to set a bit in the frames bitset
static void set_frame(uint32_t frame_addr)
{
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
static void clear_frame(uint32_t frame_addr)
{
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   frames[idx] &= ~(0x1 << off);
}

// Static function to test if a bit is set.
static uint32_t test_frame(uint32_t frame_addr)
{
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   return (frames[idx] & (0x1 << off));
}

// Static function to find the first free frame.
static uint32_t first_frame()
{
   uint32_t i, j;
   for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
   {
       if (frames[i] != 0xFFFFFFFF)
       {
           for (j = 0; j < 32; j++)
           {
               uint32_t toTest = 0x1 << j;
               if ( !(frames[i]&toTest) )
               {
                   return i*4*8+j;
               }
           }
       }
   }
}

// Function to allocate a frame.
void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
   if (page->frame != 0)
   {
       return;
   }
   else
   {
       uint32_t idx = first_frame();
       if (idx == (uint32_t)-1)
       {
           PANIC("No free frames!");
       }
       set_frame(idx*0x1000);
       page->present = 1;
       page->rw = (is_writeable)?1:0; // Should the page be writeable?
       page->user = (is_kernel)?0:1; // Should the page be user-mode?
       page->frame = idx;
   }
}

// Function to deallocate a frame.
void free_frame(page_t *page)
{
   uint32_t frame;
   if (!(frame=page->frame))
   {
       return;
   }
   else
   {
       clear_frame(frame);
       page->frame = 0x0; // Page now doesn't have a frame.
   }
}

void initialise_paging()
{
   uint32_t mem_end_page = 0x1000000;

   nframes = mem_end_page / 0x1000;
   frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));
   memset(frames, 0, INDEX_FROM_BIT(nframes));

   kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
   memset(kernel_directory, 0, sizeof(page_directory_t));
   kernel_directory->physicalAddr = (uint32_t)kernel_directory->tablesPhysical;
   current_directory = kernel_directory;

   for (int i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
      get_page(i, 1, kernel_directory);

   int i = 0;
   while (i < 0x400000)
   {
       // Kernel code is readable but not writeable from userspace.
       alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
       i += 0x1000;
   }

   for (int i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
       alloc_frame( get_page(i, 1, kernel_directory), 0, 0);

   irq_install_handler(14, page_fault);
   switch_page_directory(kernel_directory);

   kheap = create_heap(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);

   current_directory = clone_directory(kernel_directory);
   switch_page_directory(current_directory);
   
   /*
	uint32_t **dir = (uint32_t **)kmalloc_a(4096);
	uint32_t *table = (uint32_t *)kmalloc_a(4096);

	memset(dir, 0, 4096);
	memset(table, 0, 4096);


	uint32_t i = 0;  // current page descriptor within the table
	uint32_t addr = 0;  // current address
	while (i < 1024)
	{
		table[i] = addr | 3;  // RW + Supervisor + Present
		addr += 4096;
		i++;
	}

	irq_install_handler(14, page_fault);

	dir[0] = (uint32_t *)((uint32_t)table | 3); // RW + Supervisor + Present

	asm volatile("mov %0, %%cr3":: "r"(dir));
	uint32_t cr0;
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000; // Enable paging!
	asm volatile("mov %0, %%cr0":: "r"(cr0));
	*/
}

void switch_page_directory(page_directory_t *dir)
{
   current_directory = dir;

   asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
   uint32_t cr0;
   asm volatile("mov %%cr0, %0": "=r"(cr0));
   cr0 |= 0x80000000; // Enable paging!
   asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(uint32_t address, int make, page_directory_t *dir)
{
   address /= 0x1000;
   uint32_t table_idx = address / 1024;
   if (dir->tables[table_idx])
   {
       return &dir->tables[table_idx]->pages[address%1024];
   }
   else if(make)
   {
       uint32_t tmp;
       dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
       memset(dir->tables[table_idx], 0, 0x1000);
       dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
       return &dir->tables[table_idx]->pages[address%1024];
   }
   else
   {
       return 0;
   }
}

void page_fault(struct regs *regs_var)
{
   uint32_t faulting_address;
   asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

   int present   = !(regs_var->err_code & 0x1); // Page not present
   int rw = regs_var->err_code & 0x2;           // Write operation?
   int us = regs_var->err_code & 0x4;           // Processor was in user-mode?
   int reserved = regs_var->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
   int id = regs_var->err_code & 0x10;          // Caused by an instruction fetch?

   newline();
   printf("Page fault! ( \0");
   if (present) {printf("present \0");}
   if (rw) {printf("read-only \0");}
   if (us) {printf("user-mode \0");}
   if (reserved) {printf("reserved \0");}
   printf(") at \0");
   printfhex(faulting_address);
   printf("\n");
   PANIC("Page fault\0");
}

static page_table_t *clone_table(page_table_t *src, uint32_t *physAddr)
{
    page_table_t *table = (page_table_t*)kmalloc_ap(sizeof(page_table_t), physAddr);
    memset((uint8_t *)table, 0, sizeof(page_directory_t));

    int i;
    for (i = 0; i < 1024; i++)
    {
        if (!src->pages[i].frame)
            continue;

        alloc_frame(&table->pages[i], 0, 0);

        if (src->pages[i].present) table->pages[i].present = 1;
        if (src->pages[i].rw)      table->pages[i].rw = 1;
        if (src->pages[i].user)    table->pages[i].user = 1;
        if (src->pages[i].accessed)table->pages[i].accessed = 1;
        if (src->pages[i].dirty)   table->pages[i].dirty = 1;

        copy_page_physical(src->pages[i].frame*0x1000, table->pages[i].frame*0x1000);
    }
    return table;
}

page_directory_t *clone_directory(page_directory_t *src)
{
    uint32_t phys;
    page_directory_t *dir = (page_directory_t*)kmalloc_ap(sizeof(page_directory_t), &phys);
    memset((uint8_t *)dir, 0, sizeof(page_directory_t));

    uint32_t offset = (uint32_t)dir->tablesPhysical - (uint32_t)dir;

    dir->physicalAddr = phys + offset;

    int i;
    for (i = 0; i < 1024; i++)
    {
        if (!src->tables[i])
            continue;

        if (kernel_directory->tables[i] == src->tables[i])
        {
            dir->tables[i] = src->tables[i];
            dir->tablesPhysical[i] = src->tablesPhysical[i];
        }
        else
        {
            // Copy the table.
            uint32_t phys;
            dir->tables[i] = clone_table(src->tables[i], &phys);
            dir->tablesPhysical[i] = phys | 0x07;
        }
    }
    return dir;
}