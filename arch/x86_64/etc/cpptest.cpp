#include "../include/cpptest.h"
#include "../include/cppfuncs.hpp"
#include "../include/mm.h"

void TestClass::test() {
    this->a++;
    this->b = !this->b;
    return;
}
void TestClass::onCreate() {
    this->a = 0;
    this->b = false;
}
void TestClass::onDestroy() {
    return;
}

void cpptest_test00() {
    TestClass *cl = CREATE_CLASS(TestClass);
    cl->onCreate();
    cl->test();
    cl->onDestroy();
    DELETE_CLASS(cl);
    return;
}