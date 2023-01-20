#include "../include/desktop.h"

uint32_t __desktop_task_count = 0;
theme_t currentTheme;

void __desktop_init() {
    __serial_write_fmt("CPU 0 -> tos -> DESKTOP > Creating desktop...\r\n");
    //default theme
    currentTheme = {COLOR_LIGHT_GREEN, COLOR_DARK_GRAY};
    //draw bg
    __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, currentTheme.bgcolor);

    //draw task bar
    for(int i = 0; i < 80; i++) putc('\x08', __color_to_int(currentTheme.taskbarcolor), i, 0);
    __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){1, 80}, currentTheme.taskbarcolor);

    //draw start button
    //putc('+', __color_to_int(COLOR_WHITE), 0, 0);
    __gui_drawText((vector2d_t){0, 0}, (vector2d_t){1, 1}, COLOR_WHITE, "+");

    while(true);
}

void __desktop_add_task(task_t task) {
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
        //puts(tasks[i].title, __color_to_int(COLOR_WHITE), x, 0);
        __gui_drawText((vector2d_t){x, 0}, (vector2d_t){strlen(tasks[i].title), 1}, COLOR_WHITE, tasks[i].title);
    }
}

void __desktop_render_categories(){
    //draw categories
    __gui_drawRectangle((vector2d_t){0, 1}, (vector2d_t){10, 29}, COLOR_GRAY);

    for(uint16_t i = 0; i < categoriesCount; i++){
        if(currentCategory == i) __gui_drawRectangle((vector2d_t){i, 1 + currentCategory}, (vector2d_t){10, 1}, currentTheme.bgcolor);
        __gui_drawText((vector2d_t){1, 1 + currentCategory}, (vector2d_t){9, 1}, COLOR_WHITE, categories[currentCategory].title);
    }
}