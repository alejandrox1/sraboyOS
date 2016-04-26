#include "stddef.h"
#include "string.h"

void *memset(void *s, int c, size_t n) {
	uint8_t * p = s;
	while (n--) *p++ = (uint8_t)c;
	return s;
}

void *memcpy(void * restrict dest, const void * restrict src, size_t n) {
	char *dp = dest;
	const char *sp = src;
	while (n--) *dp++ = *sp++;
	return dest;
}

int strncmp(const char * s1, const char * s2, size_t n) {
	while(n--) {
		if (*s1++ != *s2++)
			return *(uint8_t *)(s1 - 1) - *(uint8_t *)(s2 - 1);
		return 0;
	}
}

char *strcpy(char * restrict dest, const char * restrict src) {
	char *ret = dest;
	while (*dest++ = *src++);
	return ret;
}

size_t strlen(const char *s) {
	size_t i = 0;
	while(s[i] != '\0') i++;
	return i;
}
