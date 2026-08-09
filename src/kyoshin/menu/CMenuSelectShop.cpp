// Auto-scaffolded catalog TU for kyoshin/menu/CMenuSelectShop.
// The retail select-shop menu object is laid out as a plain struct with the
// IUIWindow region inline; its ctor/dtor and member hooks are emitted under the
// retail symbol names.

#include <types.h>

#include "monolib/device/CDeviceVI.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfPadData.hpp"
#include <revolution/gx/GXPixel.h>
#include <revolution/gx/GXTypes.h>
#include <nw4r/lyt/lyt_drawInfo.h>

#include "kyoshin/menu/CMenuSelectShop.hpp"

// ---------------------------------------------------------------------------
// Target 4: CMenuSelectShop ctor (us-8018b39c)
// CProcess base + IUIWindow region (temp vtable + __ptmf_null callbacks + scalar
// defaults), then the composite vtable / OC / render slots, the embedded
// subobjects and scalar state. `scene` goes to mScene, `a3` to mFieldD0.
// ---------------------------------------------------------------------------
extern "C" CMenuSelectShop* __ct__CMenuSelectShop(CMenuSelectShop* self,
                                                  void* scene, void* a3) {
    __ct__8CProcessFv(self);

    self->mVtab = (u32)lbl_eu_8052D238;
    self->mPtmfFunc0 = __ptmf_null[0];
    self->mPtmfThis0 = __ptmf_null[1];
    self->mPtmfDelta0 = __ptmf_null[2];
    self->mPtmfFunc1 = __ptmf_null[0];
    self->mPtmfThis1 = __ptmf_null[1];
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

    self->mVtab = (u32)lbl_eu_80532550;
    self->mOcc6C = (u32)lbl_eu_80532550 + 0x24;
    self->mOcc70 = (u32)lbl_eu_80532550 + 0xac;
    self->mScene = (CScn*)scene;

    __ct__17UnkClass_8045F564Fv(&self->mMemRegion);
    __ct__CSelShopWin(&self->mSelShop);
    __ct__CCur18(&self->mCursor[0], 0);

    self->mState = 0;
    self->mSelIndex = 0;
    self->mFieldD0 = (u32)a3;
    return self;
}

// ---------------------------------------------------------------------------
// Target 1: func_8018A58C  (us-8018bb40)
// Factory: allocate the CMenuSelectShop singleton from work memory, run its
// ctor, store it in the .sbss global and register it as a CProcess child of
// `parent`. Returns 0 when the singleton already exists.
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// CMenuSelectShop::~CMenuSelectShop()  (us-8018b48c)
// Complete-object dtor with delete flag. Subobjects are destroyed in reverse
// construction order: CCur18@0xB0, CSelShopWin@0x88, UnkClass_8045F564@0x78,
// then the IUIWindow base. The nested flag dispatch selects operator delete.
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// Target 5: CMenuSelectShop::Init  (us-8018b504)
// Create the memory region, then build the shop window + cursor on the stack,
// copy them into their payload slots (skipping each zero-relative vtable),
// destroy the temporaries and register the render callback.
// ---------------------------------------------------------------------------
void CMenuSelectShop::Init() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    mMemRegion.createRegion(handle, 0x2000, (const char*)lbl_eu_80503AE0, 0);

    u8 rawRegion[0x8];
    __ct__14Class_8045F858FP17UnkClass_8045F564(
        reinterpret_cast<Class_8045F858*>(rawRegion), &mMemRegion);

    // Build the shop window widget on the stack, copy its payload (+0x4 .. skip
    // the vtable) into the member, then destroy the temporary.
    u8 rawShop[0x28];
    CSelShopWin* tempShop = reinterpret_cast<CSelShopWin*>(rawShop);
    __ct__CSelShopWin(tempShop);
    mSelShop.mMemRegion = tempShop->mMemRegion;
    mSelShop.mFileHandle = tempShop->mFileHandle;
    mSelShop.mAccessor = tempShop->mAccessor;
    mSelShop.mLayout = tempShop->mLayout;
    mSelShop.mAnimTransform = tempShop->mAnimTransform;
    mSelShop.mIsLayoutBuilt = tempShop->mIsLayoutBuilt;
    mSelShop.mIsLoaded = tempShop->mIsLoaded;
    mSelShop.mAnimState = tempShop->mAnimState;
    mSelShop.mAnimActive = tempShop->mAnimActive;
    __dt__11CSelShopWinFv(tempShop, -1);

    func_8022C770(&mSelShop);

    // Same pattern for the CCur18 cursor (payload +0x4..+0x15).
    void* accessor = func_801355F4();
    u8 rawCur[0x18];
    __ct__CCur18(&rawCur[0], accessor);
    CCur18Data* dst = reinterpret_cast<CCur18Data*>(&mCursor[4]);
    CCur18Data* src = reinterpret_cast<CCur18Data*>(&rawCur[4]);
    dst->m04 = src->m04;
    dst->m08 = src->m08;
    dst->m0C = src->m0C;
    dst->m10 = src->m10;
    dst->m14 = src->m14;
    dst->m15 = src->m15;
    __dt__6CCur18Fv(&rawCur[0], -1);

    reinterpret_cast<CCursor18*>(&mCursor[0])->vf2();

    func_80138078__FUl(0x29);

    void* render = this;
    if (this) render = &mOcc70;
    addRenderCB__4CScnFP10IScnRenderUlUl(mScene, render, 0xd, 0);

    mMemRegion.func_8045F810();
    __dt__14Class_8045F858Fv(reinterpret_cast<Class_8045F858*>(rawRegion), -1);
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
// Target 3: CMenuSelectShop::Move  (us-8018b6e0)
// Gate prefix (CTaskGame busy / global bit21 / scene-active / shop windows
// open), then dispatch on mState and update the shop window + cursor.
// ---------------------------------------------------------------------------
void CMenuSelectShop::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() != 0) return;
    if (lbl_eu_80663E28 & 0x200000) return;
    if (!func_8013BE50()) return;
    if (func_8018C180()) return;
    if (func_8018B398()) return;

    if (mState == 0) func_8018A200(this);
    else if (mState == 1) func_8018A248(this);
    else if (mState == 2) func_8018A2C0(this);
    else if (mState == 3) func_8018A4A0(this);

    func_8022C7C0(&mSelShop);
    func_801D202C(&mCursor[0]);
}

