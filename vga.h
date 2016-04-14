#ifndef INCLUDE_VGA_H
#define INCLUDE_VGA_H

#define INVALID_CHAR_ADDR -1
#define SUCCESS 0

enum vga_color {
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};

int put_char(int col, char c, unsigned char text_color, unsigned char bg_color);
int move_cursor(unsigned short pos);
void print(char * string, int len);

#endif //INCLUDE_VGA_H
