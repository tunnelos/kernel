#pragma once

#include "./stdint.h"
#include "./main.h"

// By SaziX
#define COLOR_RED           0x00FF0000
#define COLOR_GREEN         0x0000FF00
#define COLOR_BLUE          0x000000FF
#define COLOR_WHITE         0x00FFFFFF
#define COLOR_BLACK         0x00000000
#define COLOR_DARK_GRAY     0x00808080
#define COLOR_DARK_GREEN    0x00008000
#define COLOR_GRAY          0x00C0C0C0
#define COLOR_LIGHT_GREEN   0x0046C21D
#define COLOR_YELLOW        0X00FFF200
#define COLOR_LIGHT_GRAY    0x00BDBDB9

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

void __screen_flush(int width, int height);
void __screen_write_pixel(int pos_x, int pos_y, uint32_t color);
void __screen_reset_draw_limit();
uint32_t **__screen_get_draw_limit();
void __screen_set_draw_limit(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2);


extern uint32_t __screen_draw_x1;
extern uint32_t __screen_draw_x2; 
extern uint32_t __screen_draw_y1; 
extern uint32_t __screen_draw_y2;