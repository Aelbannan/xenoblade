// Auto-scaffolded catalog TU for kyoshin/cfsys/CfObjectImplNpc
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void func_800CA6E4(){}

void func_800CA7B4(){}

void func_800CA850(void) {}

void func_800CA854(){}

extern "C" void* __dt__Q22cf15CfObjectImplNpcFv(void* self, int deleteFlag) {
    if (self != nullptr && deleteFlag > 0) {
        operator delete(self);
    }
    return self;
}

extern "C" void* func_800CA924(void* self) { return (void*)((u8*)self + 0x68); }

extern "C" void func_800CA92C(void) {}

extern "C" void func_800CA930(void) {}

extern "C" void func_800CA934(void) {}

extern "C" void func_800CA938(void* self) { ((void(*)(void*))__dt__Q22cf15CfObjectImplNpcFv)((char*)self - 0xc); }

extern "C" void func_800CA940(void* self) { ((void(*)(void*))__dt__Q22cf15CfObjectImplNpcFv)((char*)self - 0x10); }
