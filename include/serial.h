#include "stddef.h"

void serial_conf_baud_rate(uint16_t com, uint16_t divisor);
void serial_conf_line(uint16_t com, uint8_t bitmask);
void serial_conf_buff(uint16_t com, uint8_t bitmask);
void serial_conf_modem(uint16_t com, uint8_t bitmask);
uint32_t serial_is_xmit_fifo_empty(uint32_t com);
void serial_write(uint32_t com, uint8_t * data, int len);
