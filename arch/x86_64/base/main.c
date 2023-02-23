#include "../include/early_system_init.h"
#include "../include/stdint.h"
#include "../include/tunnel.h"
#include "../include/serial.h"
#include "../include/bootboot.h"
#include "../include/desktop.h"
#include "../fat32/src/fat32.h"
#include "../include/tools.h"
#include "../include/pit.h"
#include "../include/idt.h"
#include "../include/video.h"
#include "../include/gui.h"

/******************************************
 * Entry point, called by BOOTBOOT Loader *
******************************************/

int _remaps[2] = {0x20, 0x28};

void test_fat();
void __main_smallfbtest();

void _start(){  
    tunnelos_sysinfo.cores++;

    if(__tools_get_cpu() == bootboot.bspid + 1) {
        __serial_write_fmt("CPU %d -> tos > Welcome to Tunnel OS\r\n", __tools_get_cpu() - 1);

        tunnelos_sysinfo.bootboot = bootboot;
        
        __esi_set_cores();

        __esi_setup_basic_hardware();

        // __coreshell_init();
        // wait(1000);
        __sti();
        //__desktop_init();
        __main_smallfbtest();

        //__pit_default_init();

        //test_fat();

        //__pic_remap(_remaps);
        //__pit_default_init();
        // __serial_write_fmt("- CPU %d -> tos > PIT fractions: %d\r\n", __tools_get_cpu() - 1, __pit_IRQ0_fractions);
        // __serial_write_fmt("- CPU %d -> tos > PIT ms: %d\r\n", __tools_get_cpu() - 1, __pit_IRQ0_ms);
        // __serial_write_fmt("- CPU %d -> tos > PIT frequency: %d\r\n", __tools_get_cpu() - 1, __pit_IRQ0_frequency);
        // __serial_write_fmt("- CPU %d -> tos > PIT rvalue: %d\r\n", __tools_get_cpu() - 1, __pit_reload_value);
        // __cli();
        // while(1) {
        //     __serial_write_fmt("CPU %d -> tos > Welcome to Tunnel OS\r\n", __tools_get_cpu() - 1);
        // }
        //__main_smallfbtest();
        while(1);
    } else if (__tools_get_cpu() == bootboot.bspid + 1 + 1){
        wait(500);
        __video_refresh();
    }

    __serial_write_fmt("CPU %d -> tos > Shutdown\r\n", __tools_get_cpu() - 1);

    while(1);
}

vector2d_t *positions;

bool __main_smallfbtest_isPlaced(vector2d_t vec) {
    int i = 0;
    while(i < 5000) {
        if(positions[i].x == vec.x && positions[i].y == vec.y) return true;
        i++;
    }
    return false;
}

void __main_smallfbtest() {
    vector2d_t s = __gui_getTextResolution();
    s.x /= 2;
    s.y /= 2;
    int i = 0;

    const char *data = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";

    //positions = (vector2d_t *)malloc(sizeof(vector2d_t) * 5000);

    while(1) {
        wait(250);
        
        int x = rand() % (s.x * 2);
        int y = rand() % (s.x * 2);
        printf(rand(), x, y, "%c", data[rand() % 52]);
        __gui_drawRectangle((vector2d_t){1, 1}, (vector2d_t){12, 1}, COLOR_BLACK);
        printf(0xFFFFFFFF, 1, 1, "Slide %d", i);
        i++;
    }
    // crash("123", 11, false);
}

void test_fat() {
    int a = disk_mount(1);

    struct file_s f;
    // fat_w
    int i = fat_file_open(&f, "C:/WOW.txt", 64);
    __serial_write_fmt("CPU %d -> tos > returned %d %d\r\n", __tools_get_cpu() - 1, a, i);
}
