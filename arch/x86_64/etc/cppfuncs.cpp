#include "../include/mm.h"

// Memory management functions
void *operator new(size_t size) {
    return malloc(size);
}
// Memory management functions
void *operator new[](size_t size) {
    return malloc(size);
}
// Memory management functions
void operator delete(void *p){
    free(p);
}
// Memory management functions
void operator delete[](void *p){
    free(p);
}