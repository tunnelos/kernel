#pragma once

#include "./stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint16_t tunnel_chartable[512];
void __encoder_fill_chartable();
//you should to convert it to uint16_t * after running function
uint16_t *__encoder_ascii2ct(const char *data);

#ifdef __cplusplus
}
#endif