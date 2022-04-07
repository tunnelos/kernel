#include "./include/tools.h"
#include <cpuid.h>

//На тебе сейчас задача сделать PIT
//Что это? -- Таймер на уровне железа
//Делай глобальные переменные и функции в таком стиле __pit_(функция/переменная)
//А какре у нас разрешение экрана? 640x480 ,текст в графическом режиме
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
        "inw %%dx, %%al"
        : "=a"(val)
        : "d"(port)
        );
    return val;
}
uint32_t inl(uint16_t port) {
    uint32_t val;
    asm volatile(
        "inl %%dx, %%al"
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
        "outw %%al, %%dx"
        :
        : "d"(port), "a"(val)
    );
    return;
}
void outl(uint16_t port, uint32_t val) {
    asm volatile(
        "outl %%al, %%dx"
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

void io_wait() {
    outb(0x80, 0);
    return;
}