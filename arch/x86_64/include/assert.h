#pragma once

#include "./flags.h"
#include "./screen.h"

#define assert(expr) if(!(expr)) {printf(COLOR_YELLOW, 0, 0, "Assertion failed: '" #expr "' returns false\n"); while(1);}