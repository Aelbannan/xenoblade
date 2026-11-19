#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuItem.hpp"

#include "kyoshin/menu/CMenuGCItem.hpp"
#include "kyoshin/cf/CInfoCf.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/scn/CScn.hpp"
#include "kyoshin/CItemBoxGrid.hpp"
#include "kyoshin/CTaskGame.hpp"
#include <revolution/gx/GXPixel.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// ---- Declarations private to this unit (C-style imports). ----

// C-linkage imports (retail-unmangled symbols: US strips the member
// manglings for the func_ helpers, and the CfGameManager statics carry their
// C++ mangling inside the identifier). Declared extern "C" so MWCC emits the
// retail reloc names verbatim (same scheme as CfTFile.hpp / code_800A75FC.hpp).
extern "C" {
// setPresentationFlag: canonical setPresentationFlag__Q22cf13CfGameManagerFv(bool).
void BgTex_Release_3D9C(CBgTex* self);
void teardown(CTitleAHelp* self);
void updateHelp(CTitleAHelp* self);
void CfRes_getInstPtrBC();
int KyoshinHeap_GetActive54();
u32 KyoshinHeap_GetSize500000();
void __dt__8CProcessFv(CProcess* self, int flags);
int BgTex_Acquire_3C14(CBgTex* self);
void BgTex_SetupRegion_3A24(CBgTex* self);
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.
}

// CInfoCf vtable (retail .data:0x805309B0), the UI-state singleton pointer
// (sbss; instance stored by __ct__cf_CInfoCf, cleared by the dtor), and the
// Menu singleton pointer (sbss), cleared by CMenuItem::Term.

// Title/help name-string base used by CMenuItem::Init (retail rodata).

// Render-gate mode bitfield (.sbss; bit 21 = busy), shared with the other
// menu-screen TUs.
extern u32 lbl_eu_80663E28;

// Menu/task-flow imports previously pulled in via CSkipTimer.hpp (that header
// carries a conflicting __ct__7CSysWinFUc declaration, so they are declared here).
extern u32 CMainMenu_GetInstancePtr();
extern void ArtsInfo_SetReadyFlag();
extern void playUISound(u32);


// cf::CfPadData view: only the flag words the menu input handler reads
// (+0x00 held, +0x04 pressed, +0x104 turbo/short-press).
struct CfMenuPadData {
    u32 mHeldFlags;                  // +0x00
    u32 mPressedFlags;               // +0x04
    u8 _pad08[0x104 - 0x08];         // 0x08..0x103
    u32 mTurboFlags;                 // +0x104
};

// Input-handler callees (retail unmangled names).
extern "C" void beginClose(CTitleAHelp* self);
extern "C" CfMenuPadData* getCfPadData__Q22cf13CfGameManagerFv();
extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801C412C(CTitleAHelp* self);

extern "C" void BgTex_Tick_3D54(CBgTex* self);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);


// Global: UI state flag read by InfoCfGetUiFlags.

// Members and bases are destroyed by the compiler.
CMenuItem::~CMenuItem() {}

