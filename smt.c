#include "./include/smt.h"
//#include "./include/stdio.h"

bool __smt_inSMTmode;
int __smt_tasks = 0;

void __smt_changestatus(bool status) {
    __smt_inSMTmode = status;
    return;
}
smt_task_t *__smt_create_task(void (*runner)(int id)){
    int i = 0;
    while(i < 8192 * 2) i++;
    i = 0;
    while(i < 64){
        if(!tunnelos_sysinfo.software_tasks[i].is_used) {
            tunnelos_sysinfo.software_tasks[i].id = i;
            tunnelos_sysinfo.software_tasks[i].runner = runner;
            tunnelos_sysinfo.software_tasks[i].runned_times = 0;
            tunnelos_sysinfo.software_tasks[i].one_time = false;
            tunnelos_sysinfo.software_tasks[i].is_used = true;
            __smt_tasks++;
            return &tunnelos_sysinfo.software_tasks[i];
        }
        i++;
    }
    return 0;
}
void __smt_run() {
    int i = 0;
    while(i < 8192 * 2) i++;
    i = 0;
    while(1){
        if(i == 64) i = 0;

        if(tunnelos_sysinfo.software_tasks[i].is_used) {
            tunnelos_sysinfo.software_tasks[i].runner(i);
            if(tunnelos_sysinfo.software_tasks[i].one_time){
                tunnelos_sysinfo.software_tasks[i].is_used = false;  
                __smt_tasks--;  
            } else {
                tunnelos_sysinfo.software_tasks[i].runned_times++;
            }
        }
        i++;
    }
}
