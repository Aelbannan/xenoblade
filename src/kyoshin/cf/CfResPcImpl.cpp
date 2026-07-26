// Auto-scaffolded catalog TU for kyoshin/cf/CfResPcImpl
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__cf_CfResPcImpl() {}

extern "C" void __dt__Q22cf11CfResPcImplFv() {}

extern "C" int func_8018CB14(void* p)
{
    return (unsigned int)*(unsigned short*)((char*)p + 8) >= 3;
}

extern "C" int func_8018CB34() { return 2; }

extern "C" void func_8018CB3C() {}

extern "C" void func_eu_8018E19C() {}

extern "C" void func_8018D134() {}

extern "C" void func_8018CBE8() {}

extern "C" void func_8018CD9C() {}

extern "C" void func_8018CE70() {}

extern "C" void func_8018CF08() {}

extern "C" void func_8018CF90() {}

extern "C" void func_8018D00C() {}

extern "C" void func_8018D0C4() {}

extern "C" void func_8018D154(u32 a, u32 b) { volatile int _x = 0; (void)_x; (void)a; (void)b; }

extern "C" void func_8018D288(void) {
    func_8018D154(0, 0);
}

extern "C" void func_8018D290() {}

extern "C" void func_8018D354() {}

extern "C" void func_8018D3F0() {}

extern "C" void func_8018D510() {}

extern "C" void func_8018D570() {}

extern "C" void func_8018D65C() {}

extern "C" void func_8018D79C() {}

extern "C" void func_8018DE8C() {}

extern "C" void func_8018E69C() {}

extern "C" void func_8018E7E4() {}

extern "C" void func_8018EE18() {}

extern "C" void func_8018EEF0() {}

extern "C" void __ct__Q22cf17UnkClass_8018EF3CFv(void* self) {
    *(unsigned long*)((char*)self + 0x400) = 0;
    *(unsigned long*)((char*)self + 0x404) = 0;
}

extern "C" void __dt__Q22cf17UnkClass_8018EF3CFv() {}

#include <string.h>

extern "C" void func_8018EFB4(unsigned char* p)
{
    *reinterpret_cast<unsigned int*>(p + 0x404) = 0;
    *reinterpret_cast<unsigned int*>(p + 0x400) = 0;
    memset(p, 0, 0x400);
}

extern "C" int func_8018EFCC() { return 1; }

extern "C" void func_8018EFD4() {}

extern "C" void func_8018F018() {}

extern "C" void func_8018F164() {}

extern "C" void func_8018F1FC() {}

extern "C" void func_8018F2EC() {}

extern "C" void func_8018F368() {}

extern "C" void func_8018F46C() {}

extern "C" void func_8018F510(void* self, unsigned long cond) {
    extern void func_8018F368(void*);
    extern void func_8018F46C(void*);
    if (cond == 0)
        func_8018F46C(self);
    else
        func_8018F368(self);
}

extern "C" void func_8018F520() {}

extern "C" void func_8018F5A4() {}

extern "C" void func_8018F63C() {}
