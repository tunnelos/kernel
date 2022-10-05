#include "../include/pit.h"
#include "../include/cint.h"
#include "../include/idt.h"

uint128_t current_tick;
bool (* __pit_callback[128])(uint128_t);
bool (* __pit_postintr)     (uint128_t);

uint16_t __pit_count() {
    uint16_t count;
    __cli();
    outb(__PIT_PORT3, 0b0000000);
    count = inb(__PIT_PORT0);
    count |= inb(__PIT_PORT0) << 8;
    return count;
}
void __pit_set_count(uint16_t count) {
    __cli();
    outb(__PIT_PORT0, count & 0xFF);
    outb(__PIT_PORT0, (count & 0xFF00) >> 8);
    return;
}

uint8_t __pit_currentTaskPointer = 0;

void __pit_event_timer(){
    current_tick++;
    if(__pit_callback[__pit_currentTaskPointer]) {
        if(!__pit_callback[__pit_currentTaskPointer](current_tick)) __pit_callback[__pit_currentTaskPointer] = NULL;
    }
    __pit_currentTaskPointer++;
    if(__pit_currentTaskPointer == 128) __pit_currentTaskPointer = 0;
    if(__pit_postintr) __pit_postintr(current_tick);
}

void __pit_setOnIntCallback(bool (* callback)(uint128_t)) {
    uint8_t i = 0;
    while(i < 128) {
        if(__pit_callback[i] == NULL) {
            __pit_callback[i] = callback;
            return;    
        }
        i++;
    }
}
void __pit_setPostInterrupt(bool (* callback)(uint128_t)) {
    __pit_postintr = callback;
}

bool __pit_waitTicks(uint64_t ticks, uint128_t ct) {
    if((current_tick == (ct + ticks)) || (current_tick > (ct + ticks))) return true;
    return false;
};