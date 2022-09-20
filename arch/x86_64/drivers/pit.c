#include "../include/pit.h"
#include "../include/cint.h"
#include "../include/idt.h"

uint128_t current_tick;
bool (* __pit_callback[128])(uint128_t);

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
    //current_tick++;
    if(__pit_callback[__pit_currentTaskPointer]) {
        if(!__pit_callback[__pit_currentTaskPointer](current_tick)) __pit_callback[__pit_currentTaskPointer] = NULL;
    }
    __pit_currentTaskPointer++;
    if(__pit_currentTaskPointer == 128) __pit_currentTaskPointer = 0;
    // uint8_t i = 0;
    // while(i < 128) {
    //     if(__pit_callback[i]) {
    //         if(!__pit_callback[i](current_tick)) __pit_callback[i] = NULL;
    //     }
    //     i++;
    // }
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