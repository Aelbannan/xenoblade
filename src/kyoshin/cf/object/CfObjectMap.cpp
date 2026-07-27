// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectMap
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectMap.hpp"

void cf::CfObjectMap::func_800B9A70() {}

void cf::CfObjectMap::func_800B9AB4() {}

void cf::CfObjectMap::func_800B9B78() {}

void func_800B9C14(){}

void func_800B9C64__Q22cf11CfObjectMapFv(void* self) {
    *(unsigned short*)((char*)self + 0x8e) = 0;
}

void func_800B9C70() {}

void func_800B9C74(){}

void cf::CfObjectMap::func_800B9E3C(unsigned long v) {
    extern void func_800B9C74(void*, unsigned long, unsigned long);
    func_800B9C74(this, (v >> 5) & 0x7F, (v >> 12) & 0x3FF);
}

void func_800B9E4C(){}

void func_800BA440(){}

void cf::CfObjectMap::func_800BA610() {}

extern "C" void func_800BA650__Q22cf11CfObjectMapFv(void* self, float f) {
    extern void func_80484E5C(void*);
    void* unk = *(void**)((char*)self + 0x98);
    *(float*)((char*)self + 0x2F48) = f;
    if (unk != 0) {
        func_80484E5C(unk);
    }
}

void cf::CfObjectMap::func_800BA66C() {}

extern "C" void CfObjectModel_UnkVirtualFunc19__Q22cf13CfObjectModelFv(void* self, int arg) {
    unsigned int* field = (unsigned int*)((char*)self + 0x68);
    if (arg != 0)
        *field |= 0x200000;
    else
        *field &= ~0x200000;
}

extern "C" int CfObjectModel_UnkVirtualFunc5__Q22cf13CfObjectModelFv(void* self) { return 0; }

extern "C" int CfObjectModel_UnkVirtualFunc4__Q22cf13CfObjectModelFv(void* self) { return 0; }

void cf::CfObjectModel::CfObject_UnkVirtualFunc33() {
    void** vtable = *(void***)this;
    void (*func)(void*) = (void (*)(void*))vtable[0xd0 / 4];
    func(this);
}

void cf::CfObjectModel::CfObject_UnkVirtualFunc30() {
    void** vtable = *(void***)this;
    void (*func)(void*) = (void (*)(void*))vtable[0xc4 / 4];
    func(this);
}

extern "C" void func_800BA764__Q22cf11CfObjectMapFv() {}

void cf::CfObjectMap::~CfObjectMap() {}
