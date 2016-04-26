#include "paging.h"
#include "stddef.h"
#include "isr.h"
#include "kernel.h"

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

extern uint32_t placement_addr; //in kheap.c

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
			PANIC("Out of memory!\n", 15);
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

void switch_page_directory(struct page_directory *new) {

}

struct page * get_page(uint32_t addr, int make, struct page_directory *dir) {

}

void page_fault(struct registers regs) {

}

void init_paging() {
	uint32_t mem_end_page = 0x10000000; //16MB
	nframes = mem_end_page / 0x1000;
	frames = (uint32_t *)kmalloc(INDEX_FROM_BIT(nframes));
	//kernel_directory = (struct page_directory *)kmalloc_a(sizeof(struct page_directory));
	//memset(kernel_directory, 0, sizeof(struct page_directory));
}
