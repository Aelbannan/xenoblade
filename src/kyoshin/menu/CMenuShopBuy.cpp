// Decomp of kyoshin/menu/CMenuShopBuy.
// Shop buy menu screen process (CProcess-header + IScnRender MI layout).

#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CTaskGame.hpp"

#include "kyoshin/menu/CMenuShopBuy.hpp"

#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include <revolution/gx/GXPixel.h>
#include "kyoshin/CTitleAHelpImports.hpp"
void playUISound(u32);

// Same-TU blob copy helper (defined below); Init uses it twice.
extern "C" void ShopBuy_CopyData(ShopBuyData* dst, const ShopBuyData* src);

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
    mFloat6FC = lbl_eu_80667A20;
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

    mTitleAHelp = CTitleAHelp(BdatTouchStringCell(lbl_eu_80503B20, lbl_eu_80503B20 + 9, 1), 0);

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

    addRenderCB__4CScnFP10IScnRenderUlUl(mScene, this, 0xd, 0);
}

// Tear down the shop buy menu: detach the render callback, release the
// background/title widgets and the item box line, clear the singleton, and
// hand control back to the game manager.
void CMenuShopBuy::Term() {
    CDeviceVI::waitForDrawDone();

    // The `if (this)` is the MWCC idiom that splits mr r4,r31 / beq / addi r4,+0x58.
    removeRenderCB__4CScnFP10IScnRender(mScene, this);

    BgTex_Release_3D9C(&mBgTex);
    teardown(&mTitleAHelp);
    mItemBoxLine.UnloadFiles();

    lbl_eu_806642F8 = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

void CMenuShopBuy::Move() {
    // Single short-circuit OR so MWCC emits: func test -> bne exit;
    // bit test -> beq continue / b exit (CSystemWindow::Move shape).
    if (CTaskGame::getInstance()->isFlag01Set() ||
        (lbl_eu_80663E28 & 0x200000))
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
    updateHelp(&mTitleAHelp);
}

void CMenuShopBuy::cbRenderBefore() {
    // Same single-OR guard shape as Move / CSystemWindow::cbRenderBefore.
    if (CTaskGame::getInstance()->isFlag01Set() ||
        (lbl_eu_80663E28 & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
    mItemBoxLine.DrawLayout(&drawInfo);
    drawHelp(&mTitleAHelp, &drawInfo);
}

// Copy the 0x159-byte shop-buy data blob field-by-field. Each sub-struct
// assignment comes out as a mtctr/lwzu/stwu counted loop; the u32 slot group
// is unrolled into 6 word pairs and the byte is a single lbz/stb.
extern "C" void ShopBuy_CopyData(ShopBuyData* dst, const ShopBuyData* src) {
    dst->m00 = src->m00;
    dst->m18 = src->m18;
    // Six independent word slots: MWCC schedules these as interleaved
    // r8/r5 load/store pairs rather than a counted copy loop.
    dst->mA8[0] = src->mA8[0];
    dst->mA8[1] = src->mA8[1];
    dst->mA8[2] = src->mA8[2];
    dst->mA8[3] = src->mA8[3];
    dst->mA8[4] = src->mA8[4];
    dst->mA8[5] = src->mA8[5];
    dst->mC0 = src->mC0;
    dst->mD8 = src->mD8;
    dst->mD9 = src->mD9;
}

// Shop-buy menu factory: if no instance exists yet, allocate 0x700 bytes from
// the work heap, construct the CMenuShopBuy, publish it to the singleton flag
// and register it as a CProcess child of `parent`. Returns the singleton (or 0
// when one already exists).
extern "C" CMenuShopBuy* ShopBuy_CreateMenu(CProcess* parent, CScn* scene, u32 arg) {
    if (lbl_eu_806642F8 != 0)
        return 0;

    CMenuShopBuy* obj = (CMenuShopBuy*)mtl::MemManager::allocate(
        0x700, CWorkThreadSystem::getWorkMem());
    // Placement new supplies the null check; reassigning from its result keeps
    // obj in volatile r3 (retail never spills it to a callee-saved register).
    obj = new (obj) CMenuShopBuy(scene, arg);
    lbl_eu_806642F8 = (u32)obj;
    obj->Regist(parent, 0);
    return (CMenuShopBuy*)lbl_eu_806642F8;
}

// (lbl_eu_806642F8 != 0)
extern "C" bool ShopBuy_HasInstance() { return lbl_eu_806642F8 != 0; }

// Phase 0 -> 1 (open): once the background, title bar and item box line are
// all ready, run the open sequence (title bar + item box line) and play the
// open sound, then advance the phase byte at 0x6F8.
extern "C" __declspec(noinline) void ShopBuy_OpenPhase(CMenuShopBuy* self) {
    if (BgTex_IsLoaded_3E34(&self->mBgTex) != 0) {
        if (isInitialized(&self->mTitleAHelp) != 0) {
            if (self->mItemBoxLine.GetSelectReady() != 0) {
                func_801C412C(&self->mTitleAHelp);
                self->mItemBoxLine.OpenLine();
                self->mState = 1;
                playUISound(0x6d);
            }
        }
    }
}

// Phase 1 -> 2 (advance): once the title bar is idle and the item box line
// is ready, move to the next phase.
extern "C" __declspec(noinline) void ShopBuy_AdvancePhase(CMenuShopBuy* self) {
    if (self->mTitleAHelp.isIdle() != 0) {
        if (self->mItemBoxLine.IsReadyFlag() != 0) {
            self->mState = 2;
        }
    }
}

// Phase 2 (interaction): poll the pad and drive the item-box line. The two
// branches differ only in the flag layout (input-enabled vs pointer-input
// controller); each handler ends by jumping to the shared focus-refresh.
extern "C" __declspec(noinline) void ShopBuy_HandleInput(CMenuShopBuy* self) {
    if (MenuTutorialIsCreated() != 0) return;

    // Timer clamped to lbl_eu_80667A28, reset to lbl_eu_80667A20 on confirm.
    self->mFloat6FC += lbl_eu_80667A24;
    if (self->mFloat6FC > lbl_eu_80667A28) self->mFloat6FC = lbl_eu_80667A28;

    ShopBuyPadData* pad = getCfPadData__Q22cf13CfGameManagerFv();
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        // Pointer input enabled: turbo flags move the cursor, pressed flags
        // run the confirm/cancel flow. The item-box pointer is recomputed per
        // call (addi r3,r31,0xbc) exactly like retail - no cached local.
        self->mItemBoxLine.ResetCursorB8(0);
        if ((pad->mTurboFlags & 0x02000000) &&
            self->mItemBoxLine.IsBusy() == 0 &&
            code80135FDC_getByte_64077() > 1) {
            self->mItemBoxLine.Info2SelectPrev();
        } else if ((pad->mTurboFlags & 0x04000000) &&
                   self->mItemBoxLine.IsBusy() == 0 &&
                   code80135FDC_getByte_64077() > 1) {
            self->mItemBoxLine.Info2SelectNext();
        } else if (pad->mPressedFlags & 0x00200000) {
            self->mItemBoxLine.ResolveEntry();
        } else if (pad->mPressedFlags & 0x00400000) {
            if (self->mItemBoxLine.IsBusy() != 0) {
                self->mItemBoxLine.ConfirmOverlayOrHint();
            } else {
                beginClose(&self->mTitleAHelp);
                self->mItemBoxLine.LeaveState3();
                self->mState = 3;
            }
        } else if (pad->mTurboFlags & 0x8004) {
            self->mItemBoxLine.CursorPageBack();
        } else if ((pad->mTurboFlags & 0x10000) != 0 || (pad->mTurboFlags & 0x8) != 0) {
            self->mItemBoxLine.CursorPageDown();
        } else if (pad->mTurboFlags & 0x2001) {
            self->mItemBoxLine.CursorPageUp();
        } else if (pad->mTurboFlags & 0x4002) {
            self->mItemBoxLine.CursorPageDownRow();
        } else if (pad->mPressedFlags & 0x200) {
            self->mItemBoxLine.ShowDetailWin();
        } else if (pad->mPressedFlags & 0x400) {
            self->mItemBoxLine.RefreshTabSelect();
        }
    } else {
        // Pointer input disabled: pad-held/pressed bits drive the flow, and
        // the inner A/B chain exits straight to the shared refresh.
        self->mItemBoxLine.ResetCursorB8(0);
        if ((pad->mHeldFlags & 0x1000) &&
            self->mItemBoxLine.IsBusy() == 0 &&
            code80135FDC_getByte_64077() > 1) {
            if (self->mFloat6FC > lbl_eu_80667A24) playUISound(2);
            self->mFloat6FC = lbl_eu_80667A20;
            self->mItemBoxLine.ResetCursorB8(1);
            if (pad->mTurboFlags & 0x2001) {
                self->mItemBoxLine.Info2SelectPrev();
            } else if (pad->mTurboFlags & 0x4002) {
                self->mItemBoxLine.Info2SelectNext();
            }
        } else if (pad->mPressedFlags & 0x10) {
            self->mItemBoxLine.ResolveEntry();
        } else if (pad->mPressedFlags & 0x20) {
            if (self->mItemBoxLine.IsBusy() != 0) {
                self->mItemBoxLine.ConfirmOverlayOrHint();
            } else {
                beginClose(&self->mTitleAHelp);
                self->mItemBoxLine.LeaveState3();
                self->mState = 3;
            }
        } else if (pad->mTurboFlags & 0x8004) {
            self->mItemBoxLine.CursorPageBack();
        } else if ((pad->mTurboFlags & 0x10000) != 0 || (pad->mTurboFlags & 0x8) != 0) {
            self->mItemBoxLine.CursorPageDown();
        } else if (pad->mTurboFlags & 0x2001) {
            self->mItemBoxLine.CursorPageUp();
        } else if (pad->mTurboFlags & 0x4002) {
            self->mItemBoxLine.CursorPageDownRow();
        } else if (pad->mPressedFlags & 0x200) {
            self->mItemBoxLine.ShowDetailWin();
        } else if (pad->mPressedFlags & 0x40) {
            self->mItemBoxLine.RefreshTabSelect();
        }
    }
    func_801C41E8(&self->mTitleAHelp,
                  self->mItemBoxLine.ResolveFocusId());
}

// Close: same idle+ready guard as ShopBuy_AdvancePhase, but marks the closing state
// byte at 0x54 instead of advancing the phase byte.
extern "C" __declspec(noinline) void ShopBuy_MarkClosing(CMenuShopBuy* self) {
    if (self->mTitleAHelp.isIdle() != 0) {
        if (self->mItemBoxLine.IsReadyFlag() != 0) {
            self->mField54 = 1;
        }
    }
}
