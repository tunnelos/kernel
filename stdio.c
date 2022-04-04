#include "./include/stdio.h"
#include "./include/screen.h"
#include "./include/tunnel.h"

int ty = 0, tx = 0;

void puts(char *s, uint32_t color){
    int x, y, kx = 0, line, mask, offs;
    psf2_t *font = (psf2_t*)&_binary_fonts_text_psf_start;
    int bpl = (font->width + 7) / 8;
    while(*s) {
        //72
        unsigned char *glyph = (unsigned char*)&_binary_fonts_text_psf_start + font->headersize + (*s > 0 && *s < font->numglyph ? *s : 0)*font->bytesperglyph;
        offs = (kx * (font->width + 1) * 4);
        if(*s == '\n' || kx >= 71) {
            ty += font->height;
            kx = 0;
            tx = 0 - font->width;
        } else {
            for(y = 0; y < font->height; y++) {
                line = offs; 
                mask = 1 << (font -> width - 1);
                for(x = 0; x < font->width; x++) {
                    // if(x == SCREEN_WIDTH - font->width - 1 || *s == '\n') {
                    //     x = 0;
                    //     y += font->height;
                    // }
                    *((uint32_t*)((uint64_t)&fb + line + (ty * tunnelos_sysinfo.bootboot.fb_scanline) + (tx * 4)))=((int)*glyph) & (mask) ? color : 0;
                    mask >>= 1; 
                    line += 4;
                }
                *((uint32_t*)((uint64_t)&fb + line)) = 0; glyph += bpl; offs += tunnelos_sysinfo.bootboot.fb_scanline;
            }
        }
        s++; 
        kx++;
    }
}
