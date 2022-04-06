#include "./include/shell_mouse.h"
#include "./include/mm.h"
#include "./include/window.h"
#include "./include/stdio.h"
#include "./include/shell.h"
#include "./include/smt.h"

int __shell_mouse_x, __shell_mouse_y;
window_t __shell_mouse_wnd_context;
char *__shell_mouse_wnd_old_context;

#define KEY_UP    0x48
#define KEY_DOWN  0x50
#define KEY_LEFT  0x4B
#define KEY_RIGHT 0x4D

void __shell_mouse_move(int xx, int yy) {
    __shell_mouse_y += yy;
    __shell_mouse_x += xx;
    __shell_mouse_wnd_context.wx = __shell_mouse_x;
    __shell_mouse_wnd_context.wy = __shell_mouse_y;
    __window_read_old_ctx(__shell_mouse_wnd_old_context, &__shell_mouse_wnd_context);
    putc_gui('\x07', 0x005F1F2F, __shell_mouse_x, __shell_mouse_y);
    __window_save_context(__shell_mouse_wnd_old_context, &__shell_mouse_wnd_context);
}
void __shell_mouse_thread(int id) {
    switch(scancodePub) {
        case KEY_UP: {
            if(!(__shell_mouse_y > 30 || __shell_mouse_y < 0)) __shell_mouse_move(0, -1);
            break;
        }
        case KEY_DOWN: {
            if(!(__shell_mouse_y > 30 || __shell_mouse_y < 0)) __shell_mouse_move(0, 1);
            break;
        }
        case KEY_LEFT: {
            if(!(__shell_mouse_y > 30 || __shell_mouse_y < 0)) __shell_mouse_move(-1, 0);
            break;
        }
        case KEY_RIGHT: {
            if(!(__shell_mouse_y > 30 || __shell_mouse_y < 0)) __shell_mouse_move(1, 0);
            break;
        }
    }
}
void __shell_mouse_create_wnd() {
    __shell_mouse_wnd_context.name = "MThr";
    __shell_mouse_wnd_context.sx = 1;
    __shell_mouse_wnd_context.sy = 1;
    __shell_mouse_wnd_context.wx = 9;
    __shell_mouse_wnd_context.wy = 16;
    __shell_mouse_wnd_old_context = (char *)(malloc(8 * 16).address);
    __window_save_context(__shell_mouse_wnd_old_context, &__shell_mouse_wnd_context);
    __shell_mouse_x = 9;
    __shell_mouse_y = 16;
    putc_gui('\x07', 0x005F1F2F, __shell_mouse_x, __shell_mouse_y);
    __smt_create_task(__shell_mouse_thread);
    return;
}