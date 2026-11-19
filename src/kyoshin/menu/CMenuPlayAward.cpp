#define IWORK_EVENT_INLINE_DTOR
// Auto-scaffolded catalog TU for kyoshin/menu/CMenuPlayAward
// Replace stubs with high-level C/C++ during decomp.

#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/CScrollBar.hpp"
#include <stdio.h>   // sprintf (award pane names)
#include "monolib/lib/CLibLayout.hpp"
#include <revolution/gx/GXPixel.h>  // GXSetZMode

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/code_80135FDC.hpp" // AnimRewindFrame / advanceAnimTransform (anim frame advance)
#include "kyoshin/cf/CfGameManager.hpp" // cf::CfGameManager::getCurrentPad / getCfPadData
#include "monolib/core/CPadManager.hpp" // CPad::mPressedButtonFlags
#include "kyoshin/menu/CMenuPlayAward.hpp"
#include "monolib/work/CEventFile.hpp" // CEventFile (event->mFileHandle in OnFileEvent)
#include "monolib/device/CDeviceFile.hpp"  // CDeviceFile::readFile / readCommonArchiveFile
#include "monolib/device/CDeviceVI.hpp"       // CDeviceVI::waitForDrawDone
#include "monolib/util/MemManager.hpp"        // mtl::MemManager::allocate
#include "monolib/work/CProcess.hpp"          // CProcess::Regist
#include "monolib/work/CWorkThreadSystem.hpp" // CWorkThreadSystem::getWorkMem
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
// CScn (with removeRenderCB) comes from the minimal declaration in
// CTaskGameEff.hpp via harness_catalog.hpp -- CScn.hpp redefines it.


// Out-of-line constructor (retail __ct__CMenuPlayAward, separate from the
// factory). The IUICf base, the IScnRender subobject and the null PMF slots
// are set up by the compiler; the members initialise in declaration order.
CMenuPlayAward::CMenuPlayAward(CScn* scene)
    : mScene(scene), mBgTex(0), mTitleAHelp(0, 0), mPlayAwardList(0) {
    mField115C = lbl_eu_80668998;
    mField1160 = 0;
}

CMenuPlayAward::~CMenuPlayAward() {}

// Re-initialise the award-list menu screen: rebuild each embedded widget
// (CBgTex / CTitleAHelp / CPlayAwardList) from a temporary, then register this
// screen as a render callback on the owning scene (same pattern as CMenuSkipTimer).
void CMenuPlayAward::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);

    mBgTex = CBgTex(0);
    BgTex_Acquire_3C14(&mBgTex);

    mTitleAHelp = CTitleAHelp(BdatTouchStringCell(lbl_eu_8050E7A0, lbl_eu_8050E7A0 + 0xb, 0x9), 0x66);
    mTitleAHelp.CTitleAHelp_load();

    mPlayAwardList = CPlayAwardList(0);
    PlayAward_LoadLayouts(&mPlayAwardList);

    // Register this screen as a render callback on the owning scene.
    mScene->addRenderCB(this, 0xd, 0);
}

void CMenuPlayAward::Term() {
    CDeviceVI::waitForDrawDone();

    mScene->removeRenderCB(this);

    BgTex_Release_3D9C(&mBgTex);
    mTitleAHelp.teardown();
    PlayAward_ReleaseList(&mPlayAwardList);

    sInstance = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

void CMenuPlayAward::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set()) {
        goto exit;
    }
    // Branch-over-branch guard (same shape as cbRenderBefore / CMenuTutorialList):
    // exit label before body label emits retail's `beq body; b exit`.
    if (!(gmPauseFlags & (1u << 21))) {
        goto body;
    }
    goto exit;
exit:
    return;
body:
    // Close request: the menu's close button (classic: bit 23, wii: bit 10)
    // plays the close sound and flags the closing state (0x54) while the
    // help-display byte (0x1160) leaves the phase dispatch.
    CPad* pad = cf::CfGameManager::getCurrentPad();
    u32 close;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        close = (pad->mPressedButtonFlags >> 23) & 1;
    } else {
        close = (pad->mPressedButtonFlags >> 10) & 1;
    }
    if (close != 0) {
        if (CMainMenu_GetInstancePtr() != 0) {
            ArtsInfo_SetReadyFlag();
            playUISound(6);
        }
        mField1160 = 4;
        mField54 = 1;
    }

    // Phase state machine (dispatch on the help-display byte 0x1160).
    switch (mField1160) {
    case 0:
        PlayAward_Phase0_ShowHelp(this);
        break;
    case 1:
        PlayAward_Phase1_Advance(this);
        break;
    case 2:
        PlayAward_Phase2_HandleInput(this);
        break;
    case 3:
        PlayAward_Phase3_RequestClose(this);
        break;
    }

    BgTex_Tick_3D54(&mBgTex);
    mTitleAHelp.updateHelp();
    PlayAward_UpdateList(&mPlayAwardList);
}

void CMenuPlayAward::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set()) {
        goto exit;
    }
    // Branch-over-branch guard (same shape as CMenuTutorialList::cbRenderBefore):
    // exit label before body label emits retail's `beq body; b exit`.
    if (!(gmPauseFlags & (1u << 21))) {
        goto body;
    }
    goto exit;
