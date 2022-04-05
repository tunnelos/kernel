#include "./include/window.h"
#include "./include/stdio.h"
#include "./include/cstring.h"
#include "./include/serial.h"
#include "./include/tools.h"

window_t *__window_windowlist[32];
bool __window_used[32];

int __window_id = 0;

int testi = 0;
void __window_renderer(int id) {
    testi++;
    int i = 0;
    while(i < 32) {
        if(__window_used[i]) {
            if(!__window_windowlist[i]->in_background && __window_windowlist[i]->draw_border) {
                window_t *cw = __window_windowlist[i];
                int x1 = cw->wx;
                int y1 = cw->wy;
                int i1 = x1;
                int i2 = y1 + 1;
                int i3 = strlen(cw->name);
                int i4 = 0;
                putc_gui('\x08', testi, x1, y1);
                i1++;
                while(i1 < x1 + cw->sx) {
                    if(((i1 - x1) > i3) && (i1 < (x1 + cw->sx - 1 - cw->can_be_in_background))) {
                        putc_gui('\x01', testi, i1, y1);
                    }
                    putc_gui('\x01', testi, i1, y1 + cw->sy);
                    i1++;
                }
                putc_gui('\x02', testi, i1, y1);
                putc_gui('\x04', testi, i1, y1 + cw->sy);
                putc_gui('\x03', testi, x1, y1 + cw->sy);
                i1 = x1;
                while(i2 < y1 + cw->sy) {
                    putc_gui('\x09', testi, x1, i2);
                    putc_gui('\x09', testi, x1 + cw->sx, i2);
                    i2++;
                }
                i1 = x1 + cw->sx - 1;
                if(cw->can_be_closed) {
                    putc_gui('\x05', testi, i1, y1);
                    i1--;
                }
                if(cw->can_be_in_background) putc_gui('\x06', testi, i1, y1);
                while(i4 < i3) {
                    if((x1 + i4 + 1) > i1) {
                        putc('|', testi, x1 + i4 + 1, y1);
                        i4 = i3 + 1;
                    } else {
                        putc(cw->name[i4], testi, x1 + i4 + 1, y1);
                    }
                    i4++;
                }    
            }
        }
        i++;
    }
}
void __window_init(){
    int i = 0;
    while(i < 32){
        __window_used[i] = false;
        i++;
    }
    __smt_create_task(__window_renderer);
}
void __window_create(window_t *window_settings){
    int i = 0;
    while(i < 32) {
        if(!__window_used[i]) {
            __window_used[i] = true;
            __window_windowlist[i] = window_settings;
            window_settings->id = __window_id;
            __window_id++;
            __serial_write_fmt("CPU %d -> tos > New window has been created\r\n", __tools_get_cpu() - 1);
            return;
        }
        i++;
    }
    return;
}
void __window_remove(int id) {
    int i = 0;
    while(i < 32) {
        if(__window_used[i]) {
            if(__window_windowlist[i]->id == id) {
                __window_used[i] = false;
                return;
            }
        }
        i++;
    }
    return;
}