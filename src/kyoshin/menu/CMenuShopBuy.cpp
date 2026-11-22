// Decomp of kyoshin/menu/CMenuShopBuy.
// Shop buy menu screen process (CProcess-header + IScnRender MI layout).

#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfPadData.hpp"
#include "monolib/scn/CScn.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CTaskGame.hpp"

#include "kyoshin/menu/CMenuShopBuy.hpp"

#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include <revolution/gx/GXPixel.h>
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/code_80135FDC_State.hpp"


/*
 * Constructor. The IUICf base (CTTask + mField54/mField55) and the IScnRender
 * sub-object are constructed by the compiler; the members are initialised in
 * declaration order, then the item box line is built in place (raw storage),
 * then the scalar tail (state 0 + float init).
 */
CMenuShopBuy::CMenuShopBuy(CScn* scene, u32 arg)
    : mScene(scene), mField60(arg), mBgTex(0), mTitleAHelp(0, 0),
      mItemBoxLine(0, 0) {
    mState = 0;
    mFloat6FC = 0.0f;
}

/*
 * Destructor: tears down the item box line (raw storage, so it is not a
 * member). The CBgTex / CTitleAHelp members and the IUICf / IScnRender bases
 * are destroyed by the compiler after this body.
 */
CMenuShopBuy::~CMenuShopBuy() {
}

void CMenuShopBuy::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);

    // Build a temporary CBgTex (retail unmangled ctor) and copy its data
    // fields into the member, then destroy the temp (CMenuMapSelect pattern).
    // All member/temp addresses stay inline so MWCC recomputes the addi per
    // use (retail keeps only r31=self and r30 scratch - no cached pointers).
    mBgTex = CBgTex(0);

    BgTex_Acquire_3C14(&mBgTex);

    mTitleAHelp = CTitleAHelp(BdatTouchStringCell("MNU_shop", "name", 1), 0);

    mTitleAHelp.CTitleAHelp_load();

    mItemBoxLine = CItemBoxLine((u16)mField60, 1);

    // Reset the real member and register the render callback (same
    // `if (this)` IScnRender idiom as Term).
    mItemBoxLine.ResetTabBytes();
    mItemBoxLine.PushTabByte(2);
    mItemBoxLine.PushTabByte(4);
    mItemBoxLine.PushTabByte(5);
    mItemBoxLine.PushTabByte(6);
    mItemBoxLine.PushTabByte(7);
    mItemBoxLine.PushTabByte(8);
    mItemBoxLine.PushTabByte(0xd);
    mItemBoxLine.LoadFiles();

    mScene->addRenderCB(this, 0xd, 0);
}

