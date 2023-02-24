#include "../include/gui.h"
#include "../include/screen.h"
#include "../include/tunnel.h"
#include "../include/stdint.h"
#include "../include/stdio.h"
#include "../include/cstring.h"
#include "../include/serial.h"
#include "../include/assert.h"
#include "../include/bootboot.h"
#include "../include/stdlib.h"

color_t __gui_generateRandomColor() {
    return (color_t){rand() % 256, rand() % 256, rand() % 256};
}

vector2d_t __gui_alignText(const char *text) {
    assert(text);

    vector2d_t t;
    int l = strlen(text);
    int i = 0;
    int newlines = 0;
    t.x = (__gui_getTextResolution().x - l) / 2;
    while (i < l)
    {
        if (text[i] == '\n')
            newlines++;
        i++;
    }
    t.y = (__gui_getTextResolution().y - newlines) / 2;
    return t;
}
void __gui_drawText(vector2d_t pos, vector2d_t max_size, color_t color, const char *text, text_framebuffer_t *tfb) {
    assert(text);
    assert(pos.x >= 0 && pos.y >= 0);
    assert(max_size.x >= 0 && max_size.y >= 0);
    
    int i = 0;
    int m = strlen(text);

    vector2d_t c = pos;

    while(i < m) {
        if(tfb) {
            int el = ELEMENT(c.x, c.y, tfb->size.x);
            tfb->fb[el] = text[i];
            tfb->color_table[el].a = color.a;
            tfb->color_table[el].r = color.r;
            tfb->color_table[el].g = color.g;
            tfb->color_table[el].b = color.b;
        } else {
            putc(text[i], __color_to_int(color), c.x, c.y);   
        }

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

void __gui_drawRectangle(vector2d_t pos, vector2d_t size, color_t color, text_framebuffer_t *tfb)
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
            if(tfb) {
                int el = ELEMENT(x, y, tfb->size.x);
                tfb->fb[el] = 0x13;
                tfb->color_table[el].a = color.a;
                tfb->color_table[el].r = color.r;
                tfb->color_table[el].g = color.g;
                tfb->color_table[el].b = color.b;
            } else {
                puts("\x13", __color_to_int(color), x, y);
            }
            y++;
        }
        x++;
        y = pos.y;
    }
}
void __gui_drawProgressBar(vector2d_t pos, vector2d_t maxSize, int percentage, color_t col, text_framebuffer_t *tfb)
{
    assert(pos.x >= 0 && pos.y >= 0);
    assert(maxSize.x >= 0 && maxSize.y >= 0);

    float t = (float)percentage / (float)100;
    float x = t * (float)maxSize.x;
    __gui_drawRectangle(pos, (vector2d_t){(int)x, maxSize.y}, col, tfb);
}
void __gui_drawInputBar(vector2d_t pos, const char *buffer, int maxSymbols, text_framebuffer_t *tfb) {
    assert(buffer);
    assert(pos.x >= 0 && pos.y >= 0);

    __gui_drawRectangle(pos, (vector2d_t){maxSymbols, 1}, COLOR_LIGHT_GRAY, tfb);
    int i = 0;
    int l = strlen(buffer);
    while(i < maxSymbols) {
        if(buffer) {
            if(buffer[i] != 0 || i < l) {
                if(tfb) {
                    int el = ELEMENT(pos.x, pos.y, tfb->size.x);
                    tfb->fb[el] = 0x13;
                    tfb->color_table[el].a = 0;
                    tfb->color_table[el].r = 0;
                    tfb->color_table[el].g = 0;
                    tfb->color_table[el].b = 0;
                } else {
                    putc(buffer[i], __color_to_int(COLOR_BLACK), pos.x, pos.y);
                }
                pos.x++;
            } else {
                return;
            }
        }
        i++;
    }
}
void __gui_drawTable(vector2d_t pos, int row0w, int row1w, gui_table_t table, text_framebuffer_t *tfb) {
    vector2d_t tpos = pos;
    row0w++;
    row1w++;
    __gui_drawRectangle(pos, (vector2d_t){row0w + row1w + 1, table.columnCount * 3}, COLOR_WHITE, tfb);
    __gui_drawRectangle(pos, (vector2d_t){row0w + row1w + 1, 1}, COLOR_BLACK, tfb);
    __gui_drawRectangle(pos, (vector2d_t){1, table.columnCount * 3}, COLOR_BLACK, tfb);
    tpos.x += row0w + row1w + 1;
    __gui_drawRectangle(tpos, (vector2d_t){1, table.columnCount * 3}, COLOR_BLACK, tfb);
    tpos.y += table.columnCount * 3 + table.columnCount;
    tpos.x = pos.x;
    int i = 0;
    tpos = pos;
    while(i < table.columnCount) {
        tpos.x = pos.x;
        __gui_drawRectangle(tpos, (vector2d_t){row0w + row1w + 2, 1}, COLOR_BLACK, tfb);
        tpos.y++;
        tpos.x++;
        __gui_drawInputBar(tpos, table.row0Values[i], row0w, tfb);
        tpos.x += row0w;
        __gui_drawInputBar(tpos, table.row1Values[i], row1w, tfb);
        tpos.y += 2;
        i++;
    }
    tpos.x = pos.x;
    __gui_drawRectangle(tpos, (vector2d_t){row0w + row1w + 2, 1}, COLOR_BLACK, tfb);
    tpos.x = pos.x + row0w;
    tpos.y = pos.y + 1;
    __gui_drawRectangle(tpos, (vector2d_t){1, table.columnCount * 3}, COLOR_BLACK, tfb);
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
            *((uint32_t*)((uint64_t)__video_get_fb(false) + (y * tunnelos_sysinfo.bootboot.fb_scanline) + (x * 4))) = *(uint32_t *)&col;
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
            *((uint32_t*)((uint64_t)__video_get_fb(false) + (y * tunnelos_sysinfo.bootboot.fb_scanline) + (x * 4))) = imd[p];
            y++;
            p++;
        }
        x++;
        y = pos.y;
    }
}


