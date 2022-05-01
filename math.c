#include "./include/math.h"
#include "./include/stdint.h"

int abs (int i){
  return i < 0 ? -i : i;
}

float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck? 
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

float sqrt(float number) {
    return round(Q_rsqrt(number) * (number / 2) * 2);
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
    } else{
        result = 1;
    }

    return result;
}

int floor(float x){
    return (int)x;
} 

float modf(float x, float *iptr){
    union {
        float f; 
        uint32_t i;
    } u = {x, 0};
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
