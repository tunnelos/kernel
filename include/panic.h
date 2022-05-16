#pragma once

#include "./stdio.h"
#include "./tunnel.h"

#define PANIC_NOT_ENOUGH_MEMORY_STRING "Not enough memory for continuing"
#define PANIC_NOT_ENOUGH_MEMORY_NUMBER 0x0000

#define PANIC_UNEXPECTED_INTERRUPT_STRING "Unexpected interrupt"
#define PANIC_UNEXPECTED_INTERRUPT_NUMBER 0x0001

void crash(const char *str, uint16_t id);