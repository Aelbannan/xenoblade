// Decomp of kyoshin/menu/CMenuKizunaTalkList.
// Kizuna talk list screen process (CProcess + IScnRender MI).

#include "kyoshin/menu/CMenuKizunaTalkList.hpp"

extern "C" void __dt__19CMenuKizunaTalkListFv(void*, int);
extern "C" void cbRenderBefore__19CMenuKizunaTalkListFv(void*);

#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <revolution/GX.h>

// CfGameManager.hpp cannot be included here: it declares lbl_eu_80664090 as
// void* while CKizunaTalkList.hpp (via our header) declares it u8*. The pad
// accessors are declared member-for-member on cf::CfGameManager in our header
// instead.

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
extern "C" void func_802732F4(CKizunaTalkList* self);

// ---------------------------------------------------------------------------
// Retail-unmangled imports. The widget headers (CBgTex.hpp / CTitleAHelp.hpp /
// CKizunaTalkList.hpp) declare these as C++ members, which would emit mangled
// relocs (e.g. func_801C3D9C__6CBgTexFv); retail keeps the bare names for these
// call sites, so call them through extern "C" declarations with the retail
// signature. int returns keep the caller's `!= 0` as a plain cmpwi (no u8 mask)
// like retail.
// ---------------------------------------------------------------------------
extern "C" int isIdle__11CTitleAHelpFv(CTitleAHelp* h);
extern "C" int func_8027355C(CKizunaTalkList* self);
extern "C" void func_801C3D9C(CBgTex* self);
extern "C" void func_801C40A0(CTitleAHelp* self);
extern "C" void func_8027346C(CKizunaTalkList* self);
extern "C" int func_801C3E34(CBgTex* self);
extern "C" int func_801C4114(CTitleAHelp* self);
extern "C" int func_80273518(CKizunaTalkList* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void func_80273564(CKizunaTalkList* self);

// Draw/update entry points (retail keeps the bare names at these call sites).
extern "C" int func_801C3C14(CBgTex* self);
extern "C" void func_801C3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801C3D54(CBgTex* self);
extern "C" void CTitleAHelp_load(CTitleAHelp* self);
extern "C" void func_801C4080(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801C3FF0(CTitleAHelp* self);
extern "C" void func_8027340C(CKizunaTalkList* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_8027336C(CKizunaTalkList* self);

// Skip-timer helpers used by the cancel handler in Move().
extern "C" int func_800FEDF8();
extern "C" void func_800FF914();

// nw4r DrawInfo C-ABI ct/dt (retail emits the direct calls; a C++ local would
// virtual-dispatch its scope-exit destructor and bloat the body).
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(void* self);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(void* self, int dealloc);

// Singleton instance pointer (.sbss; cleared by Term, read/written by the
// factory func_80272414 and the idle check func_80272488).
extern unsigned long lbl_eu_806648B0;

// CfGameManager one-arg controller-type query: declared extern "C" int(int)
// in CMenuTutorial.hpp-style headers; do NOT redeclare here with a different
// signature (MWCC: illegal function overloading). The inline header wrapper
// drops the -1 argument, so call the verbatim-mangled symbol directly
// (retail loads r3=-1 even though the no-arg function ignores it).

// CfGameManager one-arg init flag (retail loads r3=1 into the no-arg symbol).
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(u8);

// CW-mangled constructors/destructors referenced by Init() (extern "C" stops
// C++ re-mangling of the retail CW names).
extern "C" void __ct__CBgTex(void*, int);
extern "C" void __dt__6CBgTexFv(void*, int);
extern "C" void __ct__UnkClass_8011C974(void*, void*);
extern "C" void __ct__CTitleAHelp(void*, char*, int);
extern "C" void __dt__11CTitleAHelpFv(void*, int);
extern "C" void __ct__CKizunaTalkList(void*, int);
extern "C" void __dt__15CKizunaTalkListFv(void*, int);
extern "C" void __ct__8CProcessFv(CProcess* self);

// Retail constructor symbol (unmangled global, takes the parent). Written as
// an out-of-line free function so the factory (func_80272414) emits a real bl
// to the bare retail symbol; returns `this` in r3 like retail.
// The body mirrors the CSysWinSave ctor pattern: base ctor, temp vtable
// store, null PMF data copy, then the composite vtable + IScnRender
// sub-vtable, then each embedded widget's ctor and the final state byte.
extern "C" __declspec(noinline) CMenuKizunaTalkList* __ct__CMenuKizunaTalkList(
    CMenuKizunaTalkList* self, CProcess* parent) {
    __ct__8CProcessFv((CProcess*)self);

    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8052BF70;

    // Copy the null member-function pointer into both callback slots.
    // Retail loads [0],[1], stores [1],[0], then [2] per slot.
    u32* ptmf = (u32*)(uintptr_t)__ptmf_null;
    u32 w0 = ptmf[0];
    u32 w1 = ptmf[1];
    self->ptmf0[1] = w1;
    self->ptmf0[0] = w0;
    u32 w2 = ptmf[2];
    self->ptmf0[2] = w2;
    w0 = ptmf[0];
    w1 = ptmf[1];
    self->ptmf1[1] = w1;
    self->ptmf1[0] = w0;
    w2 = ptmf[2];
    self->ptmf1[2] = w2;
    self->mUnknown54 = 0;
    self->mUnknown55 = 0;

    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_80537CB8;
    *(u32*)((u8*)self + 0x58) = (u32)lbl_eu_80537CB8 + 0x24;
    self->mParentRef = parent;

    __ct__CBgTex(&self->mBgTex, 0);
    __ct__CTitleAHelp(&self->mTitleAHelp, 0, 0);
    __ct__CKizunaTalkList(&self->mKizunaTalkList, 0);
    self->mState = 0;
    return self;
}

// Phase handlers referenced by Move() before their definitions below.
void func_80272498(CMenuKizunaTalkList* self);
void func_80272510(CMenuKizunaTalkList* self);
void func_80272560(CMenuKizunaTalkList* self);
void func_80272694(CMenuKizunaTalkList* self);

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

    func_801C3C14(&mBgTex);

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

    CTitleAHelp_load(&mTitleAHelp);

    // --- Re-initialise the embedded CKizunaTalkList ---
    u8 tempList[0x1494];
    __ct__CKizunaTalkList(tempList, 0);

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

    // Entry count / parent (at the tail of the entry array: table ends at
    // tempList+0x148C = 0x8C + 256*0x14).
    *(u8*)((u8*)this + 0x1544)  = *(u8*)(tempList + 0x148c);
    *(u32*)((u8*)this + 0x1548) = *(u32*)(tempList + 0x1490);
    __dt__15CKizunaTalkListFv(tempList, -1);

    func_802732F4(&this->mKizunaTalkList);

    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = &mIScnRender;
    }
    reinterpret_cast<CScn*>(mParentRef)->addRenderCB(cb, 0xd, 0);
}

// Tear the talk-list screen down: remove the render callback from the parent
// scene, release each embedded widget, clear the singleton instance and reset
// the CfGameManager init flag.
void CMenuKizunaTalkList::Term() {
    CDeviceVI::waitForDrawDone();

    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = &mIScnRender;
    }
    reinterpret_cast<CScn*>(mParentRef)->removeRenderCB(renderCB);

    func_801C3D9C(&mBgTex);
    func_801C40A0(&mTitleAHelp);
    func_8027346C(&mKizunaTalkList);

    lbl_eu_806648B0 = 0;
    func_8008294C__Q22cf13CfGameManagerFv(0);
}

