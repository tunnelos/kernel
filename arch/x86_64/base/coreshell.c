#include "../include/coreshell.h"
#include "../include/tools.h"
#include "../include/tunnel.h"
#include "../include/assert.h"
#include "../include/mm.h"

drawtask_t *__coreshell_drawtasks;

void __coreshell_init_coreExecuter() {
    __coreshell_drawtasks = (drawtask_t *)malloc(1024 * sizeof(drawtask_t));
    int i = 0;
    while(i < 1024) {
        __coreshell_drawtasks[i].state = Unused;
        i++;
    }
}
void __coreshell_init_coreIOHandler() {
    while(1) {
        wait(10);

    }
}
void __coreshell_init_coreRenderer() {
    while(1) {
        wait(50);
        
    }
}

void __coreshell_init_all() {
    assert(tunnelos_sysinfo.cores >= 3);
    int cid = __tools_get_cpu() - 1;
    switch (cid) {
        case 0: {
            __coreshell_init_coreExecuter();
            break;
        }
        case 1: {
            __coreshell_init_coreIOHandler();
            break;
        }
        case 2: {
            __coreshell_init_coreRenderer();
            break;
        }
        default: {
            while(1);
        }
    }
}