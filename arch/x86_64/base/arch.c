#include "../include/arch.h"

const char *__arch_archlookup[3] = {
    "x86_64",  "aarch64", "unknown"
};
const char *__arch_envlookup [3] = {
    "Mac OS", "Linux", "Unknown"
};

#if OPTIMIZE == 0
const char *__arch_getarch() {
    return __arch_archlookup[GCCARCH_NUM];
}
const char *__arch_getenvironment() {
    return __arch_envlookup[GCCENVIRONMENT_NUM];
}
#endif