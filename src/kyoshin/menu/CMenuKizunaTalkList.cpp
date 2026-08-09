// Decomp of kyoshin/menu/CMenuKizunaTalkList.
// Kizuna talk list screen process (CProcess + IScnRender MI).

#include "kyoshin/menu/CMenuKizunaTalkList.hpp"

extern "C" void __dt__19CMenuKizunaTalkListFv(void*, int);
extern "C" void cbRenderBefore__19CMenuKizunaTalkListFv(void*);

#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/scn/CScn.hpp"

// Pad-data view exposing only the flag words this TU reads (CfPadData layout,
// see kyoshin/cf/CfPadData.hpp): CPad::mPressedButtonFlags at +0x4 and
// CfPadData::mTurboPressButtonFlags at +0x104.
struct KizunaPadData {
    u32 mPadHeldFlags;          // 0x00 - CPad::mHeldButtonFlags
    u32 mPadPressedFlags;       // 0x04 - CPad::mPressedButtonFlags
    u8 _pad[0xFC];              // 0x08-0x103 - rest of pad data
    u32 mTurboPressButtonFlags; // 0x104
};

// C++-linkage callees defined in kyoshin/CKizunaTalkList.cpp (retail symbols
// func_802735F0 etc.).
extern void func_802735F0(CKizunaTalkList* self);
extern void func_80273654(CKizunaTalkList* self);
extern void func_80273710(CKizunaTalkList* self);
extern void func_802737E0(CKizunaTalkList* self);
extern void func_8027387C(CKizunaTalkList* self);
extern void func_802732F4(CKizunaTalkList* self);

// CfGameManager one-arg controller-type query, kept as the retail-mangled C
// symbol (extern "C" stops C++ `__Fs` param mangling). The inline header
// wrapper drops the -1 argument, so it cannot be used here (retail loads r3=-1
// even though the no-arg function ignores it). Same quirk as CMenuTutorial.
extern "C" bool func_80086F9C__Q22cf13CfGameManagerFv(s16);

// CfGameManager one-arg init flag (retail loads r3=1 into the no-arg symbol).
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(u8);

// CW-mangled constructors/destructors referenced by Init() (extern "C" stops
// C++ re-mangling of the retail CW names).
extern "C" void __ct__CBgTex(void*, int);
extern "C" void __dt__6CBgTexFv(void*, int);
extern "C" void __ct__UnkClass_8011C974(void*, void*);
extern "C" void __ct__CTitleAHelp(void*, char*, int);
extern "C" void __dt__11CTitleAHelpFv(void*, int);
extern "C" void __ct__15CKizunaTalkListFv(void*, int);
extern "C" void __dt__15CKizunaTalkListFv(void*, int);

void __ct__CMenuKizunaTalkList(){}

CMenuKizunaTalkList::~CMenuKizunaTalkList() {}

