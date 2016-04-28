#include <stdint.h>
#include "isr.h"
#include "io.h"
#include "vga.h"
#include "kbd.h"

static uint8_t kbd_ctrl_read_status() {
	return inb(KBD_CTRL_PORT);
}

static void kbd_ctrl_send_cmd(uint8_t cmd) {
	while(1) {
		if ((kbd_ctrl_read_status() & KBD_CTRL_MASK_IN_BUF) == 0)
			break;
	}

	outb(KBD_CTRL_PORT, cmd);
}

static uint8_t kbd_enc_read_buf() {
	//print("kbd_enc_read_buf\n", 17);
	return inb(KBD_ENC_PORT);
}

static void kbd_enc_send_cmd(uint8_t cmd) {
	while(1) {
		if ((kbd_ctrl_read_status() & KBD_CTRL_MASK_IN_BUF) == 0)
			break;
	}

	outb(KBD_ENC_PORT, cmd);
}

static void kbd_wait() {
	//print("kbd_wait\n", 9);
	while(inb(KBD_CTRL_PORT) & 2);
}

static void kbd_callback(struct registers regs) {
	//print("kbd_callback\n", 13);
	uint8_t code;
	kbd_wait();
	code = kbd_enc_read_buf();

	if (code & 0x80) { //top bit means key released
		//check for alt, shift, etc
		code -= 0x80;
	}

	uint32_t key = kbd_scancode_std[code];
	print(&key, 1);
}

void init_kbd() {
	//register the callback
	register_interrupt_handler(IRQ1, &kbd_callback);

	outb(0x21, 0xfd); //1111 1101
	outb(0xa1, 0xff); //1111 1111
	asm("sti");

}
