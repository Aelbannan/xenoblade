// Auto-scaffolded catalog TU for kyoshin/code_8027513C
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/code_8027513C.hpp"

// Forward declaration from kyoshin/cf/object/CfObjectEff.cpp
// Declared as s8 (not u8) to match retail extsb codegen at call sites
void func_800ACC14(void* self, s8 val);

int func_802759A8(void* self) { return 0; }










void func_80275454(){}

void func_802756F0(){}

void func_80275808(UnkCode8027513C* ptr) {
    ptr->field_0x20 = -1;
    ptr->field_0x1C = 0;
    ptr->field_0x74 = nullptr;
    ptr->field_0x78 = 0;
}

void func_80275824(){}

void func_80275850(){}

void func_8027594C(void* self){}

// If field_0x74 is set and field_0x78 differs from arg, notify the child
// effect via func_800ACC14 and cache the new value
void func_802753F8(UnkCode8027513C* self, s16 arg) {
    if (self->field_0x74 != nullptr) {
        if (self->field_0x78 != arg) {
            func_800ACC14(self->field_0x74, (s8)arg);
            self->field_0x78 = arg;
        }
    }
}

cf::CfObjectImplTbox::~CfObjectImplTbox() {}


void func_802759B0(void* self) { ((cf::CfObjectImplTbox*)((u8*)self - 0xc))->~CfObjectImplTbox(); }

void func_802759B8(void* self) { func_8027594C((u8*)self - 0x10); }

void func_802759C0(void* self) { ((cf::CfObjectImplTbox*)((u8*)self - 0x10))->~CfObjectImplTbox(); }
