#include "./include/encoder.h"
#include "./include/cstring.h"
#include "./include/mm.h"

uint16_t tunnel_chartable[512];

uint16_t *__encoder_ascii2ct(const char *data) {
    int l = strlen(data);
    if(l == 0) return 0;
    uint16_t *res = (uint16_t *)malloc(l * 2);
    int i = 0;
    while(i < l) {
        res[i] = data[i] % 128;
        i++;
    }
    return res;
    
}

void __encoder_fill_chartable() {
    return;
}