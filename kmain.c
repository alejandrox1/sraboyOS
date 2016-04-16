#include "vga.h"

int kmain(void) {
	int x = 0xdeadbeef;
	vga_buf_init();

	print("sraboyos v0.1\n", 14);
	print("welcome!\n", 9);
/*
	print("3\n", 2);
	print("4\n", 2);
	print("5\n", 2);
	print("6\n", 2);
	print("7\n", 2);
	print("8\n", 2);
	print("9\n", 2);
	print("10\n", 3);
	print("11\n", 3);
	print("13\n", 3);
	print("scroll", 6);
	print("14\n", 3);
	print("15\n", 3);
	print("16\n", 3);
	print("17\n", 3);
	print("18\n", 3);
	print("19\n", 3);
	print("20\n", 3);
	print("21\n", 3);
	print("22\n", 3);
	print("23\n", 3);
	print("24\n", 3);
	print("25\n", 3);
	print("26\n", 3);
	print("27\n", 3);
	*/
	return x;
}


