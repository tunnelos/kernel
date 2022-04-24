#include "./include/shell_mouse.h"
#include "./include/mm.h"
#include "./include/window.h"
#include "./include/stdio.h"
#include "./include/shell.h"
#include "./include/smt.h"
#include "./include/serial.h"

int __shell_mouse_x, __shell_mouse_y;
window_t __shell_mouse_wnd_context;
char *__shell_mouse_wnd_old_context;
int hh = 0;
bool __shell_mouse_wnd_sel_mode = false;
bool __shell_mouse_wnd_mov_mode = false;

#define KEY_UP     0x48
#define KEY_DOWN   0x50
#define KEY_LEFT   0x4B
#define KEY_RIGHT  0x4D
#define KEY_INSERT 0x52
#define KEY_PGDOWN 0x51
#define KEY_HOME   0x47

#define MOUSE_COL_SELECTOR 0x00C01F9F
#define MOUSE_COL_STANDART 0x005F1F2F
#define MOUSE_COL_WINDMOVE 0x0000FF20

void __shell_mouse_move(int xx, int yy) {
    __window_read_old_ctx(__shell_mouse_wnd_old_context, &__shell_mouse_wnd_context);
    __shell_mouse_y += yy;
    __shell_mouse_x += xx;
    __shell_mouse_wnd_context.wx = __shell_mouse_x;
    __shell_mouse_wnd_context.wy = __shell_mouse_y;
    __window_save_context(__shell_mouse_wnd_old_context, &__shell_mouse_wnd_context);
    if(__shell_mouse_wnd_sel_mode) {
        if(__shell_mouse_wnd_mov_mode) {
            putc_gui('\x07', MOUSE_COL_WINDMOVE, __shell_mouse_x, __shell_mouse_y);
        } else {
            putc_gui('\x07', MOUSE_COL_SELECTOR, __shell_mouse_x, __shell_mouse_y);
        }
    } else {
        putc_gui('\x07', MOUSE_COL_STANDART, __shell_mouse_x, __shell_mouse_y);
    }
}
void __shell_mouse_thread(int id) {
    if(hh == 256) {
        switch(scancodePub) {
            case KEY_UP: {
                if(!(__shell_mouse_y < 1)) {
                    if(__shell_mouse_wnd_sel_mode) {
                        uint8_t x = 0;
                        while(x < 32) {
                            if(__window_used[x]) {
                                if(__window_windowlist[x]->wx == __shell_mouse_x && __window_windowlist[x]->wy == __shell_mouse_y) {
                                    __window_windowlist[x]->selected = true;
                                    __window_windowlist[x]->updated = true;
                                    __serial_write_fmt("CPU ? -> tos > Moving window!\n");
                                    __window_read_old_ctx(__window_windowlist[x]->ctx0, __window_windowlist[x]);
                                    __window_windowlist[x]->wy -= 1;
                                    __window_save_context(__window_windowlist[x]->ctx0, __window_windowlist[x]);
                                    if(__window_windowlist[x]->update_handler != 0) __window_windowlist[x]->update_handler();
                                    __window_windowlist[x]->updated = true;
                                }
                            }
                            x++;
                        }
                    }
                    __shell_mouse_move(0, -1);
                }
                break;
            }
            case KEY_DOWN: {
                if(!(__shell_mouse_y > 28)) {
                    if(__shell_mouse_wnd_sel_mode) {
                        uint8_t x = 0;
                        while(x < 32) {
                            if(__window_windowlist[x]) {
                                if(__window_windowlist[x]->wx == __shell_mouse_x && __window_windowlist[x]->wy == __shell_mouse_y) {
                                    __window_windowlist[x]->selected = true;
                                    __window_windowlist[x]->updated = true;
                                    __serial_write_fmt("CPU ? -> tos > Moving window!\n");
                                    __window_read_old_ctx(__window_windowlist[x]->ctx0, __window_windowlist[x]);
                                    __window_windowlist[x]->wy += 1;
                                    __window_save_context(__window_windowlist[x]->ctx0, __window_windowlist[x]);
                                    if(__window_windowlist[x]->update_handler != 0) __window_windowlist[x]->update_handler();
                                }
                            }
                            x++;
                        }
                    }
                    __shell_mouse_move(0, 1);
                }
                break;
            }
            case KEY_LEFT: {
                if(!(__shell_mouse_x < 1)) {
                    __shell_mouse_move(-1, 0);
                    if(__shell_mouse_wnd_sel_mode) {
                        uint8_t x = 0;
                        while(x < 32) {
                            if(__window_windowlist[x]) {
                                if(__window_windowlist[x]->wx == __shell_mouse_x && __window_windowlist[x]->wy == __shell_mouse_y) {
                                    __window_windowlist[x]->selected = true;
                                    __window_windowlist[x]->updated = true;
                                    __serial_write_fmt("CPU ? -> tos > Moving window!\n");
                                    __window_read_old_ctx(__window_windowlist[x]->ctx0, __window_windowlist[x]);
                                    __window_windowlist[x]->wx -= 1;
                                    __window_save_context(__window_windowlist[x]->ctx0, __window_windowlist[x]);
                                    if(__window_windowlist[x]->update_handler != 0) __window_windowlist[x]->update_handler();
                                }
                            }
                            x++;
                        }
                    }
                }
                break;
            }
            case KEY_RIGHT: {
                if(!(__shell_mouse_x > 78)) {
                    if(__shell_mouse_wnd_sel_mode) {
                        uint8_t x = 0;
                        while(x < 32) {
                            if(__window_windowlist[x]) {
                                if(__window_windowlist[x]->wx == __shell_mouse_x && __window_windowlist[x]->wy == __shell_mouse_y) {
                                    __window_windowlist[x]->selected = true;
                                    __window_windowlist[x]->updated = true;
                                    __serial_write_fmt("CPU ? -> tos > Moving window!\n");
                                    __window_read_old_ctx(__window_windowlist[x]->ctx0, __window_windowlist[x]);
                                    __window_windowlist[x]->wx += 1;
                                    __window_save_context(__window_windowlist[x]->ctx0, __window_windowlist[x]);
                                    if(__window_windowlist[x]->update_handler != 0) __window_windowlist[x]->update_handler();
                                }
                            }
                            x++;
                        }
                    }
                    __shell_mouse_move(1, 0);
                }
                break;
            }
            case KEY_INSERT: {
                __serial_write_fmt("CPU ? -> tos > Chagne mouse mode to %d!\n", !__shell_mouse_wnd_sel_mode);
                __shell_mouse_wnd_sel_mode = !__shell_mouse_wnd_sel_mode;
                if(__shell_mouse_wnd_sel_mode) {
                    uint8_t x = 0;
                    while(x < 32) {
                        if(__window_windowlist[x]) {
                            if(__window_windowlist[x]->wx == __shell_mouse_x && __window_windowlist[x]->wy == __shell_mouse_y) {
                                __window_windowlist[x]->selected = true;
                                __window_windowlist[x]->updated = true;
                            }
                        }
                        x++;
                    }
                } else {
                    uint8_t x = 0;
                    while(x < 32) {
                        if(__window_windowlist[x]) {
                            __window_windowlist[x]->selected = false;
                            __window_windowlist[x]->updated = true;
                        }
                        x++;
                    }
                }
                __shell_mouse_move(0, 0);
                break;
            }
            case KEY_PGDOWN: {
                __serial_write_fmt("CPU ? -> tos > Page Down Key!!\n");
                break;
            }
            case KEY_HOME: {
                __shell_mouse_wnd_mov_mode = !__shell_mouse_wnd_mov_mode;
                if(!__shell_mouse_wnd_mov_mode) {
                    __shell_mouse_wnd_sel_mode = false;
                }
            }
        }
        hh = 0;
    }
    hh++;
}
void __shell_mouse_create_wnd() {
    __shell_mouse_wnd_context.sx = 1;
    __shell_mouse_wnd_context.sy = 1;
    __shell_mouse_wnd_context.wx = 9;
    __shell_mouse_wnd_context.wy = 16;
    __shell_mouse_wnd_old_context = (char *)(malloc(8 * 16).address);
    __shell_mouse_wnd_context.ctx0 = __shell_mouse_wnd_old_context;
    __shell_mouse_x = 9;
    __shell_mouse_y = 16;
    __window_save_context(__shell_mouse_wnd_old_context, &__shell_mouse_wnd_context);
    putc_gui('\x07', 0x005F1F2F, __shell_mouse_x, __shell_mouse_y);
    __smt_create_task(__shell_mouse_thread);
    return;
}