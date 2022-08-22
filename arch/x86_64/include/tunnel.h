#pragma once

#include "./stdint.h"
#include "./smt.h"
#include "../bootboot/dist/bootboot.h"
#include "../api/tos.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)
typedef struct {
    bool free;
    void *address;
    int have;
    int id;
} tunnel_memory_block_t;
typedef struct {
    apimap_t *api_data;
    tunnel_memory_block_t meta[8320*8];
    char blockdata[8320*8][256];
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
#pragma pack(pop)

tunnelos_sysinfo_t tunnelos_sysinfo;

#ifdef __cplusplus
}
#endif