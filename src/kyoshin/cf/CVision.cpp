// Auto-scaffolded catalog TU for kyoshin/cf/CVision
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include <PowerPC_EABI_Support/Runtime/MWCPlusLib.h>
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CVision.hpp"
#include "kyoshin/cf/CBattleManager.hpp"

using namespace cf;

// --- Callees defined in other TUs as plain-C functions (no C++ header). ----
extern "C" void func_800ACC50(void* self, float v);
extern "C" void func_804E36DC(CSchedule* self, f32 dt);
extern "C" void* func_800451D8(u32 cls, int param);
extern "C" void __dl__FPv(void* ptr);
extern "C" void __destroy_arr(void* block, ConstructorDestructor* dtor, size_t size, size_t n);

// Fragment-anchored destructors for the UnkClass_801A36D0 / UnkClass_801A3728
// sub-objects are defined below and passed by address to __destroy_arr.

// Class-spec table (8-byte stride, only low word used) for func_801A808C.
struct CVisionClassTableEntry { u32 field_00; u32 field_04; };
extern "C" CVisionClassTableEntry lbl_eu_80503F60[4];

// sdata2 float constants used by func_801A929C.
extern "C" f32 lbl_eu_80667CD4;
extern "C" f32 lbl_eu_80667CF0;

cf::CVision::CVision() {}

void __ct__801A33AC(){}

void func_801A380C(){}

void func_801A39D8(){}

void func_801A4194(){}

void func_801A4578(){}

void func_801A47D0(){}

void func_801A4BC8(){}

void func_801A4CF8(){}

void func_801A506C(){}

void func_801A5260(){}

void func_801A5360(){}

void func_801A5444(){}

void func_801A5BA8(){}

void func_801A5E58(){}

void func_801A60B0(){}

void func_801A6340(){}

// Sub-object VFX management function (retail func_801A897C), stubbed here.
void func_801A897C(CVision* self, int a, int b) {}

void func_801A6540(){}

void func_801A6A7C(){}

void func_801A6BCC(){}

void func_801A70DC(){}

void func_801A74DC(){}

void func_801A7704(){}

void func_801A7D6C(){}

int* func_801A8070(int* param) {
    if (param[1] == 0) {
        return 0;
    }
    return &param[1];
}

// ---------------------------------------------------------------------------
// us-801a986c: Clear all four vision effect slots (retail func_801A8138).
// ---------------------------------------------------------------------------
void func_801A8138(CVision* self) {
    for (u8 i = 0; i < 4; i++) {
        if (self->effectArray[i] != 0) {
            self->effectArray[i]->field_B0 = 0;
            self->effectArray[i]->field_68 |= 0x40;
        }
        self->effectArray[i] = 0;
    }
}

void func_801A8244(){}

// ---------------------------------------------------------------------------
// us-801a4df0: UnkClass_801A36D0 deleting destructor (__dt__801A36D0).
// Clears the sub-object at +0x3d8c (fields at +0x3f90/+0x3f94), then frees.
// ---------------------------------------------------------------------------
extern "C" void* __dt__801A36D0(UnkClass_801A36D0* self, int deleting) {
    if (self != 0) {
        if ((char*)self + 0x3d8c != 0) {
            *(u32*)((char*)self + 0x3f94) = 0;
            *(u32*)((char*)self + 0x3f90) = 0;
        }
        if (deleting > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// ---------------------------------------------------------------------------
// us-801a4e48: UnkClass_801A3728 deleting destructor (__dt__801A3728).
// Clears field_68/field_64, then frees when the deleting flag is set.
// ---------------------------------------------------------------------------
extern "C" void* __dt__801A3728(UnkClass_801A3728* self, int deleting) {
    if (self != 0) {
        self->field_68 = 0;
        self->field_64 = 0;
        if (deleting > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

cf::CVision::~CVision() {}

// ---------------------------------------------------------------------------
// us-801a7c18: Release the zero/zero slot of the battle manager's vision
// sub-object (retail func_801A64F8).
// ---------------------------------------------------------------------------
void func_801A64F8() {
    if (CBattleManager::getInstance() &&
        &CBattleManager::getInstance()->mVision) {
        func_801A897C(&CBattleManager::getInstance()->mVision, 0, 0);
    }
}

// ---------------------------------------------------------------------------
// us-801aa050: Release an arbitrary slot of the battle manager's vision
// sub-object (retail func_801A891C). Args are forwarded untouched.
// ---------------------------------------------------------------------------
void func_801A891C(int a, int b) {
    if (CBattleManager::getInstance() &&
        &CBattleManager::getInstance()->mVision) {
        func_801A897C(&CBattleManager::getInstance()->mVision, a, b);
    }
}

// ---------------------------------------------------------------------------
// us-801a97c0: Lazily create the vision effect at `index` and record the
// owning CVision back-pointer (retail func_801A808C).
// ---------------------------------------------------------------------------
void func_801A808C(CVision* self, int index) {
    if (self->effectArray[index] == 0) {
        CVisionEffect* eff =
            (CVisionEffect*)func_800451D8(lbl_eu_80503F60[index].field_00, 0);
        self->effectArray[index] = eff;
        if (eff != 0) {
            eff->field_B0 = (u32)self;
        }
    }
}

// ---------------------------------------------------------------------------
// us-801a9830: Clear the vision effect at `index` (retail func_801A80FC).
// ---------------------------------------------------------------------------
void func_801A80FC(CVision* self, int index) {
    if (self->effectArray[index] != 0) {
        self->effectArray[index]->field_B0 = 0;
        self->effectArray[index]->field_68 |= 0x40;
    }
    self->effectArray[index] = 0;
}

// ---------------------------------------------------------------------------
// us-801a9930: Null out the first effect whose object identity matches
// `value` (retail func_801A81FC).
// ---------------------------------------------------------------------------
void func_801A81FC(CVision* self, u32 value) {
    for (int i = 0; i < 4; i++) {
        if (self->effectArray[i] == (CVisionEffect*)value) {
            self->effectArray[i] = 0;
            return;
        }
    }
}

// ---------------------------------------------------------------------------
// us-801aa980: Like func_801A81FC, but also clears the effect's owning
// back-pointer before nulling the slot (retail func_801A924C).
// ---------------------------------------------------------------------------
void func_801A924C(CVision* self, u32 value) {
    for (int i = 0; i < 4; i++) {
        if (self->effectArray[i] == (CVisionEffect*)value) {
            CVisionEffect* e = self->effectArray[i];
            e->field_B0 = 0;
            self->effectArray[i] = 0;
            return;
        }
    }
}

void cf::CVision::func_801A929C(u32 r4) {
    f32 scl = (r4 != 0) ? lbl_eu_80667CD4 : lbl_eu_80667CF0;
    for (int i = 0; i < 4; i++) {
        if (effectArray[i] != 0) {
            func_800ACC50(effectArray[i], scl);
            if (effectArray[i]->mSchedule != 0) {
                func_804E36DC(effectArray[i]->mSchedule, lbl_eu_80667CF0);
            }
        }
    }
}