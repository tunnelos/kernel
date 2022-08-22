#include "../include/cpptest.h"
#include "../include/cppfuncs.hpp"
#include "../include/mm.h"

void TestClass::test() {
    this->a++;
    this->b = !this->b;
    return;
}
TestClass::TestClass() {
    this->a = 0;
    this->b = false;
}
TestClass::~TestClass() {
    return;
}

void cpptest_test00() {
    TestClass *cl = CREATE_CLASS(TestClass);
    cl->test();
    DELETE_CLASS(cl);
    return;
}