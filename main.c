
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
/* imported virtual addresses, see linker script */
extern BOOTBOOT bootboot;
extern unsigned char environment[4096];

/******************************************
 * Entry point, called by BOOTBOOT Loader *
 ******************************************/

void _start()
{
    /*** NOTE: this code runs on all cores in parallel ***/
    tunnelos_sysinfo.bootboot = bootboot;

    if(__tools_get_cpu() == bootboot.bspid + 1) {
        __serial_write_fmt("CPU %d -> tos > Welcome to Tunnel OS\r\n", __tools_get_cpu() - 1);
        __idt_init();
        __main_core0init();
    } else {
        __serial_write_fmt("CPU %d -> tos > CPU Check...\r\n", __tools_get_cpu() - 1);
        int mycpu = __tools_get_cpu() - 1;
        if(mycpu < 0 && mycpu > MAX_CORES - 2) {
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

        if(tunnelos_sysinfo.free_memory_location_size < 8*1024*1024){
            __stdio_margin = 1;
            crash((const char *)PANIC_NOT_ENOUGH_MEMORY);
            while(1);
        }
        __serial_write_fmt("CPU %d -> tos > Memory Check complete\r\n", __tools_get_cpu() - 1);
        __stdio_margin = 1;
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
        //restart pc by crashing it
        //will replace it with the serial port in the future
        __serial_write_fmt("CPU %d -> tos > Framebuffer error! Please, restart your PC\r\n", __tools_get_cpu() - 1);
        while(1);
    }
}