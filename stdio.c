#include "./include/stdio.h"
#include "./include/screen.h"
#include "./include/tunnel.h"

void puts(char *s, uint32_t color){
    psf2_t *font = (psf2_t*)&_binary_font_psf_start;
    int x, y, kx = 0, line, mask, offs;
    int bpl = (font->width + 7) / 8;
    while(*s) {
        unsigned char *glyph = (unsigned char*)&_binary_font_psf_start + font->headersize + (*s > 0 && *s < font->numglyph ? *s : 0)*font->bytesperglyph;
        offs = (kx * (font->width + 1) * 4);
        for(y = 0; y < font->height; y++) {
            line = offs; 
            mask = 1 << (font -> width - 1);
            for(x = 0; x < font->width; x++) {
                *((uint32_t*)((uint64_t)&fb + line))=((int)*glyph) & (mask) ? color : 0;
                mask >>= 1; 
                line += 4;
            }
            *((uint32_t*)((uint64_t)&fb + line)) = 0; glyph += bpl; offs += tunnelos_sysinfo.bootboot.fb_scanline;
        }
        s++; 
        kx++;
    }
}