// ---------------------------------------------------------------------------
// Target 1: func_8018A200  (us-8018b7b4)
// When the shop window has finished loading, start its show animation and flag
// the menu state.
// ---------------------------------------------------------------------------
extern "C" void func_8018A200(CMenuSelectShop* self) {
    if (func_8022C8D0(&self->mSelShop) != 0) {
        func_8022C8E0(&self->mSelShop);
        self->mState = 1;
    }
}

// ---------------------------------------------------------------------------
// Target 5: func_8018A248  (us-8018b7fc)
// When the shop window animation is running: activate the cursor, resolve the
// selected item's screen position into a stack VEC3 (func_8022C930), hand it to
// the cursor move virtual, and advance the menu state.
// ---------------------------------------------------------------------------
extern "C" void func_8018A248(CMenuSelectShop* self) {
    if (func_8022C8D8(&self->mSelShop) != 0) {
        func_801D216C(&self->mCursor[0], 1);
        nw4r::math::VEC3 out;
        func_8022C930(&out, &self->mSelShop, (u8)self->mSelIndex);
        reinterpret_cast<CCur18View*>(&self->mCursor[0])->vf04(&out);
        self->mState = 2;
    }
}

// ---------------------------------------------------------------------------
// Target 1: func_8018A2C0  (us-8018b874)
// Select-shop input state 2 (interactive). Reads the current CfPadData once;
// which dpad bit positions to test depends on the controller type (retail
// re-runs the extraction per type). Dispatch order: dir button (turbo & 0x8004)
// moves the selection LEFT, the cancel button (turbo bits 16|3) moves RIGHT,
// then dpad OK accepts the selection and the other dpad direction closes the
// shop. The cancel mask 0x10008 is non-contiguous, so MWCC emits rlwinm(15,15)
// + rlwimi(28,28); the dpad bits are MSB-relative in retail (extrwi 1,10/1,9
// and 1,27/1,26), i.e. LSB bits 21/22 and 4/5.
// ---------------------------------------------------------------------------
extern "C" void func_8018A2C0(CMenuSelectShop* self) {
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();

    u32 dirButton, cancelButton, okButton, closeButton;
    if (cf::CfGameManager::func_80086F9C(-1) != 0) {
        u32 f = pad->mTurboPressButtonFlags;
        u32 p = pad->mPad.mPressedButtonFlags;
        dirButton = (f & 0x8004) != 0;
        cancelButton = (f & 0x10008) != 0;
        okButton = (p >> 21) & 1;
        closeButton = (p >> 22) & 1;
    } else {
        u32 f = pad->mTurboPressButtonFlags;
        u32 p = pad->mPad.mPressedButtonFlags;
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
        // Confirm: act on the selected shop entry.
        switch (self->mSelIndex) {
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

// ---------------------------------------------------------------------------
// Target 2: func_8018A4A0  (us-8018ba54)
// When the shop window animation is running, mark the menu state and the field
// 0x64 busy flag.
// ---------------------------------------------------------------------------
extern "C" void func_8018A4A0(CMenuSelectShop* self) {
    if (func_8022C8D8(&self->mSelShop) != 0) {
        self->mState = 4;
        self->mField64 = 1;
    }
}

// ---------------------------------------------------------------------------
// Target 2: CMenuSelectShop::cbRenderBefore  (us-8018ba9c)
// Gate prefix (CTaskGame busy / global bit21 / scene-active) then draw the
// embedded shop window and (when a selection is present) the cursor through a
// stack DrawInfo.
// ---------------------------------------------------------------------------
extern "C" void cbRenderBefore__15CMenuSelectShopFv(void*);
void CMenuSelectShop::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() != 0) return;
    if (lbl_eu_80663E28 & 0x200000) return;
    if (!func_8013BE50()) return;
    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250__FPQ34nw4r3lyt8DrawInfo(&drawInfo);
        func_8022C830(&mSelShop, &drawInfo);
        if ((s32)mSelIndex >= 0)
            func_801D20B0(&mCursor[0], &drawInfo);
        __dt__Q34nw4r3lyt8DrawInfoFv(&drawInfo, -1);
    }
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