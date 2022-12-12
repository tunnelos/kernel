#pragma once

#include "./panic.h"

// #define assert(expr) if(!(expr)) {printf(COLOR_YELLOW, 0, 0, "Assertion failed: '" #expr "' returns false\n"); while(1);}
#define assert(expr) if(!(expr)) { crash("Assertion failed: '" #expr "' returns false", 0x0F0F, false); }