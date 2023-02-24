#include "../include/text_framebuffer.h"
#include "../include/stdlib.h"
#include "../include/screen.h"

text_framebuffer_t __textfb_create(vector2d_t resolution) {
    text_framebuffer_t fb;

    fb.fb = (char *)malloc(resolution.x * resolution.y);
    void *a = (color_t *)malloc(resolution.x * resolution.y);
    void *b = (color_t *)malloc(resolution.x * resolution.y);
    fb.color_table = (char *)malloc(resolution.x * resolution.y);
    free(a);
    free(b);
    fb.size.x = resolution.x;
    fb.size.y = resolution.y;

    return fb;

}
void __textfb_merge(text_framebuffer_t fb1, text_framebuffer_t fb2, vector2d_t position_destination) {
    if(position_destination.x > fb1.size.x) return;
    if(position_destination.y > fb1.size.y) return;
    
    int x = 0;
    int y = 0;

    while(x < fb2.size.x) {
        while(y < fb2.size.y) {
            if(fb2.fb[ELEMENT(x, y, fb2.size.x)] != 0x00 && __color_to_int(fb2.color_table[ELEMENT(x, y, fb2.size.x)]) != 0) {
                fb1.fb[ELEMENT(x, y, fb1.size.x)] = fb2.fb[ELEMENT(x, y, fb2.size.x)];
                fb1.color_table[ELEMENT(x, y, fb1.size.x)].r = fb2.color_table[ELEMENT(x, y, fb2.size.x)].r;
                fb1.color_table[ELEMENT(x, y, fb1.size.x)].g = fb2.color_table[ELEMENT(x, y, fb2.size.x)].g;
                fb1.color_table[ELEMENT(x, y, fb1.size.x)].b = fb2.color_table[ELEMENT(x, y, fb2.size.x)].b;
            }
            y++;
        }
        y = 0;
        x++;
    }
}
// Sizes of text fb and pixel fb SHOULD match or bad things will happen
void __textfb_render(text_framebuffer_t tfb, char *pfb) {
    int x = 0;
    int y = 0;
    int w = tfb.size.x;
    while(x < (tfb.size.x)) {
        while(y < tfb.size.y) {
            char el = tfb.fb[ELEMENT(x, y, w)];
            putcfb(el, __color_to_int(tfb.color_table[ELEMENT(x, y, w)]), x, y, pfb);
            y++;
        }
        x++;
        y = 0;
    }
}