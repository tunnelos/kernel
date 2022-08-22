#pragma once

#ifdef __cplusplus
extern "C" {
#endif

extern int __easter_col;
#define __EASTER_TEXT "Happy Easter!"

void __easter_create_easter();
void __easter_thread(int id);

#ifdef __cplusplus
}
#endif