#include "../include/pit.h"
#include "../include/cint.h"
#include "../include/idt.h"

uint64_t current_tick;

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

void __pit_event_timer(){
    current_tick++;
}