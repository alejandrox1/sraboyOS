#ifndef INCLUDE_DESCTABLES_H
#define INCLUDE_DESCTABLES_H

#include <stdint.h>

struct gdt_ptr {
	uint16_t limit;  // size of table minus 1, (last valid addr in table)
	uintptr_t base;  // first entry in table
} __attribute__((packed));


struct idt_ptr {
	uint16_t limit; 		  // size of table minus 1, (last valid addr in table)
	uintptr_t base;  // first entry in table
} __attribute__((packed));

//see http://web.archive.org/web/20120304220556/http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html
struct gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t  base_middle;
	/* Access Byte Format By Bit
	 * 7	- is segment present? 1 = yes.
	 * 6-5	- descriptor privelege level (rings 0-3)
	 * 4	- descriptor type
	 * 3-0	- segment type (code or data)
	 */
	uint8_t  access;
	/* Granularity Byte Format By Bit
	 * 7	- 0 = 1 byte, 1 = 1 kilobyte
	 * 6	- operand size (0 = 16-bit, 1 = 32-bit)
	 * 5	- always 0
	 * 4	- always 0
	 * 3-0	- segment length
	 */
	uint8_t  granularity;
	uint8_t  base_high;
} __attribute__((packed));


struct idt_entry {
	uint16_t base_low;
	uint16_t selector;
	uint8_t  zero;
	/* Flags Byte Format By Bit
	 * 7	- is handler present? 1 = yes.
	 * 6-5	- descriptor privelege level (rings 0-3)
	 * 4-0	- always 0xe (0111b)
	 */
	uint8_t  flags;
	uint16_t base_high;
} __attribute__((packed));

void init_descriptor_tables();

#endif
