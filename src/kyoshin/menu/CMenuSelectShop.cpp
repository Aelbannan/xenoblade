// kyoshin/menu/CMenuSelectShop - select-shop menu screen.
//
// Real MI class (IUIWindow + IWorkEvent + IScnRender, see the header): the
// composite vtable __vt__15CMenuSelectShop, its 0x40 base list, the weak RTTI
// record and the "CMenuSelectShop" type-name string are all compiler output,
// matching retail's .data 0x100 / .rodata 0x20 / .sdata 8 byte for byte.

#define IWORK_EVENT_INLINE_DTOR
#include <types.h>
#include "kyoshin/CTaskGame.hpp"

#include "monolib/device/CDeviceVI.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfPadData.hpp"
#include <revolution/gx/GXPixel.h>
#include <revolution/gx/GXTypes.h>
#include <nw4r/lyt/lyt_drawInfo.h>

#include "kyoshin/menu/CMenuSelectShop.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// ---------------------------------------------------------------------------
// ShopSel_CreateSingleton  (us-8018bb40)
// Factory: allocate the CMenuSelectShop singleton from work memory, run its
// ctor, store it in the .sbss global and register it as a CProcess child of
// `parent`. Returns 0 when the singleton already exists.
// ---------------------------------------------------------------------------
extern "C" CMenuSelectShop* ShopSel_CreateSingleton(CProcess* parent, void* a2,
                                          void* a3) {
    if (lbl_eu_806642E8 != 0)
        return 0;

    CMenuSelectShop* obj =
        new (mtl::MemManager::allocate(0xd4, CWorkThreadSystem::getWorkMem()))
            CMenuSelectShop(a2, a3);
    lbl_eu_806642E8 = (u32)obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, 0);
    return (CMenuSelectShop*)lbl_eu_806642E8;
}

// ---------------------------------------------------------------------------
// CMenuSelectShop ctor (us-8018b39c)
// The -O4,s unit gives retail's `stmw r28,16(sp)` and keeps the ctor out of
// line (retail 0x8018b39c; the factory is a 0x7C `bl __ct__...`).
// ---------------------------------------------------------------------------
CMenuSelectShop::CMenuSelectShop(void* scene, void* a3)
    : mScene((CScn*)scene), mMemRegion(), mSelShop(), mCursor(0) {
    mState = 0;
    mSelIndex = 0;
    mFieldD0 = (u32)a3;
}

// ---------------------------------------------------------------------------
// CMenuSelectShop::~CMenuSelectShop()  (us-8018b48c)
// Complete-object dtor with delete flag. Subobjects are destroyed in reverse
// construction order: CCur18@0xB0, CSelShopWin@0x88, UnkClass_8045F564@0x78,
// then the IUIWindow base. The nested flag dispatch selects operator delete.
// ---------------------------------------------------------------------------
#pragma push
CMenuSelectShop::~CMenuSelectShop() {}
#pragma pop

// ---------------------------------------------------------------------------
// CMenuSelectShop::Init  (us-8018b504)
// Create the memory region, then build the shop window + cursor on the stack,
// copy them into their payload slots (skipping each zero-relative vtable),
// destroy the temporaries and register the render callback.
// ---------------------------------------------------------------------------
void CMenuSelectShop::Init() {
    mMemRegion.createRegion(mtl::MemManager::getHandleMEM2(), 0x2000,
                            "CMenuSelectShop", 0);

    // RAII scratch-region guard, destroyed at the end of this scope (retail
    // stack slot sp+0x8).
    Class_8045F858 region(&mMemRegion);

    // Rebuild the embedded shop window: construct a temp at sp+0x28, copy its
    // payload (all non-vtable fields), destroy the temp, then start loading
    // the window resources. The temp pointers are derived after the ctor so
    // MWCC folds the accesses to r1-relative addressing (CSysWinSave::Init
    // shape) instead of holding a callee-saved base register.
    {
        CSelShopWin tempShop;
        mSelShop.mMemRegion = tempShop.mMemRegion;
        mSelShop.mFileHandle = tempShop.mFileHandle;
        mSelShop.mAccessor = tempShop.mAccessor;
        mSelShop.mLayout = tempShop.mLayout;
        mSelShop.mAnimTransform = tempShop.mAnimTransform;
        mSelShop.mIsLayoutBuilt = tempShop.mIsLayoutBuilt;
        mSelShop.mIsLoaded = tempShop.mIsLoaded;
        mSelShop.mAnimState = tempShop.mAnimState;
        mSelShop.mAnimActive = tempShop.mAnimActive;
    }

    mSelShop.loadShopWinArc();

    // Same pattern for the CCur18 cursor (payload +0x4..+0x15, slot sp+0x10).
    mCursor = CCur18(CUICfManager_getArcResourceAccessor());

    mCursor.initLayout();

    playUISound(0x29);

    addRenderCB__4CScnFP10IScnRenderUlUl(mScene, this, 0xd, 0);

    mMemRegion.validateHeap();
}

