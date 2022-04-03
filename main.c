
#include "./include/main.h"
#include "./include/screen.h"
#include "./include/stdio.h"
#include "./include/tunnel.h"

/* imported virtual addresses, see linker script */
extern BOOTBOOT bootboot;
extern unsigned char environment[4096];

/******************************************
 * Entry point, called by BOOTBOOT Loader *
 ******************************************/
void _start()
{
    /*** NOTE: this code runs on all cores in parallel ***/
    int x, y, s = bootboot.fb_scanline, w = bootboot.fb_width, h = bootboot.fb_height;
    tunnelos_sysinfo.bootboot = bootboot;

    if(s) {
        // // red, green, blue boxes in order
        // for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+20)*4))=0x00FF0000; } }
        // for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+50)*4))=0x0000FF00; } }
        // for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+80)*4))=0x000000FF; } }

        puts("TunnelOS");
    }
    // hang for now
    while(1);
}