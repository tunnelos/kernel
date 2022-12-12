#include "../include/video.h"

#include "../include/stdlib.h"
#include "../include/screen.h"
#include "../include/tools.h"

char *__video_fb2 = NULL;

void __video_setup_core() {
    __video_fb2 = (char *)malloc(640 * 480 * 4);
    __serial_write_fmt("CPU %d -> tos > fb at %l 0x%X\r\n", __tools_get_cpu() - 1,__video_fb2);
}
void __video_refresh() {
    int frames = 0;
    goto l;
    l:
    frames++;
    memcpy(&fb, (const void *)__video_fb2, 640 * 480 * 4);
    wait(33);
    goto l;
}

char *__video_get_fb(bool hardware_fb) {
    if(hardware_fb) return &fb;

    return __video_fb2;
}