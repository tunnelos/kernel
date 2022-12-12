#include "../include/flags.h"
#include "../include/main.h"
#include "../include/screen.h"
#include "../include/stdio.h"
#include "../include/tunnel.h"
#include "../include/panic.h"
#include "../include/tools.h"
#include "../include/serial.h"
#include "../include/idt.h"
#include "../include/ide.h"
#include "../include/mm.h"
#include "../include/cpuid_tools.h"
#include "../include/sse.h"
#include "../include/avx.h"
#include "../include/desktop.h"
#include "../include/pit.h"
#include "../include/pic.h"
#include "../include/rtc.h"
#include "../include/nmi.h"
#include "../include/systemconf.h"
#if ENABLE_TEST == 1
#include "../include/test.h"
#include "../include/cpptest.h"
#endif
#include "../include/coreshell.h"
#include "../include/video.h"

extern BOOTBOOT bootboot;
extern unsigned char environment[4096];

/******************************************
 * Entry point, called by BOOTBOOT Loader *
******************************************/

uint32_t scanlines;

uint32_t bars[5] = {
    0x1F0, 0x3F6, 0x170, 0x376, 0x000
};

bool __main_core0complete = false;

void _start(){
    tunnelos_sysinfo.bootboot = bootboot;

    if(__tools_get_cpu() == bootboot.bspid + 1) {
        __serial_write_fmt("CPU %d -> tos > Welcome to Tunnel OS\r\n", __tools_get_cpu() - 1);

        if(!__cpuid_check_sse()) {
            __serial_write_fmt("CPU %d -> tos > SSE is unavaliable!\r\n", __tools_get_cpu() - 1);
            while(1);
        }

        scanlines = bootboot.fb_scanline;
        tunnelos_sysinfo.cores++;
        __cli();
        __idt_init();
        __sti();
        __sse_init();
        __pic_unmask(0);
        __ide_init(bars);
        if(__cpuid_check_avx() || __cpuid_check_avx2()) {
            tunnelos_sysinfo.avx = true;
            __avx_init();
        } else {
            tunnelos_sysinfo.avx = false;
            __serial_write_fmt("CPU %d -> tos > AVX is unavaliable! (%d %d)\r\n", __tools_get_cpu() - 1, __cpuid_check_avx(), __cpuid_check_avx2());
        }

        MMapEnt *mmap_ent = &bootboot.mmap;

        while(mmap_ent < (MMapEnt *)(&bootboot) + (&bootboot)->size) {
            __serial_write_fmt("CPU %d -> tos > Check memory type: %d\r\n", __tools_get_cpu() - 1, MMapEnt_Type(mmap_ent));
            if(MMapEnt_Type(mmap_ent) == MMAP_FREE) {
                tunnelos_sysinfo.free_memory_location_size = mmap_ent->size;
                __serial_write_fmt("CPU %d -> tos > Free memory size: %d KB\r\n", __tools_get_cpu() - 1, tunnelos_sysinfo.free_memory_location_size / 1024);
                if(tunnelos_sysinfo.free_memory_location_size / 1024 > 19000) {
                    __serial_write_fmt("CPU %d -> tos > Found one at address %l 0x%X\r\n", __tools_get_cpu() - 1, mmap_ent->ptr);
                    tunnelos_sysinfo.mm = (tunnel_memory_map_t *)((void *)(mmap_ent->ptr));
                    break;
                }
                //break;
            }

            mmap_ent++;
        }
        __mm_fillblocks();
        __video_setup_core();

        //tunnelos_sysinfo.mm = (tunnel_memory_map_t *)((void *)0x131000);

        // tunnelos_sysinfo.mm = (tunnel_memory_map_t *)((MMapEnt *)(&bootboot.mmap + 4)->ptr);
        // tunnelos_sysinfo.mm->start_point = ((MMapEnt *)(&bootboot.mmap + 4))->size;
        tunnelos_sysinfo.interrupts = true;
        tunnelos_sysinfo.pit = true;
        tunnelos_sysinfo.rtc = true;
        tunnelos_sysinfo.nmi = false;
        tunnelos_sysinfo.ide = true;
        __main_core0complete = true;
        __main_core0init();
    } else {
        tunnelos_sysinfo.cores++;
        if(__tools_get_cpu() == 2) {
            while(!__main_core0complete);
            __serial_write_fmt("CPU %d -> tos > Starting to rendering\r\n", __tools_get_cpu() - 1);
            __video_refresh();
        }
    }
    while(!__main_core0complete);
}

void __main_core0init() {
    int s = bootboot.fb_scanline;

    if(s) {
        __stdio_margin = 0;

        #if ENABLE_TEST == 1
        __test_unitest();
        cpptest_test00();
        #endif

        // wait(500);
        __coreshell_init();
        __pit_init();
        // __desktop_init();
        // __desktop_add_task("Discord");
        // __desktop_add_task("VSCode");
        // __desktop_add_task("Firefox");
        // __desktop_render_tasks();

        // // red, green, blue boxes in order
        //for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+20)*4))=0x00FF0000; } }
        // for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+50)*4))=0x0000FF00; } }
        // for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+80)*4))=0x000000FF; } }
        
    } else {
        __serial_write_fmt("CPU %d -> tos > Framebuffer error! Please, restart your PC.\r\n", __tools_get_cpu() - 1);
        while(1);
    }
}
