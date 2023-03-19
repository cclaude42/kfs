#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

void terminal_initialize (void);
void gdt_initialize (void);
void idt_initialize (void);
void pic_remap (int offset1, int offset2);
void printk (const char *s, ...);