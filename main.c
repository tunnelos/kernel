
#include "./include/main.h"
#include "./include/screen.h"
#include "./include/stdio.h"
#include "./include/tunnel.h"
#include "./include/panic.h"
#include "./include/smt.h"
#include "./include/shell.h"
#include "./include/tools.h"
#include "./include/serial.h"
#include "./include/idt.h"
#include "./include/ide.h"
#include "./include/mm.h"
#include "./include/cpuid_tools.h"
#include "./include/sse.h"
#include "./include/avx.h"
#include "./include/desktop.h"

extern BOOTBOOT bootboot;
extern unsigned char environment[4096];

/******************************************
 * Entry point, called by BOOTBOOT Loader *
 ******************************************/

uint32_t scanlines;

uint32_t bars[5] = {
    0x1F0, 0x3F6, 0x170, 0x376, 0x000
};

void _start(){
    if(__cpuid_check_sse() == 0) {
        __serial_write_fmt("CPU %d -> tos > SSE is unavaliable!", __tools_get_cpu());
        while(1);
    } else {
        __sse_init();
    }

    tunnelos_sysinfo.bootboot = bootboot;

    if(__tools_get_cpu() == bootboot.bspid + 1) {
        __serial_write_fmt("CPU %d -> tos > Welcome to Tunnel OS\r\n", __tools_get_cpu() - 1);

        __serial_write_fmt("CPU %d -> tos > SSE is supported.\r\n", __tools_get_cpu() - 1);

        scanlines = bootboot.fb_scanline;
        __idt_init();
        __ide_init(bars);
        __main_core0init();
    } else {
        __idt_init();
        __serial_write_fmt("CPU %d -> tos > CPU Check...\r\n", __tools_get_cpu() - 1);
        int mycpu = __tools_get_cpu() - 1;
        if(mycpu < 0 && (mycpu + 1) > MAX_CORES) {
            //do not use them
            while(1);
        }
        __smt_avaliable_cores++;
        __serial_write_fmt("CPU %d -> tos > Waiting for SMT\r\n", __tools_get_cpu() - 1);
        while(!__smt_inSMTmode);
        __serial_write_fmt("CPU %d -> tos > Core is ready to execute tasks\r\n", __tools_get_cpu() - 1);
        //waiting for system to be in SMT mode
        if(bootboot.numcores < MAX_CORES) {
            while(1) {
                if(cores[mycpu].haveTask) {
                    cores[mycpu].busy = true;
                    __smt_coreList[mycpu + 1] = cores[mycpu].task_to_execute->id;
                    cores[mycpu].task_to_execute->already_executing = true;
                    cores[mycpu].task_to_execute->runner(cores[mycpu].task_to_execute->id);
                    if (cores[mycpu].task_to_execute->one_time) {
                        cores[mycpu].task_to_execute->already_executing = false;
                        cores[mycpu].task_to_execute->is_used = false;
                        cores[mycpu].haveTask = false;
                        cores[mycpu].busy = false;
                        __smt_tasks--;
                        __smt_coreList[mycpu + 1] = 1024;
                    } else {
                        cores[mycpu].task_to_execute->runned_times++;
                        cores[mycpu].haveTask = false;
                        cores[mycpu].busy = false;
                        cores[mycpu].task_to_execute->already_executing = false;
                        __smt_coreList[mycpu + 1] = 1024;
                    }
                }
            }
        }
    }
}

void __main_core0init() {
    int s = bootboot.fb_scanline;

    if(s) {
        tunnelos_sysinfo.free_memory_location = (uint8_t *)((MMapEnt *)(&bootboot.mmap + 4)->ptr);
        tunnelos_sysinfo.free_memory_location_size = ((MMapEnt *)(&bootboot.mmap + 4))->size;

        if(tunnelos_sysinfo.free_memory_location_size < 8*1024*1024 + 8){
            __stdio_margin = 1;
            crash((const char *)PANIC_NOT_ENOUGH_MEMORY);
            while(1);
        }

        tunnelos_sysinfo.mm = (tunnel_memory_map_t *)((MMapEnt *)(&bootboot.mmap + 4)->ptr);
        tunnelos_sysinfo.mm->start_point = ((MMapEnt *)(&bootboot.mmap + 4))->size;

        if(__cpuid_check_avx() || __cpuid_check_avx2()) {
            __avx_init();
        } else {
            __serial_write_fmt("CPU %d -> tos > AVX is unavaliable! (%d %d)\r\n", __tools_get_cpu(), __cpuid_check_avx(), __cpuid_check_avx2());
            //while(1);
        }

        // ide_rw_t irt;

        // int jjs = 0;
        // irt.lba = 0x10000000;
        // irt.rw = ATA_WRITE;
        // irt.sectors = 255;
        // irt.selector = 0;
        // tunnel_memory_block_t bl0 = malloc(256);
        // tunnel_memory_block_t bl1 = malloc(256);
        // char *data_input = bl0.address;
        // char *data_output = bl1.address;
        // data_input[0] = 0xFF;
        // data_output[0] = 0;
        // while(jjs < 4) {
        //     if(__ide_devices[jjs].connected) {
        //         data_output[0] = 0;
        //         irt.drive = __ide_devices[jjs].drive;
        //         irt.buffer = (uint32_t)data_input;
        //         __serial_write_fmt("CPU %d -> tos > buffer address %d\r\n", __tools_get_cpu() - 1, irt.buffer);
        //         irt.rw = ATA_WRITE;
        //         __serial_write_fmt("CPU %d -> tos > trying to write to %d\r\n", __tools_get_cpu() - 1, jjs);
        //         __ide_get_access(irt);
        //         irt.rw = ATA_READ;
        //         irt.buffer = (uint32_t)data_output;
        //         __serial_write_fmt("CPU %d -> tos > buffer address %d\r\n", __tools_get_cpu() - 1, irt.buffer);
        //         __ide_get_access(irt);
        //         __serial_write_fmt("CPU %d -> tos > Result of %d: %d\r\n", __tools_get_cpu() - 1, jjs, data_output[0]);
        //     }
        //     jjs++;
        // }

        __serial_write_fmt("CPU %d -> tos > Memory Check complete\r\n", __tools_get_cpu() - 1);
        __stdio_margin = 1;

        // __desktop_init();

        _shell__create_shell(0);
        __serial_write_fmt("CPU %d -> tos > Created shell\r\n", __tools_get_cpu() - 1);
        __stdio_margin = 0;
        __serial_write_fmt("CPU %d -> tos > Swithing to SMT mode\n", __tools_get_cpu() - 1);
        if(bootboot.numcores > 1) {
            __serial_write_fmt("CPU %d -> tos > Tasks will be runned in parallel\r\n", __tools_get_cpu() - 1);
        }
        __smt_run();
        // // red, green, blue boxes in order
        //for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+20)*4))=0x00FF0000; } }
        // for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+50)*4))=0x0000FF00; } }
        // for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+80)*4))=0x000000FF; } }
        
    } else {
        __serial_write_fmt("CPU %d -> tos > Framebuffer error! Please, restart your PC\r\n", __tools_get_cpu() - 1);
        while(1);
    }
}