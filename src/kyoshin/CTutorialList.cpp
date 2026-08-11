// Auto-scaffolded catalog TU for kyoshin/CTutorialList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CTutorialList.hpp"
#include "kyoshin/CSortMenu.hpp"
#include "kyoshin/code_80135FDC.hpp"

#include <stdio.h>

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

// func_802ADA0C - rebuild the list content once the sort menu finished
// loading: gate on the sort-menu active/button flags, latch the page byte,
// seed the sub-object at +0x180, reset the page/selection ids, size the
// scrollbar to the content and play the confirm sound.
extern "C" void func_802ADA0C(CTutorialList* self) {
    if (func_801D3320(&self->mSortMenu84[0]) == 0) return;
    if (func_801D3328(&self->mSortMenu84[0]) == 0) return;
    u8 page = (u8)func_801D3808(&self->mSortMenu84[0]);
    self->mField17E = (s8)page;
    func_802ACC30(self->mSubObj180, 0, page);
    self->mField178 = 0;
    func_801F36BC(&self->mScrollBar, 0xa, self->mField280);
    self->mField17A = 0;
    func_801F3850(&self->mScrollBar, self->mField17A);
    func_802AD98C(self, 1);
    func_802ADCE8(self);
    func_80138078(3);
}

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

// func_802ADE18 - move the cursor onto the current tutorial row: format the
// page number, resolve the root pane, find the page-name pane and the
// scrollbar thumb pane, ask func_80137924 for the position between them and
// feed it to the cursor's Move virtual (vtable +0x10).
extern "C" __declspec(noinline) void func_802ADE18(CTutorialList* self) {
    char name[0x2C];
    nw4r::math::VEC3 pos;
    sprintf(name, &lbl_eu_80510B78[0x82], (int)self->mField178 + 1);
    func_801D216C(&self->mGap2C[0], 1);
    nw4r::lyt::Pane* pagePane =
        self->mLayout20->GetRootPane()->FindPaneByName(name, true);
    nw4r::lyt::Pane* barPane =
        self->mLayout20->GetRootPane()->FindPaneByName(&lbl_eu_80510B78[0x4e], true);
    func_80137924(&pos, pagePane, barPane, self->mLayout20->GetRootPane());
    ((CTutorialCurView*)self->mGap2C)->vf2(&pos);
}

// func_802ADEE4 - rebuild the sort menu entries: reset the menu, push the
// four tutorial-row labels (text ids 0x25-0x28) and select the current page.
extern "C" void func_802ADEE4(CTutorialList* self) {
    if (func_801D3320(&self->mSortMenu84[0]) != 0) return;
    func_801D350C(&self->mSortMenu84[0]);
    func_801D3518(&self->mSortMenu84[0],
                  func_80136190(&lbl_eu_80510B78[0x8e], &lbl_eu_80510B78[0x97], 0x25));
    func_801D3518(&self->mSortMenu84[0],
                  func_80136190(&lbl_eu_80510B78[0x8e], &lbl_eu_80510B78[0x97], 0x26));
    func_801D3518(&self->mSortMenu84[0],
                  func_80136190(&lbl_eu_80510B78[0x8e], &lbl_eu_80510B78[0x97], 0x27));
    func_801D3518(&self->mSortMenu84[0],
                  func_80136190(&lbl_eu_80510B78[0x8e], &lbl_eu_80510B78[0x97], 0x28));
    func_801D353C(&self->mSortMenu84[0], self->mField17E);
}

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

// func_802AE38C - play the tutorial voice: gate on the handle's +0x3F00
// bit-1 flag, the voice-idle virtual (vtable slot 0x2BC), the battle-state
// mapping and a 0xA0 handle-buffer allocation, then play voice 0xDD2 through
// the embedded +0x3E9C sub-object. Always returns 0.
extern "C" int func_802AE38C(CVoiceHandle* self) {
    if ((self->field_0x3F00 & 0x2) == 0) return 0;
    if (((CVoiceHandleVTV*)self)->isActive() != 0) return 0;
    if (func_802A77E8(self) != 5) return 0;
    if (func_802A330C(0xa0, 1) == 0) return 0;
    CCharVoice* voice = (CCharVoice*)self;
    if (self != 0) voice = &self->voice;
    func_802A3D54(voice, 0xdd2, 0xa0);
    return 0;
}

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
// func_802AD1F4 - release the list widget: free the CBdat index, close the
// two file handles, release the bound layout and resource accessor, then run
// the cursor/scrollbar/sort-menu/region teardown helpers.
extern "C" void func_802AD1F4(CTutorialList* self) {
    func_8003AA8C__5CBdatFUl(4);
    func_801390E0((CFileHandle**)&self->mField14);
    func_801390E0((CFileHandle**)&self->mField18);
    self->mField174 = 0;
    if (self->mLayout20 != 0) {
        delete self->mLayout20;
        self->mLayout20 = 0;
    }
    func_80139124((nw4r::lyt::ArcResourceAccessor*)self->mField1C);
    lbl_eu_80664BF0 = 0;
    ((CTutorialCurView*)self->mGap2C)->vf1();
    func_801F35DC(&self->mScrollBar);
    func_801D3258(&self->mSortMenu84[0]);
    func_8045F778__17UnkClass_8045F564Fv(&self->mGap04[0]);
}

extern "C" __declspec(noinline) void func_802ACC30(u8* self, u16 a, int b) {}
extern "C" void func_802ACE04() {}

u8 CTutorialList::func_802AD2A4() {
    if (CScrollBar_isVisible(&mScrollBar) == 0) return 0;
    if (func_801D32DC(mSortMenu84) == 0) return 0;
    return mField176;
}
