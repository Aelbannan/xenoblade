#pragma once

#include <types.h>
#include "kyoshin/cf/CActParamAnimGame.hpp"

// Sub-object within CModelDisp, stride 0xFF0. Its implicit ctor/dtor are
// compiler-emitted (weak __ct__/__dt__13CModelDispSubFv, emitted right after
// the CModelDisp ctor that first needs them).
// name-guess: CModelDispSub
struct CModelDispSub {
    void* mpController;                  // 0x00 - pointer to controller (vcalled)
    void* field_04;                      // 0x04
    void* field_08;                      // 0x08
    cf::CActParamAnimGame mObj0C;        // 0x0C - embedded act-param object (ends 0x548)
    s32 mTimer548;                       // 0x548 - signed countdown; reset to 150
    s32 field_54C;                       // 0x54C
    cf::CActParamAnimGame mActParams[2]; // 0x550 - two act-param objects (ends 0xFC8)
    void* mSlotPtrA;                     // 0xFC8
    void* mSlotPtrB;                     // 0xFCC
    u32 mFlagFD0;                        // 0xFD0
    u32 mFlagFD4;                        // 0xFD4
    u8 _FD8[0x18];                       // 0xFD8 - padding to 0xFF0
};

// CModelDisp is not polymorphic in retail: the vtable pointer at +0x00 is a
// plain data member (lbl_eu_80535390) written by the constructor - the retail
// dtor performs no vtable reset, so no compiler-generated vtable is wanted.
class CModelDisp {
public:
    CModelDisp(u8* initParam);
    ~CModelDisp();

    u32* mVtbl;                  // 0x00 - vtable pointer (lbl_eu_80535390)
    void* mInitParam;            // 0x04 - init parameter
    CModelDispSub mSubs[3];      // 0x08 - three sub-objects
    u8 field_2FD8;               // 0x2FD8
    u8 _pad2FD9[3];              // 0x2FD9
    f32 field_2FDC;              // 0x2FDC - display alpha/transition value
    f32 field_2FE0;              // 0x2FE0 - accumulator timer
    u8 field_2FE4;               // 0x2FE4
};

// --- retail data symbols (imports; global scope keeps names unmangled) ---
extern u32 lbl_eu_80535390[];     // CModelDisp vtable (.data)
extern const f32 lbl_eu_806681E8;  // .sdata2 float -> field_2FDC target
extern f32 lbl_eu_806681EC;       // .sdata2 float -> field_2FE0
extern const f32 lbl_eu_806681F0; // .sdata2 float (ModelDispInitPoseSlots vec arg)
extern const f32 lbl_eu_806681F4; // .sdata2 float (ModelDispInitPoseSlots vec arg)
extern const f32 lbl_eu_806681F8; // .sdata2 float -> field_2FDC increment

// (runtime imports and act-param view types come from
// kyoshin/menu/parts/CModelDispEquip.hpp, pulled in via harness_catalog.hpp)

// Filter-word tables copied to the stack by ModelDispScanRebind (.rodata).
extern u32 lbl_eu_80507CDC[];
extern u32 lbl_eu_80507CD0[];

// Overlay view of CActParamHolder words the shared view does not expose:
// the owner back-pointers at +0x10/+0x40 and the act-param flag word at
// +0x18 (bit 23 cleared, bits 0x160 set by ModelDispUpdateSlots).
struct CDispHolderWordView {
    u8 _00[0x10];
    void* owner10; // +0x10 back-pointer to the owning CModelDisp
    u8 _14[0x18 - 0x14];
    u32 flags18;   // +0x18 act-param flag word
    u8 _1C[0x40 - 0x1C];
    void* owner40; // +0x40 back-pointer to the owning CModelDisp
};

// (was CDispVt40 here: fake +0x40 dispatch pad on the equipment param
// objects. Retail slot +0x40 is CResLookup::isInUse (idx14); call sites
// now use the owning class directly.)

// C++-linkage helpers owned by kyoshin/CModelDisp.
int ModelDispGetDoneFlag(void* self);
int simVtableFalse3(void* self);
int simVtableFalse4(void* self);
