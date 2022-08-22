#pragma once

#ifdef __cplusplus
extern "C" {
#endif

extern int __cpuid_check_sse();
extern int __cpuid_check_avx();
extern int __cpuid_check_avx2();
extern int __cpuid_check_avx512();

#ifdef __cplusplus
}
#endif