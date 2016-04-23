#include "timer.h"
#include "isr.h"
#include "vga.h"

uint32_t tick = 0;

static void timer_callback(struct registers regs) {
	tick++;
	print("Tick!\n", 6);
}

void init_timer(uint32_t freq) {
	//register the callback
	register_interrupt_handler(IRQ0, &timer_callback);

	//divisor = 1193180 Hz / frequency (in Hz)
	//time interval: 10Hz = 1ms
	uint32_t divisor = 1193180 / freq;

	outb(0x43, 0x36); //cmd byte

	//lower/higher bytes of divisor
	uint8_t l = (uint8_t)(divisor & 0xff);
	uint8_t h = (uint8_t)((divisor >> 8) & 0xff);

	outb(0x40, l);
	outb(0x40, h);
	asm("sti");
}
