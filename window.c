#include "./include/window.h"
#include "./include/stdio.h"
#include "./include/cstring.h"
#include "./include/serial.h"
#include "./include/tools.h"
#include "./include/screen.h"

bool __window_lock = false;
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
                putc_gui('\x08', (cw->rainbow) ? testi : cw->color, x1, y1);
                i1++;
                while(i1 < x1 + cw->sx) {
                    if(((i1 - x1) > i3) && (i1 < (x1 + cw->sx - 1 - cw->can_be_in_background))) {
                        putc_gui('\x01', (cw->rainbow) ? testi : cw->color, i1, y1);
                    }
                    putc_gui('\x01', (cw->rainbow) ? testi : cw->color, i1, y1 + cw->sy);
                    i1++;
                }
                putc_gui('\x02', (cw->rainbow) ? testi : cw->color, i1, y1);
                putc_gui('\x04', (cw->rainbow) ? testi : cw->color, i1, y1 + cw->sy);
                putc_gui('\x03', (cw->rainbow) ? testi : cw->color, x1, y1 + cw->sy);
                i1 = x1;
                while(i2 < y1 + cw->sy) {
                    putc_gui('\x09', (cw->rainbow) ? testi : cw->color, x1, i2);
                    putc_gui('\x09', (cw->rainbow) ? testi : cw->color, x1 + cw->sx, i2);
                    i2++;
                }
                i1 = x1 + cw->sx - 1;
                if(cw->can_be_closed) {
                    putc_gui('\x05', (cw->rainbow) ? testi : cw->color, i1, y1);
                    i1--;
                }
                if(cw->can_be_in_background) putc_gui('\x06', (cw->rainbow) ? testi : cw->color, i1, y1);
                while(i4 < i3) {
                    if((x1 + i4 + cw->can_be_closed + cw->can_be_in_background + 2) > i1) {
                        putc('|', (cw->rainbow) ? testi : cw->color, x1 + i4 + 1, y1);
                        i4 = i3 + 1;
                    } else {
                        putc(cw->name[i4], (cw->rainbow) ? testi : cw->color, x1 + i4 + 1, y1);
                    }
                    i4++;
                }    
            }
            __window_windowlist[i]->updated = true;
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
    if(__window_lock) {
        __serial_write_fmt("CPU %d -> tos > Waiting for unlocking\r\n", __tools_get_cpu() - 1);
        while(__window_lock);
    }
    __window_lock = true;
    __serial_write_fmt("CPU %d -> tos > Creating new window\r\n", __tools_get_cpu() - 1);
    int i = 0;
    while(i < 32) {
        if(!__window_used[i]) {
            __window_used[i] = true;
            __window_windowlist[i] = window_settings;
            window_settings->id = __window_id;
            __window_id++;
            __serial_write_fmt("CPU %d -> tos > New window has been created\r\n", __tools_get_cpu() - 1);
            __window_lock = false;
            return;
        }
        i++;
    }
    __window_lock = false;
    return;
}
void __window_remove(int id) {
    if(__window_lock) while(__window_lock);
    __window_lock = true;
    int i = 0;
    while(i < 32) {
        if(__window_used[i]) {
            if(__window_windowlist[i]->id == id) {
                __window_used[i] = false;
                __window_lock = false;
                return;
            }
        }
        i++;
    }
    __window_lock = false;
    return;
}
//copies part of framebuffer to old_context, uses x and y from new_context
void __window_save_context(char *old_context, window_t *new_context) {
    int x1 = (new_context->wx + 300) * 8; int x2 = x1 + (new_context->sx + 300) * 8;
    int y1 = new_context->wy * 16; int y2 = y1 + new_context->sy * 16;
    int ocp = 0; int s = tunnelos_sysinfo.bootboot.fb_scanline;
    uint8_t *fbp = (uint8_t *)&fb;
    
    while(x1 < x2) {
        while(y1 < y2){
            old_context[ocp] = fbp[(s * y1) + x1];
            ocp++;
            y1++;
        }
        x1++;
        y1 = new_context->wy * 16;
    }
    return;
}
void __window_read_old_ctx(char *old_context, window_t *crt_context) {
    int x1 = (crt_context->wx + 300) * 8; int x2 = x1 + (crt_context->sx + 300) * 8;
    int y1 = crt_context->wy * 16; int y2 = y1 + crt_context->sy * 16;
    int ocp = 0; int s = tunnelos_sysinfo.bootboot.fb_scanline;
    uint8_t *fbp = (uint8_t *)&fb;
    
    while(x1 < x2) {
        while(y1 < y2){
            fbp[(s * y1) + x1] = old_context[ocp];
            ocp++;
            y1++;
        }
        x1++;
        y1 = crt_context->wy * 16;
    }

    return;
}