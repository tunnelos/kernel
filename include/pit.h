#pragma once

#include "./stdint.h"

#define __PIT_PORT0 0x40 //A
#define __PIT_PORT1 0x41 //B
#define __PIT_PORT2 0x42 //C
#define __PIT_PORT3 0x43 //Control

#define __PIT_MODE0 0
#define __PIT_MODE1 0
#define __PIT_MODE2 0
#define __PIT_MODE3 0
#define __PIT_MODE4 0
#define __PIT_MODE5 0

#define __PIT_MASK 0xFF
#define __PIT_DIVIDER 1193180
#define __PIT_SET 0x36

typedef struct {
    bool avaliable: true;
    uint64_t every;
    bool multiple;
    void (*callback)(void);
    uint64_t next_on;
} pit_timer_t;

extern uint64_t current_tick;
// extern pit_timer_t __pit_timers[396];

void __pit_event_timer();
// void __pit_create_timer(void (*callback)(void), bool multiple, uint64_t every);
extern void __pit_init();

extern uint16_t __pit_system_timer_fractions;
extern uint32_t __pit_system_timer_ms;
extern uint32_t __pit_IRQ0_fractions;
extern uint32_t __pit_IRQ0_ms;
extern uint32_t __pit_IRQ0_frequency;
extern uint16_t __pit_reload_value;
