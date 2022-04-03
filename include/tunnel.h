#pragma once

#include "./stdint.h"
#include "../bootboot/dist/bootboot.h"

typedef struct {
    BOOTBOOT bootboot;
} tunnelos_sysinfo_t;

tunnelos_sysinfo_t tunnelos_sysinfo;