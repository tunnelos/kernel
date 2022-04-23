#include "./include/easter.h"
#include "./include/smt.h"
#include "./include/stdio.h"
//От оригинала ему далеко
//так там находится самое важное (puts, putc, printf)
//puts - выводит строку на экран
//puts_gui - выводит строку с прозрачностю на экран
//putc - выводит символ на экран
//putc_gui - выводит символ с прозрачностью на экран
//printf - выводит строку с форматированием на экран
//printf_gui - выводит строку с прозрачностью и форматированием на экран
//я отойду ок
// Можно краткий экскурс что делают эти функции?
int __easter_col;

void __easter_create_easter() {
    __smt_create_task(__easter_thread);
}
void __easter_thread(int id) {
    __easter_col++;
    puts(__EASTER_TEXT, __easter_col, 0, 28);
    return;
}