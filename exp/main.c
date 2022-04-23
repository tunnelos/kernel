#include <stdio.h>

int pdow(int x, int y){
    int result = x;
    if(y != 0){
        for(int i = 1; i < y; i++){
            result *= x; 
        }
        return result; // Блин я не уверен, надо протестить у себя
    } else{
        return 1;
    }
}

int flooor(float x){
    return (int)x;
} 

float sqrtt(float x){
    float n = x;
    float y = 1;
    float e = 0.000001;
    while(n - y > e){
        x = (n + y) / 2;
        y = x / n;
        n--;
    }
    return n;
}
float test(float sq) {
    return sq;
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

	return y * number;
}

int main(){
	printf("Pow 5,3: %d\n", pdow(5,3));
	printf("Pow 6,0: %d\n", pdow(6,0));
    printf("Pow 4,2: %d\n", pdow(4,2));
    printf("floor 4.5: %d\n", flooor(4.5));
    printf("floor 5.7: %d\n", flooor(5.7));
    printf("floor 3: %d\n", flooor(3));
    printf("floor 5.7: %d\n", flooor(2.1));
    //__uint128_t h = 5;
    //h += 1024;
    //printf("128bit number: %lld\n", h);
    //так это неточный алгоритм
    printf("Корень из 64: %f", Q_rsqrt(64) * 64); // тут должно быть 8, а не 7,99 > так это неточный алгоритм, но зато быстрый ладно
	return 0;
}

