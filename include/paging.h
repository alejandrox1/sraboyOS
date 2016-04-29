#ifndef __PAGING_H__
#define __PAGING_H__

#include <stdint.h>
#include "isr.h"

struct page {
	uint32_t present 	: 1; 	//page present in memory
	uint32_t rw 		: 1; 	//read-write if set
	uint32_t user	 	: 1; 	//kernel page if set
	uint32_t accessed 	: 1; 	//accessed since last refresh?
	uint32_t dirty	 	: 1; 	//written since last refresh?
	uint32_t unused 	: 7; 	//random unused bits
	uint32_t frame	 	: 20; 	//frame_addr >> 12
};

struct page_table {
	struct page pages[1024];
};

struct page_directory {
	struct page_table *tables[1024];
	uint32_t tables_phys[1024]; //array of tables' physical addresses
	uint32_t physical_addr; 	//addr of tables_phys
};

void init_paging();
void switch_page_directory(struct page_directory *dir); //loads pagedir into CR3
struct page * get_page(uint32_t addr, int make_if_not_exist, struct page_directory *dir);
void page_fault(struct registers regs);

#endif
