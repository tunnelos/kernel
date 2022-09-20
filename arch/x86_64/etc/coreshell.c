#include "../include/coreshell.h"
#include "../include/pit.h"
#include "../include/stdio.h"
#include "../include/screen.h"
#include "../include/tools.h"
#include "../include/tools.h"
#include "../include/gui.h"
#include "../include/sounds.h"

uint32_t pit0_col;

unsigned char _binary_test_snd_start;

bool __coreshell_onPIT(uint128_t tick) {
    __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){30, 4}, pit0_col);
    printf(COLOR_YELLOW, 32, 1, "%X", pit0_col);
    pit0_col++;
    return true;
}
void __coreshell_init() {
    __pit_setOnIntCallback(__coreshell_onPIT);
    __sounds_initThread();
    __sounds_queueSoundData((pcspeaker_sound_t *)&_binary_test_snd_start);
}