#include "./include/panic.h"
#include "./include/screen.h"

void crash(const char *str) {
    puts(str, COLOR_RED, 0, 0);
    while(1);
}