#pragma once

#include "bmp.h"
#include "color.h"
#include "vector2d.h"

#include  "./text_framebuffer.h"

#pragma pack(push, 1)
typedef struct gui_table_t {
    const char **row0Values;
    const char **row1Values;
    int columnCount;
} gui_table_t;
#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

vector2d_t __gui_alignText(const char *text);
// if tfb is null, gui will draw things to main pixel framebuffer
void __gui_drawText(vector2d_t pos, vector2d_t max_size, color_t color, const char *text, text_framebuffer_t *tfb);
// if tfb is null, gui will draw things to main pixel framebuffer
void __gui_drawRectangle(vector2d_t pos, vector2d_t size, color_t color, text_framebuffer_t *tfb);
// if tfb is null, gui will draw things to main pixel framebuffer
void __gui_drawInputBar(vector2d_t pos, const char *buffer, int maxSymbols, text_framebuffer_t *tfb);
void __gui_drawProgressBar(vector2d_t pos, vector2d_t maxSize, int percentage, color_t color, text_framebuffer_t *tfb);
void __gui_drawTable(vector2d_t pos, int row0w, int row1w, gui_table_t table, text_framebuffer_t *tfb);
void __gui_drawImage24(BMPImage *image, vector2d_t pos);
void __gui_drawImage32(BMPImage *image, vector2d_t pos);
color_t __gui_generateRandomColor();
vector2d_t __gui_getScreenResolution();
vector2d_t __gui_getTextResolution();

#ifdef __cplusplus
}
#endif