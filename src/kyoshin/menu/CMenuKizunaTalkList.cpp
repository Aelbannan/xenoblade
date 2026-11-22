// Decomp of kyoshin/menu/CMenuKizunaTalkList.
// Kizuna talk list screen process (CProcess + IScnRender MI).

#include "kyoshin/cf/CfBdat.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/menu/CMenuKizunaTalkList.hpp"
#include "kyoshin/CMainMenu.hpp" // CMainMenu_GetInstancePtr
#include "kyoshin/cf/CfGameManager.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <revolution/GX.h>

#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

// CfGameManager.hpp cannot be included here: it declares ::cf::CfBdat::spBtlPcListFileData as
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
// Global render-gate mode bitfield (.sbss; bit 21), read by Move/cbRenderBefore.
extern u32 gmPauseFlags;

// Retail beginClose symbol (unmangled global): free-call form so the reloc
// is bare (same pattern as CMenuShopBuy.hpp).

// (memberized: needsTalkRebuild -> CKizunaTalkList::needsTalkRebuild; declaration moved into class CKizunaTalkList)
extern "C" void BgTex_Release_3D9C(CBgTex* self);
// (memberized: teardownTalkList -> CKizunaTalkList::teardownTalkList; declaration moved into class CKizunaTalkList)
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);

// (memberized: showTalkList -> CKizunaTalkList::showTalkList; declaration moved into class CKizunaTalkList)

// Draw/update entry points (retail keeps the bare names at these call sites).
extern "C" int BgTex_Acquire_3C14(CBgTex* self);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void BgTex_Tick_3D54(CBgTex* self);
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.
// (memberized: drawTalkList -> CKizunaTalkList::drawTalkList; declaration moved into class CKizunaTalkList)
// (memberized: updateTalkList -> CKizunaTalkList::updateTalkList; declaration moved into class CKizunaTalkList)

// Skip-timer helpers used by the cancel handler in Move().
// Declared by kyoshin/CMainMenu.hpp.
extern "C" void ArtsInfo_SetReadyFlag();

// CfGameManager one-arg controller-type query: declared extern "C" int(int)
// in CMenuTutorial.hpp-style headers; do NOT redeclare here with a different
// signature (MWCC: illegal function overloading). The inline header wrapper
// drops the -1 argument, so call the verbatim-mangled symbol directly
// (retail loads r3=-1 even though the no-arg function ignores it).

// CfGameManager one-arg init flag (retail loads r3=1 into the no-arg symbol).
// setPresentationFlag: canonical setPresentationFlag__Q22cf13CfGameManagerFv(bool).


CMenuKizunaTalkList::CMenuKizunaTalkList(CProcess* parent)
    : mParentRef(parent), mBgTex(0), mTitleAHelp(0, 0), mKizunaTalkList(0), mState(0) {}

CMenuKizunaTalkList::~CMenuKizunaTalkList() {}

// Phase handlers referenced by Move() before their definitions below.
void KizunaList_Phase0Show(CMenuKizunaTalkList* self);
void KizunaList_Phase1Ready(CMenuKizunaTalkList* self);
void KizunaList_Phase2Input(CMenuKizunaTalkList* self);
void KizunaList_Phase3Flag(CMenuKizunaTalkList* self);

// Initialise the Kizuna talk list screen. Re-initialises each embedded widget
// by assignment from a temporary (implicit operator=, inlined: one site per
// class and under the per-function inline budget), then registers this
// screen as a render callback on its parent scene.
void CMenuKizunaTalkList::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);

    // --- Re-initialise the background CBgTex via a temporary ---
    mBgTex = CBgTex(0);

    BgTex_Acquire_3C14(&mBgTex);

    // --- Re-initialise the embedded CTitleAHelp ---
    mTitleAHelp = CTitleAHelp(BdatTouchStringCell("MNU_update", "info", 0x11), 0);

    mTitleAHelp.CTitleAHelp_load();

    mKizunaTalkList = CKizunaTalkList(0);

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
    mTitleAHelp.teardown();
    (&mKizunaTalkList)->teardownTalkList();

    sInstance = nullptr;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

// Per-frame update of the kizuna talk-list screen. Gate on the task/busy
// flags, read the cancel (B) button (classic vs wiimote bit), then drive the
// 4-state FSM and refresh the bg / help bar / list each frame.
void CMenuKizunaTalkList::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (gmPauseFlags & 0x200000))
        return;

    CPad* pad = cf::CfGameManager::getCurrentPad();
    u32 cancel;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
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
    mTitleAHelp.updateHelp();
    (&mKizunaTalkList)->updateTalkList();
}

// Render the kizuna talk-list screen through a stack DrawInfo: gate on the
// task/busy flags and the scene-active check, then draw bg, talk list and
// title/help bar in that order.
void CMenuKizunaTalkList::cbRenderBefore(CScn*) {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (gmPauseFlags & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
    (&mKizunaTalkList)->drawTalkList(&drawInfo);
    mTitleAHelp.drawHelp(&drawInfo);
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
        self->mTitleAHelp.isInitialized() != 0 &&
        (&self->mKizunaTalkList)->hasVisibleTalks() != 0) {
        self->mTitleAHelp.func_801C412C();
        (&self->mKizunaTalkList)->showTalkList();
        self->mState = 1;
        playUISound(0x6d);
    }
}

// Advance the talk-list phase (retail KizunaList_Phase1Ready): once the title/help bar
// is idle and the list has entries waiting, mark the main state byte as 2.
void KizunaList_Phase1Ready(CMenuKizunaTalkList* self) {
    if (self->mTitleAHelp.func_801C4124() != 0 &&
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
    if (cf::CfGameManager::isClassicController(-1) != 0) {
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
        self->mTitleAHelp.beginClose();
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
    if (self->mTitleAHelp.func_801C4124() != 0 &&
        (&self->mKizunaTalkList)->needsTalkRebuild() != 0) {
        self->mField54 = 1;
    }
}