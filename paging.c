#include "paging.h"
#include <stdint.h>
#include "isr.h"
#include "kernel.h"
#include "kheap.h"

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

extern uint32_t placement_addr; //in kheap.c
struct page_directory * kernel_directory;
struct page_directory * current_directory;

//bitsets for used/free frames
uint32_t *frames;
uint32_t nframes;

//set bit in frames bitset
static void set_frame(uint32_t frame_addr) {
	uint32_t frame = frame_addr / 0x1000;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (0x1 << off);
}

//clear bit in frames bitset
static void clear_frame(uint32_t frame_addr) {
	uint32_t frame = frame_addr / 0x1000;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	frames[idx] &= ~(0x1 << off);
}

//check if bit is set in frames bitset
static uint32_t test_frame(uint32_t frame_addr) {
	uint32_t frame = frame_addr / 0x1000;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	return frames[idx] &= ~(frames[idx] & (0x1 << off));
}

//find first free frame
static uint32_t first_frame() {
	for (uint32_t i = 0; i < INDEX_FROM_BIT(nframes); i++) {
		if(frames[i] != 0xffffffff) { //only if something is free
			for (uint32_t j = 0; j < 32; j++) {
				uint32_t test = 0x1 << j;
				if (!(frames[i] & test)) {
					return i * 4 * 8 + j;
				}
			}
		}
	}

}

void alloc_frame(struct page * page, int is_kernel, int is_writeable) {
	if(page->frame != 0) return; //already allocated
	else {
		uint32_t idx = first_frame();
		if (idx == (uint32_t)-1) {
			PANIC("Out of memory!\n");
		}

		set_frame(idx * 0x1000); //allocated for us
		page->present = 1;
		page->rw = (is_writeable) ? 1 : 0;
		page->user = (is_kernel) ? 0 : 1;
		page->frame = idx;
	}
}

void free_frame(struct page * page) {
	uint32_t frame;

	if(!(frame = page->frame)) return; //it wasn't allocated
	else {
		clear_frame(frame);
		page->frame = NULL;
	}
}

void switch_page_directory(struct page_directory *dir) {
	current_directory = dir;
	asm volatile("mov %0, %%cr3":: "r"(&dir->tables_phys));
	uint32_t cr0;
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000; //enables paging
	asm volatile("mov %0, %%cr0":: "r"(cr0));
}

struct page * get_page(uint32_t addr, int make_if_not_exist, struct page_directory *dir) {
	addr /= 0x1000; 			//turns addr into index
	uint32_t idx = addr / 1024; //find table with this addr

	if (dir->tables[idx]) { //already assigned
		return &dir->tables[idx]->pages[addr % 1024];
	} else if (make_if_not_exist) {
		uint32_t tmp;
		dir->tables[idx] = (struct page_table *)kmalloc_ap(sizeof(struct page_table), &tmp);
		memset(dir->tables[idx], 0, 0x1000);
		dir->tables_phys[idx] = tmp | 0x7; //0111b = Present, RW, user-accessible
		return &dir->tables[idx]->pages[addr % 1024];
	} else return 0;
}

void page_fault(struct registers regs) {
	//fault addr is in CR2 after a page fault exception
	uint32_t fault_addr;
	asm volatile("mov %%cr2, %0" : "=r" (fault_addr));

	//error code has all the dirty details
	int present = !(regs.err_code & 0x1); 	//mark not present
	int rw		= regs.err_code & 0x2;		//was it read or write?
	int us		= regs.err_code & 0x4;		//in user mode?
	int rsvd	= regs.err_code & 0x8;		//did it overwrite reserved bits?
	int id		= regs.err_code & 0x10;		//was it an instruction fetch?

	PANIC("Page fault!\n");
}

void init_paging() {
	//create our bitset
	uint32_t mem_end_page = 0x1000000; //16MB total mem
	nframes = mem_end_page / 0x1000;
	frames = (uint32_t *)kmalloc(INDEX_FROM_BIT(nframes));
	memset(frames, 0, INDEX_FROM_BIT(nframes));

	//allocate page-aligned mem for directory
	uint32_t phys;
	kernel_directory = (struct page_directory *)kmalloc_ap(sizeof(struct page_directory), &phys);
	memset(kernel_directory, 0, sizeof(struct page_directory));
	current_directory = kernel_directory;

	//identity map the kernel
	int i = 0;
	while (i < placement_addr) { //while, not for, since we change placement_addr
		alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
		i += 0x1000;
	}

	register_interrupt_handler(14, page_fault);

	//enable paging
	switch_page_directory(kernel_directory);
}