exit:
    return;
body:
    if (IsMenuState621F0() == 0) {
        goto exit;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
    PlayAward_DrawList(&mPlayAwardList, &drawInfo);
    mTitleAHelp.drawHelp(&drawInfo);
}

// ---------- Factory: lazily allocate + construct + register the menu ----------

CMenuPlayAward* CMenuPlayAward::sInstance;

CMenuPlayAward* CMenuPlayAward::create(CProcess* parent, CScn* scene) {
    if (sInstance != nullptr) {
        return nullptr;
    }
    CMenuPlayAward* menu = new (CWorkThreadSystem::getWorkMem()) CMenuPlayAward(scene);
    sInstance = menu;
    menu->Regist(parent, false);
    return sInstance;
}

// Menus ask whether an award screen is live before drawing it.
extern "C" unsigned long PlayAward_IsActive(void) { return CMenuPlayAward::sInstance != 0; }

// Help-bar show: when the background, title/help bar and award list are all
// ready, display the help bar, play its sound, and flag the menu (0x1160 = 1).
void PlayAward_Phase0_ShowHelp(CMenuPlayAward* self) {
    if (BgTex_IsLoaded_3E34(&self->mBgTex) != 0
        && self->mTitleAHelp.isInitialized() != 0
        && PlayAward_IsListReady(&self->mPlayAwardList) != 0) {
        self->mTitleAHelp.func_801C412C();
        PlayAward_RequestOpen(&self->mPlayAwardList);
        self->mField1160 = 1;
        playUISound(0x6d);
    }
}

// Help-bar show: when the title/help bar is idle and the award list is in its
// "ready" state, display the help bar (byte at 0x1160 = 2).
void PlayAward_Phase1_Advance(CMenuPlayAward* self) {
    if (self->mTitleAHelp.func_801C4124() != 0 && PlayAward_IsReadyFlag(&self->mPlayAwardList) != 0) {
        self->mField1160 = 2;
    }
}

void PlayAward_Phase2_HandleInput(CMenuPlayAward* self) {
    self->mField115C += lbl_eu_8066899C;
    if (self->mField115C > lbl_eu_806689A0) {
        self->mField115C = lbl_eu_806689A0;
    }

    CPlayAwardPadData* pad =
        reinterpret_cast<CPlayAwardPadData*>(cf::CfGameManager::getCfPadData());

    // Directional/confirm trigger bits differ between controller types; the
    // whole extraction block is re-run per branch (retail reloads both flag
    // words inside each branch). Same shape as CMenuOption's input handler.
    bool up, down, pageUp, pageDown;
    bool pageLeft, left, right;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        u32 turbo = pad->mTurboPressButtonFlags;   // +0x104
        u32 pressed = pad->mPadPressedFlags;       // +0x04
        up = (turbo & 0x8004) != 0;
        down = (turbo & 0x10008) != 0;
        pageUp = (turbo & 0x2001) != 0;
        pageDown = (turbo & 0x4002) != 0;
        left = (pressed >> 21) & 1;
        right = (pressed >> 22) & 1;
        pageLeft = (pressed >> 10) & 1;
    } else {
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        up = (turbo & 0x8004) != 0;
        down = (turbo & 0x10008) != 0;
        pageUp = (turbo & 0x2001) != 0;
        pageDown = (turbo & 0x4002) != 0;
        left = (pressed >> 4) & 1;
        right = (pressed >> 5) & 1;
        pageLeft = (pressed >> 6) & 1;
    }

    // Input dispatch: a held close button (right) closes the award list;
    // otherwise the turbo inputs drive cursor/scroll/page moves (pageLeft =
    // page flip, lowest priority). left blocks the other inputs.
    if (left) {
        // nothing -- blocks the remaining input handlers
    } else if (right) {
        self->mTitleAHelp.beginClose();
        PlayAward_RequestClose(&self->mPlayAwardList);
        self->mField1160 = 3;
    } else if (up) {
        PlayAward_MoveCursorUp(&self->mPlayAwardList);
    } else if (down) {
        PlayAward_MoveCursorDown(&self->mPlayAwardList);
    } else if (pageUp) {
        PlayAward_ScrollPageUp(&self->mPlayAwardList);
    } else if (pageDown) {
        PlayAward_ScrollPageDown(&self->mPlayAwardList);
    } else if (pageLeft) {
        PlayAward_FlipPage(&self->mPlayAwardList);
    }
    s32 page = PlayAward_GetPageCue(&self->mPlayAwardList);
    self->mTitleAHelp.func_801C41E8((u8)page);
}

// Help-bar hide: when the title/help bar is idle and the award list is in its
// "ready" state, mark the menu as closing (byte at 0x54 = 1).
void PlayAward_Phase3_RequestClose(CMenuPlayAward* self) {
    if (self->mTitleAHelp.func_801C4124() != 0 && PlayAward_IsReadyFlag(&self->mPlayAwardList) != 0) {
        self->mField54 = 1;
    }
}
