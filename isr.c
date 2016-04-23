#include "isr.h"
#include "io.h"
#include "vga.h"

isr_t interrupt_handlers[256];

void irq_handler(struct registers regs) {
    //send EOI (end of interrupt) to PIC(s)

    if (regs.int_no >= 40) { //if slave PIC was used
        outb(0xa0, 0x20);    //send reset/ack to slave
    }

    outb(0x20, 0x20); //send reset/ack to master (and slave, if needed)

    if (interrupt_handlers[regs.int_no] != 0) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}

void isr_handler(struct registers regs) {
    print("interrupted!\n", 13);
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}
