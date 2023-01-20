#include "../include/desktop.h"

uint32_t __desktop_task_count = 0;
Theme currentTheme;

void __desktop_init() {
    __serial_write_fmt("CPU 0 -> tos -> DESKTOP > Creating desktop...\r\n");
    //default theme
    currentTheme = {COLOR_LIGHT_GREEN, COLOR_DARK_GRAY};
    //draw bg
    for(int j = 0; j < 30; j++){
        for(int i = 0; i < 80; i++){
            putc('\x08', __color_to_int(currentTheme.bgcolor), i, j);
        }
    }

    //draw task bar
    for(int i = 0; i < 80; i++) putc('\x08', __color_to_int(currentTheme.taskbarcolor), i, 0);

    //draw start button
    putc('+', __color_to_int(COLOR_WHITE), 0, 0);

    while(true);
}

void __desktop_add_task(Task task) {
    __desktop_task_count++;
    tasks[__desktop_task_count] = task;
}

void __desktop_terminate_task(uint32_t pid) {
    for(int i = 0; i < __desktop_task_count; i++){
        if(tasks[i].pid == pid){
            tasks[i] = NULL;
            for(int j = i; j < __desktop_task_count; j++) tasks[j] = tasks[j + 1];
        }
    }
    __desktop_task_count--;
}

void __desktop_render_tasks(){
    uint16_t x = 2;
    for(int i = 0; i < __desktop_task_count; i++){
        x = x + strlen(tasks[i].title) + 1;
        puts(tasks[i].title, __color_to_int(COLOR_WHITE), x, 0);
    }
}

void __desktop_render_categories(){
    //draw categories
    for(int j = 1; j < 30; j++){
        for(int i = 0; i < 10; i++){
            putc('\x08', __color_to_int(COLOR_GRAY), i, j);
        }
    }

    for(uint16_t i = 0; i < categoriesCount; i++){
        if(currentCategory == i) for(int i = 0; i < 10; i++) putc('\x08', __color_to_int(currentTheme.bgcolor), i, 1 + currentCategory);

        puts(categories[currentCategory].title, __color_to_int(COLOR_WHITE), 1, 1 + currentCategory);
    }
}