// ---------------------------------------------------------------------------
// CMenuSelectShop::Term()  (us-8018b670)
// Wait for draw-done, detach the render callback from the owning scene, tear
// down the shop window, run the CCur18 cursor update virtual, release the
// memory region and clear the singleton global. The `if (this)` prepending on
// the render-cb arg reproduces the retail `mr / beq / addi r4,+0x70` idiom.
// ---------------------------------------------------------------------------
void CMenuSelectShop::Term() {
    CDeviceVI::waitForDrawDone();

    removeRenderCB__4CScnFP10IScnRender(mScene, this);

    mSelShop.teardownShopWin();
    mCursor.cleanup();
    mMemRegion.deleteRegion();
    lbl_eu_806642E8 = 0;
}

// ---------------------------------------------------------------------------
// CMenuSelectShop::Move  (us-8018b6e0)
// Gate prefix (CTaskGame busy / global bit21 / scene-active / shop windows
// open), then dispatch on mState and update the shop window + cursor.
// ---------------------------------------------------------------------------
void CMenuSelectShop::Move() {
    // Single short-circuit OR per gate pair so MWCC emits the retail branch
    // shapes: first test -> bne exit, bit test -> beq continue / b exit
    // (CMenuShopBuy::Move / CSystemWindow::Move shape).
    if (CTaskGame::getInstance()->isFlag01Set() ||
        (gmPauseFlags & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;
    if (ShopBuy_HasInstance() || MenuShopSellIsCreated())
        return;

    // Compare-chain switch (retail groups all tests, then the case bodies).
    switch (mState) {
    case 0:
        ShopSel_State0Show(this);
        break;
    case 1:
        ShopSel_State1Cursor(this);
        break;
    case 2:
        ShopSel_State2Input(this);
        break;
    case 3:
        ShopSel_State3Done(this);
        break;
    }

    mSelShop.driveShopWinAnim();
    mCursor.Update();
}

// ---------------------------------------------------------------------------
// ShopSel_State0Show  (us-8018b7b4)
// When the shop window has finished loading, start its show animation and flag
// the menu state.
// ---------------------------------------------------------------------------
extern "C" void ShopSel_State0Show(CMenuSelectShop* self) {
    if (self->mSelShop.isShopWinLoaded() != 0) {
        self->mSelShop.showShopWin();
        self->mState = 1;
    }
}

// ---------------------------------------------------------------------------
// ShopSel_State1Cursor  (us-8018b7fc)
// When the shop window animation is running: activate the cursor, resolve the
// selected item's screen position into a stack VEC3 (getShopWinPanePos), hand it to
// the cursor move virtual, and advance the menu state.
// ---------------------------------------------------------------------------
extern "C" void ShopSel_State1Cursor(CMenuSelectShop* self) {
    if (self->mSelShop.isShopWinAnimActive() != 0) {
        self->mCursor.SetVisible(1);
        nw4r::math::VEC3 out;
        getShopWinPanePos(&out, &self->mSelShop, (u8)self->mSelIndex);
        self->mCursor.setRootPaneTranslate(&out);
        self->mState = 2;
    }
}

// ---------------------------------------------------------------------------
// ShopSel_State2Input  (us-8018b874)
// Select-shop input state 2 (interactive). Reads the current CfPadData once;
// which dpad bit positions to test depends on the controller type (retail
// re-runs the extraction per type). Dispatch order: dir button (turbo & 0x8004)
// moves the selection LEFT, the cancel button (turbo bits 16|3) moves RIGHT,
// then dpad OK accepts the selection and the other dpad direction closes the
// shop. The cancel mask 0x10008 is non-contiguous, so MWCC emits rlwinm(15,15)
// + rlwimi(28,28); the dpad bits are MSB-relative in retail (extrwi 1,10/1,9
// and 1,27/1,26), i.e. LSB bits 21/22 and 4/5.
// ---------------------------------------------------------------------------
extern "C" void ShopSel_State2Input(CMenuSelectShop* self) {
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();

    u32 dirButton, cancelButton, okButton, closeButton;
    // Direct C-ABI call: the retail symbol is the no-arg Fv form but the call
    // site still loads r3=-1 (the s16 overload's arg). Calling the C-ABI decl
    // (which takes an int) reproduces the li.
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        // p declared first so the register allocator gives it r0 (retail
        // lwz r0, 4(r30)); f lands in r5.
        u32 p = pad->mPad.mPressedButtonFlags;
        u32 f = pad->mTurboPressButtonFlags;
        dirButton = (f & 0x8004) != 0;
        cancelButton = (f & 0x10008) != 0;
        okButton = (p >> 21) & 1;
        closeButton = (p >> 22) & 1;
    } else {
        u32 p = pad->mPad.mPressedButtonFlags;
        u32 f = pad->mTurboPressButtonFlags;
        dirButton = (f & 0x8004) != 0;
        cancelButton = (f & 0x10008) != 0;
        okButton = (p >> 4) & 1;
        closeButton = (p >> 5) & 1;
    }

    if (dirButton != 0) {
        // DPAD left: decrease the selection, wrapping 0 -> 2.
        nw4r::math::VEC3 out;
        s32 v = (s32)self->mSelIndex - 1;
        self->mSelIndex = (u32)v;
        if (v < 0) self->mSelIndex = 2;
        playUISound(1);
        getShopWinPanePos(&out, &self->mSelShop, (u8)self->mSelIndex);
        self->mCursor.setRootPaneTranslate(&out);
    } else if (cancelButton != 0) {
        // DPAD right: increase the selection, wrapping 2 -> 0.
        nw4r::math::VEC3 out;
        s32 v = (s32)self->mSelIndex + 1;
        self->mSelIndex = (u32)v;
        if (v > 2) self->mSelIndex = 0;
        playUISound(1);
        getShopWinPanePos(&out, &self->mSelShop, (u8)self->mSelIndex);
        self->mCursor.setRootPaneTranslate(&out);
    } else if (okButton != 0) {
        // Confirm: act on the selected shop entry (signed compare like retail).
        switch ((s32)self->mSelIndex) {
        case 0:
            CUICfManager_queueShopBuyMenu(self->mFieldD0);
            playUISound(3);
            break;
        case 1:
            CUICfManager_queueShopSellMenu();
            playUISound(3);
            break;
        case 2:
            self->mState = 3;
            self->mSelShop.hideShopWin();
            self->mCursor.SetVisible(0);
            break;
        }
    } else if (closeButton != 0) {
        // Other dpad direction: close the shop window and leave this state.
        self->mState = 3;
        self->mSelShop.hideShopWin();
        self->mCursor.SetVisible(0);
    }
}

// ---------------------------------------------------------------------------
// ShopSel_State3Done  (us-8018ba54)
// When the shop window animation is running, mark the menu state and the field
// 0x64 busy flag.
// ---------------------------------------------------------------------------
extern "C" void ShopSel_State3Done(CMenuSelectShop* self) {
    if (self->mSelShop.isShopWinAnimActive() != 0) {
        self->mState = 4;
        self->mField64 = 1;
    }
}

// ---------------------------------------------------------------------------
// CMenuSelectShop::cbRenderBefore  (us-8018ba9c)
// Gate prefix (CTaskGame busy / global bit21 / scene-active) then draw the
// embedded shop window and (when a selection is present) the cursor through a
// stack DrawInfo.
// ---------------------------------------------------------------------------
void CMenuSelectShop::cbRenderBefore(CScn*) {
    // Single short-circuit OR gate pair (CMenuShopBuy / CSystemWindow shape).
    if (CTaskGame::getInstance()->isFlag01Set() ||
        (gmPauseFlags & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    mSelShop.drawShopWin(&drawInfo);
    if ((s32)mSelIndex >= 0)
        mCursor.DrawLayout(&drawInfo);
}

extern "C" u32 ShopSel_GetSingleton() { return (u32)lbl_eu_806642E8; }

// .sbss singleton; retail's slice is 8 bytes (pad word keeps the size).
u32 lbl_eu_806642E8 = 0;
u32 selectShop_sbss_padEC = 0;
