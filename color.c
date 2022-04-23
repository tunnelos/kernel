#include "./include/color.h"

// int HSBtoRGB(uint8_t h, uint8_t s, uint8_t b){
//     int integer, fractional = mathModf(h / 60);
//     int p = b * (1 - s), q = b * (1 - s * fractional), t = b * (1 - (1 - fractional) * s);

//     if(integer == 0){
//         return mathFloor(b * 255), mathFloor(t * 255), mathFloor(p * 255);
//     }else if(integer == 1){
//         return mathFloor(q * 255), mathFloor(b * 255), mathFloor(p * 255);
//     }else if(integer == 2){
//         return mathFloor(p * 255), mathFloor(b * 255), mathFloor(t * 255);
//     }else if(integer == 3){
//         return mathFloor(p * 255), mathFloor(q * 255), mathFloor(b * 255);
//     }else if(integer == 4){
//         return mathFloor(t * 255), mathFloor(p * 255), mathFloor(b * 255);
//     }else{
//         return mathFloor(b * 255), mathFloor(p * 255), mathFloor(q * 255);
//     }
    
//}