#pragma once
#include <types.h>
class TestClass {
public:
    TestClass();
    virtual ~TestClass();
    void foo();
    u8 _pad[0x774];
    u8 field1;
    u8 field2;
};