// Initialise the Kizuna talk list screen. Re-initialises each embedded widget
// via a temporary object + copy (see CMenuTutorial::Init), then registers this
// screen as a render callback on its parent scene.
void CMenuKizunaTalkList::Init() {
    func_8008294C__Q22cf13CfGameManagerFv(1);

    // --- Re-initialise the background CBgTex via a temporary ---
    u8 tempBgTex[0x20];
    __ct__CBgTex(tempBgTex, 0);

    __ct__UnkClass_8011C974((u8*)this + 0x64, tempBgTex + 0x4);
    *(u32*)((u8*)this + 0x74) = *(u32*)(tempBgTex + 0x14);
    *(u32*)((u8*)this + 0x78) = *(u32*)(tempBgTex + 0x18);
    *(u8*)((u8*)this + 0x7c)  = *(u8*)(tempBgTex + 0x1c);
    *(u8*)((u8*)this + 0x7d)  = *(u8*)(tempBgTex + 0x1d);
    *(u8*)((u8*)this + 0x7e)  = *(u8*)(tempBgTex + 0x1e);
    __dt__6CBgTexFv(tempBgTex, -1);

    this->mBgTex.func_801C3C14();

    // --- Re-initialise the embedded CTitleAHelp ---
    char* name = func_80136190(lbl_eu_8050E970, lbl_eu_8050E970 + 0xb, 0x11);

    u8 tempTitle[0x38];
    __ct__CTitleAHelp(tempTitle, name, 0x0);

    __ct__UnkClass_8011C974((u8*)this + 0x84, tempTitle + 0x4);
    *(u32*)((u8*)this + 0x94) = *(u32*)(tempTitle + 0x14);
    *(u32*)((u8*)this + 0x98) = *(u32*)(tempTitle + 0x18);
    *(u32*)((u8*)this + 0x9c) = *(u32*)(tempTitle + 0x1c);
    *(u32*)((u8*)this + 0xa0) = *(u32*)(tempTitle + 0x20);
    *(u32*)((u8*)this + 0xa4) = *(u32*)(tempTitle + 0x24);
    *(u8*)((u8*)this + 0xa8)  = *(u8*)(tempTitle + 0x28);
    *(u32*)((u8*)this + 0xac) = *(u32*)(tempTitle + 0x2c);
    *(u32*)((u8*)this + 0xb0) = *(u32*)(tempTitle + 0x30);
    *(u8*)((u8*)this + 0xb4)  = *(u8*)(tempTitle + 0x34);
    *(u8*)((u8*)this + 0xb5)  = *(u8*)(tempTitle + 0x35);
    *(u8*)((u8*)this + 0xb6)  = *(u8*)(tempTitle + 0x36);
    *(u8*)((u8*)this + 0xb7)  = *(u8*)(tempTitle + 0x37);
    __dt__11CTitleAHelpFv(tempTitle, -1);

    this->mTitleAHelp.CTitleAHelp_load();

    // --- Re-initialise the embedded CKizunaTalkList ---
    u8 tempList[0x1494];
    __ct__15CKizunaTalkListFv(tempList, 0);

    __ct__UnkClass_8011C974((u8*)this + 0xbc, tempList + 0x4);
    *(u32*)((u8*)this + 0xcc) = *(u32*)(tempList + 0x14);
    *(u32*)((u8*)this + 0xd0) = *(u32*)(tempList + 0x18);
    *(u32*)((u8*)this + 0xd4) = *(u32*)(tempList + 0x1c);
    *(u32*)((u8*)this + 0xd8) = *(u32*)(tempList + 0x20);
    *(u32*)((u8*)this + 0xdc) = *(u32*)(tempList + 0x24);
    *(u32*)((u8*)this + 0xe0) = *(u32*)(tempList + 0x28);
    *(u32*)((u8*)this + 0xe8) = *(u32*)(tempList + 0x30);
    *(u32*)((u8*)this + 0xec) = *(u32*)(tempList + 0x34);
    *(u32*)((u8*)this + 0xf0) = *(u32*)(tempList + 0x38);
    *(u32*)((u8*)this + 0xf4) = *(u32*)(tempList + 0x3c);
    *(u8*)((u8*)this + 0xf8)  = *(u8*)(tempList + 0x40);
    *(u8*)((u8*)this + 0xf9)  = *(u8*)(tempList + 0x41);

    __ct__UnkClass_8011C974((u8*)this + 0x100, tempList + 0x48);
    *(u32*)((u8*)this + 0x110) = *(u32*)(tempList + 0x58);
    *(u32*)((u8*)this + 0x114) = *(u32*)(tempList + 0x5c);
    *(u32*)((u8*)this + 0x118) = *(u32*)(tempList + 0x60);
    *(u32*)((u8*)this + 0x11c) = *(u32*)(tempList + 0x64);
    *(u8*)((u8*)this + 0x120)  = *(u8*)(tempList + 0x68);
    *(u8*)((u8*)this + 0x121)  = *(u8*)(tempList + 0x69);
    *(u8*)((u8*)this + 0x122)  = *(u8*)(tempList + 0x6a);
    *(u8*)((u8*)this + 0x123)  = *(u8*)(tempList + 0x6b);
    *(float*)((u8*)this + 0x124) = *(float*)(tempList + 0x6c);
    *(float*)((u8*)this + 0x128) = *(float*)(tempList + 0x70);
    *(float*)((u8*)this + 0x12c) = *(float*)(tempList + 0x74);
    *(float*)((u8*)this + 0x130) = *(float*)(tempList + 0x78);
    *(float*)((u8*)this + 0x134) = *(float*)(tempList + 0x7c);
    *(u8*)((u8*)this + 0x138)  = *(u8*)(tempList + 0x80);
    *(u8*)((u8*)this + 0x13c)  = *(u8*)(tempList + 0x84);
    *(u8*)((u8*)this + 0x13d)  = *(u8*)(tempList + 0x85);
    *(u8*)((u8*)this + 0x13e)  = *(u8*)(tempList + 0x86);
    *(u8*)((u8*)this + 0x13f)  = *(u8*)(tempList + 0x87);
    *(u8*)((u8*)this + 0x140)  = *(u8*)(tempList + 0x88);
    *(short*)((u8*)this + 0x142) = *(short*)(tempList + 0x8a);

    // Talk-list entry table (256 entries of 0x14 bytes) is copied element by
    // element so the compiler emits the retail 0x14-byte loop. The end bound is
    // the first byte past the table at +0x1544.
    // Talk-list entry table (256 entries of 0x14 bytes) is copied element by
    // element so the compiler emits the retail 0x14-byte loop.
    TalkListEntry* dst = mKizunaTalkList.mEntryArray.mEntries;
    TalkListEntry* src = (TalkListEntry*)(tempList + 0x8c);
    do {
        dst->field_00 = src->field_00;
        dst->field_04 = src->field_04;
        dst->field_08 = src->field_08;
        dst->field_0C = src->field_0C;
        dst->field_10 = src->field_10;
        dst->field_12 = src->field_12;
        dst->field_13 = src->field_13;
        dst++;
        src++;
    } while (dst < mKizunaTalkList.mEntryArray.mEntries + 256);

    // Entry count / parent (at the tail of the entry array).
    *(u8*)((u8*)this + 0x1544)  = *(u8*)(tempList + 0x14ec);
    *(u32*)((u8*)this + 0x1548) = *(u32*)(tempList + 0x14f0);
    __dt__15CKizunaTalkListFv(tempList, -1);

    func_802732F4(&this->mKizunaTalkList);

    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = &mIScnRender;
    }
    reinterpret_cast<CScn*>(mParentRef)->addRenderCB(cb, 0xd, 0);
}

