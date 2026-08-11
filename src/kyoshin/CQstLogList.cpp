// Auto-scaffolded catalog TU for kyoshin/CQstLogList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CQstLogList.hpp"
#include "kyoshin/code_80135FDC.hpp"

#include <stdio.h>

void __ct__11CQstLogListFv() {}

u8 CQstLogList::func_80227CCC() { return mSortEnabled; }

// Copies a 0x22-byte quest-log entry: 2 head bytes + 4x8-byte word loop.
// Retail emits an mtctr/bdnz counted loop; under the unit-locked -O4,p this
// shape unrolls no matter the source form (MWCC_REFERENCE walls 6/13) - the
// bottom-test do-while below is the closest surviving loop shape.
void func_80227994(CQstLogListEntry* pDst, const CQstLogListEntry* pSrc) {
    pDst->mField0 = pSrc->mField0;
    pDst->mField1 = pSrc->mField1;
    u32* dstWords = (u32*)pDst->mData;
    const u32* srcWords = (const u32*)pSrc->mData;
    u32 i = 4;
    do {
        dstWords[0] = srcWords[0];
        dstWords[1] = srcWords[1];
        dstWords += 2;
        srcWords += 2;
    } while (--i != 0);
}


u8 CQstLogList::func_80227CD4() { return mSortDescending; }

// Returns the quest-list's own "layout loaded" byte only when the scroll bar
// AND the sort menu are both visible (mirrors CSortMenu::func_801D32DC's
// scrollbar-gate pattern).
u8 func_80227C70(CQstLogList* self) {
    if (CScrollBar_isVisible(&self->mScrollBar) == 0) {
        return 0;
    }
    if (func_801D32DC(&self->mSortMenu) == 0) {
        return 0;
    }
    return self->field_0x178;
}


void func_80227EC8(){}

void func_80227FC0(){}

void func_8022807C(){}

void func_80228164(){}

void func_80228280(){}

void func_802282F8(){}

// retail: lwz r0,lbl_eu_80664730@sda21; clrlwi r3,r0,16 = (u16)global
extern "C" u32 func_80228394() { extern u32 lbl_eu_80664730; return lbl_eu_80664730 & 0xFFFF; }

void func_802283A0(CQstLogList* self) {
    f32 frame = lbl_eu_80668584;
    nw4r::lyt::AnimTransform* anim = self->mpAnim0;
    if (func_80137444(anim, frame) != 0) {
        self->field_0x174 = 2;
        func_80228544(self);
    }
}

void func_802283EC(CQstLogList* self) {
    f32 frame = lbl_eu_80668584;
    nw4r::lyt::AnimTransform* anim = self->mpAnim1;
    if (func_80137444(anim, frame) != 0) {
        self->field_0x174 = 3;
        func_801D216C(&self->pad_0028[0], 1);
        func_80228B10(self);
        self->mSortEnabled = 1;
    }
}

void func_8022844C(CQstLogList* self) {
    if (func_80137510(self->mpAnim1, lbl_eu_80668584) != 0) {
        self->field_0x174 = 5;
        func_802284E4(self);
    }
}

void func_80228498(CQstLogList* self) {
    if (func_80137510(self->mpAnim0, lbl_eu_80668584) != 0) {
        self->field_0x174 = 0;
        self->mSortEnabled = 1;
    }
}

extern "C" __declspec(noinline) void func_802284E4(CQstLogList* self) {
    self->mpLayout->SetAnimationEnable(self->mpAnim1, false);
    self->mpLayout->SetAnimationEnable(self->mpAnim0, true);
}

extern "C" __declspec(noinline) void func_80228544(CQstLogList* self) {
    self->mpLayout->SetAnimationEnable(self->mpAnim0, false);
    self->mpLayout->SetAnimationEnable(self->mpAnim1, true);
}

void func_802285A4(){}

void func_802286F4(){}

void func_802289F8(){}

// Unmatched stub (0xF4 retail bytes); signature widened so func_802283EC can
// pass self - retail func_802283EC does `mr r3, r31` before the bl.
extern "C" void func_80228B10(CQstLogList* self) {}

void func_80228C04(){}

void func_80228C98(CQstLogList* self) {
    func_80136A1C(self->mpLayout, &lbl_eu_80509AB4[0x107],
                  &lbl_eu_80576670[(self->field_0x17B + self->field_0x17C) * 0x22] + 2, 0);
}

extern "C" void OnFileEvent__12CQstLogListFv() {}

// Constructor for the 0x22-byte quest-log entry records; called as the array
// element ctor by sinit_80229378 via __construct_array. Retail symbol is
// unmangled (C linkage) - extern "C" keeps the sinit reloc bound to the
// retail name, and the entry ctor returns self (retail `mr r3, r31`).
extern "C" CQstLogListEntry* func_80229398(CQstLogListEntry* self) {
    self->mField0 = 0xFF;
    self->mField1 = 0x00;
    // "%s" (lbl_eu_80509AB4 + 0x10) with a null pointer - clears the text.
    sprintf(&self->mData[0], &lbl_eu_80509AB4[0x10], 0);
    return self;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void __construct_array(void*, void*, int, int, int);
extern "C" void sinit_80229378() {
    __construct_array(lbl_eu_80576670, (void*)func_80229398, 0, 0x22, 0x20);
}

extern "C" void func_80227A60() {}
extern "C" void func_80227AC4() {}
extern "C" void func_80227B6C() {}
extern "C" void func_80227BD8() {}
extern "C" void func_80227CDC() {}
extern "C" void func_80227D78() {}
extern "C" void func_80227DE8() {}
