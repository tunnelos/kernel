#include "./include/stdio.h"
#include "./include/screen.h"
#include "./include/serial.h"
#include "./include/cstring.h"
#include "./include/str.h"
#include "./include/stdint.h"

uint32_t __desktop_task_count = 0;
//unsigned int __desktop_tasks_length = 0;
int __desktop_ids[10];
string __desktop_titles[25];

void __desktop_init() {
    __serial_write_fmt("CPU 0 -> tos -> DESKTOP > Creating desktop...\n");
    for(int j = 0; j < 30; j++){
        for(int i = 0; i < 80; i++){
            if(j == 0){
                putc('\x08', COLOR_DARK_GRAY, i, j);
            }else{
                putc('\x08', COLOR_LIGHT_GREEN, i, j);
            }
        }
    }
    putc_gui('+', COLOR_WHITE, 0, 0);
    for(int j = 1; j < 30; j++){
        for(int i = 0; i < 10; i++){
            putc('\x08', COLOR_GRAY, i, j);
        }
    }
    for(int i = 0; i < 10; i++){
        putc('\x08', COLOR_LIGHT_GREEN, i, 2);
    }
    puts_gui("Apps", COLOR_WHITE, 1, 2);
    puts_gui("Games", COLOR_WHITE, 1, 3);
    puts_gui("System", COLOR_WHITE, 1, 4);

    while(true);
}

void __desktop_add_task(string task) {
    __desktop_titles[__desktop_ids[__desktop_task_count]] = task;
    __desktop_task_count++;
    //__desktop_tasks_length += strlen(task);
}

void __desktop_terminate_task(string task) {
    __desktop_task_count--;
    //__desktop_tasks_length -= strlen((const string)task);
}

/*
int __desktop_get_current_tasks() {
    
}
*/
void __desktop_render_tasks(){
    int calculateX = 2;
    for(int i = 0; i < __desktop_task_count; i++){
        for(int j = 0; j < i; j++){
            calculateX += strlen(__desktop_titles[__desktop_ids[i]]);
        }
        puts_gui(__desktop_titles[__desktop_ids[i]], COLOR_WHITE, calculateX, 0);
        calculateX = 2;
    }
}