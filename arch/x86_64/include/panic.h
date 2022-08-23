#pragma once

#include "./stdio.h"
#include "./tunnel.h"
#include "./flags.h"

#define PANIC_NOT_ENOUGH_MEMORY_STRING "Not enough memory for continuing"
#define PANIC_NOT_ENOUGH_MEMORY_NUMBER 0x0000

#define PANIC_UNEXPECTED_INTERRUPT_STRING "Unexpected interrupt"
#define PANIC_UNEXPECTED_INTERRUPT_NUMBER 0x0001

#ifdef __cplusplus
extern "C" {
#endif

#if INTERNAL_FUNCS_IN_HEADERS == 1
// Bool to string (true/false)
const char *pcb(bool j);
#endif
// Kernel panic function
void crash(const char *str, uint16_t id, bool interrupt);

#ifdef __cplusplus
}
#endif