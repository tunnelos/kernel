#include "../include/desktop.h"
#include "../include/stdlib.h"

uint32_t __desktop_task_count = 0;
theme_t currentTheme;
uint32_t __desktop_task_count;
uint8_t currentCategory = 0;
uint8_t categoriesCount = 2;
task_t *tasks;

category_t categories[256] = {{"Apps"}, {"Games"}};

void __desktop_init() {
    __serial_write_fmt("CPU 0 -> tos -> DESKTOP > Creating desktop...\r\n");

    tasks = (task_t *)malloc(sizeof(task_t) * 512);

    //default theme
    currentTheme = (theme_t){COLOR_LIGHT_GREEN, COLOR_DARK_GRAY};
    //draw bg
    __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, currentTheme.bgcolor);

    //draw task bar
    for(int i = 0; i < 80; i++) putc('\x08', __color_to_int(currentTheme.taskbarcolor), i, 0);
    __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 1}, currentTheme.taskbarcolor);

    //draw start button
    //putc('+', __color_to_int(COLOR_WHITE), 0, 0);
    __gui_drawText((vector2d_t){0, 0}, (vector2d_t){1, 1}, COLOR_WHITE, "+");

    __desktop_render_categories();
    __desktop_render_tasks();

    while(true);
}

void __desktop_add_task(task_t task) {
    int i = 0;
    while(i < 512) {
        if(!tasks[i].used) {
            tasks[i].used = true;
            tasks[i].pid = task.pid;
            tasks[i].title = task.title;

            return;
        }
        i++;
    }
}

void __desktop_terminate_task(uint32_t pid) {
    for(int i = 0; i < 512; i++){
        if(tasks[i].pid == pid){
            tasks[i].used = false;
        }
    }
}

void __desktop_render_tasks(){
    uint16_t x = 2;
    for(int i = 0; i < 512; i++){
        if(tasks[i].used) {
            x = x + strlen(tasks[i].title) + 1;
            //puts(tasks[i].title, __color_to_int(COLOR_WHITE), x, 0);
            __gui_drawText((vector2d_t){x, 0}, (vector2d_t){strlen(tasks[i].title), 1}, COLOR_WHITE, tasks[i].title);
        }
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