#include "./include/shell.h"
#include "./include/screen.h"
#include "./include/smt.h"
#include "./include/keyboard_ps2.h"
#include "./include/tools.h"
#include "./include/cint.h"
#include "./include/serial.h"
#include "./include/window.h"

char current_key = '?';
char scancodePub = 0;
bool initalInit = false;

uint64_t counter = 0;

void __shell_draw_statusbar(int id){
    int i = 0;
    puts("Tunnel OS v0.1", 0x00FFFFFF, 2, 0);
    printf(COLOR_RED, 17, 0, "%c", current_key);
    if(!__keyboard_ps2_ascii_only[scancodePub]) {
        printf(0, 19, 0, "\b\b");
    } else {
        printf(0, 19, 0, "\b\b");
        printf(COLOR_RED, 19, 0, "%X", scancodePub);
    }
    if(counter % 3900000000) {
        counter = 0;
        i = 45;
        //clean screen
        // while(i < 77) {
        //     putc('\b', COLOR_GREEN + COLOR_RED, i, 0);
        //     i++;
        // }
        puts("TST per CPU:", 0x00FFFFFF, 47, 0);
        uint8_t *time = &tunnelos_sysinfo.bootboot.datetime;
        uint8_t time_offset = 30;
        printf(COLOR_GREEN, time_offset, 0, "%d", time[4]);
        time_offset += 2;
        putc(':', COLOR_GREEN, time_offset, 0);
        time_offset++;
        printf(COLOR_GREEN, time_offset, 0, "%d", time[5]);
        time_offset += 2;
        putc(':', COLOR_GREEN, time_offset, 0);
        time_offset++;
        printf(COLOR_GREEN, time_offset, 0, "%d", time[6]);
        //printf("%s")
        i = 0;
        int ax = 59 + 1;
        while(i < __smt_avaliable_cores + 1) {
            if(i == (__tools_get_cpu() - 1)) {
                if(maincpu_tid == 1024) {
                    puts("I", 0x00FFFFFF, ax, 0);
                    ax += 2;
                } else {
                    puts("A", 0x00FFFFFF, ax, 0);
                    ax += 2;
                }
            } else {
                if(__smt_coreList[i] == 1024) {
                    puts("I", 0x00FFFFFF, ax, 0);
                    ax += 2;
                } else {
                    puts("A", 0x00FFFFFF, ax, 0);
                    ax += 2;
                }
            }
            i++;
        }
    }
    counter++;
    return;
}
void __shell_draw_taskbar(int id){
    puts("Start", COLOR_GREEN + COLOR_RED, 1, 29);
}
void __shell_keyboard_input(int id){
    if(!(inb(KPS2_SR) & KPS2_OB)) {
        // scancodePub = 0;
        // current_key = '?';
        return;
    }

    uint8_t scancode = inb(KPS2_DP);
    if(scancode == 0) return;
    scancodePub = scancode;
    current_key = __keyboard_ps2_lookuptable[scancode];
}

char qm = '?';

window_t windowtest;

void _shell__create_shell(int id){
    __stdio_margin = 0;
    windowtest.can_be_closed = true;
    windowtest.can_be_in_background = true;
    windowtest.draw_border = true;
    windowtest.name = "Title";
    windowtest.sx = 32;
    windowtest.sy = 8;
    windowtest.wx = 10;
    windowtest.wy = 10;
    __window_init();
    __window_create(&windowtest);
    //clear screen
    int mx = 80, my = 29;
    int i1 = 1,  i2 = 0;
    while(i1 < my){
        while(i2 < mx){
            putc('\x08', 0x00FFFFFF, i2, i1);
            i2++;
        }
        i1++;
        i2 = 0;
    }
    int i = 0;
    while(i < 80){
        putc('\x08', COLOR_GREEN + COLOR_RED, i, 0);
        i++;
    }
    i = 0;
    while(i < 80){
        putc('\x08', COLOR_GREEN + COLOR_RED, i, 29);
        i++;
    }

    int i22 = 59;
    while(i22 < 256){
        __keyboard_ps2_lookuptable[i22] = '?';
        sc_name[i22] = &qm;
        i22++;
    }
    i22 = 0;
    while(i22 < 256){
        if(sc_name[i22] == &qm) __keyboard_ps2_ascii_only[i22] = false;
        else {
            __keyboard_ps2_ascii_only[i22] = true;
        }
        i22++;
    }

    __keyboard_ps2_init();
    __smt_create_task(__shell_draw_statusbar);
    __smt_create_task(__shell_draw_taskbar);
    __smt_create_task(__shell_keyboard_input);
}