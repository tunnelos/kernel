#pragma once

#include "./stdint.h"
#include "../bootboot/dist/bootboot.h"

extern uint32_t scanlines;

void _start();
void __main_core0init();