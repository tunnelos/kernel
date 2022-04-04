
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
    int s = bootboot.fb_scanline;
    tunnelos_sysinfo.bootboot = bootboot;

    if(s) {
        printf(COLOR_GREEN, "%s %d\n", "test int 1:", 1024);
        printf(COLOR_GREEN, "%s %d\n", "test int 2:", 513);

        // // red, green, blue boxes in order
        //for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+20)*4))=0x00FF0000; } }
        // for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+50)*4))=0x0000FF00; } }
        // for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+80)*4))=0x000000FF; } }
    }
    // hang for now
    while(1);
}