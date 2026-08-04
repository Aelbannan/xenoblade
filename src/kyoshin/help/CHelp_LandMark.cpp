#include "kyoshin/help/CHelp_LandMark.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

extern "C" int func_8009CF8C(void*);
extern "C" void* func_8009D414(void*);
extern "C" void func_8009D514(void*);
extern u32 lbl_eu_80663E24;
extern "C" void __dl__FPv(void*);

// CHelp base constructor with Fv symbol (retail name)
extern "C" void __ct__Q22cf5CHelpFv(cf::CHelp* self, void* owner, u32 param);

// Vtable symbol
extern cf::CHelpVtbl lbl_eu_8053B5F8;

// C-linkage constructor to match retail symbol __ct__cf_CHelp_LandMark
extern "C" cf::CHelp_LandMark* __ct__cf_CHelp_LandMark(cf::CHelp_LandMark* self, void* owner, u32 param, u32 field_10_val) {
    // Call CHelp base constructor; r4/r5 (owner/param) passed through from caller
    __ct__Q22cf5CHelpFv(self, owner, param);

    // Load vtable pointer and zero value early (matching MWCC scheduling)
    cf::CHelpVtbl* vtbl = &lbl_eu_8053B5F8;
    s32 zero = 0;

    // Set up vtable pointers and fields
    self->mVtbl = vtbl;
    self->mSecondBase = (char*)vtbl + 0x1c;
    self->field_10 = field_10_val;
    self->mTimer = zero;

    // Call func_8009D414 on second base subobject, or self if null
    // Retail: default r3 = self (mr), then conditionally override with self+0xc (beq+addi)
    void* subobj = (void*)self;
    if (self != nullptr) {
        subobj = (char*)self + 0xc;
    }
    func_8009D414(subobj);

    return self;
}

// Destructor — extern "C" with explicit deleteFlag parameter matching retail __dt__ symbol
// Uses the same pattern as the constructor (which is 100% matched)
extern "C" void* __dt__Q22cf14CHelp_LandMarkFv(cf::CHelp_LandMark* self, s32 deleteFlag) {
    if (self != nullptr) {
        // Restore vtable before destruction
        cf::CHelpVtbl* vtbl = &lbl_eu_8053B5F8;
        self->mVtbl = vtbl;
        self->mSecondBase = reinterpret_cast<void*>(reinterpret_cast<u32>(vtbl) + 0x1c);

        // Call subobject destructor on second base (same pattern as constructor)
        void* subobj = reinterpret_cast<void*>(self);
        if (self != nullptr) {
            subobj = reinterpret_cast<void*>(reinterpret_cast<u32>(subobj) + 0xc);
        }
        func_8009D514(subobj);

        if (deleteFlag > 0) {
            __dl__FPv(self);
        }
    }

    return self;
}

// func_802B8290 — extern "C" to match the Fv retail symbol (takes params in registers)
// 100% matched
extern "C" void func_802B8290__Q22cf14CHelp_LandMarkFv(cf::CHelp_LandMark* self, u32 param1, u32 param2) {
    // Use bool to trigger MWCC's neg/or/rlwinm idiom for != 0 check
    bool hasResult = func_8009CF8C(self->mOwner) != 0;
    if (hasResult) return;
    if ((s32)(self->field_10 + 0x20c8) != (s32)param1) return;
    if (param2 == 0) return;

    self->mSavedFlags = cf::CfGameManager::getEnabledInputFlags();
    cf::CfGameManager::enablePadFlags(-1, false);
    lbl_eu_80663E24 |= 0x2000;
    self->mTimer = 0x4B;
}

// func_802B8388 — thunk: adjusts this by -0xC and tail-calls func_802B8290
// Retail is 0x8 bytes (subi r3, r3, 12; b func_802B8290); cannot be expressed in C++
extern "C" void func_802B8388__Q22cf14CHelp_LandMarkFv(cf::CHelp_LandMark* self, u32 param1, u32 param2) {
    func_802B8290__Q22cf14CHelp_LandMarkFv((cf::CHelp_LandMark*)((char*)self - 0xc), param1, param2);
}

// func_802B8390 — thunk: adjusts this by -0xC and tail-calls destructor
// Retail is 0x8 bytes (subi r3, r3, 12; b __dt__); cannot be expressed in C++
extern "C" void func_802B8390__Q22cf14CHelp_LandMarkFv(cf::CHelp_LandMark* self) {
    // Call the destructor with adjusted this pointer and delete flag of -1
    __dt__Q22cf14CHelp_LandMarkFv((cf::CHelp_LandMark*)((char*)self - 0xc), -1);
}

namespace cf {

void CHelp_LandMark::func_802B8280() {
    mTimer = 0;
    mSavedFlags = 0;
}

u32 CHelp_LandMark::func_802B8328() {
    if (mTimer <= 0) {
        return 0;
    }
    s32 newVal = mTimer - 1;
    mTimer = newVal;
    if (newVal > 0) {
        return 0;
    }
    CfGameManager::enablePadFlags(mSavedFlags, true);
    lbl_eu_80663E24 &= ~0x2000;
    return 1;
}

} // namespace cf