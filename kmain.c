#include "vga.h"
#include "serial.h"
#include "stddef.h"
#include "descriptor_tables.h"

int kmain(void) {
	int x = 0xdeadbeef;
	vga_buf_init();

	print("sraboyos v0.1\n", 14);
	print("welcome!\n", 9);

	init_descriptor_tables();
	print("Descriptor Tables Loaded!\n", 26);

	//test_scroll();
	//test_serial_write();
	//test_interrupts();
	init_timer(50); //about 5ms

	return x;
}

void test_interrupts() {
	asm volatile ("int $0x3");
	asm volatile ("int $0x4");
}

void test_serial_write() {
	uint16_t port = 0x3f8;
	serial_conf_baud_rate(port, 3);
	serial_conf_line(port, 0x3);
	serial_conf_buff(port, 0xc7);
	serial_conf_modem(port, 0xb);

	uint8_t * stuff = "stuff";
	serial_write(port, stuff, 5);
}

void test_scroll() {
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
	print("28\n", 3);
}

