#pragma once

#include "./stdint.h"

#define jmp goto

#define check_flag(flags, n) ((flags) & bit(n))
#define bit(n) (1<<(n))

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
uint64_t get_cycles();
void wait(uint64_t ms);
void waitb(uint64_t ms);
void wait_ns(uint128_t ns);
void accwait(uint64_t ms);
void insl(uint16_t reg, uint32_t *buffer, int quads);
// Gets ID of CPU running this function
int __tools_get_cpu();
// Interrupt test
// extern void __tools_int_test(uint8_t interrupt_number);

#ifdef __cplusplus
}
#endif