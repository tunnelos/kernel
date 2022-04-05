#include "./include/tools.h"
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
void outb(uint16_t port, uint8_t val){
    asm volatile(
        "outb %%al, %%dx"
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