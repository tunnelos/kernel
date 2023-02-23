#include "../include/video.h"

#include "../include/stdlib.h"
#include "../include/screen.h"
#include "../include/tools.h"

#include "../include/gui.h"

char *__video_fb2 = NULL;
bool __video_failure = false;

void __video_setup_core() {
    vector2d_t s = __gui_getScreenResolution();
    __video_fb2 = (char *)malloc(s.x * s.y * 4);
    __serial_write_fmt("CPU %d -> tos > fb at %l 0x%X\r\n", __tools_get_cpu() - 1,__video_fb2);
}
void __video_switch_to_failure_mode() {
    __video_fb2 = (char *)&fb;
    __video_failure = true;
}
void __video_refresh() {
    int frames = 0;
    vector2d_t s = __gui_getScreenResolution();
    int n = s.x * s.y * 4;
    goto l;
    l:
    frames++;
    //memcpy(&fb, (const void *)__video_fb2, s.x * s.y * 4);
    size_t i = 0;
    char *d = (char *)&fb;
    char *c = (char *)__video_fb2;
    while(i < n) {
        if(__video_failure) return;
        d[i] = c[i];
        i++;
    }
    __serial_write_fmt("CPU %d -> video > rendered %d frames (%d)\r\n", __tools_get_cpu() - 1, frames, frames / 30);
    wait(33);
    goto l;
}

char *__video_get_fb(bool hardware_fb) {
    if(hardware_fb) return &fb;

    return __video_fb2;
}