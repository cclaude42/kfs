#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

void terminal_initialize (void);
void gdt_initialize (void);
void printk (const char *s, ...);
void t_setcolor (uint8_t color);