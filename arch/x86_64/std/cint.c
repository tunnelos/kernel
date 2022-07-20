#include "../include/cint.h"

int digitcount(unsigned int num) {
    int res = 0;
    while(num != 0) {
        ++res;
        num /= 10;
    }
    return res;
}