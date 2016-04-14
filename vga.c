#include "vga.h"
#include "io.h"

#define VGA_BUF_HIGH_BYTE_CMD 0x0e
#define VGA_BUF_LOW_BYTE_CMD  0x0f
#define VGA_BUF_CMD_PORT 0x03d4
#define VGA_BUF_DAT_PORT 0x03d5
#define VGA_BUF_ADDR 0x000b8000
#define VGA_BUF_SIZE 2000 //in chars/positions: 80 columns * 25 rows

char * framebuffer = VGA_BUF_ADDR;
short fb_pos = 0;

void vga_buf_init() {
	for(int i = 0; i < VGA_BUF_SIZE; i+=2) {
		put_char(i, ' ', COLOR_GREEN, COLOR_BLACK);
	}
}

/** put_char:
 *  Outputs a character to the framebuffer
 *  @param pos  		location in the framebuffer
 *  @param c    		character to write
 *  @param text_color	foreground color
 *  @param bg_color		background color
 */
int put_char(int pos, char c, unsigned char text_color, unsigned char bg_color) {
	if(pos % 2 != 0 || pos >= 2000)
		return INVALID_CHAR_ADDR;

	framebuffer[pos] = c;
	framebuffer[pos + 1] = text_color | (bg_color << 4);

	return SUCCESS;
}

/** move_cursor:
 *  Moves the framebuffer's cursor to the specified position
 *
 * out 0x3D4, 14      ; tell framebuffer to expect high 8 bits of pos
 * out 0x3D5, 0x0     ; send the highest 8 bits of 0x50
 * out 0x3D4, 15      ; tell framebuffer to expect low 8 bits of pos
 * out 0x3D5, 0x50    ; send the lowest 8 bits of 0x50
 *
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
 * 	@param pos	location to move the cursor to
 */
int move_cursor(unsigned short pos) {

	if(pos % 2 != 0 || pos >= 2000)
		return INVALID_CHAR_ADDR;

	outb(VGA_BUF_CMD_PORT, VGA_BUF_HIGH_BYTE_CMD);
	outb(VGA_BUF_DAT_PORT, (unsigned char)((pos >> 8) & 0x00ff));
	outb(VGA_BUF_CMD_PORT, VGA_BUF_LOW_BYTE_CMD);
	outb(VGA_BUF_DAT_PORT, (unsigned char)(pos & 0x00ff));

	return SUCCESS;
}

/** print:
 *  Prints the specified string the terminal
 *
 * 	@param string	the string to print
 *  @param len		the length of the string, excluding null terminator
 */
void print(char * string, int len) {
	for(int i = 0; i < len; i++) {
		put_char(fb_pos, string[i], COLOR_GREEN, COLOR_BLACK);
		fb_pos += 2;
		move_cursor(fb_pos / 2);
	}
}
