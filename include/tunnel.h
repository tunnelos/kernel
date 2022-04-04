#pragma once

#include "./stdint.h"
#include "./smt.h"
#include "../bootboot/dist/bootboot.h"

typedef struct {
    int size;
    void *address;
} tunnel_memory_block_t;
typedef struct {
    uint8_t alloc_table[8*1024*1024];
} tunnel_memory_map_t;
typedef struct {
    BOOTBOOT bootboot;
    uint8_t *free_memory_location;
    uint64_t free_memory_location_size;
    tunnel_memory_map_t *mm;
    smt_task_t software_tasks[64];
} tunnelos_sysinfo_t;

tunnelos_sysinfo_t tunnelos_sysinfo;