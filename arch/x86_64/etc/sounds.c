#include "../include/sounds.h"
#include "../include/stdint.h"
#include "../include/speaker.h"
#include "../include/pit.h"
#include "../include/serial.h"

pcspeaker_sound_t *__sounds_queue[64] = {};
uint8_t __sounds_queueP = 0;
int __sounds_dataP = 0;
int __sounds_dataTick = 0;

bool __sounds_thread(uint128_t tick) {
    if(__sounds_queueP == 64) __sounds_queueP = 0;
    if(__sounds_queue[__sounds_queueP] == NULL) return true;
    if(__sounds_queue[__sounds_queueP][__sounds_dataP].soundticks == 0) {
        __sounds_dataP = 0;
        __sounds_queue[__sounds_queueP] = NULL;
        __sounds_queueP++;
        __speaker_stopSound();
        __serial_write_fmt("[Sound System] Sound %d has been stopped\n", __sounds_queueP - 1);
        return true;
    } else {
        if(__sounds_queue[__sounds_queueP][__sounds_dataP].soundticks == __sounds_dataTick) {
            __sounds_dataTick = 0;
            __sounds_dataP++;

            return true;
        }
        if(__sounds_queue[__sounds_queueP][__sounds_dataP].frequency < 18) {
            __speaker_stopSound(); // Prevent crash
            __sounds_dataTick++;

            return true;
        } else {
            __speaker_playSound(__sounds_queue[__sounds_queueP][__sounds_dataP].frequency);
            __sounds_dataTick++;
        }
        //__serial_write_fmt("[Sound System] Playing sound (%d:%d:%d) %d times already with frequency of %d\n", __sounds_queueP, __sounds_dataP, __sounds_queue[__sounds_queueP][__sounds_dataP].soundticks, __sounds_dataTick, __sounds_queue[__sounds_queueP][__sounds_dataP].frequency);
    }
    return true;
}

void __sounds_queueSoundData(pcspeaker_sound_t *soundArray) {
    uint8_t i = 0;
    while(i < 64) {
        if(__sounds_queue[i] == NULL) {
            __sounds_queue[i] = soundArray;
            __serial_write_fmt("[Sound System] Sound %d has been queued\n", i);
            return;
        }
        i++;
    }
}

void __sounds_initThread() {
    __pit_setOnIntCallback(__sounds_thread);
    return;
}