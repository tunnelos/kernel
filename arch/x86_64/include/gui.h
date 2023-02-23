#pragma once

#include "bmp.h"
#include "color.h"

#pragma pack(push, 1)
typedef struct vector2d_t {
    int x;
    int y;
} vector2d_t;
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
void __gui_drawText(vector2d_t pos, vector2d_t max_size, color_t color, const char *text);
void __gui_drawRectangle(vector2d_t pos, vector2d_t size, color_t color);
void __gui_drawInputBar(vector2d_t pos, const char *buffer, int maxSymbols);
void __gui_drawProgressBar(vector2d_t pos, vector2d_t maxSize, int percentage, color_t color);
void __gui_drawTable(vector2d_t pos, int row0w, int row1w, gui_table_t table);
void __gui_drawImage24(BMPImage *image, vector2d_t pos);
void __gui_drawImage32(BMPImage *image, vector2d_t pos);
color_t __gui_generateRandomColor();
vector2d_t __gui_getScreenResolution();
vector2d_t __gui_getTextResolution();

#ifdef __cplusplus
}
#endif