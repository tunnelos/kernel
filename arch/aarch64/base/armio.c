#include "../include/armio.h"
#include "../include/stdint.h"

#if GCCARCH_NUM == 1 

void __armio_write(long reg, unsigned uint32_t value){
    *(volatile uint32_t *)reg = value;
}

uint32_t  __armio_read(long reg){
    return *(volatile uint32_t *)reg;


void __armio_uart_init(){
    __armio_write(PERIPHERAL_BASE)
}

#endif