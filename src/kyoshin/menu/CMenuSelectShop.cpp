// Auto-scaffolded catalog TU for kyoshin/menu/CMenuSelectShop.
// The retail select-shop menu object is laid out as a plain struct with the
// IUIWindow region inline; its ctor/dtor and member hooks are emitted under the
// retail symbol names.

#include <types.h>

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

// ---------------------------------------------------------------------------
// func_8018A58C  (us-8018bb40)
// Factory: allocate the CMenuSelectShop singleton from work memory, run its
// ctor, store it in the .sbss global and register it as a CProcess child of
// `parent`. Returns 0 when the singleton already exists.
// optimize_for_size merges the r29-r31 callee-save set into retail stmw r29.
// ---------------------------------------------------------------------------
#pragma push
#pragma optimize_for_size on
extern "C" CMenuSelectShop* func_8018A58C(CProcess* parent, void* a2,
                                          void* a3) {
    if (lbl_eu_806642E8 != 0)
        return 0;

    mtl::ALLOC_HANDLE workMem = CWorkThreadSystem::getWorkMem();
    CMenuSelectShop* obj =
        (CMenuSelectShop*)mtl::MemManager::allocate(0xd4, workMem);
    if (obj != 0) {
        obj = __ct__CMenuSelectShop(obj, a2, a3);
    }
    lbl_eu_806642E8 = (u32)obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, 0);
    return (CMenuSelectShop*)lbl_eu_806642E8;
}
#pragma pop

// ---------------------------------------------------------------------------
// CMenuSelectShop ctor (us-8018b39c)
// CProcess base + IUIWindow region (temp vtable + __ptmf_null callbacks +
// scalar defaults), then the composite vtable / OC / render slots, the
// embedded subobjects and scalar state. `scene` goes to mScene, `a3` to
// mFieldD0. Written in the CMenuShopBuy ctor shape: direct two-stage vtable
// stores (no cached composite-vtable local) so MWCC allocates the retail
// r28-r30 callee-save set and emits the lwzu/lwz ptmf load pair.
// optimize_for_size merges the r28-r31 callee-save set into retail stmw r28.
// __declspec(noinline) keeps -inline auto from inlining it into the factory
// (optimize_for_size flips MWCC's inline decision; auto_inline off does not
// override it).
// ---------------------------------------------------------------------------
#pragma push
#pragma optimize_for_size on

extern "C" __declspec(noinline) CMenuSelectShop* __ct__CMenuSelectShop(CMenuSelectShop* self,
                                                  void* scene, void* a3) {
    __ct__8CProcessFv(self);

    // Interface sub-table stores referenced here so the composite vtable
    // address is materialized before the __ptmf_null base.
    self->mVtab = (u32)lbl_eu_8052D238;
    self->mOcc6C = (u32)lbl_eu_80532550 + 0x24;
    self->mOcc70 = (u32)lbl_eu_80532550 + 0xAC;

    // Retail stores the second slot of each null-ptmf pair first.
    self->mPtmfThis0 = __ptmf_null[1];
    self->mPtmfFunc0 = __ptmf_null[0];
    self->mPtmfDelta0 = __ptmf_null[2];
    self->mPtmfThis1 = __ptmf_null[1];
    self->mPtmfFunc1 = __ptmf_null[0];
    self->mPtmfDelta1 = __ptmf_null[2];
    self->mLayout = 0;
    self->mField58 = 0;
    self->mField5C = 0;
    self->mField60 = -1;
    self->mField64 = 0;
    self->mField65 = 0;
    self->mField66 = 0;
    self->mField67 = 1;
    self->mField68 = 0;

    // Derived-class overrides: composite vtable + interface sub-tables.
    self->mVtab = (u32)lbl_eu_80532550;
    self->mScene = (CScn*)scene;

    __ct__17UnkClass_8045F564Fv(&self->mMemRegion);
    __ct__CSelShopWin(&self->mSelShop);
    __ct__CCur18(&self->mCursor[0], 0);

    self->mState = 0;
    self->mSelIndex = 0;
    self->mFieldD0 = (u32)a3;
    return self;
}
#pragma pop

