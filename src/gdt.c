#include "kernel.h"

// To set permissions
#define SEG_PRES(x)		(x << 0x07)
#define SEG_PRIV(x)		(x << 0x05)
#define SEG_TYPE(x)		(x << 0x04)
// To set type flags
#define SEG_CODE(x)		(x << 0x03)
#define SEG_CONF(x)		(x << 0x02)
#define SEG_READ(x)		(x << 0x01)
#define SEG_ACCE(x)		(x << 0x00)
// To set other flags
#define SEG_GRAN(x)		(x << 0x03)
#define SEG_32BT(x)		(x << 0x02)
#define SEG_64BT(x)		(x << 0x01)
#define SEG_AVAI(x)		(x << 0x00)

// Permissions
#define SEG_PERM_KERN	SEG_PRES(1) | SEG_PRIV(0) | SEG_TYPE(1)
#define SEG_PERM_USER	SEG_PRES(1) | SEG_PRIV(3) | SEG_TYPE(1)
// Type flags
#define SEG_TYPE_CODE	SEG_CODE(1) | SEG_CONF(0) | SEG_READ(1) | SEG_ACCE(0)
#define SEG_TYPE_DATA	SEG_CODE(0) | SEG_CONF(0) | SEG_READ(1) | SEG_ACCE(0)
#define SEG_TYPE_STCK	SEG_CODE(0) | SEG_CONF(1) | SEG_READ(1) | SEG_ACCE(0)
// Other flags
#define SEG_FLAGS		SEG_GRAN(1) | SEG_32BT(1) | SEG_64BT(0) | SEG_AVAI(0)


// GDT structures
typedef struct s_gdt_segment {
	uint16_t	limit_low;		// first 16 bits of limit
	uint16_t	base_low;		// first 16 bits of base
	uint8_t		base_mid;		// next 8 bits of base
	uint8_t		access;			// present (1 bit), privilege (2 bits), type (1 bit), type flags (4 bits) [code, conforming, readable, accessed]
	uint8_t		limit_high :4;	// last 4 bits of limit
	uint8_t		flags :4;		// other flags (4 bits) [granularity, 32 bits, 64 bits, AVL]
	uint8_t		base_high;		// last 8 bits of base
} __attribute__((packed)) gdt_segment;

typedef struct s_gdt_descriptor {
	uint16_t	limit;			// size of the GDT description (all GDT segments)
	uint32_t	base;			// start of the GDT description (address of segment array)
} __attribute__((packed)) gdt_descriptor;


// Assembler function
extern void gdt_flush (uint32_t gdt_ptr);


// GDT globals
__attribute__((section(".gdt")))
gdt_segment		gdt[7];
gdt_descriptor	gdt_ptr;



void gdt_define_segment (uint16_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags, char *comment)
{
	// Set base (start of segment)
	gdt[index].base_low = base & 0xFFFF;
	gdt[index].base_mid = (base >> 16) & 0xFF;
	gdt[index].base_high = (base >> 24) & 0xFF;

	// Set limit (size of segment)
	gdt[index].limit_low = limit & 0xFFFF;
	gdt[index].limit_high = (limit >> 16) & 0xF;

	// Set access and flags
	gdt[index].access = access;
	gdt[index].flags = flags & 0xF;

	printk("[%s] Set up %s (base %x, limit %x, access %x, flags %x)\n", "GDT", comment, base, limit, access, flags);
}

void gdt_initialize (void)
{
	// Store table address in ptr struct
	gdt_ptr.base = (uint32_t)gdt;
	gdt_ptr.limit = sizeof(gdt) - 1;

	// Define segments
	gdt_define_segment(0, 0, 0, 0, 0, "null segment");
	gdt_define_segment(1, 0x00000000, 0xFFFFF, SEG_PERM_KERN | SEG_TYPE_CODE, SEG_FLAGS, "kernel code ");
	gdt_define_segment(2, 0x00000000, 0xFFFFF, SEG_PERM_KERN | SEG_TYPE_DATA, SEG_FLAGS, "kernel data ");
	gdt_define_segment(3, 0x00000000, 0xFFFFF, SEG_PERM_KERN | SEG_TYPE_STCK, SEG_FLAGS, "kernel stack");
	gdt_define_segment(4, 0x00000000, 0xFFFFF, SEG_PERM_USER | SEG_TYPE_CODE, SEG_FLAGS, "user code   ");
	gdt_define_segment(5, 0x00000000, 0xFFFFF, SEG_PERM_USER | SEG_TYPE_DATA, SEG_FLAGS, "user data   ");
	gdt_define_segment(6, 0x00000000, 0xFFFFF, SEG_PERM_USER | SEG_TYPE_STCK, SEG_FLAGS, "user stack  ");

	// Initialize table with external ASM function
	gdt_flush((uint32_t)&gdt_ptr);
}
