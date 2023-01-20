#pragma once

#include "./mm.h"

#ifndef ENABLE_TRANDOM
#define ENABLE_TRANDOM 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern int __stdlib_seed;

// Set seed to rand function
void srand(int seed);
// Generates random value
int rand();

#if ENABLE_TRANDOM > 0
// RNG is based on PIT's output
extern int TUNNEL_RANDOM();
#endif

#ifdef __cplusplus
}
#endif