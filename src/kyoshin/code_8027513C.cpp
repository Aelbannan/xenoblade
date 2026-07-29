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

// If field_0x74 is set, clear its 0xB0 field, mark its mFlags68 with 0x40, and detach
void func_80275824(UnkCode8027513C* self) {
    CfObjectEff* eff = self->field_0x74;
    if (eff == nullptr) return;
    reinterpret_cast<CfObjectEffLayout*>(eff)->field_0xB0 = 0;
    reinterpret_cast<CfObjectEffLayout*>(eff)->mFlags68 |= 0x40;
    self->field_0x74 = nullptr;
}

void func_80275850(){}

// If eff matches the currently attached effect object, clear both sides and tail-call func_800CFFA0
void func_8027594C(UnkCode8027513C* self, CfObjectEff* eff) {
    if (self->field_0x74 == eff) {
        reinterpret_cast<CfObjectEffLayout*>(eff)->field_0xB0 = 0;
        self->field_0x74 = nullptr;
    }
    func_800CFFA0(self);
}

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

ICamControlBase::~ICamControlBase() {}

cf::CfObjectImplTbox::~CfObjectImplTbox() {}


void func_802759B0(void* self) { ((cf::CfObjectImplTbox*)((u8*)self - 0xc))->~CfObjectImplTbox(); }

void func_802759B8(void* self) { func_8027594C((UnkCode8027513C*)((u8*)self - 0x10), nullptr); }

// Initialize camera control instances and call their setup method with self
void func_802751F8(UnkCode8027513C* self) {
    ICamControlBase* cam = initCamControlInstances();
    cam->vfunc_10(self);
}

void func_802759C0(void* self) { ((cf::CfObjectImplTbox*)((u8*)self - 0x10))->~CfObjectImplTbox(); }
