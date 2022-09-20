#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)
typedef struct {
    unsigned short frequency;
    unsigned int soundticks;
} pcspeaker_sound_t;
#pragma pack(pop)

void __sounds_initThread();
void __sounds_queueSoundData(pcspeaker_sound_t *soundArray);

// Will add later
// pcspeaker_sound_t *__sounds_getCurrentArray();
// void __sounds_stopSound();
// void __sounds_pauseSound();

#ifdef __cplusplus
}
#endif
