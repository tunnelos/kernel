#pragma once

#include "./stdint.h"

#define COLOR_RED   0x00FF0000
#define COLOR_GREEN 0x0000FF00
#define COLOR_BLUE  0x000000FF
#define COLOR_WHITE 0x00FFFFFF

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

extern volatile unsigned char _binary_fonts_text_psf_start;
extern volatile unsigned char _binary_fonts_gui_psf_start;
typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t headersize;
    uint32_t flags;
    uint32_t numglyph;
    uint32_t bytesperglyph;
    uint32_t height;
    uint32_t width;
    uint8_t glyphs;
} __attribute__((packed)) psf2_t;

extern uint8_t fb;

//#define WRITE_PIXEL(x, y, color, fb, scanline) *((uint32_t *)(&fb + scanline * y + x * 4)) = color