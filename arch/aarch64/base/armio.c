#include "../include/armio.h"

void __armio_write(long reg, uint32_t value){
    *(volatile uint32_t *)reg = value;
}

uint32_t __armio_read(long reg){
    return *(volatile uint32_t *)reg;
}

void __armio_uart_init(){
    __armio_write(AUX_BASE + 4, 1); //Enable UART1
    __armio_write(AUX_BASE + 68, 0);
    __armio_write(AUX_BASE + 96, 0);
    __armio_write(AUX_BASE + 76, 3); //8 bits
    __armio_write(AUX_BASE + 80, 0);
    __armio_write(AUX_BASE + 68, 0);
    __armio_write(AUX_BASE + 72, 0xC6); //Disable interrupts
    __armio_write(AUX_BASE + 104, AUX_MU_BAUD(115200));

    __armio_write(AUX_BASE + 96, 3); //Enable RX/TX 
}

uint32_t __armio_uart_is_write_byte_ready(){
    return __armio_read(AUX_BASE + 84) & 0x20;
}

void __armio_uart_write_byte_blocking_actual(unsigned char ch){
    while(!__armio_uart_is_write_byte_ready());
    __armio_write(AUX_BASE + 64, (uint32_t)ch);
}

void __armio_uart_write_text(char *text){
    while(*text){
        if(*text == '\n') __armio_uart_write_byte_blocking_actual('\r');
        __armio_uart_write_byte_blocking_actual(*text++);
    }
}