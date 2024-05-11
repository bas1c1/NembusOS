#include "../include/gdt.h"
#include "../include/tss.h"

struct gdt_entry gdt[3];
struct gdt_ptr gp;

extern void gdt_flush();
extern void tss_flush();

tss_entry_t tss_entry;

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

static void write_tss(int32_t num, uint16_t ss0, uint32_t esp0);

void gdt_install()
{
    gp.limit = (sizeof(struct gdt_entry) * 6) - 1;
    gp.base = &gdt;

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
    write_tss(5, 0x10, 0x0);

    gdt_flush();
    tss_flush();
}

static void write_tss(int32_t num, uint16_t ss0, uint32_t esp0)
{
    uint32_t base = (uint32_t) &tss_entry;
    uint32_t limit = base + sizeof(tss_entry);
    
    gdt_set_gate(num, base, limit, 0xE9, 0x00);

    memset((uint8_t *)&tss_entry, 0, sizeof(tss_entry));

    tss_entry.ss0  = ss0;
    tss_entry.esp0 = esp0;
    
    tss_entry.cs   = 0x0b;     
    tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
}

void set_kernel_stack(uint32_t stack)
{
    tss_entry.esp0 = stack;
}

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load();

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags | 0x60;
}

void idt_install()
{
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = &idt;
    memset(&idt, 0, sizeof(struct idt_entry) * 256);
    idt_load();
}