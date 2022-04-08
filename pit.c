#include "./include/pit.h"
#include "./include/cint.h"

uint64_t current_tick;
pit_timer_t __pit_timers[396];

void __pit_create_timer(void (*callback)(void), bool multiple, uint64_t every) {
    int id = 0;

    do { id++; } while(!__pit_timers[id].avaliable);
    
    pit_timer_t *ct = &__pit_timers[id];
    ct->next_on = current_tick + every;
    ct->avaliable = false;
    ct->callback = callback;
    ct->every = every;
    ct->multiple = multiple;
    
    return;
}

void __pit_event_timer(){
    int i = 0;
    while(i < 396) {
        if(!__pit_timers[i].avaliable && __pit_timers[i].next_on == current_tick) {
            current_tick++;
            __pit_timers[i].next_on = current_tick + __pit_timers[i].every;
            __pit_timers[i].callback();
            if(!__pit_timers[i].multiple) __pit_timers[i].avaliable = true;
        }
        i++;
    }
}