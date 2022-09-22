#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)
typedef struct pcspeaker_sound_t {
    unsigned short frequency;
    unsigned int soundticks;
} pcspeaker_sound_t;
#pragma pack(pop)

void __sounds_initThread();
void __sounds_queueSoundData(pcspeaker_sound_t *soundArray);

pcspeaker_sound_t *__sounds_getCurrentArray();
void __sounds_stopSound();
void __sounds_pauseSound();
void __sounds_resumeSound();

#ifdef __cplusplus
}
#endif
