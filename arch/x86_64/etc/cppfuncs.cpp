#include "../include/mm.h"
#include "../include/tags.h"

// Memory management functions
cpp_code void *operator new(size_t size) {
    return malloc(size);
}
// Memory management functions
cpp_code void *operator new[](size_t size) {
    return malloc(size);
}
// Memory management functions
cpp_code void operator delete(void *p){
    free(p);
}
// Memory management functions
cpp_code void operator delete[](void *p){
    free(p);
}
cpp_code void operator delete(void *p, unsigned long) {
    free(p);
}