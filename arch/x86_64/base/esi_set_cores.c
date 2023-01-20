#include "../include/early_system_init.h"
#include "../include/stdio.h"

void __esi_set_cores() {
    __stdio_margin = 0;

    __esi_setup_memory();
    __esi_testing();
}