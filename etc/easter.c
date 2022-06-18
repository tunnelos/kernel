#include "../include/easter.h"
#include "../include/smt.h"
#include "../include/stdio.h"

int __easter_col;

void __easter_create_easter() {
    __smt_create_task(__easter_thread);
}
void __easter_thread(int id) {
    __easter_col++;
    puts(__EASTER_TEXT, __easter_col, 0, 28);
    return;
}