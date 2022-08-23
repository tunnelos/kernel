#pragma once

#include "./window.h"
#include "./tools.h"
#include "./stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

extern window_t __wWelcome_context;
extern char *__wWelcome_old_context;

// "Welcome" window
void __wWelcome_start();

#ifdef __cplusplus
}
#endif