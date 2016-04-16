#include "vga.h"
#include "io.h"

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

char * framebuffer = VGA_BUF_ADDR;
short fb_pos;

struct cursor_pos {
	short row;
	short column;
} cursor_pos_t;

void vga_buf_init() {
	for(int i = 0; i < VGA_BUF_SIZE; i+=2) {
		put_char(' ', COLOR_GREEN, COLOR_BLACK);
	}

	fb_pos = 0;
	cursor_pos_t.row = 0;
	cursor_pos_t.column = 0;

	move_cursor(0);
}

/** put_char:
 *  Outputs a character to the framebuffer
 *  @param pos  		location in the framebuffer
 *  @param c    		character to write
 *  @param text_color	foreground color
 *  @param bg_color		background color
 */
void put_char(char c, unsigned char text_color, unsigned char bg_color) {
	//CHECKPOS();
	//fb_pos = pos;

	/*switch(c) {
		case '\n':
			fb_pos += VGA_BUF_COLS * 2;//(VGA_BUF_COLS - len + 1) * 2; //+1 to compensate for 0-index
			move_cursor(fb_pos / 2);
	}*/

	framebuffer[(cursor_pos_t.row * VGA_BUF_COLS) + cursor_pos_t.column] = c;
	framebuffer[++cursor_pos_t.column] = text_color | (bg_color << 4);
	cursor_pos_t.column++;
	//if(pos >= VGA_BUF_SIZE * 2 - 2) {
	//	scroll_terminal();
	//}
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
void move_cursor(unsigned short pos) {
	//if(pos >= VGA_BUF_SIZE * 2)
	//	vga_buf_init(); //scroll_terminal();

	outb(VGA_BUF_CMD_PORT, VGA_BUF_HIGH_BYTE_CMD);
	outb(VGA_BUF_DAT_PORT, (unsigned char)((pos >> 8) & 0x00ff));
	outb(VGA_BUF_CMD_PORT, VGA_BUF_LOW_BYTE_CMD);
	outb(VGA_BUF_DAT_PORT, (unsigned char)(pos & 0x00ff));
}

/** print:
 *  Prints the specified string the terminal
 *
 * 	@param string	the string to print
 *  @param len		the length of the string, excluding null terminator
 */
void print(char * string, int len) {
	for(int i = 0; i < len; i++) {
		put_char(string[i], COLOR_GREEN, COLOR_BLACK);
		//move_cursor((fb_pos / 2));
	}
}

void scroll_terminal() {
	//copy all lines to the line above
	for(int src = 80, dest = 0; src < VGA_BUF_SIZE * 2; src++, dest++) {
		framebuffer[dest] = framebuffer[src];
	}

	//1920 = first char in last row
	//for(int i = 1920; i < VGA_BUF_SIZE * 2; i++) {
	//	put_char(i, ' ', COLOR_GREEN, COLOR_BLACK);
	//}

	fb_pos = 1920 * 2;
	//move_cursor(fb_pos);
}
