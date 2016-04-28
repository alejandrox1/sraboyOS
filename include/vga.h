#ifndef INCLUDE_VGA_H
#define INCLUDE_VGA_H

#include <stdint.h>

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

struct cursor_pos {
	uint16_t row;
	uint16_t column;
};

struct terminal_color {
	enum vga_color foreground;
	enum vga_color background;
};

void put_char(uint8_t c, struct cursor_pos* cpos, struct terminal_color* tcol);
void move_cursor(uint16_t pos);
void print(uint8_t * string, int len);

#endif //INCLUDE_VGA_H
