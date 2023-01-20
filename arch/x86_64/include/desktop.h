#pragma once
#include "stdio.h"
#include "screen.h"
#include "serial.h"
#include "cstring.h"
#include "str.h"
#include "stdint.h"
#include "color.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Task {
    uint32_t pid;
    string title;
};

struct Theme {
    Color bgcolor;
    Color taskbarcolor;
};

struct Category {
    string title;    
};

extern uint16_t __desktop_task_count;
extern uint8_t currentCategory = 0;
extern uint8_t categoriesCount = 0;
extern Task tasks[512];
extern Theme currentTheme;
extern Category categories[256] = {{"Apps"}, {"Games"}};

void __desktop_init();
void __desktop_add_task(Task task);
void __desktop_terminate_task(uint32_t pid);
void __desktop_render_tasks();
void __desktop_render_categories();

#ifdef __cplusplus
}
#endif