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

uint32_t pit0_col;

unsigned char _binary_test_snd_start;
unsigned char _binary_coreshell_installationstage0_snd_start;

int __coreshell_remaps[2] = {0x20, 0x28};

bool __coreshell_ideError = false;

void __coreshell_install_selectDrive() {}
void __coreshell_install_base() {}

bool __coreshell_onPIT2(uint128_t tick) {
    if(inb(0x64) & 1) {
        uint8_t scancode = inb(0x60);
        if(!(scancode & 0x80)) {
            if(scancode == 28) {
                if(__coreshell_ideError) {
                    __coreshell_install_selectDrive();
                    return false;
                } else {
                    __coreshell_install_base();
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
                __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, 0x00FFFFFF);
                vector2d_t a = alignText("Coreshell Configuration");
                puts("Coreshell Configuration", 0, a.x, 1);
                uint8_t i = 0;
                uint8_t devices;
                while(i < 4) {
                    devices += __ide_devices[i].connected;
                    i++;
                }
                if(!devices) {
                    __sounds_queueSoundData((pcspeaker_sound_t *)&_binary_coreshell_installationstage0_snd_start);
                    __sounds_resumeSound();
                    puts("ERROR", COLOR_RED, 1, 3);
                    puts("There is no IDE devices connected to this computer.", COLOR_BLACK, 1, 4);
                    puts("Settings will not be saved.", COLOR_BLACK, 1, 5);
                    puts("Press Enter to continue", COLOR_DARK_GREEN, 1, 7);
                    __coreshell_ideError = true;
                }
                __pit_setOnIntCallback(__coreshell_onPIT2);
                return false;
            }
        }
    }
    return true;
}
void __coreshell_init() {
    __pit_setOnIntCallback(__coreshell_onPIT);
    __sounds_initThread();
    __sounds_queueSoundData((pcspeaker_sound_t *)&_binary_test_snd_start);
    __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, 0x00FFFFFF);
    vector2d_t a = alignText("Coreshell Configuration");
    puts("Coreshell Configuration", 0, a.x, 1);
    puts("Welcome to the Tunnel OS. This is a singlecore", 0, 1, 4);
    puts("text-based 64-bit operating system made by", 0, 1, 5);
    puts("@SergeyMC9730 (GitHub)", 0, 1, 6);
    puts("Press Enter to continue", COLOR_DARK_GREEN, 1, 8);
    __pic_remap(__coreshell_remaps);
}