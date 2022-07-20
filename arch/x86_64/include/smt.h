#pragma once

#include "./stdint.h"
#define MAX_CORES 8 + 1

extern bool __smt_inSMTmode;
extern int __smt_tasks;
extern uint16_t __smt_coreList[MAX_CORES];
extern int maincpu_tid;
extern int __smt_avaliable_cores;

typedef struct {
    int id;
    void (*runner)(int id);
    int runned_times;
    bool one_time;
    bool is_used;
    bool already_executing;
} smt_task_t;

typedef struct {
    int coreID;
    bool busy;
    bool haveTask;
    smt_task_t *task_to_execute;
    bool exception;
} core_t;

core_t cores[MAX_CORES - 1];

#include "./tunnel.h"

void __smt_changestatus(bool status);
void __smt_run();
smt_task_t *__smt_create_task(void (*runner)(int id));