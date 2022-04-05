#include "./include/stdio.h"
#include "./include/screen.h"
#include "./include/tunnel.h"
#include <stdarg.h>
#include "./include/cstring.h"
#include "./include/cint.h"
#include "./include/serial.h"

int __stdio_margin = 0;
int __stdio_gui_margin = 0;
bool terminal_block = false;

void puts(char *s, uint32_t color, int x4, int y4){
    int tx = x4 * 8;
    int ty = y4 * 16;
    int kx = 0;
    if(terminal_block) while(terminal_block);

    terminal_block = true;
    int x, y, line, mask, offs;
    psf2_t *font = (psf2_t*)&_binary_fonts_text_psf_start;
    int bpl = (font->width + 7) / 8;
    while(*s) {
        //72
        unsigned char *glyph = (unsigned char *)&_binary_fonts_text_psf_start + font->headersize + (*s > 0 && *s < font->numglyph ? *s : 0)*font->bytesperglyph;
        offs = (kx * (font->width + __stdio_margin) * 4);
        if(*s == '\n' || kx >= 71) {
            ty += font->height;
            kx = 0;
            tx = 0 - font->width;
        } else {
            for(y = 0; y < font->height; y++) {
                line = offs; 
                mask = 1 << (font -> width - 1);
                for(x = 0; x < font->width; x++) {
                    *((uint32_t*)((uint64_t)&fb + line + (ty * tunnelos_sysinfo.bootboot.fb_scanline) + (tx * 4))) = ((int)*glyph) & (mask) ? color : 0xFF000000;
                    mask >>= 1; 
                    line += 4;
                } 
                glyph += bpl;
                offs += tunnelos_sysinfo.bootboot.fb_scanline;
            }
        }
        s++; 
        kx++;
    }
    terminal_block = false;
}

void putc(char c, uint32_t color, int x, int y) {
    char str[2] = {c, '\0'};
    puts(str, color, x, y);
    return;
}

void puts_gui(char *s, uint32_t color, int x4, int y4){
    int tx = x4 * 8;
    int ty = y4 * 16;
    int kx = 0;
    if(terminal_block) while(terminal_block);

    terminal_block = true;
    int x, y, line, mask, offs;
    psf2_t *font = (psf2_t*)&_binary_fonts_gui_psf_start;
    int bpl = (font->width + 7) / 8;
    while(*s) {
        //72
        unsigned char *glyph = (unsigned char*)&_binary_fonts_gui_psf_start + font->headersize + (*s > 0 && *s < font->numglyph ? *s : 0)*font->bytesperglyph;
        offs = (kx * (font->width + __stdio_gui_margin) * 4);
        if(*s == '\n' || kx >= 71) {
            ty += font->height;
            kx = 0;
            tx = 0 - font->width;
        } else {
            for(y = 0; y < font->height; y++) {
                line = offs; 
                mask = 1 << (font -> width - 1);
                for(x = 0; x < font->width; x++) {
                    *((uint32_t*)((uint64_t)&fb + line + (ty * tunnelos_sysinfo.bootboot.fb_scanline) + (tx * 4))) = ((int)*glyph) & (mask) ? color : *((uint32_t*)((uint64_t)&fb + line + (ty * tunnelos_sysinfo.bootboot.fb_scanline) + (tx * 4)));
                    mask >>= 1; 
                    line += 4;
                } 
                glyph += bpl;
                offs += tunnelos_sysinfo.bootboot.fb_scanline;
            }
        }
        s++; 
        kx++;
    }
    terminal_block = false;
}

void putc_gui(char c, uint32_t color, int x, int y) {
    char str[2] = {c, '\0'};
    puts_gui(str, color, x, y);
    return;
}

void printf(uint32_t color, int x, int y, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int i = 0;
    int len = strlen(fmt);
    
    while(i < len) {
        switch(fmt[i]){
            case '%': {
                switch(fmt[i + 1]) {
                    case 'c': {
                        char arg = va_arg(ap, int);
                        putc(arg, color, x, y);
                        i += 2;
                        break;
                    }
                    case 's': {
                        char *arg = va_arg(ap, char *);
                        puts(arg, color, x, y);
                        i += 2;
                        break;
                    }
                    case 'i':
                    case 'd': {
                        int arg = va_arg(ap, int);
                        if(arg == 0) puts("0", color, x, y);
                        else {
                            char buffer[20];
                            puts(itoa(arg, buffer, 10), color, x, y);
                        }
                        i += 2;
                        break;
                    }
                    case 'x':
                    case 'X': {
                        int arg = va_arg(ap, int);
                        char buffer[20];
                        puts(itoa(arg, buffer, 16), color, x, y);
                        i += 2;
                        break;
                    }
                    case 'o': {
                        int arg = va_arg(ap, int);
                        char buffer[20];
                        puts(itoa(arg, buffer, 8), color, x, y);
                        i += 2;
                        break;
                    }
                    default: {
                        va_end(ap);
                        break;
                    }
                }
            }
            default: {
                putc(fmt[i], color, x, y);
                va_end(ap);
                break;
            }
        }
        i++;
    }
    return;
}

char *itoa(int num, char *buffer, int base) {
    int buffer_size = 20;
    int counter = 0;
    int digit = 0;
    
    while(num != 0 && counter < buffer_size){
        digit = (num % base);
        if(digit > 9){
            buffer[counter++] = itoh(digit, true);
        } else {
            buffer[counter++] = itoc(digit);
        }
        num /= base;
    }

    buffer[counter] = '\0';
    return strrev(buffer);
}

char itoh(int num, bool upper) {
    if(upper) return num - 10 + 'A';
    return num - 10 + 'a';
}

char itoc(int num) {
    return '0' + num;
}

char* strrev(char* src) {
    char tmp;
    int str_index = 0;
    int last_char = strlen(src) - 1;
    
    while(str_index < last_char){
        tmp = src[str_index];
        src[str_index] = src[last_char];
        src[last_char] = tmp;
        last_char--;
        str_index++;
    }

    src[strlen(src)] = '\0';

    return src;
}
