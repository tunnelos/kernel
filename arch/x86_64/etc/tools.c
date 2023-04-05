#include "../include/tools.h"
#include "../include/idt.h"
#include "../include/time.h"
#include <cpuid.h>

uint8_t inb(uint16_t port) {
    uint8_t val;
    asm volatile(
        "inb %%dx, %%al"
        : "=a"(val)
        : "d"(port)
    );
    return val;
}
uint16_t inw(uint16_t port) {
    uint16_t val;
    asm volatile(
        "inw %%dx, %%ax"
        : "=a"(val)
        : "d"(port)
        );
    return val;
}
uint32_t inl(uint16_t port) {
    uint32_t val;
    asm volatile(
        "inl %%edx, %%eax"
        : "=a"(val)
        : "d"(port)
    );
    return val;
}

void outb(uint16_t port, uint8_t val){
    asm volatile(
        "outb %%al, %%dx"
        :
        : "d"(port), "a"(val)
    );
    return;
}
void outw(uint16_t port, uint16_t val) {
    asm volatile(
        "outw %%ax, %%dx"
        :
        : "d"(port), "a"(val)
    );
    return;
}
void outl(uint16_t port, uint32_t val) {
    asm volatile(
        "outl %%eax, %%dx"
        :
        : "d"(port), "a"(val)
    );
    return;
}

int __tools_get_cpu() {
    uint32_t eax, ebx, u1, u2;
    __cpuid(1, eax, ebx, u1, u2);
    return (ebx >> 24) + 1;
}

uint64_t get_cycles() {
    uint32_t eax;
    uint32_t edx;
    asm volatile("rdtsc" : "=a"(eax), "=d"(edx));
    return ((uint64_t)edx << 32 | eax);
}

//waits 1-4 ns
void io_wait() {
    outb(0x81, 0);
    return;
}
void wait(uint64_t ms) {
    ms *= 10;
    uint128_t i = 0;
    while(i < ms * 1000) {
        outb(0x80, 0);
        i++;
    }
    return;
}
void waitb(uint64_t ms) {
    ms *= 10;
    uint128_t i = 0;
    while(i < ms * 1000) {
        outb(0x80, 0);
        i++;
    }
    return;
}
void wait_ns(uint128_t ns) {
    uint128_t i = 0;
    while(i < ns) {
        outb(0x80, 0);
        i++;
    }
    return;
}
void accwait(uint64_t ms) {
    uint64_t timeold = __time_get_uptimeMS();
    uint64_t timeexpected = timeold + ms;
    while(1) {
        uint64_t timenew = __time_get_uptimeMS();
        if(timenew == timeexpected || timenew > timeexpected) return;
    }
}

void insl(uint16_t reg, uint32_t *buffer, int quads) {
    int index = 0;
    while(index < quads) buffer[index++] = inl(reg);
    return;
}
