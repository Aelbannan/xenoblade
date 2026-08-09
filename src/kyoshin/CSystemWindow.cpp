// CSystemWindow - system window process (singleton).
//
// Matching session for unit kyoshin/CSystemWindow.

#include "kyoshin/CSystemWindow.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include <revolution/gx/GXPixel.h>

// Retail declares the constructor as a C-ABI global `__ct__CSystemWindow` (the
// split symbol carries no class-length mangling), so it is kept as a plain
// global function rather than a member ctor. Its body is matched elsewhere.
extern "C" CSystemWindow* __ct__CSystemWindow(CSystemWindow* self, void* arg1,
                                              void* arg2, void* arg3, void* arg4) {
    // Placeholder/black-box body. Not a matching target - it only needs a real
    // out-of-line call that consumes all four construction args and returns
    // `this` (the MWCC ctor ABI returns the object pointer in r3), so
    // func_80124AEC preserves/forwards args and keeps `obj` in r3 like retail.
    CProcess* proc = reinterpret_cast<CProcess*>(self);
    __ct__8CProcessFv(proc);
    self->mWorkEvent = (u32)arg1;
    self->mScnRender = (u32)arg2;
    self->field_B5 = (u8)((u32)arg3);
    self->mFlag2B6 = (u8)((u32)arg4);
    return self;
}

CSystemWindow::~CSystemWindow() {
    // Destroy the embedded CSysWin storage (C-linkage dtor; -1 = delete scalar).
    __dt__7CSysWinFv(&mSysWin[0], -1);
    // CProcess base is destroyed implicitly here (offset 0).
}

void CSystemWindow::Init() {
    u32 w = lbl_eu_80663E24;
    mFlag2B6 = (w >> 1) & 1;
    if (mFlag2B6 == 0)
        func_8008294C__Q22cf13CfGameManagerFv(true);

    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->addRenderCB(render, 0xd, 0);

    // Rebuild the embedded CSysWin storage: construct a temp, copy the body
    // (member vtable at +0 is already set and is intentionally not copied),
    // then destroy the temp. Finally call CSysWin virtual slot 0x88.
    u8 temp[0x3A];
    __ct__CSysWin(temp, field_B5);
    // (body copy elided placeholder -- see rematch)
    __dt__7CSysWinFv(temp, -1);

    u32 vt = *(u32*)&mSysWin[0];
    ((void (*)(void*))*(u32*)(vt + 0x88))(&mSysWin[0]);
}

void CSystemWindow::Term() {
    CDeviceVI::waitForDrawDone();
    func_8022B7F4(&mSysWin[0]);

    lbl_eu_80663FD8 = 0;

    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->removeRenderCB(render);

    if (mFlag2B6 == 0)
        func_8008294C__Q22cf13CfGameManagerFv(false);
}

void CSystemWindow::Move() {
    if (CTaskGame::getInstance()->func_800426F0()) return;
    if (lbl_eu_80663E28 & 0x200000) return;
    if (func_8013BE50() == 0) return;

    switch (mState) {
    case 0: {
        // Window just became ready: load strings and start animation.
        if (CSysWin_isReady(&mSysWin[0])) {
            mState = 1;
            func_8022B9B4(&mSysWin[0], &mStr1[0], &mStr2[0]);
            func_8022BFC8(&mSysWin[0], 1);
            func_8022B8B8(&mSysWin[0]);
        }
        break;
    }
    case 1:
        // Wait for the system window to become active.
        if (CSysWin_isActive(&mSysWin[0])) {
            mState = 2;
        }
        break;
    case 2: {
        CPad* pad = cf::CfGameManager::getCurrentPad();
        int confirmBtn;
        // In co-op, the player-specific button (Classic A); otherwise Wiimote A.
        if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0)
            confirmBtn = (pad->mPressedButtonFlags >> 21) & 1;
        else
            confirmBtn = (pad->mPressedButtonFlags >> 4) & 1;
        if (confirmBtn != 0) {
            mState = 3;
            func_8022B8E4(&mSysWin[0]);
            func_80138078__FUl(3);
        }
        break;
    }
    case 3:
        // Closing transition finished - hand control back to the UI.
        if (CSysWin_isActive(&mSysWin[0])) {
            field_67 = 0;
            field_64 = 1;
        }
        break;
    }

    func_8022B748(&mSysWin[0]);
}


void CSystemWindow::cbRenderBefore() {
    CTaskGame* taskGame = CTaskGame::getInstance();
    if (taskGame->func_800426F0() == 0) {
        if ((lbl_eu_80663E28 & 0x200000) == 0) {
            if (func_8013BE50() != 0) {
                GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
                nw4r::lyt::DrawInfo drawInfo;
                func_80137250(&drawInfo);
                func_8022B7C8(&mSysWin[0], &drawInfo);
                drawInfo.~DrawInfo();
            }
        }
    }
}

// Creates the singleton CSystemWindow on the work heap and registers it as a
// CProcess under `parent`. Returns 0 if the singleton already exists.
CSystemWindow* func_80124AEC(CProcess* parent, void* arg1, void* arg2,
                             void* arg3, void* arg4) {
    if (lbl_eu_80663FD8 != 0) return 0;

    u32 workMem = CWorkThreadSystem::getWorkMem();
    CSystemWindow* obj =
        (CSystemWindow*)mtl::MemManager::allocate(0x2b8, workMem);
    if (obj != 0) {
        obj = __ct__CSystemWindow(obj, arg1, arg2, arg3, arg4);
    }
    lbl_eu_80663FD8 = obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, 0);
    return lbl_eu_80663FD8;
}

/* Returns the singleton CSystemWindow instance (lbl_eu_80663FD8 in .sbss). */
CSystemWindow* func_80124B78() {
    return lbl_eu_80663FD8;
}