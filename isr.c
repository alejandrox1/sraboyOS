#include "isr.h"
#include "vga.h" //REMOVE THIS AFTER TESTING

void isr_handler(struct registers regs) {
    print("interrupted!\n", 13);
}
