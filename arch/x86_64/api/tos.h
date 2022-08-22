#pragma once

#define API_STARTPOINT 0

// #include "../bootboot/dist/bootboot.h"

// typedef struct {
//     BOOTBOOT bootboot;
//     uint8_t *free_memory_location;
//     uint64_t free_memory_location_size;
//     void *mm;
//     smt_task_t software_tasks[256];
//     bool avx;
//     bool sse;
//     bool sse2;
//     bool rtc;
//     bool interrupts;
//     bool nmi;
//     bool pit;
//     bool ide;
//     int cores;
// } API_tunnelos_sysinfo_t;

// typedef struct {
//     API_tunnelos_sysinfo_t *sys;
//     void (*create_apimap)(void);
// } apimap_t;