#pragma once

#include "./stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

extern char scancodePub;
void _shell__create_shell(int id);
void __shell_keyboard_input(int id);

#ifdef __cplusplus
}
#endif