#ifndef INCLUDE_KBD_H
#define INCLUDE_KBD_H

#include "isr.h"

static void kbd_callback(struct registers regs);
void init_kbd();

#endif
