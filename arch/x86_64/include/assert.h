#pragma once

#include "./flags.h"
#include "./smt.h"
#include "./screen.h"

#define assert(expr) if(!(expr)) {printf(COLOR_YELLOW, 0, 0, "Assertion failed: '" #expr "' returns false\n"); __smt_changestatus(false); while(1);}