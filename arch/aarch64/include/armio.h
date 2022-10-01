#pragma once

#include "arch.h"
#include "../include/stdint.h"

#define AUX_MU_BAUD(baud) ((AUX_UART_CLOCK / (baud * 8)) - 1)
#define GPIO_MAX_PIN 53
#define GPIO_FUNCTION_ALTS 2
#define NONE 0

enum {
    PERIPHERAL_BASE = 0x3F000000,                   // Peripheral base for Cortex-A72 is 0xFE000000
    GDFSEL0         = PERIPHERAL_BASE + 0x200000,
    GPSET0          = PERIPHERAL_BASE + 0x20001C,
    GPCLR0          = PERIPHERAL_BASE + 0x200028,
    GPPUPPDN0       = PERIPHERAL_BASE + 0x2000E4
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