void CMenuKizunaTalkList::Term() {}

void CMenuKizunaTalkList::Move() {}

void CMenuKizunaTalkList::cbRenderBefore() {}

void func_80272414(){}

void stub_us_8027490c() {}

void func_80272498(){}

void func_80272510(){}

// Input handler for the Kizuna talk list. Reads the pad state and dispatches
// to the list's directional/confirm handlers depending on which control the
// player pressed this frame.
void func_80272560(CMenuKizunaTalkList* self) {
    KizunaPadData* pad = (KizunaPadData*)cf::CfGameManager::getCfPadData();

    bool up;
    bool down;
    bool pageUp;
    bool pageDown;
    int trig;

    // Classic controllers shift the menu-trigger bit to bit 9 of the pressed
    // flags; Wiimote/Nunchuk use bit 26. The four directional buttons
    // (up/down/page-up/page-down) are read from the turbo flags.
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        up = (turbo & 0x8004) != 0;
        down = (turbo & 0x10008) != 0;
        pageUp = (turbo & 0x2001) != 0;
        pageDown = (turbo & 0x4002) != 0;
        trig = (pressed >> 22) & 1;
    } else {
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        up = (turbo & 0x8004) != 0;
        down = (turbo & 0x10008) != 0;
        pageUp = (turbo & 0x2001) != 0;
        pageDown = (turbo & 0x4002) != 0;
        trig = (pressed >> 5) & 1;
    }

    if (trig) {
        self->mTitleAHelp.func_801C414C();
        func_802735F0(&self->mKizunaTalkList);
        self->mState = 3;
    } else if (up) {
        func_80273654(&self->mKizunaTalkList);
    } else if (down) {
        func_80273710(&self->mKizunaTalkList);
    } else if (pageUp) {
        func_802737E0(&self->mKizunaTalkList);
    } else if (pageDown) {
        func_8027387C(&self->mKizunaTalkList);
    }
}

void func_80272694(){}

// IScnRender vtable adjustor thunk for cbRenderBefore.
// When IScnRender virtual functions dispatch through IScnRender*,
// 'this' points to the IScnRender subobject at offset +0x58 within
// CMenuKizunaTalkList. This thunk adjusts it back and tail-calls the
// real implementation.
extern "C" void func_802726E4(void* self) {
    ((void(*)(void*))cbRenderBefore__19CMenuKizunaTalkListFv)((char*)self - 0x58);
}

// IScnRender vtable adjustor thunk for ~CMenuKizunaTalkList.
extern "C" void func_802726EC(void* self) {
    ((void(*)(void*))__dt__19CMenuKizunaTalkListFv)((char*)self - 0x58);
}

extern unsigned long lbl_eu_806648B0;
extern "C" unsigned long func_80272488(void) { return lbl_eu_806648B0 != 0; }