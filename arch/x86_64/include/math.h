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

#define PI 3.14159265359
#define E 2.71828182846

#ifdef __cplusplus
extern "C" {
#endif

//utils
int16_t compare_float(double f1, double f2);

//math.h functions
int32_t abs(int x);
double fabs(double x);
float sqrt(float number);
float round(float number);
int32_t ceil(float x);
double cos(double x);
double fmod(double x, double y);
double sin(double x);
int32_t pow(int x, int y);
int32_t floor(float x);
float floorf(float x);
double floord(double x);
long double floorl(long double x);
double truncate(double x);

#ifdef __cplusplus
}
#endif
