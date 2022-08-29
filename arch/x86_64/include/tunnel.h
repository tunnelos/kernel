#pragma once

#include "./stdint.h"
#include "../bootboot/dist/bootboot.h"
#include "../api/tos.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)
typedef struct tunnel_memory_block_t {
    bool free;
    void *address;
    int have;
    int id;
} tunnel_memory_block_t;
typedef struct tunnel_memory_map_t {
    tunnel_memory_block_t meta[8320*8];
    char blockdata[8320*8][256];
    uint64_t start_point;
} tunnel_memory_map_t;
typedef struct tunnelos_sysinfo_t {
    BOOTBOOT bootboot;
    uint8_t *free_memory_location;
    uint64_t free_memory_location_size;
    tunnel_memory_map_t *mm;
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

extern tunnelos_sysinfo_t tunnelos_sysinfo;

void __tunnel_shutdown();
void __tunnel_reboot();
void push(int x);
int pop();

#ifdef __cplusplus
}
#endif