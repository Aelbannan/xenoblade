// Decomp of kyoshin/menu/CMenuKizunaTalkList.
// Kizuna talk list screen process (CProcess + IScnRender MI).

#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/menu/CMenuKizunaTalkList.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <revolution/GX.h>
#include "kyoshin/CKizunaTalkListImports.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"
void playUISound(u32);

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

// C-linkage callees defined in kyoshin/CKizunaTalkList.cpp (retail symbols
// are bare globals, so extern "C" like initTalkList below).
// (memberized: hideTalkList -> CKizunaTalkList::hideTalkList; declaration moved into class CKizunaTalkList)
// (memberized: scrollTalkListUp -> CKizunaTalkList::scrollTalkListUp; declaration moved into class CKizunaTalkList)
// (memberized: scrollTalkListDown -> CKizunaTalkList::scrollTalkListDown; declaration moved into class CKizunaTalkList)
// (memberized: pageTalkListUp -> CKizunaTalkList::pageTalkListUp; declaration moved into class CKizunaTalkList)
// (memberized: pageTalkListDown -> CKizunaTalkList::pageTalkListDown; declaration moved into class CKizunaTalkList)
// (memberized: initTalkList -> CKizunaTalkList::initTalkList; declaration moved into class CKizunaTalkList)

// ---------------------------------------------------------------------------
// Retail-unmangled imports. The widget headers (CBgTex.hpp / CTitleAHelp.hpp /
// CKizunaTalkList.hpp) declare these as C++ members, which would emit mangled
// relocs (e.g. BgTex_Release_3D9C__6CBgTexFv); retail keeps the bare names for these
// call sites, so call them through extern "C" declarations with the retail
// signature. int returns keep the caller's `!= 0` as a plain cmpwi (no u8 mask)
// like retail.
// ---------------------------------------------------------------------------
// Shared string pool (.rodata) used by Init.
extern char lbl_eu_8050E970[];

// Global render-gate mode bitfield (.sbss; bit 21), read by Move/cbRenderBefore.
extern u32 lbl_eu_80663E28;

// Retail beginClose symbol (unmangled global): free-call form so the reloc
// is bare (same pattern as CMenuShopBuy.hpp).
extern "C" void beginClose(CTitleAHelp* self);

// (memberized: needsTalkRebuild -> CKizunaTalkList::needsTalkRebuild; declaration moved into class CKizunaTalkList)
extern "C" void BgTex_Release_3D9C(CBgTex* self);
extern "C" void teardown(CTitleAHelp* self);
// (memberized: teardownTalkList -> CKizunaTalkList::teardownTalkList; declaration moved into class CKizunaTalkList)
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);
extern "C" int isInitialized(CTitleAHelp* self);
// (memberized: hasVisibleTalks -> CKizunaTalkList::hasVisibleTalks; declaration moved into class CKizunaTalkList)
extern "C" void func_801C412C(CTitleAHelp* self);
// (memberized: showTalkList -> CKizunaTalkList::showTalkList; declaration moved into class CKizunaTalkList)

// Draw/update entry points (retail keeps the bare names at these call sites).
extern "C" int BgTex_Acquire_3C14(CBgTex* self);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void BgTex_Tick_3D54(CBgTex* self);
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.
extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void updateHelp(CTitleAHelp* self);
// (memberized: drawTalkList -> CKizunaTalkList::drawTalkList; declaration moved into class CKizunaTalkList)
// (memberized: updateTalkList -> CKizunaTalkList::updateTalkList; declaration moved into class CKizunaTalkList)

// Skip-timer helpers used by the cancel handler in Move().
extern "C" int CMainMenu_GetInstancePtr();
extern "C" void ArtsInfo_SetReadyFlag();

// CfGameManager one-arg controller-type query: declared extern "C" int(int)
// in CMenuTutorial.hpp-style headers; do NOT redeclare here with a different
// signature (MWCC: illegal function overloading). The inline header wrapper
// drops the -1 argument, so call the verbatim-mangled symbol directly
// (retail loads r3=-1 even though the no-arg function ignores it).

