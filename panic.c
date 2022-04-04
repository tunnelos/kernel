#include "./include/panic.h"
#include "./include/screen.h"

void crash(const char *str) {
    puts(str, COLOR_RED);
    while(1);
}