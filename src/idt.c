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
extern void isr_wrapper (void);
extern void isr0  (void);
extern void isr1  (void);
extern void isr2  (void);
extern void isr3  (void);
extern void isr4  (void);
extern void isr5  (void);
extern void isr6  (void);
extern void isr7  (void);
extern void isr8  (void);
extern void isr9  (void);
extern void isr10 (void);
extern void isr11 (void);
extern void isr12 (void);
extern void isr13 (void);
extern void isr14 (void);
extern void isr15 (void);
extern void isr16 (void);
extern void isr17 (void);
extern void isr18 (void);
extern void isr19 (void);
extern void isr20 (void);
extern void isr21 (void);
extern void isr22 (void);
extern void isr23 (void);
extern void isr24 (void);
extern void isr25 (void);
extern void isr26 (void);
extern void isr27 (void);
extern void isr28 (void);
extern void isr29 (void);
extern void isr30 (void);
extern void isr31 (void);


// IDT globals
idt_segment		idt[256];
idt_descriptor	idt_ptr;


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
	idt_define_segment(0,  (uint32_t)&isr0,  0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(1,  (uint32_t)&isr1,  0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(2,  (uint32_t)&isr2,  0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(3,  (uint32_t)&isr3,  0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(4,  (uint32_t)&isr4,  0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(5,  (uint32_t)&isr5,  0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(6,  (uint32_t)&isr6,  0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(7,  (uint32_t)&isr7,  0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(8,  (uint32_t)&isr8,  0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(9,  (uint32_t)&isr9,  0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(10, (uint32_t)&isr10, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(11, (uint32_t)&isr11, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(12, (uint32_t)&isr12, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(13, (uint32_t)&isr13, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(14, (uint32_t)&isr14, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(15, (uint32_t)&isr15, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(16, (uint32_t)&isr16, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(17, (uint32_t)&isr17, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(18, (uint32_t)&isr18, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(19, (uint32_t)&isr19, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(20, (uint32_t)&isr20, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(21, (uint32_t)&isr21, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(22, (uint32_t)&isr22, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(23, (uint32_t)&isr23, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(24, (uint32_t)&isr24, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(25, (uint32_t)&isr25, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(26, (uint32_t)&isr26, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(27, (uint32_t)&isr27, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(28, (uint32_t)&isr28, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(29, (uint32_t)&isr29, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(30, (uint32_t)&isr30, 0x0008, DEFAULT_PERM, INT_32BIT);
	idt_define_segment(31, (uint32_t)&isr31, 0x0008, DEFAULT_PERM, INT_32BIT);

	// Initialize table with external ASM function
	idt_flush((uint32_t)&idt_ptr);
}
