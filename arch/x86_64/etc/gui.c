#include "../include/gui.h"
#include "../include/screen.h"
#include "../include/tunnel.h"
#include "../include/stdint.h"
#include "../include/stdio.h"
#include "../include/cstring.h"

vector2d_t alignText(const char *text)
{
    vector2d_t t;
    int l = strlen(text);
    int i = 0;
    int newlines = 0;
    t.x = (80 - l) / 2;
    while (i < l)
    {
        if (text[i] == '\n')
            newlines++;
        i++;
    }
    t.y = (30 - newlines) / 2;
    return t;
}

void __gui_drawRectangle(vector2d_t pos, vector2d_t size, int color)
{
    int x = pos.x;
    int y = pos.y;
    int xm = x + size.x;
    int ym = y + size.y;
    while (x < xm)
    {
        while (y < ym)
        {
            puts("\x13", color, x, y);
            y++;
        }
        x++;
        y = pos.y;
    }
}
void __gui_drawProgressBar(vector2d_t pos, vector2d_t maxSize, int percentage, int col)
{
    float t = (float)percentage / (float)100;
    float x = t * (float)maxSize.x;
    __gui_drawRectangle(pos, (vector2d_t){(int)x, maxSize.y}, col);
}
void __gui_drawInputBar(vector2d_t pos, const char *buffer, int maxSymbols) {
    __gui_drawRectangle(pos, (vector2d_t){maxSymbols, 1}, COLOR_LIGHT_GRAY);
    int i = 0;
    char b[2] = {' ', 0};
    while(i < maxSymbols) {
        if(buffer[i] != 0) {
            b[0] = buffer[i];
            puts(b, COLOR_BLACK, pos.x, pos.y);
            pos.x++;
        }
        i++;
    }
}
void __gui_drawImage24(BMPImage *image, vector2d_t pos) {
    int x = pos.x;
    int y = pos.y;
    int xm = image->header.width_px + pos.x;
    int ym = image->header.height_px + pos.y; 
    int p = 0;
    char col[4] = { //ARGB
        0x00, image->data[p + 0], image->data[p + 1], image->data[p + 2]
    };
    while(x < xm) {
        while(y < ym) {
            col[1] = image->data[p + 0];
            col[2] = image->data[p + 1];
            col[3] = image->data[p + 2];
            *((uint32_t*)((uint64_t)&fb + (y * tunnelos_sysinfo.bootboot.fb_scanline) + (x * 4))) = *(uint32_t *)&col;
            y++;
            p += 3;
        }
        x++;
        y = pos.y;
    }
}
void __gui_drawImage32(BMPImage *image, vector2d_t pos) {
    int x = pos.x;
    int y = pos.y;
    int xm = image->header.width_px + pos.x;
    int ym = image->header.height_px + pos.y; 
    int p = 0;
    int *imd = (int *)&image->data;
    while(x < xm) {
        while(y < ym) {
            *((uint32_t*)((uint64_t)&fb + (y * tunnelos_sysinfo.bootboot.fb_scanline) + (x * 4))) = imd[p];
            y++;
            p++;
        }
        x++;
        y = pos.y;
    }
}