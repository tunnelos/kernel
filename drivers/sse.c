#include "../include/sse.h"
#include "../include/mm.h"

char region[512] __attribute__((aligned(16)));

void __sse_allocate() {
    asm volatile("fxsave %0"::"m"(region));
    return;
}