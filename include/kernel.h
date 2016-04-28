#ifndef INCLUDE_KERNEL_H
#define INCLUDE_KERNEL_H

#include <stdint.h>
#include "vga.h"

#define PANIC(str, strlen) print(str, strlen);

#endif
