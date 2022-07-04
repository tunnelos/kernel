#pragma once

#ifndef ENABLE_TRANDOM
#define ENABLE_TRANDOM 1
#endif

extern int _stdlib__seed;

void srand(int seed);
int rand();

#if ENABLE_TRANDOM >= 0
extern int TUNNEL_RANDOM();
#endif