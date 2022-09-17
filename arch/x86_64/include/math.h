#pragma once

#include "./stdint.h"

#define s1 1.f/3.f
#define s2 1.f/5.f
#define s3 1.f/7.f
#define s4 1.f/9.f
#define s5 1.f/11.f

#define POLYNOMIAL2(xx) ((((s5 * (xx) + s4) * (xx) + s3) * (xx) + s2) * (xx))
#define POLYNOMIAL(xx) (POLYNOMIAL2 (xx) + s1)

#define TAYLOR_SIN(xx, x, dx) \
({                                                                           \
    double t = ((POLYNOMIAL (xx)  * (x) - 0.5 * (dx))  * (xx) + (dx));        \
    double res = (x) + t;                                                       \
    res;                                                                        \
})

#ifdef __cplusplus
extern "C" {
#endif

int abs(int i);
double fabs(double x);
float acos(float x);
int pow(int x, int y);
int floor(float x);
float floorf(float x);
double floord(double x);
float round(float number);
float sqrt(float number);
float modf(float x, float *iptr);
float acos(float x);

#ifdef __cplusplus
}
#endif