// ---------------------------------------------------------------------------
// CMenuSelectShop::~CMenuSelectShop()  (us-8018b48c)
// Complete-object dtor with delete flag. Subobjects are destroyed in reverse
// construction order: CCur18@0xB0, CSelShopWin@0x88, UnkClass_8045F564@0x78,
// then the IUIWindow base. The nested flag dispatch selects operator delete.
// optimize_for_size merges the r30/r31 callee-save pair into the retail
// stmw r30,8(sp) block (MWCC_CASES stmw/lmw dtor pattern).
// ---------------------------------------------------------------------------
#pragma push
#pragma optimize_for_size on
extern "C" CMenuSelectShop* __dt__15CMenuSelectShopFv(CMenuSelectShop* self, int flags) {
    if (self != 0) {
        __dt__6CCur18Fv(&self->mCursor[0], -1);
        __dt__11CSelShopWinFv(&self->mSelShop, -1);
        __dt__17UnkClass_8045F564Fv(&self->mMemRegion, -1);
        __dt__9IUIWindowFv(self, 0);
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}
#pragma pop

// ---------------------------------------------------------------------------
// CMenuSelectShop::Init  (us-8018b504)
// Create the memory region, then build the shop window + cursor on the stack,
// copy them into their payload slots (skipping each zero-relative vtable),
// destroy the temporaries and register the render callback.
// ---------------------------------------------------------------------------
void CMenuSelectShop::Init() {
    mMemRegion.createRegion(mtl::MemManager::getHandleMEM2(), 0x2000,
                            (const char*)lbl_eu_80503AE0, 0);

    // RAII scratch-region guard, destroyed at the end of this scope (retail
    // stack slot sp+0x8).
    Class_8045F858 region(&mMemRegion);

    // Rebuild the embedded shop window: construct a temp at sp+0x28, copy its
    // payload (all non-vtable fields), destroy the temp, then start loading
    // the window resources. The temp pointers are derived after the ctor so
    // MWCC folds the accesses to r1-relative addressing (CSysWinSave::Init
    // shape) instead of holding a callee-saved base register.
    u8 rawShop[0x28];
    __ct__CSelShopWin(reinterpret_cast<CSelShopWin*>(rawShop));
    CSelShopWin* tempShop = reinterpret_cast<CSelShopWin*>(rawShop);
    mSelShop.mMemRegion = tempShop->mMemRegion;
    mSelShop.mFileHandle = tempShop->mFileHandle;
    mSelShop.mAccessor = tempShop->mAccessor;
    mSelShop.mLayout = tempShop->mLayout;
    mSelShop.mAnimTransform = tempShop->mAnimTransform;
    mSelShop.mIsLayoutBuilt = tempShop->mIsLayoutBuilt;
    mSelShop.mIsLoaded = tempShop->mIsLoaded;
    mSelShop.mAnimState = tempShop->mAnimState;
    mSelShop.mAnimActive = tempShop->mAnimActive;
    __dt__11CSelShopWinFv(rawShop, -1);

    func_8022C770(&mSelShop);

    // Same pattern for the CCur18 cursor (payload +0x4..+0x15, slot sp+0x10).
    u8 rawCur[0x18];
    __ct__CCur18(rawCur, func_801355F4());
    // CCur18Data models the payload tail (+0x4..+0x15), so the pointers start
    // 4 bytes into each cursor image (vtable skipped).
    CCur18Data* dst = reinterpret_cast<CCur18Data*>(&mCursor[4]);
    CCur18Data* src = reinterpret_cast<CCur18Data*>(&rawCur[4]);
    dst->m04 = src->m04;
    dst->m08 = src->m08;
    dst->m0C = src->m0C;
    dst->m10 = src->m10;
    dst->m14 = src->m14;
    dst->m15 = src->m15;
    __dt__6CCur18Fv(rawCur, -1);

    reinterpret_cast<CCursor18*>(&mCursor[0])->vf2();

    func_80138078__FUl(0x29);

    void* render = this;
    if (this) render = &mOcc70;
    addRenderCB__4CScnFP10IScnRenderUlUl(mScene, render, 0xd, 0);

    mMemRegion.func_8045F810();
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

    void* render = this;
    if (this) render = &mOcc70;
    removeRenderCB__4CScnFP10IScnRender(mScene, render);

    func_8022C85C(&mSelShop);
    reinterpret_cast<CCursor18*>(&mCursor[0])->vf3();
    mMemRegion.func_8045F778();
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
    if (isFlag01Set__9CTaskGameFv(getInstance__9CTaskGameFv()) ||
        (lbl_eu_80663E28 & 0x200000))
        return;
    if (func_8013BE50() == 0) return;
    if (func_8018C180() || func_8018B398())
        return;

    // Compare-chain switch (retail groups all tests, then the case bodies).
    switch (mState) {
    case 0:
        func_8018A200(this);
        break;
    case 1:
        func_8018A248(this);
        break;
    case 2:
        func_8018A2C0(this);
        break;
    case 3:
        func_8018A4A0(this);
        break;
    }

    func_8022C7C0(&mSelShop);
    func_801D202C(&mCursor[0]);
}

// ---------------------------------------------------------------------------
// func_8018A200  (us-8018b7b4)
// When the shop window has finished loading, start its show animation and flag
// the menu state. auto_inline off keeps retail's out-of-line `bl` from Move.
// ---------------------------------------------------------------------------
#pragma push
#pragma auto_inline off
extern "C" void func_8018A200(CMenuSelectShop* self) {
    if (func_8022C8D0(&self->mSelShop) != 0) {
        func_8022C8E0(&self->mSelShop);
        self->mState = 1;
    }
}
#pragma pop

// ---------------------------------------------------------------------------
// func_8018A248  (us-8018b7fc)
// When the shop window animation is running: activate the cursor, resolve the
// selected item's screen position into a stack VEC3 (func_8022C930), hand it to
// the cursor move virtual, and advance the menu state.
// ---------------------------------------------------------------------------
#pragma push
#pragma auto_inline off
extern "C" void func_8018A248(CMenuSelectShop* self) {
    if (func_8022C8D8(&self->mSelShop) != 0) {
        func_801D216C(&self->mCursor[0], 1);
        nw4r::math::VEC3 out;
        func_8022C930(&out, &self->mSelShop, (u8)self->mSelIndex);
        reinterpret_cast<CCur18View*>(&self->mCursor[0])->vf04(&out);
        self->mState = 2;
    }
}
#pragma pop

// ---------------------------------------------------------------------------
// func_8018A2C0  (us-8018b874)
// Select-shop input state 2 (interactive). Reads the current CfPadData once;
// which dpad bit positions to test depends on the controller type (retail
// re-runs the extraction per type). Dispatch order: dir button (turbo & 0x8004)
// moves the selection LEFT, the cancel button (turbo bits 16|3) moves RIGHT,
// then dpad OK accepts the selection and the other dpad direction closes the
// shop. The cancel mask 0x10008 is non-contiguous, so MWCC emits rlwinm(15,15)
// + rlwimi(28,28); the dpad bits are MSB-relative in retail (extrwi 1,10/1,9
// and 1,27/1,26), i.e. LSB bits 21/22 and 4/5.
// ---------------------------------------------------------------------------
#pragma push
#pragma auto_inline off
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_8018A2C0(CMenuSelectShop* self) {
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();

    u32 dirButton, cancelButton, okButton, closeButton;
    // Direct C-ABI call: the retail symbol is the no-arg Fv form but the call
    // site still loads r3=-1 (the s16 overload's arg). Calling the C-ABI decl
    // (which takes an int) reproduces the li.
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
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
        func_80138078(1);
        func_8022C930(&out, &self->mSelShop, (u8)self->mSelIndex);
        reinterpret_cast<CCur18View*>(&self->mCursor[0])->vf04(&out);
    } else if (cancelButton != 0) {
        // DPAD right: increase the selection, wrapping 2 -> 0.
        nw4r::math::VEC3 out;
        s32 v = (s32)self->mSelIndex + 1;
        self->mSelIndex = (u32)v;
        if (v > 2) self->mSelIndex = 0;
        func_80138078(1);
        func_8022C930(&out, &self->mSelShop, (u8)self->mSelIndex);
        reinterpret_cast<CCur18View*>(&self->mCursor[0])->vf04(&out);
    } else if (okButton != 0) {
        // Confirm: act on the selected shop entry (signed compare like retail).
        switch ((s32)self->mSelIndex) {
        case 0:
            func_80134024(self->mFieldD0);
            func_80138078(3);
            break;
        case 1:
            func_80134100();
            func_80138078(3);
            break;
        case 2:
            self->mState = 3;
            func_8022C908(&self->mSelShop);
            func_801D216C(&self->mCursor[0], 0);
            break;
        }
    } else if (closeButton != 0) {
        // Other dpad direction: close the shop window and leave this state.
        self->mState = 3;
        func_8022C908(&self->mSelShop);
        func_801D216C(&self->mCursor[0], 0);
    }
}
#pragma pop

