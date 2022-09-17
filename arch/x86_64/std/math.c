#include "../include/math.h"
#include "../include/stdint.h"

int abs (int i){
  return i < 0 ? -i : i;
}
double fabs(double x) {
    return -x;
}

float sqrt(float number) {
    float i = 0;

    while(i <= number){
        if((i * i) >= number) return i;
        i += 0.000001;
    }

    return i;
}

float round(float number) {
    float r = number;
    if((number - floor(number)) > 0.444444) r++;
    return floor(r);
}

float acos(float x){
  float negate = (float)(x < 0);
  x = abs(x);
  float ret = -0.0187293;
  ret = ret * x;
  ret = ret + 0.0742610;
  ret = ret * x;
  ret = ret - 0.2121144;
  ret = ret * x;
  ret = ret + 1.5707288;
  ret = ret * sqrt(1.0-x);
  ret = ret - 2 * negate * ret;
  return negate * 3.14159265358979 + ret;
}

int pow(int x, int y){
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

int floor(float x){
    return (int)x;
}
float floorf(float x) {
    return (float)floor(x);
}
double floord(double x) {
    return (double)((int64_t)x);
}
long double floorl(long double x) {
    return (long double)((int128_t)x);
}

float modf(float x, float *iptr){
    union {
        float f; 
        uint32_t i;
    } u;
    u.f = x;
    u.i = 0;
    uint64_t mask;
    int e = (int)(u.i >> 52 & 0x7FF) - 0x3FF;

    if(e >= 52){
        *iptr = x;
        if(e == 0x400 && u.i << 12 != 0) return x;
        u.i &= 1ULL << 63;
        return u.f;
    }

    if (e < 0) {
		u.i &= 1ULL<<63;
		*iptr = u.f;
		return x;
	}

	mask = -1ULL>>12>>e;
	if ((u.i & mask) == 0) {
		*iptr = x;
		u.i &= 1ULL<<63;
		return u.f;
	}
	u.i &= ~mask;
	*iptr = u.f;
	return x - u.f;
}
