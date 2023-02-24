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

bool _memory_complete = false;

void _start(){  
    tunnelos_sysinfo.cores++;

    if(__tools_get_cpu() == bootboot.bspid + 1) {
        __serial_write_fmt("CPU %d -> tos > Welcome to Tunnel OS\r\n", __tools_get_cpu() - 1);

        tunnelos_sysinfo.bootboot = bootboot;
        
        __esi_set_cores();

        __esi_setup_basic_hardware();
        //_memory_complete = true;

        // __coreshell_init();
        // wait(1000);
        malloc(10000);
        __cli();
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
        //while(!_memory_complete);
        //wait(500);
        //__video_refresh();
    }

    __serial_write_fmt("CPU %d -> tos > Shutdown\r\n", __tools_get_cpu() - 1);

    while(1);
}

vector2d_t *positions;

void __main_smallfbtest() {
    int i = 0;

    const char *data = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";

    //positions = (vector2d_t *)malloc(sizeof(vector2d_t) * 5000);


    vector2d_t res = __gui_getTextResolution();
    vector2d_t res2 = __gui_getTextResolution();
    res.y -= 5;

    text_framebuffer_t myfb = __textfb_create(res);

    myfb.fb = 0;
    myfb.color_table = 0;

    bool pause = false;

    while(1) {
        wait(100);

        uint8_t val = 1;

        if(inb(0x64) & 1) {
            uint8_t scancode = inb(0x60);
            if(!(scancode & 0x80)) {
                //__serial_write_fmt("CPU %d -> tos > pressed %d\r\n", __tools_get_cpu() - 1, scancode);
                if(scancode == 28) {
                    myfb.fb += 0x100;
                    myfb.color_table += 0x100;
                } else if (scancode == 57) {
                    pause = !pause;
                }
            }
        }

        if(!pause) {
            myfb.fb += val;
            myfb.color_table += val;
        }
        //__gui_drawText((vector2d_t){x, y}, (vector2d_t){1, 1}, __gui_generateRandomColor(), "\x13", &myfb);
        //__gui_drawRectangle((vector2d_t){0, 0}, myfb.size, COLOR_WHITE, &myfb);
        //__gui_drawRectangle((vector2d_t){1, 1}, (vector2d_t){12, 2}, COLOR_BLACK, &myfb2);
        //__gui_drawText((vector2d_t){1, 1}, (vector2d_t){9, 1}, COLOR_BLUE, "Glory to", &myfb2);
        //__gui_drawText((vector2d_t){1, 2}, (vector2d_t){9, 1}, COLOR_YELLOW, "Ukraine", &myfb2);
        __gui_drawRectangle((vector2d_t){0, 0}, res2, COLOR_BLACK, NULL);
        //__textfb_merge(myfb, myfb2, (vector2d_t){0, 0});
        __textfb_render(myfb, __video_get_fb(false));
        printf(0x00FFFFFF, 3, 27, "Previewing address %X", myfb.fb);
        printf(__color_to_int(COLOR_YELLOW), 40, 26, "Press Enter to move by 0x100");
        printf(__color_to_int(COLOR_YELLOW), 40, 28, "Press Space to pause/unpause");
        //__textfb_render(myfb2, __video_get_fb(true));
        memcpy(__video_get_fb(true), (const void *)__video_get_fb(false), 640 * 480* 4);

        //__serial_write_fmt("CPU %d -> tos > rendered %d\r\n", __tools_get_cpu() - 1, i);
        
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
