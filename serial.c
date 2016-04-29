#include <stdint.h>
#include "io.h"
#include "serial.h"

#define SERIAL_COM1_BASE			0x3f8	//COM1 base port
#define SERIAL_DATA_PORT(base)		(base)
#define SERIAL_BUFF_CMD_PORT(base)	(base + 2)
#define SERIAL_LINE_CMD_PORT(base)	(base + 3)
#define SERIAL_MODEM_CMD_PORT(base)	(base + 4)
#define SERIAL_LINE_STAT_PORT(base)	(base + 5)

// tells serial port to expect high 8 bits then low 8 bits
// DLAB = Divisor Latch Access Bit (see: http://wiki.osdev.org/Serial_Ports#Port_Addresses)
#define SERIAL_LINE_ENABLE_DLAB		0x80

void serial_conf_baud_rate(uint16_t com, uint16_t divisor) {
	outb(SERIAL_LINE_CMD_PORT(com), SERIAL_LINE_ENABLE_DLAB);
	outb(SERIAL_DATA_PORT(com), (divisor >> 8));
	outb(SERIAL_DATA_PORT(com), divisor);
}


/** serial_conf_line
 * Configures the line of the specified serial port
 * with the specified settings.
 *
 * ------------------------------------
 * Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
 * Content: | d | b | prty  | s | dl  |
 * ------------------------------------
 * d 	= enable/disable DLAB (1 = enable)
 * b 	= enable/disable break control (1 = enable)
 * prty = number of parity bits to use
 * s 	= number of stop bits to use (s = 0 means 1; s = 1 means 1.5 or 2)
 * dl 	= desribes length of data
 * ------------------------------------
 *
 * Generally, use 0x3 for bitmask.
 */
void serial_conf_line(uint16_t com, uint8_t bitmask) {
	outb(SERIAL_LINE_CMD_PORT(com), bitmask);
}

/** serial_conf_buff
 * Configures the FIFO buffer of the specified serial port
 * with the specified settings.
 *
 * -----------------------------------------------
 * Bit:     | 7 6 | 5  | 4 | 3   | 2   | 1   | 0 |
 * Content: | lvl | bs | r | dma | clt | clr | e |
 * -----------------------------------------------
 * lvl 	= bytes to store in FIFO buffer
 * bs 	= buffers should be 16 or 64 bytes large
 * r 	= reserved
 * dma	= how serial port data should be accessed
 * clt	= clear xmit FIFO buffer
 * clr	= clear recv FIFO buffer
 * e	= enable/disable FIFO buffer
 */
void serial_conf_buff(uint16_t com, uint8_t bitmask) {
	outb(SERIAL_BUFF_CMD_PORT(com), bitmask);

	/*
	 * Use the value 0xC7 = 11000111 that:
	 * 	Enables FIFO
	 * 	Clear both receiver and transmission FIFO queues
	 * 	Use 14 bytes as size of queue
	 *
	 */
}

/** serial_conf_modem
 * Configures the modem of the specified serial port
 * with the specified settings
 *
 * ----------------------------------------------------
 * Bit:     | 7 | 6 | 5  | 4  | 3   | 2   | 1   | 0   |
 * Content: | r | r | af | lb | ao2 | ao1 | rts | dtr |
 * ----------------------------------------------------
 *
 * r	Reserved
 * af	Autoflow control enabled
 * lb	Loopback mode (used for debugging serial ports)
 * ao2	Auxiliary output 2, used for receiving interrupts
 * ao1	Auxiliary output 1
 * rts	Ready To Transmit
 * dtr	Data Terminal Ready
 *
 */
void serial_conf_modem(uint16_t com, uint8_t bitmask) {
	outb(SERIAL_MODEM_CMD_PORT(com), bitmask);

	//0x3: RTS = 1, DTS = 1 => Ready to send data
}

uint32_t serial_is_xmit_fifo_empty(uint32_t com) {
	//0x20 = 0010 0000
	return inb(SERIAL_LINE_STAT_PORT(com)) & 0x20;
}

void serial_write(uint32_t com, const char * data, int len) {
	//print("serial_write\n", 13);
	for(int i = 0; i < len; i++) {
		serial_put_char(com, data[i]);
	}
}

void serial_put_char(uint32_t com, uint8_t c) {
	//print("serial_put_char\n", 16);
	while(serial_is_xmit_fifo_empty(com) == 0);
	outb(com, c);
}
