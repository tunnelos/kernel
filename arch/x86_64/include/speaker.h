#pragma once

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

void __speaker_playSound(uint32_t frequency);
void __speaker_stopSound();

#ifdef __cplusplus
}
#endif