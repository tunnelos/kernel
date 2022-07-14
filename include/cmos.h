#pragma once

#include "stdint.h"

#define CMOS_YEAR 2022

#pragma pack(push, 1)
typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint32_t year;
    uint32_t century;
} cmos_t;
#pragma pack(pop)

extern uint8_t __cmos_data[128];
extern cmos_t __cmos_time;
extern bool __cmos_firstInt;

int __cmos_getUpdate();
uint8_t __cmos_getRTCreg(uint8_t reg);
void __cmos_getRTC();
void __cmos_read(uint8_t *data_array);