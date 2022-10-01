#include "../include/main.h"
#include "../include/armio.h"

void main(){
    __armio_uart_init();
    __armio_uart_write_text("test\n");
    while(1);
}