// CfGameManager one-arg init flag (retail loads r3=1 into the no-arg symbol).
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(u8);

// CW-mangled constructors/destructors referenced by Init() (extern "C" stops
// C++ re-mangling of the retail CW names).
extern "C" void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void*, void*);
extern "C" void __ct__15CKizunaTalkListFi(void*, int);
extern "C" void __dt__15CKizunaTalkListFv(void*, int);

CMenuKizunaTalkList::CMenuKizunaTalkList(CProcess* parent)
    : mParentRef(parent), mBgTex(0), mTitleAHelp(0, 0), mKizunaTalkList(0), mState(0) {}

CMenuKizunaTalkList::~CMenuKizunaTalkList() {}

// Phase handlers referenced by Move() before their definitions below.
void KizunaList_Phase0Show(CMenuKizunaTalkList* self);
void KizunaList_Phase1Ready(CMenuKizunaTalkList* self);
void KizunaList_Phase2Input(CMenuKizunaTalkList* self);
void KizunaList_Phase3Flag(CMenuKizunaTalkList* self);

// Initialise the Kizuna talk list screen. Re-initialises each embedded widget
// via a temporary object + copy (see CMenuTutorial::Init), then registers this
// screen as a render callback on its parent scene.
void CMenuKizunaTalkList::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);

    // --- Re-initialise the background CBgTex via a temporary ---
    mBgTex = CBgTex(0);

    BgTex_Acquire_3C14(&mBgTex);

    // --- Re-initialise the embedded CTitleAHelp ---
    char* name = BdatTouchStringCell(lbl_eu_8050E970, lbl_eu_8050E970 + 0xb, 0x11);
    mTitleAHelp = CTitleAHelp(name, 0);

    mTitleAHelp.CTitleAHelp_load();

    // --- Re-initialise the embedded CKizunaTalkList ---
    // Retail: `mKizunaTalkList = CKizunaTalkList(0);`. Spelled out by hand
    // because CScrollBar is still a non-polymorphic view whose hand vptr
    // member the implicit operator= would copy (retail skips +0xFC).
    u8 tempList[0x1494];
    __ct__15CKizunaTalkListFi(tempList, 0);

    __as__17UnkClass_8045F564FRC17UnkClass_8045F564((u8*)this + 0xbc, tempList + 0x4);
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

    __as__17UnkClass_8045F564FRC17UnkClass_8045F564((u8*)this + 0x100, tempList + 0x48);
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

    (&this->mKizunaTalkList)->initTalkList();

    reinterpret_cast<CScn*>(mParentRef)->addRenderCB(this, 0xd, 0);
}

// Tear the talk-list screen down: remove the render callback from the parent
// scene, release each embedded widget, clear the singleton instance and reset
// the CfGameManager init flag.
void CMenuKizunaTalkList::Term() {
    CDeviceVI::waitForDrawDone();

    reinterpret_cast<CScn*>(mParentRef)->removeRenderCB(this);

    BgTex_Release_3D9C(&mBgTex);
    teardown(&mTitleAHelp);
    (&mKizunaTalkList)->teardownTalkList();

    sInstance = nullptr;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

// Per-frame update of the kizuna talk-list screen. Gate on the task/busy
// flags, read the cancel (B) button (classic vs wiimote bit), then drive the
// 4-state FSM and refresh the bg / help bar / list each frame.
void CMenuKizunaTalkList::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;

    CPad* pad = cf::CfGameManager::getCurrentPad();
    u32 cancel;
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        cancel = (pad->mPressedButtonFlags & 0x800000) != 0;
    } else {
        cancel = (pad->mPressedButtonFlags & 0x400) != 0;
    }

    if (cancel != 0) {
        if (CMainMenu_GetInstancePtr() != 0) {
            ArtsInfo_SetReadyFlag();
            playUISound(6);
        }
        mState = 4;
        mField54 = 1;
    }

    switch (mState) {
    case 0:
        KizunaList_Phase0Show(this);
        break;
    case 1:
        KizunaList_Phase1Ready(this);
        break;
    case 2:
        KizunaList_Phase2Input(this);
        break;
    case 3:
        KizunaList_Phase3Flag(this);
        break;
    }

    BgTex_Tick_3D54(&mBgTex);
    updateHelp(&mTitleAHelp);
    (&mKizunaTalkList)->updateTalkList();
}

