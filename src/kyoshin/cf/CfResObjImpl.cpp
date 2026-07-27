// Auto-scaffolded catalog TU for kyoshin/cf/CfResObjImpl
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void __ct__cf_CfResObjImpl(){}

void func_8016C860(){}

extern "C" int func_8016C880(void* self) { return 128; }

void func_8016C888(){}

void func_8016C950(){}

void func_8016C98C(){}

void func_8016CCBC(){}

extern "C" void func_8016CCDC(void) {}

void func_8016CCE0(){}

extern "C" int func_8016CD54(void* self) { return 0; }

extern "C" int func_8016CD5C(void* self) { return 0; }

extern "C" void func_8016CD64(void) {}

extern "C" void func_8016CD68(void* self, int index, int value) {
    if (index < 2) {
        int* arr = (int*)((char*)self + 0x14);
        arr[index] = value;
    }
}

void cf::CfResObjImpl::~CfResObjImpl() {}
