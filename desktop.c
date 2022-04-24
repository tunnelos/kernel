#include "./include/stdio.h"
#include "./include/screen.h"
#include "./include/serial.h"
#include "./include/cstring.h"

int current_tasks[];
char task_name[];

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
    puts_gui("Apps", COLOR_GRAY, 1, 2);
    puts_gui("Games", COLOR_GRAY, 1, 3);
    puts_gui("System", COLOR_GRAY, 1, 4);

    while(true);
}

unsigned short __desktop_tasks_count = 0;
int __desktop_ids[10];
char *__desktop_titles[25];

void __desktop_add_task(char task[]) {
    __desktop_titles[__desktop_ids[__desktop_tasks_count]] = task;
    __desktop_tasks_count++;
}

void __desktop_termanaite_task() {
    __desktop_tasks_count--;
}

int __desktop_get_current_tasks() {
    
}
void __desktop_render_tasks(){
    // for(int i = 1; i < __desktop_tasks_count; i++){
    //     putc('\x08', COLOR_DARK_GRAY, );
    //     puts_gui(__desktop_titles[__desktop_ids[i]]);
    // }
}