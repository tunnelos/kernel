#pragma once

#include "./stdint.h"
extern bool __smt_inSMTmode;
extern int __smt_tasks;

typedef struct {
    int id;
    void (*runner)(int id);
    int runned_times;
    bool one_time;
    bool is_used;
} smt_task_t;

#include "./tunnel.h"

void __smt_changestatus(bool status);
void __smt_run();
smt_task_t *__smt_create_task(void (*runner)(int id));