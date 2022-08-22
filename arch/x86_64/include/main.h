#pragma once

#include "./stdint.h"
#include "./arch.h"
#if GCCARCH_NUM == 0
#include "../bootboot/dist/bootboot.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t scanlines;

#ifdef __cplusplus
}
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

void _start();
void __main_core0init();

#ifdef __cplusplus
}
#endif