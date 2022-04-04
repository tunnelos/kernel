#pragma once

#include "./stdint.h"
#include "../bootboot/dist/bootboot.h"

typedef struct {
    BOOTBOOT bootboot;
    uint8_t *free_memory_location;
    uint64_t free_memory_location_size;
} tunnelos_sysinfo_t;

tunnelos_sysinfo_t tunnelos_sysinfo;