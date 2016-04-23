#include "isr.h"
#include "io.h"
#include "vga.h"

static void kbd_callback(struct registers regs) {
	print("Key: ", 5);
	char c = inb(0x60);
	print(c, 1);
	print(" \n", 2);
}

void init_kbd() {
	//register the callback
	register_interrupt_handler(IRQ1, &kbd_callback);

	outb(0x21, 0xfd);
	outb(0xa1, 0xff);
	asm("sti");
}
