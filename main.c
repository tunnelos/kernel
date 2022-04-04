
#include "./include/main.h"
#include "./include/screen.h"
#include "./include/stdio.h"
#include "./include/tunnel.h"
#include "./include/panic.h"
#include "./include/smt.h"
#include "./include/shell.h"

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
        __stdio_margin = 0;
        puts("\nTunnelOS is loading.\n\b", COLOR_GREEN + COLOR_RED);
        __stdio_margin = 8;
        tunnelos_sysinfo.free_memory_location = (uint8_t *)((MMapEnt *)(&bootboot.mmap + 4)->ptr);
        tunnelos_sysinfo.free_memory_location_size = ((MMapEnt *)(&bootboot.mmap + 4))->size;

        if(tunnelos_sysinfo.free_memory_location_size < 8*1024*1024){
            __stdio_margin = 1;
            crash((const char *)PANIC_NOT_ENOUGH_MEMORY);
            while(1);
        } else {
            putc('\b', COLOR_GREEN + COLOR_RED);
        }
        int tempdata[3] = {tx, ty, kx};
        __stdio_setTerminalXY(0, 3 * 16);
        __stdio_margin = 1;
        printf(COLOR_GREEN, "Cores: %d", bootboot.numcores);
        __smt_create_task(_shell__create_shell)->one_time = true;
        __smt_run();
        // // red, green, blue boxes in order
        //for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+20)*4))=0x00FF0000; } }
        // for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+50)*4))=0x0000FF00; } }
        // for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+80)*4))=0x000000FF; } }
    } else {
        //restart pc by crashing it
        //will replace it with the serial port in the future
        *(uint8_t *)0xfffffffff7000000 = 254;
    }
    while(1);
}