int kmain(void) {
	int x = 0xdeadbeef;
	write_32(0x000b8000);
	write_char(0x000b8000);
	return x;
}

int write_32(int * dest) {
	dest[0] = 0x0f41;
	return 0;
}

int write_char(char * dest) {
	dest[2] = 0x42;
	dest[3] = 0x0f;
	return 0;
}
