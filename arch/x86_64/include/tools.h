#pragma once

#include "./stdint.h"

#define jmp goto

#ifdef __cplusplus
extern "C" {
#endif

uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);
void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);
void outl(uint16_t port, uint32_t val);
void io_wait(void);
/*
* Due to issues i have with timers, this function is not very presise and even can make lag
*
* wait function works on io_wait() function. IO operations requires some time to execute but it is random (from 1 to 4 nanoseconds)
*/
void wait(uint64_t ms);
/*
* Due to issues i have with timers, this function is not very presise and even can make lag
*
* wait_ns works on io_wait() function. IO operations requires some time to execute but it is random (from 1 to 4 nanoseconds)
*/
void wait_ns(uint128_t ns);
void insl(uint16_t reg, uint32_t *buffer, int quads);
// Gets ID of CPU running this function
int __tools_get_cpu();
// Interrupt test
extern void __tools_int_test(uint8_t interrupt_number);

#ifdef __cplusplus
}
#endif