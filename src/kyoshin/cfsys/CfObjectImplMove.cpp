// Auto-scaffolded catalog TU for kyoshin/cfsys/CfObjectImplMove
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void func_800CA948() {}

extern "C" void func_800CA964() {}

extern "C" void func_800CAA44() {}

extern "C" void func_800CAB00() {}

extern "C" void func_800CAB2C(void) {}

extern "C" void func_800CAB30() {}

extern "C" void func_800CB21C() {}

extern "C" void func_800CB454() {}

extern "C" void func_800CB94C() {}

extern "C" void func_800CB9AC() {}

extern "C" void func_800CBBD8() {}

extern "C" void func_800CC01C(void) {}

extern "C" void func_800CC020() {}

extern "C" void func_800CC5DC() {}

extern "C" void func_800CC638() {}

extern "C" void func_800CC964() {}

extern "C" void func_800CD268() {}

extern "C" void func_800CD460() {}

extern "C" void func_800BE824(void*, unsigned int);

extern "C" void func_800CD5C0(char* self, unsigned int a, unsigned int b) {
    void* inner = *(void**)(self + 0x14);
    if (a == *(unsigned int*)((char*)inner + 0xc4)) {
        func_800BE824(inner, b);
    }
}

extern "C" void func_800CD5DC() {}

extern "C" void func_800CE544() {}

extern "C" void func_800CE6A0() {}

extern "C" void func_800CE8AC() {}

extern "C" void func_800CE8E4() {}

extern "C" void func_800CEA34() {}

extern "C" void func_800CEB68() {}

extern "C" void func_800CEBE0() {}

extern "C" void func_800CED64() {}

extern "C" void func_800CEE28() {}

extern "C" void func_800CEE7C(void) {}

extern "C" void func_800CEE80() {}

extern "C" void func_800CF064() {}

extern "C" void func_800CF810() {}

extern "C" void func_800CFFA0(unsigned int* self, unsigned int* param) {
    if (param == (unsigned int*)self[7]) {
        param[44] = 0;
        self[7] = 0;
    }
}

extern "C" void __dt__Q22cf16CfObjectImplMoveFv(void* self);
extern "C" void func_800CFFBC(void* self) { ((void(*)(void*))__dt__Q22cf16CfObjectImplMoveFv)((char*)self - 0xc); }

extern "C" void func_800CFFC4(void* self) { ((void(*)(void*))__dt__Q22cf16CfObjectImplMoveFv)((char*)self - 0x10); }
