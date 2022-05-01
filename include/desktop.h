#pragma once

#include "./str.h"
#include "./stdint.h"

void __desktop_add_task(string task);
void __desktop_terminate_task(string task);
void __desktop_render_tasks();
int __desktop_get_current_tasks(); //unused
void __desktop_init();

extern uint32_t __desktop_task_count;
extern int __desktop_ids[10];
extern string __desktop_titles[25];