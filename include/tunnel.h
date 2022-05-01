#pragma once

#include "./stdint.h"
#include "./smt.h"
#include "../bootboot/dist/bootboot.h"

typedef struct {
    int size;
    int buffer_index;
    void *address;
    int struct_index;
    struct tunnel_memory_block_t *next;
    struct tunnel_memory_block_t *prev;
} tunnel_memory_block_t;
typedef struct {
    uint8_t alloc_table[8*1024*1024];
    uint64_t start_point;
} tunnel_memory_map_t;
typedef struct {
    BOOTBOOT bootboot;
    uint8_t *free_memory_location;
    uint64_t free_memory_location_size;
    tunnel_memory_map_t *mm;
    smt_task_t software_tasks[256];
    bool avx;
    bool sse;
    bool sse2;
    bool rtc;
    bool interrupts;
    bool nmi;
    bool pit;
    bool ide;
    int cores;
} tunnelos_sysinfo_t;

tunnelos_sysinfo_t tunnelos_sysinfo;