#include "kheap.h"
#include <stdint.h>
#include <stddef.h>

#define PG_BIT_ON 0x80000000

uint32_t placement_addr;

uint32_t kmalloc_a(uint32_t size) {
	return _kmalloc_int(size, NULL, 1);
}
uint32_t kmalloc_p(uint32_t size, uint32_t * phys) {
	return _kmalloc_int(size, phys, NULL);
}
uint32_t kmalloc_ap(uint32_t size, uint32_t * phys) {
	return _kmalloc_int(size, phys, 1);
}
uint32_t kmalloc(uint32_t size) {
	return _kmalloc_int(size, NULL, NULL);
}

static uint32_t _kmalloc_int(uint32_t size, uint32_t * phys, uint8_t align) {
	// Page-align it, if needed. The & operation
	// verifies the bottom 12 bits are 0, which is
	// the case for memory addresses on a 4KB boundary
	if (align == 1 && (placement_addr & 0xfffff000)) {
		placement_addr &= 0xfffff000;
		placement_addr += 0x1000;
	}

	// Track the physical address, in addition to the virtual,
	// for the sake of being able to clone a page directory
	if (phys) {
		*phys = placement_addr;
	}

	uint32_t temp = placement_addr;
	placement_addr += size;
	return temp;
}
