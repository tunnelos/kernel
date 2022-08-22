#pragma once

#include "extern.h"

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