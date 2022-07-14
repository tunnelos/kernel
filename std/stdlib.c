#include "../include/stdlib.h"

int __stdlib_seed = 0;

void srand(int seed) {
    __stdlib_seed = seed;
    return;
}
int rand() {
    __stdlib_seed = __stdlib_seed * 1103515245 + 12345;
    return (unsigned int) (__stdlib_seed / 65536) % 32768;
}