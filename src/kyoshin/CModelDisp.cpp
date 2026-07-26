// Auto-scaffolded catalog TU for kyoshin/CModelDisp
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" u8 func_801FC114(void* self) { return ((u8*)self)[0x2FE4]; }











extern "C" void func_801FC218() {}

extern "C" void func_801FC2B4() {}

extern "C" void func_801FC3B0() {}

extern "C" int func_801FCAC0(void* self) { return 0; }

extern "C" void func_801FCAC8() {}

extern "C" void func_801FCB4C() {}

extern "C" int func_801FCBEC(void* self) { return 0; }

extern "C" void func_801FCBF4() {}

extern "C" void func_801FCDB4() {}

extern "C" void func_801FC11C(void* self) {
    if (*(u8*)((u8*)self + 0x2FD8) != 0) return;
    *(u8*)((u8*)self + 0x2FD8) = 1;
    *(u8*)((u8*)self + 0x2FE4) = 0;
}
