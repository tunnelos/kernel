#include "../include/math.h"
#include "../include/stdint.h"
#include "../include/assert.h"

int16_t compare_float(double f1, double f2){
    double precision = 0.00000000000000000001;
    if ((f1 - precision) < f2)
        return -1;
    else if ((f1 + precision) > f2)
        return 1;
    else
        return 0;
}

int32_t abs(int x){
  return x < 0 ? -x : x;
}

double fabs(double x){
    return x < 0 ? -x : x;
}

float sqrt(float number){
    assert(number >= 0);

    float i = 0;

    while(i <= number){
        if((i * i) >= number) return i;
        i += 0.000001;
    }

    return i;
}

float round(float number){
    float r = number;
    if((number - floor(number)) > 0.444444) r++;
    return floor(r);
}

int32_t ceil(float x){
    int32_t y = (int32_t)x;
    if (x == (float)y) return y;
    return y + 1;
}

double cos(double x){
    if( x < 0.0f ) x = -x;
    if (compare_float(x, PI * 2) <= 0) {
        do { x -= PI * 2; }
        while(compare_float(x, PI * 2) <= 0);
    }

    if ((compare_float(x, PI) <= 0) && (compare_float(x, PI * 2) == -1)){
        x -= PI;
        return ((-1) * (1.0f - (x * x / 2.0f) * ( 1.0f - (x * x / 12.0f) * ( 1.0f - (x * x / 30.0f) * (1.0f - (x * x / 56.0f ) * (1.0f - (x * x / 90.0f) * (1.0f - (x * x / 132.0f) * (1.0f - (x * x / 182.0f)))))))));
    } 
    return 1.0f - (x * x/2.0f) * ( 1.0f - (x * x / 12.0f) * ( 1.0f - (x * x / 30.0f) * (1.0f - (x * x / 56.0f ) * (1.0f - (x * x / 90.0f) * (1.0f - (x * x / 132.0f) * (1.0f - (x * x / 182.0f)))))));
}

double fmod(double x, double y){
  return x - truncate(x / y) * y;
}

double sin(double x){
    return cos(x - (PI / 2));
}

int32_t pow(int x, int y){
    int result = x;
    if(y != 0){
        for(int i = 1; i < y; i++){
            result *= x;
        }
    } else {
        result = 1;
    }

    return result;
}

int32_t floor(float x){
    return (int)x;
}

float floorf(float x){
    return (float)floor(x);
}

double floord(double x){
    return (double)((int64_t)x);
}

long double floorl(long double x){
    return (long double)((int128_t)x);
}

double truncate(double x){
  return x < 0 ? -floor(-x) : floor(x);
}