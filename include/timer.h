#ifndef INCLUDE_TIMER_H
#define INCLUDE_TIMER_H

#include "stddef.h"

void init_timer(uint32_t freq);
static void timer_callback(struct registers regs);

#endif
