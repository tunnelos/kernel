#pragma once

#include "./stdint.h"

#define __PIT_PORT0 0x40
#define __PIT_PORT1 0x41
#define __PIT_PORT2 0x42
#define __PIT_PORT3 0x43

#define __PIT_MODE0 0;
#define __PIT_MODE1 0;
#define __PIT_MODE2 0;
#define __PIT_MODE3 0;
#define __PIT_MODE4 0;
#define __PIT_MODE5 0;

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

