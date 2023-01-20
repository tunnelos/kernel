#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "registers.h"

typedef struct task_t {
    uint16_t id;
    uint8_t state; // ssssiiii (s - state; i - idle mode)

    const char *name;

    void *_stack;
    register_set_t _registers;
} task_t;

void __task_create_task_internals(task_t *task);

void __task_switch();

#ifdef __cplusplus
}
#endif
