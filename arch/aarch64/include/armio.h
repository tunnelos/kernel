#pragma once

// на всякий случай можно проверить архитектуру компилятора
#include "arch.h"

#if GCCARCH_NUM == 1 // aarch64

enum {
    PERIPHERAL_BASE = 0x0 //..? мб 0xFE000000 или 0x3E000000
}

#endif