#include "../include/stdlib.h"
#include "../include/cstring.h"

int __stdlib_seed = 0;

void srand(int seed) {
    __stdlib_seed = seed;
    return;
}
int rand() {
    __stdlib_seed = __stdlib_seed * 1103515245 + 12345;
    return (unsigned int) (__stdlib_seed / 65536) % 32768;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    return strncmp((const char *)s1, (const char *)s2, n);
}