// ---------------------------------------------------------------------------
// func_8018A4A0  (us-8018ba54)
// When the shop window animation is running, mark the menu state and the field
// 0x64 busy flag.
// ---------------------------------------------------------------------------
#pragma push
#pragma auto_inline off
extern "C" void func_8018A4A0(CMenuSelectShop* self) {
    if (func_8022C8D8(&self->mSelShop) != 0) {
        self->mState = 4;
        self->mField64 = 1;
    }
}
#pragma pop

// ---------------------------------------------------------------------------
// CMenuSelectShop::cbRenderBefore  (us-8018ba9c)
// Gate prefix (CTaskGame busy / global bit21 / scene-active) then draw the
// embedded shop window and (when a selection is present) the cursor through a
// stack DrawInfo.
// ---------------------------------------------------------------------------
extern "C" void cbRenderBefore__15CMenuSelectShopFv(void*);
void CMenuSelectShop::cbRenderBefore() {
    // Single short-circuit OR gate pair (CMenuShopBuy / CSystemWindow shape).
    if (isFlag01Set__9CTaskGameFv(getInstance__9CTaskGameFv()) ||
        (lbl_eu_80663E28 & 0x200000))
        return;
    if (func_8013BE50() == 0) return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via the C-ABI ct/dt calls so the
    // scope-exit destructor is not auto-emitted (CMenuShopBuy scheme).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0]);
    func_80137250__FPQ34nw4r3lyt8DrawInfo(&drawInfo[0]);
    func_8022C830(&mSelShop, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    if ((s32)mSelIndex >= 0)
        func_801D20B0(&mCursor[0], (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    __dt__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0], -1);
}

extern "C" u32 func_8018A608() { return (u32)lbl_eu_806642E8; }

// OC/render subobject this-adjusting thunks (not match targets). The IScnRender
// subobject sits at +0x70, the IWorkEvent/OC subobject at +0x6C.
void func_8018A610(void* sub) {
    ((void(*)(void*))__dt__15CMenuSelectShopFv)((char*)sub - 0x6c);
}

void func_8018A618(void* sub) {
    ((void(*)(void*))cbRenderBefore__15CMenuSelectShopFv)((char*)sub - 0x70);
}

extern "C" void func_8018A620(void* sub) {
    ((void(*)(void*))__dt__15CMenuSelectShopFv)((char*)sub - 0x70);
}