// Render the kizuna talk-list screen through a stack DrawInfo: gate on the
// task/busy flags and the scene-active check, then draw bg, talk list and
// title/help bar in that order.
void CMenuKizunaTalkList::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
    (&mKizunaTalkList)->drawTalkList(&drawInfo);
    drawHelp(&mTitleAHelp, &drawInfo);
}

CMenuKizunaTalkList* CMenuKizunaTalkList::sInstance;

// Only one kizuna talk-list screen exists at a time. Returns the stored
// instance (or 0 if one already exists).
CMenuKizunaTalkList* CMenuKizunaTalkList::create(CProcess* self, CProcess* parent) {
    if (sInstance != nullptr) {
        return nullptr;
    }

    CMenuKizunaTalkList* menu = new (CWorkThreadSystem::getWorkMem()) CMenuKizunaTalkList(parent);
    sInstance = menu;
    menu->Regist(self, false);
    return sInstance;
}

extern "C" unsigned long KizunaList_IsPresent(void) { return CMenuKizunaTalkList::sInstance != nullptr; }

// void stub_us_8027490c() {} // empty retail stub; nothing references it
// within this split, so it is not emitted.

// Advance the talk-list screen (retail KizunaList_Phase0Show): once the background,
// title/help bar and list are all ready, begin showing the list, mark state 1
// and play the open sound.
void KizunaList_Phase0Show(CMenuKizunaTalkList* self) {
    if (BgTex_IsLoaded_3E34(&self->mBgTex) != 0 &&
        isInitialized(&self->mTitleAHelp) != 0 &&
        (&self->mKizunaTalkList)->hasVisibleTalks() != 0) {
        func_801C412C(&self->mTitleAHelp);
        (&self->mKizunaTalkList)->showTalkList();
        self->mState = 1;
        playUISound(0x6d);
    }
}

// Advance the talk-list phase (retail KizunaList_Phase1Ready): once the title/help bar
// is idle and the list has entries waiting, mark the main state byte as 2.
void KizunaList_Phase1Ready(CMenuKizunaTalkList* self) {
    if (self->mTitleAHelp.isIdle() != 0 &&
        (&self->mKizunaTalkList)->needsTalkRebuild() != 0) {
        self->mState = 2;
    }
}

// Input handler for the Kizuna talk list. Reads the pad state and dispatches
// to the list's directional/confirm handlers depending on which control the
// player pressed this frame.
void KizunaList_Phase2Input(CMenuKizunaTalkList* self) {
    KizunaPadData* pad = (KizunaPadData*)cf::CfGameManager::getCfPadData();

    bool up;
    bool down;
    bool pageUp;
    bool pageDown;
    int trig;

    // Classic controllers shift the menu-trigger bit to bit 9 of the pressed
    // flags; Wiimote/Nunchuk use bit 26. The four directional buttons
    // (up/down/page-up/page-down) are read from the turbo flags.
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
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
        beginClose(&self->mTitleAHelp);
        (&self->mKizunaTalkList)->hideTalkList();
        self->mState = 3;
    } else if (up) {
        (&self->mKizunaTalkList)->scrollTalkListUp();
    } else if (down) {
        (&self->mKizunaTalkList)->scrollTalkListDown();
    } else if (pageUp) {
        (&self->mKizunaTalkList)->pageTalkListUp();
    } else if (pageDown) {
        (&self->mKizunaTalkList)->pageTalkListDown();
    }
}

// Same idle check as KizunaList_Phase1Ready, but marks the 0x54 phase flag instead of
// the main state byte.
void KizunaList_Phase3Flag(CMenuKizunaTalkList* self) {
    if (self->mTitleAHelp.isIdle() != 0 &&
        (&self->mKizunaTalkList)->needsTalkRebuild() != 0) {
        self->mField54 = 1;
    }
}