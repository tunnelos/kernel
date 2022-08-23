#pragma once

#include "unitype.h"
#include "flags.h"

#ifdef __cplusplus
extern "C" {
#endif

#if ENABLE_TEST == 1
extern unitype_t *__test_univariable;
// Unitype variable test
void __test_unitest();
#endif

#ifdef __cplusplus
}
#endif