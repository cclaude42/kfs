#include "kernel.h"

// To set permissions
#define SEG_PRES(x)		(x << 0x03)
#define SEG_PRIV(x)		(x << 0x01)
// Gate type
#define TASK_GATE		0x5 // Task gate (offset is unused)
#define INT_16BIT		0x6 // 16-bit Interrupt Gate
#define TRAP_16BIT		0x7 // 16-bit Trap Gate
#define INT_32BIT		0xE // 32-bit Interrupt Gate
#define TRAP_32BIT		0xF // 32-bit Trap Gate

// Permissions
#define DEFAULT_PERM	SEG_PRES(1) | SEG_PRIV(0)


// IDT structures
typedef struct s_idt_segment {
	uint16_t	offset_low;		// first 16 bits of offset
	uint16_t	selector;		// segment selector
	uint8_t		reserved;		// reserved (fill with zero)
	uint8_t		access :4;		// present (1 bit), privilege level (2 bits), 0 (1 bit)
	uint8_t		type :4;		// 4 bits of type
	uint16_t	offset_high;	// last 16 bits of offset
} __attribute__((packed)) idt_segment;

typedef struct s_idt_descriptor {
	uint16_t	limit;			// size of the IDT description (all IDT segments)
	uint32_t	base;			// start of the IDT description (address of segment array)
} __attribute__((packed)) idt_descriptor;


// Assembler function
extern void idt_flush (uint32_t idt_ptr);


// IDT globals
idt_segment		idt[256];
idt_descriptor	idt_ptr;

extern void isr_wrapper (void);


typedef struct registers
{
   uint32_t ds;                  // Data segment selector
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
   uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} __attribute__((packed)) registers_t; 

void isr_handler (registers_t cock)
{
	(void)cock;
	printk("cock and balls\n");
}

void idt_define_segment (uint16_t index, uint32_t offset, uint16_t selector, uint8_t access, uint8_t type)
{
	// Set offset (ISR entrypoint address)
	idt[index].offset_low = offset & 0xFFFF;
	idt[index].offset_high = (offset >> 16) & 0xFFFF;

	// Set selector (points to GDT code segment)
	idt[index].selector = selector;

	// Set access and type
	idt[index].access = access & 0xF;
	idt[index].type = type & 0xF;

	// Set reserved
	idt[index].reserved = 0x0;

	// printk("[%s] Set up segment (offset %x, selector %x, type %x, access %x)\n", "IDT", offset, selector, type, access);
}

void idt_initialize (void)
{
	// Store table address in ptr struct
	idt_ptr.base = (uint32_t)idt;
	idt_ptr.limit = sizeof(idt) - 1;

	// Define segments
	for (uint16_t i = 0 ; i < 256 ; i++)
		idt_define_segment(i, (uint32_t)&isr_wrapper, 0x0008, DEFAULT_PERM, INT_32BIT);

	// Initialize table with external ASM function
	idt_flush((uint32_t)&idt_ptr);
}
