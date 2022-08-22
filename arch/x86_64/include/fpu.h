#pragma once

#include "./include/stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

extern bool __fpu_check();
extern uint8_t __fpu_cr0_ts;
extern uint8_t __fpu_cr0_em;

#ifdef __cplusplus
}
#endif