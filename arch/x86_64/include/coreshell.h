#pragma once

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

bool __coreshell_onPIT(uint128_t t);
void __coreshell_init();

#ifdef __cplusplus
}
#endif