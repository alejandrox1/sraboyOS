#include "vga.h"

int kmain(void) {
	int x = 0xdeadbeef;
	vga_buf_init();
	print("test", 4);

	return x;
}


