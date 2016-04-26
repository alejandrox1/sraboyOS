#ifndef __KHEAP_H__
#define __KHEAP_H__

#include "stddef.h"

static uint32_t _kmalloc_int(uint32_t size, uint32_t * phys, uint8_t align);
uint32_t kmalloc_a(uint32_t size);
uint32_t kmalloc_p(uint32_t size, uint32_t * phys);
uint32_t kmalloc_ap(uint32_t size, uint32_t * phys);
uint32_t kmalloc(uint32_t size);

#endif
