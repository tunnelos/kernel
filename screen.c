#include "./include/screen.h"

uint32_t blend(uint32_t *colors, uint32_t size) {
    uint32_t i = 0;
    uint32_t r = 0;
    while(i < size){
        r += *(colors + i);
        i++;
    }
    return r;
}