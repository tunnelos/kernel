#pragma once

#include "./stdint.h"
#include "./arch.h"
#if GCCARCH_NUM == 0
#include "../bootboot/dist/bootboot.h"

extern uint32_t scanlines;
#endif

void _start();
void __main_core0init();