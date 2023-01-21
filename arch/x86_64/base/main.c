#include "../include/early_system_init.h"
#include "../include/stdint.h"
#include "../include/tunnel.h"
#include "../include/serial.h"
#include "../include/bootboot.h"
#include "../include/desktop.h"
#include "../fat32/src/fat32.h"
#include "../include/tools.h"

/******************************************
 * Entry point, called by BOOTBOOT Loader *
******************************************/

int _remaps[2] = {0x20, 0x28};

void test_fat();

void _start(){
    tunnelos_sysinfo.cores++;

    if(__tools_get_cpu() == bootboot.bspid + 1) {
        __serial_write_fmt("CPU %d -> tos > Welcome to Tunnel OS\r\n", __tools_get_cpu() - 1);

        tunnelos_sysinfo.bootboot = bootboot;

        __esi_setup_basic_hardware();
        __esi_set_cores();;

        // __coreshell_init();
        // wait(1000);

        test_fat();
        //__desktop_init();
        __pic_remap(_remaps);
        __pit_init();
        while(1);
    }

    while(1);
}

void test_fat() {
    int a = disk_mount(DISK_IDE_DRV0);

    struct file_s f;
    // fat_w
    int i = fat_file_open(&f, "C:/WOW.txt", 64);
    __serial_write_fmt("CPU %d -> tos > returned %d %d\r\n", __tools_get_cpu() - 1, a, i);
}
