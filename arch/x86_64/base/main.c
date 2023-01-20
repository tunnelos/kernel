#include "../include/early_system_init.h"
#include "../include/stdint.h"
#include "../include/tunnel.h"
#include "../include/serial.h"
#include "../include/bootboot.h"
#include "../include/desktop.h"

/******************************************
 * Entry point, called by BOOTBOOT Loader *
******************************************/

int _remaps[2] = {0x20, 0x28};

void _start(){
    tunnelos_sysinfo.cores++;

    if(__tools_get_cpu() == bootboot.bspid + 1) {
        __serial_write_fmt("CPU %d -> tos > Welcome to Tunnel OS\r\n", __tools_get_cpu() - 1);

        tunnelos_sysinfo.bootboot = bootboot;

        __esi_setup_basic_hardware();
        __esi_set_cores();

        // __coreshell_init();
        __desktop_init();
        __pic_remap(_remaps);
        __pit_init();
        while(1);
    }
}