// Tear down the shop buy menu: detach the render callback, release the
// background/title widgets and the item box line, clear the singleton, and
// hand control back to the game manager.
void CMenuShopBuy::Term() {
    CDeviceVI::waitForDrawDone();

    // The `if (this)` is the MWCC idiom that splits mr r4,r31 / beq / addi r4,+0x58.
    mScene->removeRenderCB(this);

    BgTex_Release_3D9C(&mBgTex);
    mTitleAHelp.teardown();
    mItemBoxLine.UnloadFiles();

    CMenuShopBuy::sInstance = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

void CMenuShopBuy::Move() {
    // Single short-circuit OR so MWCC emits: func test -> bne exit;
    // bit test -> beq continue / b exit (CSystemWindow::Move shape).
    if (CTaskGame::getInstance()->isFlag01Set() ||
        (gmPauseFlags & 0x200000))
        return;

    switch (mState) {
    case 0:
        ShopBuy_OpenPhase(this);
        break;
    case 1:
        ShopBuy_AdvancePhase(this);
        break;
    case 2:
        ShopBuy_HandleInput(this);
        break;
    case 3:
        ShopBuy_MarkClosing(this);
        break;
    default:
        break;
    }

    BgTex_Tick_3D54(&mBgTex);
    mItemBoxLine.UpdateStates();
    mTitleAHelp.updateHelp();
}

void CMenuShopBuy::cbRenderBefore(CScn*) {
    // Same single-OR guard shape as Move / CSystemWindow::cbRenderBefore.
    if (CTaskGame::getInstance()->isFlag01Set() ||
        (gmPauseFlags & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
    mItemBoxLine.DrawLayout(&drawInfo);
    mTitleAHelp.drawHelp(&drawInfo);
}

// Shop-buy menu factory: if no instance exists yet, allocate 0x700 bytes from
// the work heap, construct the CMenuShopBuy, publish it to the singleton flag
// and register it as a CProcess child of `parent`. Returns the singleton (or 0
// when one already exists).
extern "C" CMenuShopBuy* ShopBuy_CreateMenu(CProcess* parent, CScn* scene, u32 arg) {
    if (CMenuShopBuy::sInstance != 0)
        return 0;

    CMenuShopBuy* obj = (CMenuShopBuy*)mtl::MemManager::allocate(
        0x700, CWorkThreadSystem::getWorkMem());
    // Placement new supplies the null check; reassigning from its result keeps
    // obj in volatile r3 (retail never spills it to a callee-saved register).
    obj = new (obj) CMenuShopBuy(scene, arg);
    CMenuShopBuy::sInstance = obj;
    obj->Regist(parent, 0);
    return CMenuShopBuy::sInstance;
}

// (CMenuShopBuy::sInstance != 0)
extern "C" bool ShopBuy_HasInstance() { return CMenuShopBuy::sInstance != 0; }

// Phase 0 -> 1 (open): once the background, title bar and item box line are
// all ready, run the open sequence (title bar + item box line) and play the
// open sound, then advance the phase byte at 0x6F8.
extern "C" void ShopBuy_OpenPhase(CMenuShopBuy* self) {
    if (BgTex_IsLoaded_3E34(&self->mBgTex) != 0) {
        if (self->mTitleAHelp.isInitialized() != 0) {
            if (self->mItemBoxLine.GetSelectReady() != 0) {
                self->mTitleAHelp.func_801C412C();
                self->mItemBoxLine.OpenLine();
                self->mState = 1;
                playUISound(0x6d);
            }
        }
    }
}

// Phase 1 -> 2 (advance): once the title bar is idle and the item box line
// is ready, move to the next phase.
extern "C" void ShopBuy_AdvancePhase(CMenuShopBuy* self) {
    if (self->mTitleAHelp.func_801C4124() != 0) {
        if (self->mItemBoxLine.IsReadyFlag() != 0) {
            self->mState = 2;
        }
    }
}

// Phase 2 (interaction): poll the pad and drive the item-box line. The two
// branches differ only in the flag layout (input-enabled vs pointer-input
// controller); each handler ends by jumping to the shared focus-refresh.
extern "C" void ShopBuy_HandleInput(CMenuShopBuy* self) {
    if (MenuTutorialIsCreated() != 0) return;

    // Timer clamped to 10.0f, reset to 0.0f on confirm.
    self->mFloat6FC += 1.0f;
    if (self->mFloat6FC > 10.0f) self->mFloat6FC = 10.0f;

    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        // Pointer input enabled: turbo flags move the cursor, pressed flags
        // run the confirm/cancel flow. The item-box pointer is recomputed per
        // call (addi r3,r31,0xbc) exactly like retail - no cached local.
        self->mItemBoxLine.ResetCursorB8(0);
        if ((pad->mTurboPressButtonFlags & 0x02000000) &&
            self->mItemBoxLine.IsBusy() == 0 &&
            code80135FDC_getByte_64077() > 1) {
            self->mItemBoxLine.Info2SelectPrev();
        } else if ((pad->mTurboPressButtonFlags & 0x04000000) &&
                   self->mItemBoxLine.IsBusy() == 0 &&
                   code80135FDC_getByte_64077() > 1) {
            self->mItemBoxLine.Info2SelectNext();
        } else if (pad->mPad.mPressedButtonFlags & 0x00200000) {
            self->mItemBoxLine.ResolveEntry();
        } else if (pad->mPad.mPressedButtonFlags & 0x00400000) {
            if (self->mItemBoxLine.IsBusy() != 0) {
                self->mItemBoxLine.ConfirmOverlayOrHint();
            } else {
                self->mTitleAHelp.beginClose();
                self->mItemBoxLine.LeaveState3();
                self->mState = 3;
            }
        } else if (pad->mTurboPressButtonFlags & 0x8004) {
            self->mItemBoxLine.CursorPageBack();
        } else if ((pad->mTurboPressButtonFlags & 0x10000) != 0 || (pad->mTurboPressButtonFlags & 0x8) != 0) {
            self->mItemBoxLine.CursorPageDown();
        } else if (pad->mTurboPressButtonFlags & 0x2001) {
            self->mItemBoxLine.CursorPageUp();
        } else if (pad->mTurboPressButtonFlags & 0x4002) {
            self->mItemBoxLine.CursorPageDownRow();
        } else if (pad->mPad.mPressedButtonFlags & 0x200) {
            self->mItemBoxLine.ShowDetailWin();
        } else if (pad->mPad.mPressedButtonFlags & 0x400) {
            self->mItemBoxLine.RefreshTabSelect();
        }
    } else {
        // Pointer input disabled: pad-held/pressed bits drive the flow, and
        // the inner A/B chain exits straight to the shared refresh.
        self->mItemBoxLine.ResetCursorB8(0);
        if ((pad->mPad.mHeldButtonFlags & 0x1000) &&
            self->mItemBoxLine.IsBusy() == 0 &&
            code80135FDC_getByte_64077() > 1) {
            if (self->mFloat6FC > 1.0f) playUISound(2);
            self->mFloat6FC = 0.0f;
            self->mItemBoxLine.ResetCursorB8(1);
            if (pad->mTurboPressButtonFlags & 0x2001) {
                self->mItemBoxLine.Info2SelectPrev();
            } else if (pad->mTurboPressButtonFlags & 0x4002) {
                self->mItemBoxLine.Info2SelectNext();
            }
        } else if (pad->mPad.mPressedButtonFlags & 0x10) {
            self->mItemBoxLine.ResolveEntry();
        } else if (pad->mPad.mPressedButtonFlags & 0x20) {
            if (self->mItemBoxLine.IsBusy() != 0) {
                self->mItemBoxLine.ConfirmOverlayOrHint();
            } else {
                self->mTitleAHelp.beginClose();
                self->mItemBoxLine.LeaveState3();
                self->mState = 3;
            }
        } else if (pad->mTurboPressButtonFlags & 0x8004) {
            self->mItemBoxLine.CursorPageBack();
        } else if ((pad->mTurboPressButtonFlags & 0x10000) != 0 || (pad->mTurboPressButtonFlags & 0x8) != 0) {
            self->mItemBoxLine.CursorPageDown();
        } else if (pad->mTurboPressButtonFlags & 0x2001) {
            self->mItemBoxLine.CursorPageUp();
        } else if (pad->mTurboPressButtonFlags & 0x4002) {
            self->mItemBoxLine.CursorPageDownRow();
        } else if (pad->mPad.mPressedButtonFlags & 0x200) {
            self->mItemBoxLine.ShowDetailWin();
        } else if (pad->mPad.mPressedButtonFlags & 0x40) {
            self->mItemBoxLine.RefreshTabSelect();
        }
    }
    self->mTitleAHelp.func_801C41E8(self->mItemBoxLine.ResolveFocusId());
}

// Close: same idle+ready guard as ShopBuy_AdvancePhase, but marks the closing state
// byte at 0x54 instead of advancing the phase byte.
extern "C" void ShopBuy_MarkClosing(CMenuShopBuy* self) {
    if (self->mTitleAHelp.func_801C4124() != 0) {
        if (self->mItemBoxLine.IsReadyFlag() != 0) {
            self->mField54 = 1;
        }
    }
}

// name-guess: 0x806642F8 is the only 8-byte .sbss slot between
// sInstance__13CMenuShopSell (0x806642F0) and the 0x80664300 area, so this is
// CMenuShopBuy's static instance pointer, mirrored from CMenuShopSell.
CMenuShopBuy* CMenuShopBuy::sInstance;
