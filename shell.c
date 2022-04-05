#include "./include/shell.h"
#include "./include/screen.h"
#include "./include/smt.h"
#include "./include/keyboard_ps2.h"
#include "./include/tools.h"
#include "./include/cint.h"
#include "./include/serial.h"

char current_key = '?';
char scancodePub = 0;
bool initalInit = false;

int counter = 0;

void __shell_draw_statusbar(int id){
    int i = 0;
    puts("Tunnel OS v0.1", 0x00FFFFFF, 2, 0);
    printf(COLOR_RED, 20, 0, "CuKey: %c", current_key);
    printf(COLOR_RED, 31, 0, "Scancode: %d", scancodePub);
    if(counter % -(__INT32_MAX__ - 8192)) {
        __serial_write_fmt(" - Action! %d\r\n", counter);
        counter = 0;
        i = 45;
        //clean screen
        while(i < 77) {
            putc('\b', COLOR_GREEN + COLOR_RED, i, 0);
            i++;
        }
        puts("TID per CPU: ", 0x00FFFFFF, 45, 0);
        i = 0;
        int ax = 45 + 14;
        while(i < tunnelos_sysinfo.bootboot.numcores) {
            if(i == (__tools_get_cpu() - 1)) {
                if(maincpu_tid == 1024) {
                    puts("-", 0x00FFFFFF, ax, 0);
                    ax += 2;
                } else {
                    printf(0x00FFFFFF, ax, 0, "%d", maincpu_tid);
                    ax += digitcount(maincpu_tid);
                }
            } else {
                if(__smt_coreList[i] == 1024) {
                    puts("-", 0x00FFFFFF, ax, 0);
                    ax += 2;
                } else {
                    printf(0x00FFFFFF, ax, 0, "%d", __smt_coreList[i]);
                    ax += digitcount(__smt_coreList[i]);
                }
            }
            i++;
        }
    }
    if(__tools_get_cpu() == (tunnelos_sysinfo.bootboot.bspid + 1)) {
        putc('M', COLOR_GREEN, 78, 0);
    } else {
        putc('A', COLOR_GREEN, 78, 0);
    }
    counter++;
    return;
}
void __shell_draw_taskbar(int id){
    puts("Start", COLOR_GREEN + COLOR_RED, 1, 29);
}
void __shell_keyboard_input(int id){
    if(!(inb(KPS2_SR) & KPS2_OB)) {
        scancodePub = 0;
        current_key = '?';
    }

    uint8_t scancode = inb(KPS2_DP);
    scancodePub = scancode;
    current_key = __keyboard_ps2_lookuptable[scancode];
}

void _shell__create_shell(int id){
    __stdio_margin = 0;
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
    while(i22 < 255){
        __keyboard_ps2_lookuptable[i22] = '?';
        i22++;
    }

    __keyboard_ps2_init();
    __smt_create_task(__shell_draw_statusbar);
    __smt_create_task(__shell_draw_taskbar);
    __smt_create_task(__shell_keyboard_input);
}