#pragma once
#include "stdio.h"
#include "screen.h"
#include "serial.h"
#include "cstring.h"
#include "str.h"
#include "stdint.h"
#include "color.h"
#include "gui.h"

#ifdef __cplusplus
extern "C" {
#endif

struct task_t {
    uint32_t pid;
    string title;
};

struct theme_t {
    color_t bgcolor;
    color_t taskbarcolor;
};

struct category_t {
    string title;    
};

extern uint16_t __desktop_task_count;
extern uint8_t currentCategory = 0;
extern uint8_t categoriesCount = 0;
extern task_t tasks[512];
extern theme_t currentTheme;
extern categories_t categories[256] = {{"Apps"}, {"Games"}};

void __desktop_init();
void __desktop_add_task(task_t task);
void __desktop_terminate_task(uint32_t pid);
void __desktop_render_tasks();
void __desktop_render_categories();

#ifdef __cplusplus
}
#endif