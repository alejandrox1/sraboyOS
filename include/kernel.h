#ifndef INCLUDE_KERNEL_H
#define INCLUDE_KERNEL_H

#include <stdint.h>
#include "vga.h"
#include "string.h"

#define PANIC(str) print(str, strlen(str));

#endif
