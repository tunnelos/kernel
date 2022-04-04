#pragma once

#include "./stdio.h"

#define PANIC_NOT_ENOUGH_MEMORY "PANIC\n\n * Reason: not enough memory for continuing"

void crash(const char *str);