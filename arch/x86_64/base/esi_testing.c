#include "../include/screen.h"
#include "../include/bootboot.h"
#if ENABLE_TEST == 1
#include "../include/test.h"
#include "../include/cpptest.h"
#endif

void __esi_testing() {
    #if ENABLE_TEST == 1
    int x = 0;
    int y = 0;
    int s = bootboot.fb_scanline;

    for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+20)*4))=0x00FF0000; } } // R 
    __test_unitest();
    for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+50)*4))=0x0000FF00; } } // G
    cpptest_test00();
    for(y=0;y<20;y++) { for(x=0;x<20;x++) { *((uint32_t*)(&fb + s*(y+20) + (x+80)*4))=0x000000FF; } } // B

    #endif
}