void CMenuItem::Init() {
    // Gate the game-manager handoff until the menu flag is clear (retail
    // reads mField4AC5 before any widget setup).
    if (mField4AC5 == 0) {
        setPresentationFlag__Q22cf13CfGameManagerFv(1);
    }

    // --- Re-initialise the embedded CBgTex via a temporary ---
    mBgTex = CBgTex(0);

    if (BgTex_Acquire_3C14(&mBgTex) != 0) {
        BgTex_SetupRegion_3A24(&mBgTex);
    }

    // --- Re-initialise the embedded CTitleAHelp via a temporary ---
    char* name = BdatTouchStringCell("MNU_item", "name", 1);
    mTitleAHelp = CTitleAHelp(name, 1);
    mTitleAHelp.CTitleAHelp_load();

    // --- Re-initialise the embedded CItemBoxGrid via a temporary ---
    mItemBoxGrid = CItemBoxGrid(mField4AC5 ? 3 : 0, 0, (u32)mScene, mField4AC6);

    mItemBoxGrid.ClearListSlots();

    if (mField4AC5 == 0) {
        // Single-player item list ordering.
        mItemBoxGrid.PushToList(2);
        mItemBoxGrid.PushToList(4);
        mItemBoxGrid.PushToList(5);
        mItemBoxGrid.PushToList(6);
        mItemBoxGrid.PushToList(7);
        mItemBoxGrid.PushToList(8);
        mItemBoxGrid.PushToList(0xb);
        mItemBoxGrid.PushToList(0xd);
        mItemBoxGrid.PushToList(0xc);
        mItemBoxGrid.PushToList(3);
        mItemBoxGrid.PushToList(9);
        mItemBoxGrid.PushToList(0xa);
    } else {
        mItemBoxGrid.PushToList(mField4AC5);
    }
    mItemBoxGrid.LoadItemBoxFiles();

    mScene->addRenderCB(this, 0xd, 0);
}

// Tear down the menu screen: wait for the draw callback to finish, remove the
// render callback from the owning scene, release each embedded widget, clear
// the singleton flag, and hand control back to the game manager when the
// menu-state byte is clear.
void CMenuItem::Term() {
    CDeviceVI::waitForDrawDone();

    mScene->removeRenderCB(this);

    BgTex_Release_3D9C(&mBgTex);
    teardown(&mTitleAHelp);
    mItemBoxGrid.UnloadItemBox();

    sInstance = nullptr;
    if (mField4AC5 == 0) {
        setPresentationFlag__Q22cf13CfGameManagerFv(0);
    }
}

void CMenuItem::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;

    // Button input: the confirm trigger comes from a different pad bit in
    // co-op vs single-player mode.
    CPad* pad = cf::CfGameManager::getCurrentPad();
    u32 pressed;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        pressed = (pad->mPressedButtonFlags >> 23) & 1;
    } else {
        pressed = (pad->mPressedButtonFlags >> 10) & 1;
    }
    if (pressed != 0) {
        // Close a pending item-box interaction, then mark the menu done.
        if (mItemBoxGrid.IsSubWinActive() == 0) {
            if (CMainMenu_GetInstancePtr() != 0) {
                ArtsInfo_SetReadyFlag();
            }
            playUISound(6);
            mState = 4;
            mField54 = 1;
        }
    }

    switch (mState) {
    case 0:
        // Once the bg texture, title bar and item grid are all ready,
        // start the panel intro animations and play the open cue.
        if (BgTex_IsLoaded_3E34(&mBgTex) != 0 && mTitleAHelp.isInitialized() != 0 &&
            mItemBoxGrid.IsItemBoxReady() != 0) {
            func_801C412C(&mTitleAHelp);
            mItemBoxGrid.InitItemDisplay();
            mState = 1;
            playUISound(0x6d);
        }
        break;
    case 1:
        // Advance to phase 2 once the title bar is idle and the grid ready.
        if (mTitleAHelp.isIdle() != 0 && mItemBoxGrid.GetField61() != 0) {
            mState = 2;
        }
        break;
    case 2:
        HandleInput();
        break;
    case 3:
        // Same idle+ready check, but sets the phase flag at 0x54 instead.
        if (mTitleAHelp.isIdle() != 0 && mItemBoxGrid.GetField61() != 0) {
            mField54 = 1;
        }
        break;
    }

    BgTex_Tick_3D54(&mBgTex);
    mItemBoxGrid.UpdateItemBox();
    updateHelp(&mTitleAHelp);
}

void CMenuItem::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
    mItemBoxGrid.DrawItemBoxGrid(&drawInfo);
    drawHelp(&mTitleAHelp, &drawInfo);
}