vector2d_t __gui_getScreenResolution() {
    int i = 0;
    int l = strlen(environment);

    vector2d_t size = (vector2d_t){0, 0};

    while(i < l) {
        char *el0 = (char *)malloc(64);
        char *el1 = (char *)malloc(64);
        int i2 = 0;
        while(i2 < 64) {
            el0[i2] = environment[i];
            if(el0[i2] == '=') {
                el0[i2] = 0;
                break;
            }
            if(el0[i2] == 0 || el0[i2] == '\n') {
                free(el0);
                free(el1);
                return size;
            }

            i2++;
            i++;
        }

        i2 = 0;
        i++;
        while(i2 < 64) {
            el1[i2] = environment[i];

            if(el1[i2] == 0 || el1[i2] == '\n') {
                el1[i2] = 0;
                break;
            }

            i2++;
            i++;
        }

        if(!strcmp(el0, "screen")) {
            int i3 = 0;
            int i4 = 0;
            int l2 = strlen(el1) + 1;
            char *buf1 = (char *)malloc(16);

            while(i3 < 16) {
                buf1[i3] = el1[i4];

                if(buf1[i3] == 'x') {
                    buf1[i3] = 0;
                    break;
                }
                
                i4++;
                i3++;
            }

            size.x = atoi(buf1);

            i4++;
            i3 = 0;
            memset(buf1, 0, 16);
            while(i3 < 16) {
                buf1[i3] = el1[i4];

                if(buf1[i3] == 0) {
                    break;
                }
                
                i4++;
                i3++;
            }
            size.y = atoi(buf1);;

            free(buf1);
            free(el0);
            free(el1);

            return size;
        }

        free(el0);
        free(el1);
        i++;
        i2 = 0;
    }

    return size;
}

vector2d_t __gui_getTextResolution() {
    vector2d_t vec = __gui_getScreenResolution();

    vec.x /= 8;
    vec.y /= 16;
    
    return vec;
}