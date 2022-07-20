#pragma once

#include "arch.h"
#include "../include/stdint.h"

#if GCCARCH_NUM == 1 // aarch64

#define AUX_MU_BAUD(baud) ((AUX_UART_CLOCK / (baud * 8)) - 1)

enum {
    PERIPHERAL_BASE = 0xFE000000 //..? maybe 0xFE000000 or 0x3E000000
};

enum {
    AUX_BASE = PERIPHERAL_BASE + 0x215000,
    AUX_UART_CLOCK = 500000000
};

void __armio_write(long reg, uint32_t value);
uint32_t __armio_read(long reg);
void __armio_uart_init();
void __armio_uart_write_text(char *text);
uint32_t __armio_uart_is_write_byte_ready();
void __armio_uart_write_byte_blocking_actual(unsigned char ch);
void __armio_uart_write_text(char *text);

#endif