#ifndef INCLUDE_KERNEL_H
#define INCLUDE_KERNEL_H

#include "stddef.h"
#include "vga.h"

#define PANIC(str, strlen) print(str, strlen);

#endif
