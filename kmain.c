/* Framebuffer layout:
 * Bit:     | 15 14 13 12 11 10 9 8 | 7 6 5 4 | 3 2 1 0 |
 * Content: | ASCII                 | FG      | BG      |
*/

#define VGA_BUF 0xb8000;




char * framebuffer = VGA_BUF;

int kmain(void) {
	int x = 0xdeadbeef;
	put_char(0, 'A', 0x8, 0x2);
	return x;
}

void put_char(int col, char c, unsigned char fg, unsigned char bg) {
	framebuffer[col] = c;
	framebuffer[col + 1] = (fg << 4) | bg;
}
