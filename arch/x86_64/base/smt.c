#include "../include/smt.h"
#include "../include/tools.h"
#include "../include/serial.h"
#include "../include/stdlib.h"

bool __smt_inSMTmode = false;
int __smt_tasks = 0;
int __smt_lastmax = 0;
uint16_t __smt_coreList[MAX_CORES] = {};
int maincpu_tid = 1024;
int __smt_avaliable_cores = 0;

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
            if(i < __smt_lastmax) __smt_lastmax = i;
            __serial_write_fmt("CPU %d -> tos > Created task %d\r\n", __tools_get_cpu() - 1, i);
            return &tunnelos_sysinfo.software_tasks[i];
        }
        i++;
    }
    return 0;
}
void __smt_run() {
    __smt_inSMTmode = true;
    int i = 0;
    while(i < tunnelos_sysinfo.bootboot.numcores) {
        if(i > 0) {
            cores[i - 1].busy = false;
            cores[i - 1].haveTask = false;
            cores[i - 1].coreID = i;
        }
        __smt_coreList[i] = 1024;
        i++;
    }
    i = 0;
    __serial_write_fmt("CPU %d -> tos > SMT is ready\r\n", __tools_get_cpu() - 1);
    while(1){
        if(!__smt_inSMTmode) return;
        
        i++;
        if(i < __smt_lastmax) i = 0;

        if(tunnelos_sysinfo.software_tasks[i].is_used && !tunnelos_sysinfo.software_tasks[i].already_executing) {
            int ii = 0;
            bool functionExecuted = false;
            while(ii < tunnelos_sysinfo.bootboot.numcores - 1){
                if((!cores[ii].busy && !cores[ii].haveTask) && !functionExecuted) {
                    cores[ii].task_to_execute = &tunnelos_sysinfo.software_tasks[i];
                    cores[ii].haveTask = true;
                    functionExecuted = true;
                }
                ii++;
            }
            ii = 0;
            if(!functionExecuted) {
                //execute on main cpu
                maincpu_tid = tunnelos_sysinfo.software_tasks[i].id;
                tunnelos_sysinfo.software_tasks[i].already_executing = true;
                tunnelos_sysinfo.software_tasks[i].runner(tunnelos_sysinfo.software_tasks[i].id);
                if(tunnelos_sysinfo.software_tasks[i].one_time) {
                    tunnelos_sysinfo.software_tasks[i].is_used = false;
                    tunnelos_sysinfo.software_tasks[i].already_executing = false;
                    maincpu_tid = 1024;
                } else {
                    tunnelos_sysinfo.software_tasks[i].runned_times++;
                    tunnelos_sysinfo.software_tasks[i].already_executing = false;
                    maincpu_tid = 1024;
                }
            }
        }
        i++;
    }
}
