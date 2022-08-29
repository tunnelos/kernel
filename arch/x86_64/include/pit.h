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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pit_timer_t {
    bool avaliable: true;
    uint64_t every;
    bool multiple;
    void (*callback)(void);
    uint64_t next_on;
} pit_timer_t;

extern uint64_t current_tick;

void __pit_event_timer();
uint16_t __pit_count();
void __pit_set_count(uint16_t count);
extern void __pit_init();
extern void __pit_eoi();

extern uint16_t __pit_system_timer_fractions;
extern uint32_t __pit_system_timer_ms;
extern uint32_t __pit_IRQ0_fractions;
extern uint32_t __pit_IRQ0_ms;
extern uint32_t __pit_IRQ0_frequency;
extern uint16_t __pit_reload_value;

#ifdef __cplusplus
}
#endif
