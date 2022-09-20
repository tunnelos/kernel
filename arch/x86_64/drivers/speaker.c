#include "../include/speaker.h"
#include "../include/tools.h"

void __speaker_playSound(uint32_t frequency) {
    uint32_t div;
    uint8_t tmp;

    div = 1193180 / frequency;
    outb(0x43, 0xB6);
    outb(0x42, (uint8_t)(div));
    outb(0x42, (uint8_t)(div >> 8));
    
    tmp = inb(0x61);
    if(tmp != (tmp | 3)) {
        outb(0x61, tmp | 3);
    }
}
void __speaker_stopSound() {
    uint8_t tmp = inb(0x61) & 0xFC;
    outb(0x61, tmp);
}