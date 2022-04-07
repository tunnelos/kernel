#pragma once

#include "./stdint.h"

uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);
void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);
void outl(uint16_t port, uint32_t val);
void io_wait();
void wait(uint64_t ms);
void insl(uint16_t reg, uint32_t *buffer, int quads);

int __tools_get_cpu();
