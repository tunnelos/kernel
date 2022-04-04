#include "./include/shell.h"
#include "./include/screen.h"
#include "./include/smt.h"
#include "./include/keyboard_ps2.h"
#include "./include/tools.h"

char current_key = '?';
char scancodePub = 0;

void __shell_draw_statusbar(int id){
    __stdio_setTerminalXY(2 * 8, 0);
    puts("Tunnel OS v0.1", 0x00FFFFFF);
    __stdio_setTerminalXY((24 + 4) * 8, 0);
    printf(COLOR_RED, "CuKey: %c", current_key);
    __stdio_setTerminalXY((35 + 4) * 8, 0);
    //if(current_key == '?') scancodePub = 0;
    printf(COLOR_RED, "Scancode: %d", scancodePub);
    __stdio_setTerminalXY(70 * 8, 0);
    printf(0x00FFFFFF, "Tasks: %d", __smt_tasks);
    return;
}
void __shell_draw_taskbar(int id){
    __stdio_setTerminalXY(1 * 8, 29 * 16);
    puts("Start", COLOR_GREEN + COLOR_RED);
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
    printf(0x0000FF00, "Shell init");
    __stdio_margin = 0;
    //clear screen
    int mx = 80, my = 30;
    int i1 = 1,  i2 = 0;
    while(i1 < my){
        while(i2 < mx){
            __stdio_setTerminalXY(i2 * 8, i1 * 16);
            putc('\x08', 0x00FFFFFF);
            i2++;
        }
        i1++;
        i2 = 0;
    }
    __stdio_setTerminalXY(0, 0);
    int i = 0;
    while(i < 80){
        __stdio_setTerminalXY(i * 8, 0);
        putc('\x08', COLOR_GREEN + COLOR_RED);
        i++;
    }
    i = 0;
    while(i < 80){
        __stdio_setTerminalXY(i * 8, 29 * 16);
        putc('\x08', COLOR_GREEN + COLOR_RED);
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