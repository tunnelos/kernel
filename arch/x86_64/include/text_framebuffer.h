#pragma once

#include "color.h"
#include "vector2d.h"

#define ELEMENT(x, y, w) x * w + y

typedef struct {
    // 0x00 is alpha channel
    char *fb;
    color_t *color_table;

    vector2d_t size;
} text_framebuffer_t;

text_framebuffer_t __textfb_create(vector2d_t resolution);
// Merge fb2 to fb1
void __textfb_merge(text_framebuffer_t fb1, text_framebuffer_t fb2, vector2d_t position_destination);
// Sizes of text fb and pixel fb SHOULD match or bad things will happen
void __textfb_render(text_framebuffer_t tfb, char *pfb);