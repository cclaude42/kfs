#include "kernel.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

void terminal_initialize (void);
void gdt_initialize (void);
void printk (const char *s, ...);

void kernel_main(void) 
{
	/* Initialize terminal interface */
	terminal_initialize();

	/* Initialize Global Descriptor Table */
	gdt_initialize();

	/* Initialize Interrupt Descriptor Table */
	idt_initialize();

	/* Print data to screen */
	printk("42\n");

	asm volatile ("int $0x3");
	asm volatile ("int $0x4"); 
}