// Per-frame update of the kizuna talk-list screen. Gate on the task/busy
// flags, read the cancel (B) button (classic vs wiimote bit), then drive the
// 4-state FSM and refresh the bg / help bar / list each frame.
void CMenuKizunaTalkList::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;

    CPad* pad = cf::CfGameManager::getCurrentPad();
    u32 cancel;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        cancel = (pad->mPressedButtonFlags & 0x800000) != 0;
    } else {
        cancel = (pad->mPressedButtonFlags & 0x400) != 0;
    }

    if (cancel != 0) {
        if (func_800FEDF8() != 0) {
            func_800FF914();
            func_80138078__FUl(6);
        }
        mState = 4;
        mUnknown54 = 1;
    }

    switch (mState) {
    case 0:
        func_80272498(this);
        break;
    case 1:
        func_80272510(this);
        break;
    case 2:
        func_80272560(this);
        break;
    case 3:
        func_80272694(this);
        break;
    }

    func_801C3D54(&mBgTex);
    func_801C3FF0(&mTitleAHelp);
    func_8027336C(&mKizunaTalkList);
}

// Render the kizuna talk-list screen through a stack DrawInfo: gate on the
// task/busy flags and the scene-active check, then draw bg, talk list and
// title/help bar in that order.
void CMenuKizunaTalkList::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (func_8013BE50() == 0) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via C-ABI pre-mangled ct/dt calls
    // (a C++ local would virtual-dispatch its scope-exit destructor).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801C3D7C(&mBgTex, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_8027340C(&mKizunaTalkList, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801C4080(&mTitleAHelp, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    __dt__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0], -1);
}

// Factory: lazily allocate + construct the single kizuna talk-list instance
// and register it under `self`. Returns the stored instance (or 0 if one
// already exists).
CMenuKizunaTalkList* func_80272414(CProcess* self, CProcess* parent) {
    if (lbl_eu_806648B0 != 0) {
        return 0;
    }
    CMenuKizunaTalkList* obj = (CMenuKizunaTalkList*)mtl::MemManager::allocate(
        0x1550, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        obj = __ct__CMenuKizunaTalkList(obj, parent);
    }
    lbl_eu_806648B0 = (unsigned long)obj;
    obj->Regist(self, 0);
    return (CMenuKizunaTalkList*)lbl_eu_806648B0;
}

// void stub_us_8027490c() {} // empty retail stub; nothing references it
// within this split, so it is not emitted.

// Advance the talk-list screen (retail func_80272498): once the background,
// title/help bar and list are all ready, begin showing the list, mark state 1
// and play the open sound.
void func_80272498(CMenuKizunaTalkList* self) {
    if (func_801C3E34(&self->mBgTex) != 0 &&
        func_801C4114(&self->mTitleAHelp) != 0 &&
        func_80273518(&self->mKizunaTalkList) != 0) {
        func_801C412C(&self->mTitleAHelp);
        func_80273564(&self->mKizunaTalkList);
        self->mState = 1;
        func_80138078__FUl(0x6d);
    }
}

// Advance the talk-list phase (retail func_80272510): once the title/help bar
// is idle and the list has entries waiting, mark the main state byte as 2.
void func_80272510(CMenuKizunaTalkList* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_8027355C(&self->mKizunaTalkList) != 0) {
        self->mState = 2;
    }
}

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

// Same idle check as func_80272510, but marks the 0x54 phase flag instead of
// the main state byte.
void func_80272694(CMenuKizunaTalkList* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_8027355C(&self->mKizunaTalkList) != 0) {
        self->mUnknown54 = 1;
    }
}

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

extern "C" unsigned long func_80272488(void) { return lbl_eu_806648B0 != 0; }