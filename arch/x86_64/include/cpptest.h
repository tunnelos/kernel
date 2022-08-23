#pragma once

#include "extern.h"
#include "flags.h"

#if ENABLE_TEST == 1

#ifdef __cplusplus

#pragma pack(push, 1)
class TestClass {
    private:
    int a;
    bool b;

    public:
    void onCreate();
    void onDestroy();
    void test();
};
#pragma pack(pop)

#endif

EXTERNC void cpptest_test00();

#endif