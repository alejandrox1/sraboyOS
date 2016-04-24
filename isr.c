#include "isr.h"
#include "io.h"
#include "vga.h"

#define PIC1_CMD_PORT 0x20
#define PIC2_CMD_PORT 0xa0

isr_t interrupt_handlers[256];

void irq_handler(struct registers regs) {
    //send EOI (end of interrupt) to PIC(s)

    if (interrupt_handlers[regs.int_no] != 0) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
    irq_ack(regs.int_no);
}

void irq_ack(uint32_t int_no) {
    if (int_no >= 40) { //int > 40 = irq > 7
        print("slave ack\n", 10);
        outb(PIC2_CMD_PORT, 0x20);    //send reset/ack to slave
    }
    print("master ack\n", 11);
    outb(PIC1_CMD_PORT, 0x20); //send reset/ack to master (and slave, if needed)
}

void isr_handler(struct registers regs) {
    //print("interrupted!\n", 13);
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}
