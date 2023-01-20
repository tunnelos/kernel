#include "../include/gui.h"
#include "../include/screen.h"
#include "../include/tunnel.h"
#include "../include/stdint.h"
#include "../include/stdio.h"
#include "../include/cstring.h"
#include "../include/serial.h"
#include "../include/assert.h"

vector2d_t __gui_alignText(const char *text) {
    assert(text);

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
void __gui_drawText(vector2d_t pos, vector2d_t max_size, color_t color, const char *text) {
    assert(text);
    assert(pos.x >= 0 && pos.y >= 0);
    assert(max_size.x >= 0 && max_size.y >= 0);
    
    int i = 0;
    int m = strlen(text);

    vector2d_t c = pos;

    while(i < m) {
        putc(text[i], __color_to_int(color), c.x, c.y);

        i++;
        c.x++;
        if(c.x > max_size.x + pos.x) {
            c.x = pos.x;
            c.y++;
        }
        if(c.y > max_size.y + pos.y) {
            c.y = pos.y;
            c.x++;
        }
    }
}

void __gui_drawRectangle(vector2d_t pos, vector2d_t size, color_t color)
{
    assert(pos.x >= 0 && pos.y >= 0);
    assert(size.x >= 0 && size.y >= 0);

    int x = pos.x;
    int y = pos.y;
    int xm = x + size.x;
    int ym = y + size.y;
    while (x < xm)
    {
        while (y < ym)
        {
            puts("\x13", __color_to_int(color), x, y);
            y++;
        }
        x++;
        y = pos.y;
    }
}
void __gui_drawProgressBar(vector2d_t pos, vector2d_t maxSize, int percentage, color_t col)
{
    assert(pos.x >= 0 && pos.y >= 0);
    assert(maxSize.x >= 0 && maxSize.y >= 0);

    float t = (float)percentage / (float)100;
    float x = t * (float)maxSize.x;
    __gui_drawRectangle(pos, (vector2d_t){(int)x, maxSize.y}, col);
}
void __gui_drawInputBar(vector2d_t pos, const char *buffer, int maxSymbols) {
    assert(buffer);
    assert(pos.x >= 0 && pos.y >= 0);

    __gui_drawRectangle(pos, (vector2d_t){maxSymbols, 1}, COLOR_LIGHT_GRAY);
    int i = 0;
    int l = strlen(buffer);
    while(i < maxSymbols) {
        if(buffer) {
            if(buffer[i] != 0 || i < l) {
                putc(buffer[i], __color_to_int(COLOR_BLACK), pos.x, pos.y);
                pos.x++;
            } else {
                return;
            }
        }
        i++;
    }
}
void __gui_drawTable(vector2d_t pos, int row0w, int row1w, gui_table_t table) {
    vector2d_t tpos = pos;
    row0w++;
    row1w++;
    __gui_drawRectangle(pos, (vector2d_t){row0w + row1w + 1, table.columnCount * 3}, COLOR_WHITE);
    __gui_drawRectangle(pos, (vector2d_t){row0w + row1w + 1, 1}, COLOR_BLACK);
    __gui_drawRectangle(pos, (vector2d_t){1, table.columnCount * 3}, COLOR_BLACK);
    tpos.x += row0w + row1w + 1;
    __gui_drawRectangle(tpos, (vector2d_t){1, table.columnCount * 3}, COLOR_BLACK);
    tpos.y += table.columnCount * 3 + table.columnCount;
    tpos.x = pos.x;
    int i = 0;
    tpos = pos;
    while(i < table.columnCount) {
        tpos.x = pos.x;
        __gui_drawRectangle(tpos, (vector2d_t){row0w + row1w + 2, 1}, COLOR_BLACK);
        tpos.y++;
        tpos.x++;
        __gui_drawInputBar(tpos, table.row0Values[i], row0w);
        tpos.x += row0w;
        __gui_drawInputBar(tpos, table.row1Values[i], row1w);
        tpos.y += 2;
        i++;
    }
    tpos.x = pos.x;
    __gui_drawRectangle(tpos, (vector2d_t){row0w + row1w + 2, 1}, COLOR_BLACK);
    tpos.x = pos.x + row0w;
    tpos.y = pos.y + 1;
    __gui_drawRectangle(tpos, (vector2d_t){1, table.columnCount * 3}, COLOR_BLACK);
    return;
}
void __gui_drawImage24(BMPImage *image, vector2d_t pos) {
    assert(image);
    assert(pos.x >= 0 && pos.y >= 0);
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
    assert(image);
    assert(pos.x >= 0 && pos.y >= 0);

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