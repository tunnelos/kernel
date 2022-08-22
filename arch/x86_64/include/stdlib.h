#pragma once

#ifndef ENABLE_TRANDOM
#define ENABLE_TRANDOM 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern int __stdlib_seed;

void srand(int seed);
int rand();

#if ENABLE_TRANDOM > 0
extern int TUNNEL_RANDOM();
#endif

#ifdef __cplusplus
}
#endif