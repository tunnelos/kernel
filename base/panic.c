#include "../include/panic.h"
#include "../include/screen.h"
#include "../include/ide.h"
#include "../include/smt.h"
#include "../include/idt.h"

const char *pcb(bool j) {
    if(!j) return "false";
    return "true ";
}

void crash(const char *str, uint16_t id, bool interrupt) {
    printf(COLOR_RED, 0, 0, "PANIC\n\n * Reason: %s", str);
    int ides = 0;
    int i = 0;
    while(i < 4) {
        if(__ide_devices[i].connected) ides++;
        i++;
    }
    tunnelos_sysinfo_t *t = &tunnelos_sysinfo;
    int totalram = t->free_memory_location_size;
    i = 0;
    int mapedram = totalram;
    int allocram = 0;
    while(i < 8320 * 8) {
        if(!t->mm->meta[i].free) allocram += 256;
        i++;
    }
    //calculate memory map size
    mapedram -= (sizeof(t->mm->api_data) + sizeof(t->mm->blockdata) + sizeof(t->mm->meta) + sizeof(t->mm->start_point));
    
    if(!interrupt) {//Allocated 
        printf(COLOR_YELLOW, 0, 3, " * Avaliable information:\n   * Memory: Maped     %d  KB of %d  KB\n             Allocated %d  KB of %d  KB\n   * Connected IDE drives: %d\n\n   * SMT : %s  | AVX: %s  | SSE       : %s\n\n     SSE2: %s  | RTC: %s  | Interrupts: %s\n\n     NMI : %s  | PIT: %s  | IDE       : %s\n\n   * Cores: %d\n\n * Minimum System Requirements:\n   * Memory: 20 MB\n   * CPU with SSE support\n   * PS/2 support", 
            (totalram / 1024) - (mapedram / 1024), totalram / 1024,
            allocram / 256, sizeof(t->mm->blockdata) / 1024,
            ides, 
            pcb(__smt_inSMTmode), pcb(t->avx), pcb(t->sse), pcb(t->sse2), pcb(t->rtc),
            pcb(t->interrupts), pcb(t->nmi), pcb(t->pit), pcb(t->ide),
            t->cores
        );
    } else {
        printf(COLOR_YELLOW, 0, 3, " * Avaliable information:\n   * Memory: Maped     %d  KB of %d  KB\n             Allocated %d  KB of %d  KB\n   * Connected IDE drives: %d\n\n   * SMT : %s  | AVX: %s  | SSE       : %s\n\n     SSE2: %s  | RTC: %s  | Interrupts: %s\n\n     NMI : %s  | PIT: %s  | IDE       : %s\n\n   * Cores: %d  | Interrupt ID: %d  | Critical Interrupt: %s\n\n * Minimum System Requirements:\n   * Memory: 20 MB\n   * CPU with SSE support\n   * PS/2 support", 
            (totalram / 1024) - (mapedram / 1024), totalram / 1024,
            allocram / 256, sizeof(t->mm->blockdata) / 1024,
            ides, 
            pcb(__smt_inSMTmode), pcb(t->avx), pcb(t->sse), pcb(t->sse2), pcb(t->rtc),
            pcb(t->interrupts), pcb(t->nmi), pcb(t->pit), pcb(t->ide),
            t->cores, current_interrupt.interrupt_id, pcb(current_interrupt.critical)
        );
    }
    __smt_changestatus(false);
    while(1);
}