/*
 * Menu item-screen singleton factory. If the singleton already exists, returns NULL. Otherwise
 * constructs the process in work memory, registers it under `parent`, stores
 * the singleton pointer and returns it.
 */
CMenuItem* CMenuItem::sInstance;

CMenuItem* CMenuItem::create(CProcess* parent, CScn* scene, u8 flag5, u8 flag6) {
    if (sInstance != nullptr) {
        return nullptr;
    }

    CMenuItem* obj = new (CWorkThreadSystem::getWorkMem()) CMenuItem(scene, flag5, flag6);

    sInstance = obj;
    obj->Regist(parent, false);

    return sInstance;
}

unsigned long ItemMenu_IsPresent(void) { return CMenuItem::sInstance != nullptr; }

/* Item-menu main input handler. Syncs the help bar to the grid mode, then
 * decodes the pad: turbo/pressed masks for cancel/page-turn/decide, and
 * pressed-bit positions that depend on the game-manager controller mode
 * (classic vs. others). Dispatches one grid action in priority order. */
void CMenuItem::HandleInput() {
    // Help bar follows the grid's current mode.
    u8 mode = mItemBoxGrid.GetPromptState();
    func_801C41E8(&mTitleAHelp, mode);

    CfMenuPadData* pad = getCfPadData__Q22cf13CfGameManagerFv();
    bool up, down;
    int turboCancel, cancel, pageTurn, decide;
    bool leftTrigger, xHeld, yHeld;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        // Classic controller bit layout.
        turboCancel = (pad->mTurboFlags & 0x8004) != 0;
        pageTurn = (pad->mTurboFlags & 0x10008) != 0;
        up = (pad->mPressedFlags & 0x00200000) != 0;
        down = (pad->mPressedFlags & 0x00400000) != 0;
        cancel = (pad->mTurboFlags & 0x2001) != 0;
        decide = (pad->mTurboFlags & 0x4002) != 0;
        leftTrigger = (pad->mPressedFlags & 0x01000000) != 0;
        xHeld = (pad->mPressedFlags & 0x00000200) != 0;
        yHeld = (pad->mPressedFlags & 0x00000400) != 0;
    } else {
        turboCancel = (pad->mTurboFlags & 0x8004) != 0;
        pageTurn = (pad->mTurboFlags & 0x10008) != 0;
        up = (pad->mPressedFlags & 0x00000010) != 0;
        down = (pad->mPressedFlags & 0x00000020) != 0;
        cancel = (pad->mTurboFlags & 0x2001) != 0;
        decide = (pad->mTurboFlags & 0x4002) != 0;
        leftTrigger = (pad->mPressedFlags & 0x00000800) != 0;
        xHeld = (pad->mPressedFlags & 0x00000200) != 0;
        yHeld = (pad->mPressedFlags & 0x00000040) != 0;
    }

    if (up != 0) {
        mItemBoxGrid.func_801CCAF0();
    } else if (down != 0) {
        if (mItemBoxGrid.IsItemBoxActive() != 0) {
            mItemBoxGrid.HandleCancelBtn(0);
        } else {
            // Scroll failed: hide the help prompt, refresh the grid and move
            // to state 3.
            beginClose(&mTitleAHelp);
            mItemBoxGrid.AdvanceBoxState();
            mState = 3;
        }
    } else if (turboCancel != 0) {
        mItemBoxGrid.GridMoveCursorLeft();
    } else if (pageTurn != 0) {
        mItemBoxGrid.GridMoveCursorRight();
    } else if (cancel != 0) {
        mItemBoxGrid.GridMoveCursorDown();
    } else if (decide != 0) {
        mItemBoxGrid.GridMoveCursorUp();
    } else if (leftTrigger != 0) {
        mItemBoxGrid.OpenSortMenu();
    } else if (xHeld != 0) {
        mItemBoxGrid.TickItemBoxGrid();
    } else if (yHeld != 0) {
        mItemBoxGrid.SelectCatRow();
    }
}
