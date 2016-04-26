#ifndef INCLUDE_STRING_H
#define INCLUDE_STRING_H

#include "stddef.h"

void *memset(void *s, int c, size_t n);
void *memcpy(void * restrict dest, const void * restrict src, size_t n);

int strncmp(const char * s1, const char * s2, size_t n);
char *strcpy(char * restrict dest, const char * restrict src);
size_t strlen(const char *s);
#endif
