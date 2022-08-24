#include "../include/tunnel.h"
#include "../include/idt.h"

tunnelos_sysinfo_t tunnelos_sysinfo;

void __tunnel_reboot() {
    uint8_t t;
    __cli();
    while(check_flag(t, 1) != 0) {
        t = inb(0x64);
        if(check_flag(t, 0) != 0) inb(0x60);
    }
    outb(0x64, 0xFE);
    hlt:
    asm volatile("hlt");
    jmp hlt;
}