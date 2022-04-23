#include "./include/stdio.h"
#include "./include/screen.h"
#include "./include/serial.h"
//Сделаю для начала набросок как это будет выглядить
//main.c пока что запускает твой десктоп
//Я хочу посмотреть на это
void __desktop_init() {
    __serial_write_fmt("CPU 0 -> tos -> DESKTOP > Creating desktop...\n");
    for(int j = 0; j < 30; j++){
        for(int i = 0; i < 80; i++){
            putc('\x08', COLOR_DARK_GREEN, i, j);
        }
    }
    for(int j = 0; j < 30; j++){
        for(int i = 0; i < 10; i++){
            putc('\x08', COLOR_DARK_GRAY, i, j);
        }
    }
    for(int i = 0; i < 10; i++){
        putc('\x08', COLOR_DARK_GREEN, i, 1);
    }
    puts_gui("Apps", COLOR_GRAY, 1, 1);
    puts_gui("Games", COLOR_GRAY, 1, 2);
    puts_gui("System", COLOR_GRAY, 1, 3); 

    while(true);
}

