// Auto-scaffolded catalog TU for kyoshin/cf/CfResPcImpl
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfResPcImpl.hpp"

namespace cf {}
using namespace cf;

void __ct__cf_CfResPcImpl(){}

cf::CfResPcImpl::~CfResPcImpl() {}

int func_8018CB14(void* p)
{
    return (unsigned int)*(unsigned short*)((char*)p + 8) >= 3;
}

int func_8018CB34() { return 2; }

void func_8018CB3C(){}

void func_eu_8018E19C(){}

void func_8018D134(){}

void func_8018CBE8(){}

void func_8018CD9C(){}

void func_8018CE70(){}

void func_8018CF08(){}

void func_8018CF90(){}

void func_8018D00C(){}

void func_8018D0C4(){}

extern "C" __declspec(noinline) void func_8018D154(u32 a, u32 b, u32 c){ volatile int _x = 0; (void)_x; (void)a; (void)b; (void)c; }

// retail: clrlwi r5, r4, 22; b func_8018D154 (r5 = arg2 & 0x3FF)
extern "C" void func_8018D288(void* self, u32 a, u32 b){
    func_8018D154((u32)self, a, a & 0x3FF);
}

void func_8018D290(){}

void func_8018D354(){}

void func_8018D3F0(){}

void func_8018D510(){}

void func_8018D570(){}

void func_8018D65C(){}

void func_8018D79C(){}

void func_8018DE8C(){}

void func_8018E69C(){}

void func_8018E7E4(){}

void func_8018EE18(){}

void func_8018EEF0(){}



cf::UnkClass_8018EF3C::~UnkClass_8018EF3C() {}

#include <string.h>

void func_8018EFB4(unsigned char* p)
{
    *reinterpret_cast<unsigned int*>(p + 0x404) = 0;
    *reinterpret_cast<unsigned int*>(p + 0x400) = 0;
    memset(p, 0, 0x400);
}

int func_8018EFCC() { return 1; }

void func_8018EFD4(){}

void func_8018F018(){}

void func_8018F164(){}

void func_8018F1FC(){}

void func_8018F2EC(){}

void func_8018F368(){}

void func_8018F46C(){}

void func_8018F510(void* self, unsigned long cond){
    extern void func_8018F368(void*);
    extern void func_8018F46C(void*);
    if (cond == 0)
        func_8018F46C(self);
    else
        func_8018F368(self);
}

void func_8018F520(){}

void func_8018F5A4(){}

void func_8018F63C(){}
