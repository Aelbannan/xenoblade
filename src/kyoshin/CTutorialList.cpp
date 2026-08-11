// Auto-scaffolded catalog TU for kyoshin/CTutorialList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CTutorialList.hpp"
#include "kyoshin/CSortMenu.hpp"
#include "kyoshin/code_80135FDC.hpp"

// Same-TU helpers (retail unmangled symbols); extern "C" so call sites below
// bind to the unmangled names (func_802ADC88 / func_802ADE18).
extern "C" void func_802ADC88(CTutorialList* self);
extern "C" void func_802ADE18(CTutorialList* self);

u8 CTutorialList::func_802AD300() { return ((u8*)this)[0x177]; }


void __ct__CTutorialList(){}


void func_802AD308(){}

void func_802AD3A0(){}

void func_802AD404(){}

void func_802AD514(){}

void func_802AD638(){}

void func_802AD728(){}

void func_802AD838(){}

#pragma push
#pragma auto_inline off
extern "C" void func_802ADCE8(void* self){}
#pragma pop
extern "C" void func_802AD854(void* self) { func_802ADCE8(self); }

void func_802AD858(){}

u8 func_801D3320(void* self);
u8 CTutorialList::func_802AD984() { return func_801D3320((u8*)this + 0x84); }

void func_802AD98C(){}

void func_802ADA0C(){}

void func_802ADAB8(){}

// Animation-finish handlers: the +0x24/+0x28 anim transform reached its end
// frame (bound in .sdata2) -> latch the state byte and run the follow-up.
void CTutorialList::func_802ADAE8() {
    if (func_80137444(mAnim24, lbl_eu_80668DE4) != 0) {
        mState175 = 2;
        func_802ADC88(this);
        func_801F367C(&mScrollBar);
    }
}

void CTutorialList::func_802ADB3C() {
    if (func_80137444(mAnim28, lbl_eu_80668DE4) != 0) {
        mState175 = 3;
        func_802ADE18(this);
        mInitialized = 1;
    }
}

void func_802ADB90(){}

void CTutorialList::func_802ADBDC() {
    if (func_80137510(mAnim24, lbl_eu_80668DE4) != 0) {
        mState175 = 0;
        mInitialized = 1;
    }
}

void func_802ADC28(){}

extern "C" __declspec(noinline) void func_802ADC88(CTutorialList* self) {}

void func_802ADCE8(){}

extern "C" __declspec(noinline) void func_802ADE18(CTutorialList* self) {}

void func_802ADEE4(){}

void CTutorialList::func_802ADFA8() {
    char* text = func_80136190(&lbl_eu_80510B78[0x8e], &lbl_eu_80510B78[0x97], mField17E + 0x25);
    func_80136B4C(mLayout20, &lbl_eu_80510B78[0x9c], text, 0);
}

void func_802AE004(){}

void CTutorialList::OnFileEvent() {}

void func_802AE38C(){}

extern "C" void func_802ACBDC(u8* self) {
    *(unsigned short*)(self + 0x100) = 0;
    *(unsigned short*)(self + 0x102) = 0;
    *(unsigned short*)(self + 0x104) = 0;
}

extern "C" void func_802ACC30() {}
extern "C" void func_802ACE04() {}
extern "C" void func_802AD060() {}
extern "C" void func_802AD0E0() {}
extern "C" void func_802AD188() {}
extern "C" void func_802AD1F4() {}
u8 CTutorialList::func_802AD2A4() {
    if (CScrollBar_isVisible(&mScrollBar) == 0) return 0;
    if (func_801D32DC(mSortMenu84) == 0) return 0;
    return mField176;
}
