#include "stddef.h"
#include "stdlib.h"

void swap(uint32_t * a, uint32_t * b) {
	*a = *a ^ *b;
	*b = *a ^ *b;
	*a = *b ^ *a;
}

void reverse(char * str, int len) {
	int start = 0;
	int end = len - 1;

	while (start < end) {
		swap(*(str+start++), *(str+end--));
	}
}

char * itoa(int num, char * str) {
	return itoa_b(num, str, 10);
}

char * itoa_b(int num, char * str, int base) {
	int i = 0;
	int is_neg = 0;

	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}

	if (num < 0 && base == 10) {
		is_neg = 1;
		num = -num;
	}

	while (num != 0) {
		int rem = num % base;
		str[i++] = (rem > 9) ? (rem - base) + 'a' : rem + '0';
		num /= base;
	}

	if(is_neg) str[i++] = '-';
	str[i] = '\0';

	reverse(str, i);

	return str;
}

int atoi(char *str) {
	int res = 0;
	for (int i = 0; str[i] != '\0'; i++) res = res * 10 + str[i] - '0';
	return res;
}
