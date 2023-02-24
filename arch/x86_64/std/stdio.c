#include "../include/stdio.h"
#include "../include/screen.h"
#include "../include/tunnel.h"
#include <stdarg.h>
#include "../include/cstring.h"
#include "../include/cint.h"
#include "../include/serial.h"
#include "../include/mm.h"
#include "../include/panic.h"
#include "../include/video.h"

int __stdio_margin = 0;
int __stdio_gui_margin = 0;
bool terminal_block = false;

void putsfb(const char *s, uint32_t color, int x4, int y4, char *framebuffer){
    int tx = x4 * 8;
    int ty = y4 * 16;
    int kx = 0;
    if(terminal_block) while(terminal_block);

    terminal_block = true;
    int x, y, line, mask, offs;
    psf2_t *font = (psf2_t*)&_binary_gui_psf_start;
    int bpl = (font->width + 7) / 8;
    while(*s) {
        //72
        unsigned char *glyph = (unsigned char*)&_binary_gui_psf_start + font->headersize + (*s > 0 && *s < font->numglyph ? *s : 0)*font->bytesperglyph;
        offs = (kx * (font->width + __stdio_gui_margin) * 4);
        if(kx >= 71) {
            ty += font->height;
            kx = 0;
            tx = 0 - font->width;
        } else {
            for(y = 0; y < font->height; y++) {
                line = offs; 
                mask = 1 << (font -> width - 1);
                for(x = 0; x < font->width; x++) {
                    *((uint32_t*)((uint64_t)framebuffer + line + (ty * tunnelos_sysinfo.bootboot.fb_scanline) + (tx * 4))) = ((int)*glyph) & (mask) ? color : *((uint32_t*)((uint64_t)framebuffer + line + (ty * tunnelos_sysinfo.bootboot.fb_scanline) + (tx * 4)));
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

void putcfb(const char c, uint32_t color, int x, int y, char *framebuffer) {
    char str[2] = {c, '\0'};
    putsfb(str, color, x, y, framebuffer);
    return;
}

void puts(const char *s, uint32_t color, int x4, int y4){
    putsfb(s, color, x4, y4, __video_get_fb(false));
}

void putc(const char c, uint32_t color, int x, int y) {
    putcfb(c, color, x, y, __video_get_fb(false));
}

char *itoa(int num, char *buffer, int base, int x, int y, int color, bool use_additional) {
    int buffer_size = 20;
    int counter = 0;
    int digit = 0;
    
    if(num == 0) return "0";

    while(num != 0 && counter < buffer_size){
        digit = (num % base);
        if(digit > 9){
            buffer[counter++] = itoh(digit, true, x, y, color, use_additional);
        } else {
            buffer[counter++] = itoc(digit);
        }
        num /= base;
    }

    buffer[counter] = '\0';
    return strrev(buffer);
}
char *itoalong(uint64_t num, char *buffer, int base, int x, int y, int color, bool use_additional) {
    int buffer_size = 20;
    int counter = 0;
    uint64_t digit = 0;
    
    while(num != 0 && counter < buffer_size){
        digit = (num % base);
        if(digit > 9){
            buffer[counter++] = itoh(digit, true, x, y, color, use_additional);
        } else {
            buffer[counter++] = itoc(digit);
        }
        num /= base;
    }

    buffer[counter] = '\0';
    return strrev(buffer);
}

char *stditoa(int num, char *buffer, int base) {
    return itoa(num, buffer, base, 0, 0, 0, false);
}
char *stditoalong(uint64_t num, char *buffer, int base) {
    return itoalong(num, buffer, base, 0, 0, 0, false);
}
char stditoh(int num, bool upper) {
    return itoh(num, upper, 0, 0, 0, false);
}

int sprintf(char *str, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int i = 0;
    int si = 0;
    int len = strlen(fmt);
    bool setlong = false;

    while(i < len) {
        switch(fmt[i]){
            case '%': {
                __serial_write_fmt("Case %c\n", fmt[i + 1]);
                switch(fmt[i + 1]) {
                    case 'c': {
                        char arg = va_arg(ap, int);
                        str[si] = arg;
                        si++;
                        i += 2;
                        break;
                    }
                    case 's': {
                        char *arg = va_arg(ap, char *);
                        strcpy(&str[strlen(arg) + si], arg);
                        si += strlen(arg);
                        i += 2;
                        break;
                    }
                    case 'l': {
                        //set long int flag and reset on 'd' or 'X' after execution
                        setlong = true;
                        i += 2;
                        break;
                    }
                    case 'i':
                    case 'd': {
                        if(setlong) {
                            setlong = false;
                            uint64_t arg = va_arg(ap, uint64_t);
                            if(arg == 0) str[si++] = '0';
                            else {
                                char buffer[20];
                                itoalong(arg, buffer, 10, 0, 0, 0, false);
                                strncmp(&str[si], buffer, strlen(buffer));
                                si += strlen(buffer);
                            }
                            i += 2;
                        } else {
                            int arg = va_arg(ap, int);
                            if(arg == 0) str[si++] = '0';
                            else {
                                char buffer[20];
                                itoa(arg, buffer, 10, 0, 0, 0, false);
                                strncmp(&str[si], buffer, strlen(buffer));
                                si += strlen(buffer);
                            }
                            i += 2;
                        }
                        break;
                    }
                    case 'x':
                    case 'X': {
                        if(setlong) {
                            setlong = false;
                            uint64_t arg = va_arg(ap, uint64_t);
                            char buffer[20];
                            itoalong(arg, buffer, 16, 0, 0, 0, false);
                            strncmp(&str[si], buffer, strlen(buffer));
                            i += 2;
                            si += strlen(buffer);
                        } else {
                            int arg = va_arg(ap, int);
                            char buffer[20];
                            itoa(arg, buffer, 16, 0, 0, 0, false);
                            strncmp(&str[si], buffer, strlen(buffer));
                            i += 2;
                            si += strlen(buffer);
                        }
                        break;
                    }
                    default: {
                        va_end(ap);
                        break;
                    }
                }
                va_end(ap);
                break;
            }
            case '\n': {
                str[si] = '\n';
                si++;
                va_end(ap);
                break;
            }
            default: {
                str[si] = fmt[i];
                si++;
                va_end(ap);
                break;
            }
        }
        i++;
    }
    return i;
}

void printf(uint32_t color, int x, int y, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int i = 0;
    int len = strlen(fmt);
    bool setlong = false;

    while(i < len) {
        switch(fmt[i]){
            case '%': {
                switch(fmt[i + 1]) {
                    case 'c': {
                        char arg = va_arg(ap, int);
                        putc(arg, color, x, y);
                        i += 2;
                        x++;
                        break;
                    }
                    case 's': {
                        char *arg = va_arg(ap, char *);
                        puts(arg, color, x, y);
                        x += strlen(arg);
                        i += 2;
                        break;
                    }
                    case 'l': {
                        //set long int flag and reset on 'd' or 'X' after execution
                        setlong = true;
                        i += 2;
                        break;
                    }
                    case 'i':
                    case 'd': {
                        if(setlong) {
                            setlong = false;
                            uint64_t arg = va_arg(ap, uint64_t);
                            if(arg == 0) puts("0", color, x, y);
                            else {
                                char buffer[20];
                                puts(itoalong(arg, buffer, 10, x, y, color, true), color, x, y);
                                x += strlen(itoalong(arg, buffer, 10, x, y, color, true));
                            }
                        } else {
                            int arg = va_arg(ap, int);
                            if(arg == 0) puts("0", color, x, y);
                            else {
                                char buffer[20];
                                puts(itoa(arg, buffer, 10, x, y, color, true), color, x, y);
                                x += strlen(itoa(arg, buffer, 10, x, y, color, true));
                            }
                            i += 2;
                        }
                        break;
                    }
                    case 'x':
                    case 'X': {
                        if(setlong) {
                            setlong = false;
                            uint64_t arg = va_arg(ap, uint64_t);
                            char buffer[20];
                            puts(itoalong(arg, buffer, 16, x, y, color, true), color, x, y);
                            i += 2;
                            x += strlen(itoalong(arg, buffer, 16, x, y, color, true));
                        } else {
                            int arg = va_arg(ap, int);
                            char buffer[20];
                            puts(itoa(arg, buffer, 16, x, y, color, true), color, x, y);
                            i += 2;
                            x += strlen(itoa(arg, buffer, 16, x, y, color, true));
                        }
                        break;
                    }
                    default: {
                        va_end(ap);
                        break;
                    }
                }
                va_end(ap);
                break;
            }
            case '\n': {
                y++;
                x = 0;
                va_end(ap);
                break;
            }
            default: {
                putc(fmt[i], color, x, y);
                x++;
                va_end(ap);
                break;
            }
        }
        i++;
    }
    return;
}

char itoh(int num, bool upper, int x, int y, int color, bool use_additional) {
    if(num < 16 && use_additional) putc('0', color, x, y);
    if(upper) return num - 10 + 'A';
    return num - 10 + 'a';
}

char itoc(int num) {
    return '0' + num;
}

int atoi(const char *str) {
    int res = 0;
    for (int i = 0; str[i] != '\0'; ++i) res = res * 10 + str[i] - '0';
    return res;
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

const char *__stdio_stub = "Warning, accessed stub";

FILE *fopen(const char *path, const char *mode) {
    __serial_write_fmt("CPU %d -> tos > %s fopen(const char *, const char *)\r\n", __stdio_stub, __tools_get_cpu() - 1);

    return NULL;
}
int fscanf(FILE *stream, const char *format, ...) {
    __serial_write_fmt("CPU %d -> tos > %s fscanf(FILE *, const char *, ...)\r\n", __stdio_stub, __tools_get_cpu() - 1);

    return 0;
}
int feof(FILE *stream) {
    __serial_write_fmt("CPU %d -> tos > %s feof(FILE *)\r\n", __stdio_stub, __tools_get_cpu() - 1);

    return 0;
}
int fgetc(FILE *stream) {
    __serial_write_fmt("CPU %d -> tos > %s fgetc(FILE *)\r\n", __stdio_stub, __tools_get_cpu() - 1);

    return 0;
}

void perror(const char *s) {
    crash(s, PANIC_CUSTOM_NUMBER, false);
}