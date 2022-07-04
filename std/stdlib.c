#include "../include/stdlib.h"

int _stdlib__seed = 0;

void srand(int seed) {
    _stdlib__seed = seed;
    return;
}
int rand() {
    _stdlib__seed = _stdlib__seed * 1103515245 + 12345;
    return (unsigned int) (_stdlib__seed / 65536) % 32768;
}