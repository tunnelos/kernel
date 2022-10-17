#include "../include/coreshell.h"
#include "../include/pit.h"
#include "../include/stdio.h"
#include "../include/screen.h"
#include "../include/tools.h"
#include "../include/tools.h"
#include "../include/gui.h"
#include "../include/sounds.h"
#include "../include/pic.h"
#include "../include/serial.h"
#include "../include/ide.h"
#include "../include/stdlib.h"
#include "../include/cstring.h"

uint32_t pit0_col;

extern unsigned char _binary_hello_snd_start;
extern unsigned char _binary_error_snd_start;

int __coreshell_remaps[2] = {0x20, 0x28};

bool __coreshell_ideError = false;
int __coreshell_installStage = 0;

char *testvalues0[2] = {};
char *testvalues1[2] = {};

void __coreshell_install_base(int drive_number) {
    __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, COLOR_WHITE);
    vector2d_t a = alignText("Coreshell Configuration");
    puts("Coreshell Configuration", 0, a.x, 1);
    puts("You need to create an ", 0, 1, 3);
    puts("account ", COLOR_YELLOW, 23, 3);
    puts("for logging in.", COLOR_BLACK, 31, 3);
    return;
}
char *__coreshell_getDrive(uint8_t id) {
    char *drv0 = (char *)calloc(100);
    char *buffer = (char *)calloc(128);
    strcat(drv0, "Drive ");
    strcat(drv0, stditoa(id, buffer, 10));
    strcat(drv0, " (");
    ide_dev_t ideDevice = __ide_devices[id];
    if(ideDevice.connected) {
        free(buffer);
        char *buffer = (char *)calloc(128);
        strcat(drv0, stditoa(ideDevice.size / 1024 / 2, buffer, 10));
        strcat(drv0, " MB)");
    } else {
        strcat(drv0, "not connected)");
    }
    free(buffer);
    return drv0;
}
int __coreshell_install_selectDrive() {
    __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, COLOR_WHITE);
    vector2d_t a = alignText("Coreshell Configuration");
    puts("Coreshell Configuration", 0, a.x, 1);
    puts("Lets select drive where configuration will be stored on.", 0, 1, 3);
    gui_table_t tabletest;
    tabletest.columnCount = 2;
    testvalues0[0] = __coreshell_getDrive(0);
    testvalues0[1] = __coreshell_getDrive(1);
    testvalues1[0] = __coreshell_getDrive(2);
    testvalues1[1] = __coreshell_getDrive(3);
    tabletest.row0Values = testvalues0;
    tabletest.row1Values = testvalues1;
    __gui_drawTable((vector2d_t){3, 5}, 35, 35, tabletest);
    return 0;
}
bool __coreshell_onPIT2(uint128_t tick) {
    if(inb(0x64) & 1) {
        uint8_t scancode = inb(0x60);
        if(!(scancode & 0x80)) {
            if(scancode == 28) {
                if(__coreshell_ideError) {
            	    //__coreshell_install_base(-1);
                    __coreshell_install_base(-1);
                    return false;
                } else {
                    __coreshell_install_selectDrive();
                    return false;
                }
            }
        }
    }
    return true;
}

bool __coreshell_onPIT(uint128_t tick) {
    if(inb(0x64) & 1) {
        uint8_t scancode = inb(0x60);
        if(!(scancode & 0x80)) {
            if(scancode == 28) {
                uint8_t i = 0;
                uint8_t devices;
                while(i < 4) {
                    devices += __ide_devices[i].connected;
                    i++;
                }
                if(!devices) {
                    __coreshell_installStage = 1;
                    __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, 0x00FFFFFF);
                    vector2d_t a = alignText("Coreshell Configuration");
                    puts("Coreshell Configuration", 0, a.x, 1);
                    __sounds_queueSoundData((pcspeaker_sound_t *)&_binary_error_snd_start);
                    __sounds_resumeSound();
                    puts("ERROR", COLOR_RED, 1, 3);
                    puts("There is no IDE devices connected to this computer.", COLOR_BLACK, 1, 4);
                    puts("Settings will not be saved.", COLOR_BLACK, 1, 5);
                    puts("Press Enter to continue", COLOR_DARK_GREEN, 1, 7);
                    __coreshell_ideError = true;
                    __pit_setOnIntCallback(__coreshell_onPIT2);
                } else {
                    __coreshell_install_selectDrive();
                }
                return false;
            }
        }
    }
    return true;
}
void __coreshell_init() {
    __pit_setOnIntCallback(__coreshell_onPIT);
    __sounds_initThread();
    //__sounds_queueSoundData((pcspeaker_sound_t *)&_binary_hello_snd_start);
    __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, 0x00FFFFFF);
    vector2d_t a = alignText("Coreshell Configuration");
    puts("Coreshell Configuration", 0, a.x, 1);
    puts("Welcome to the Tunnel OS. This is a singlecore", 0, 1, 4);
    puts("text-based 64-bit operating system made by", 0, 1, 5);
    puts("@SergeyMC9730 (GitHub)", 0, 1, 6);
    puts("Press Enter to continue", COLOR_DARK_GREEN, 1, 8);
    __pic_remap(__coreshell_remaps);
}
