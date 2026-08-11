// Auto-scaffolded catalog TU for kyoshin/CTutorialList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CTutorialList.hpp"
#include "kyoshin/CSortMenu.hpp"
#include "kyoshin/code_80135FDC.hpp"

u8 CTutorialList::func_802AD300() { return ((u8*)this)[0x177]; }


void __ct__CTutorialList(){}


// func_802AD308 - initialise the list widget: latch the state byte, clear the
// initialised flag, seed the scrollbar fade colours and sizes, then run the
// two per-frame helpers.
extern "C" void func_802AD308(CTutorialList* self) {
    if (self->mState175 != 0) return;
    self->mState175 = 1;
    self->mInitialized = 0;
    func_802ADC28(self);

    f32 v[3];
    v[0] = lbl_eu_80668DD8;
    v[1] = lbl_eu_80668DDC;
    v[2] = lbl_eu_80668DE0;
    func_801F3670(&self->mScrollBar, v);
    func_801F36BC(&self->mScrollBar, 0xa, self->mField280);
    func_801F3850(&self->mScrollBar, self->mField17A);
    func_802ADCE8(self);
    func_802ADE18(self);
}

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

u8 CTutorialList::func_802AD984() { return func_801D3320(&mSortMenu84[0]); }

// func_802AD98C - advance the list: gate on the sort-menu active/button flags,
// move the cursor, refresh the sort menu, run the per-frame helpers and (when
// requested) play the confirm sound.
extern "C" void func_802AD98C(CTutorialList* self, int arg) {
    if (func_801D3320(&self->mSortMenu84[0]) == 0) return;
    if (func_801D3328(&self->mSortMenu84[0]) == 0) return;
    func_801D216C(&self->mGap2C[0], 1);
    func_801D3408(&self->mSortMenu84[0]);
    func_802ADFA8(self);
    func_802ADE18(self);
    if (arg == 0) {
        func_80138078(6);
    }
}

void func_802ADA0C(){}

void func_802ADAB8(){}

// Animation-finish handlers: the +0x24/+0x28 anim transform reached its end
// frame (bound in .sdata2) -> latch the state byte and run the follow-up.
extern "C" void func_802ADAE8(CTutorialList* self) {
    if (func_80137444(self->mAnim24, lbl_eu_80668DE4) != 0) {
        self->mState175 = 2;
        func_802ADC88(self);
        func_801F367C(&self->mScrollBar);
    }
}

extern "C" void func_802ADB3C(CTutorialList* self) {
    if (func_80137444(self->mAnim28, lbl_eu_80668DE4) != 0) {
        self->mState175 = 3;
        func_802ADE18(self);
        self->mInitialized = 1;
    }
}

extern "C" __declspec(noinline) void func_802ADB90(CTutorialList* self) {}

extern "C" void func_802ADBDC(CTutorialList* self) {
    if (func_80137510(self->mAnim24, lbl_eu_80668DE4) != 0) {
        self->mState175 = 0;
        self->mInitialized = 1;
    }
}

extern "C" __declspec(noinline) void func_802ADC28(CTutorialList* self) {}

extern "C" __declspec(noinline) void func_802ADC88(CTutorialList* self) {}

extern "C" __declspec(noinline) void func_802ADE18(CTutorialList* self) {}

void func_802ADEE4(){}

extern "C" void func_802ADFA8(CTutorialList* self) {
    char* text = func_80136190(&lbl_eu_80510B78[0x8e], &lbl_eu_80510B78[0x97], self->mField17E + 0x25);
    func_80136B4C(self->mLayout20, &lbl_eu_80510B78[0x9c], text, 0);
}

// func_802AE004 - open the tutorial list once the layout resource finished
// loading: flag the visible selection, seed the sub-object at +0x180, copy the
// row/selection ids and run the per-frame helper.
extern "C" void func_802AE004(CTutorialList* self) {
    if (self->mField1C != 0) {
        if (self->mField18 == 0) {
            self->mField176 = 1;
            func_802ACC30(self->mSubObj180, self->mField17C, 0);
            self->mField174 = 1;
            self->mField178 = (s8)self->mField282;
            self->mField17A = self->mField284;
            func_802ADCE8(self);
        }
    }
}

void CTutorialList::OnFileEvent() {}

void func_802AE38C(){}

extern "C" void func_802ACBDC(u8* self) {
    *(unsigned short*)(self + 0x100) = 0;
    *(unsigned short*)(self + 0x102) = 0;
    *(unsigned short*)(self + 0x104) = 0;
}

// func_802AD060 - file loading: request the list file and the common archive
// file, then initialise the scrollbar and the sort-menu sub-object.
extern "C" void func_802AD060(CTutorialList* self) {
    self->mField14 = readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (unsigned long)getHandleMEM2__Q23mtl10MemManagerFv(), &lbl_eu_80510B78[0xf], self, 0, 0);
    func_800A9D90();
    self->mField18 = readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (unsigned long)func_800A9D90(), &lbl_eu_80510B78[0x28], self, 0, 0);
    func_801F34F4(&self->mScrollBar);
    func_801D3064(&self->mSortMenu84[0]);
    self->mField176 = 0;
}

// func_802AD0E0 - per-frame update: dispatch on the anim state byte, animate
// the layout, then move the cursor, scrollbar and sort-menu sub-object.
extern "C" void func_802AD0E0(CTutorialList* self) {
    if (self->mField174 == 0) return;
    switch (self->mState175) {
    case 1:
        func_802ADAE8(self);
        break;
    case 2:
        func_802ADB3C(self);
        break;
    case 4:
        func_802ADB90(self);
        break;
    case 5:
        func_802ADBDC(self);
        break;
    }
    self->mLayout20->Animate(0);
    func_801D202C(&self->mGap2C[0]);
    func_801F3540(&self->mScrollBar);
    func_801D3160(&self->mSortMenu84[0]);
}

extern "C" void func_802AD188() {}
extern "C" void func_802AD1F4() {}

extern "C" __declspec(noinline) void func_802ACC30(u8* self, u16 a, int b) {}
extern "C" void func_802ACE04() {}

u8 CTutorialList::func_802AD2A4() {
    if (CScrollBar_isVisible(&mScrollBar) == 0) return 0;
    if (func_801D32DC(mSortMenu84) == 0) return 0;
    return mField176;
}
