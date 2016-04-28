#include "vga.h"
#include "io.h"
#include <stdint.h>

#define VGA_BUF_HIGH_BYTE_CMD 0x0e
#define VGA_BUF_LOW_BYTE_CMD  0x0f
#define VGA_BUF_CMD_PORT 0x03d4
#define VGA_BUF_DAT_PORT 0x03d5
#define VGA_BUF_ADDR 0x000b8000
#define VGA_BUF_COLS 80
#define VGA_BUF_ROWS 25
#define VGA_BUF_SIZE 2000 //in chars/positions: 80 columns * 25 rows

//the second byte in the pos is for color info
//#define CHECKPOS() if(pos % 2 != 0 || pos >= VGA_BUF_SIZE * 2) return INVALID_CHAR_ADDR;

uint16_t* framebuffer = VGA_BUF_ADDR;

static struct cursor_pos cursor_pos_t = { .row = 0, .column = 0 };
static struct terminal_color terminal_color_t = { .foreground = COLOR_LIGHT_GREEN,
												  .background = COLOR_BLACK };

void vga_buf_init() {
	for(int y = 0; y < VGA_BUF_ROWS; y++) {
		for(int x = 0; x < VGA_BUF_COLS; x++) {
			put_char(' ', &cursor_pos_t, &terminal_color_t);
		}
	}

	cursor_pos_t.row = 0;
	cursor_pos_t.column = 0;

	move_cursor(0);
}

void put_char(uint8_t c, struct cursor_pos* cpos, struct terminal_color* tcol) {
	uint16_t c16 = c;
	uint8_t fg = tcol->foreground;
	uint8_t bg = tcol->background;

	uint16_t color = (bg << 4 | fg) << 8;
	int pos = (cpos->row * VGA_BUF_COLS) + cpos->column;
	uint16_t value = c16 | color;

	if(c != '\n') {
		framebuffer[pos] = value;

		if(++(cpos->column) >= VGA_BUF_COLS) {
			cpos->column = 0;
			cpos->row++;
		}
	} else {
		cpos->column = 0;
		cpos->row++;
	}

	if(cpos->row > VGA_BUF_ROWS) {
		cpos->row--;
		pos = (VGA_BUF_ROWS - 1) * VGA_BUF_COLS;
		scroll_terminal();
	}

	move_cursor(pos);
}

/** move_cursor:
 *  Moves the framebuffer's cursor to the specified position
 *
 * out 0x3D4, 14      ; tell framebuffer to expect high 8 bits of pos
 * out 0x3D5, 0x0     ; send the highest 8 bits of 0x50
 * out 0x3D4, 15      ; tell framebuffer to expect low 8 bits of pos
 * out 0x3D5, 0x50    ; send the lowest 8 bits of 0x50
 *
 * ; stack:  [esp + 8] data
 * ;		 [esp + 4] I/O port
 * ;		 [esp    ] return addr
 * outb:
 * 	 mov al, [esp + 8]  ; unsigned char data
 * 	 mov dx, [esp + 4]  ; unsigned short port
 * 	 out dx, al			; outb(unsigned short port, unsigned char data)
 *   ret
 *
 */
void move_cursor(uint16_t pos) {

	outb(VGA_BUF_CMD_PORT, VGA_BUF_HIGH_BYTE_CMD);
	outb(VGA_BUF_DAT_PORT, (uint8_t)((pos >> 8) & 0x00ff));
	outb(VGA_BUF_CMD_PORT, VGA_BUF_LOW_BYTE_CMD);
	outb(VGA_BUF_DAT_PORT, (uint8_t)(pos & 0x00ff));
}

/** print:
 *  Prints the specified string the terminal
 *
 * 	@param string	the string to print
 *  @param len		the length of the string, excluding null terminator
 */
void print(uint8_t * string, int len) {
	for(int i = 0; i < len; i++) {
		put_char(string[i], &cursor_pos_t, &terminal_color_t);
	}
}

void scroll_terminal() {
	//copy all lines to the line above
	for(int src = 80, dest = 0; src < VGA_BUF_SIZE * 2; src++, dest++) {
		framebuffer[dest] = framebuffer[src];
	}
}
