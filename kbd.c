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
	print("kbd_enc_read_buf\n", 17);
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
	print("kbd_wait\n", 9);
	while(inb(KBD_CTRL_PORT) & 2);
}

static void kbd_callback(struct registers regs) {
	print("kbd_callback\n", 13);
	uint8_t code;
	kbd_wait();
	code = kbd_enc_read_buf();

	if (code & 0x80) { //top bit means key released
		//check for alt, shift, etc
		print("up!\n", 4);
		code -= 0x80;

	} else {
		print("down!\n", 6);

		/*
		print(kbdus[code], 1);
		print("\n", 1);

		switch(code) {
			case 0x2:
				print("set 1: 0x2 = 1\n", 15);
			case 0x16:
				print("set 2: 0x16 = 1\n", 16);
		}
		*/

	}

	uint32_t key = kbd_scancode_std[code];
	print(&key, 1);

/*
	switch(code) {
		case '\x20':
		case '\xa0':
			print("key: d\n", 7);
	}
*/

	//print(code, 1);
	//print(code & 0x80, 1);
	//print(c, 1);
	//print(" \n", 2);
}

void init_kbd() {
	//register the callback
	register_interrupt_handler(IRQ1, &kbd_callback);
/*
	outb(0x60, 0xf0);

	char c = 0;
	int set = -5;

	while(1) {
		c = inb(0x60);
		if(c == 0xfa || c == 0xfe) {
			//outb(0x60, 0x0);
			//set = inb(0x60);
			goto done;
		}
	}

done:
	print("done\n", 5);
	if(set != -5) {
		print("set: ", 5);
		print(set, 1);
		print("\n", 1);
	}

*/
	outb(0x21, 0xfd); //1111 1101
	outb(0xa1, 0xff); //1111 1111
	asm("sti");

}
