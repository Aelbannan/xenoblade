// Auto-scaffolded catalog TU for kyoshin/CUIErrMesWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/CUIErrMesWin.hpp"
#include "monolib/device/CDeviceVI.hpp"

// Forward declarations (retail functions defined later in this TU).
void func_802B5148(CErrMesSub* self);
void func_802B515C(CErrMesSub* self, int flag);
void func_802B58A4(CErrMesSub* self);

// func_802B4B84 (us-802b75f4) - reset an error-message record, then hand the
// embedded sub-object to func_802B5148. Defined BEFORE func_802B5148's body so
// MWCC keeps the call out-of-line (retail emits `bl func_802B5148`; placing it
// after the stub definition would inline the empty stub away).
void func_802B4B84(CErrMesEntry* self) {
    func_80081E90__Q22cf13CfGameManagerFv(0, 0, 0);
    self->field_0 = 0;
    self->field_4 = 0;
    self->field_8 = 0;
    func_802B5148(&self->sub);
}

void func_802B5130(void* self) { ((void(*)(void*))__dt__12CUIErrMesWinFv)((char*)self - 0x54); }


// Dispatch the window-state ptmf table (lbl_eu_8053AEB0, indexed by the state
// byte), then advance the embedded system window.
void CUIErrMesWin::Move() {
    (this->*lbl_eu_8053AEB0[field_0x9C])();
    func_8022B748(&mSysWin);
}

// CUIErrMesWin::Term (us-802b7768) - detach the render callback, release the
// system window, and clear the global active flag.
void CUIErrMesWin::Term() {
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->removeRenderCB(render);
    func_8022B7F4(&mSysWin);
    lbl_eu_80664C28 = 0;
}

void CUIErrMesWin::cbRenderBefore() {}

void __ct__802B4DF4(){}

void func_802B4EF4(void){}

// func_802B4F00 (us-802b7970) - once the embedded CSysWin is ready, mark the
// window state byte as "ready" (2).
void func_802B4F00(CUIErrMesWin* self) {
    if (CSysWin_isReady(&self->mSysWin)) {
        self->field_0x9C = 2;
    }
}

void func_802B4F40(){}

// func_802B4FA8 (us-802b7a18) - once the embedded CSysWin is active, mark the
// window state byte as "active" (4).
void func_802B4FA8(CUIErrMesWin* self) {
    if (CSysWin_isActive(&self->mSysWin)) {
        self->field_0x9C = 4;
    }
}

void func_802B4FE8(){}

void func_802B5054(CUIErrMesWin* self) {
    if (CSysWin_isActive(&self->mSysWin)) {
        self->field_0x9C = 6;
    }
}

void func_802B5094(void){}


void func_802B5138(void* self) { ((void(*)(void*))cbRenderBefore__12CUIErrMesWinFv)((char*)self - 0x58); }

void func_802B5140(void* self) { ((void(*)(void*))__dt__12CUIErrMesWinFv)((char*)self - 0x58); }

// func_802B5148 (us-802b7bb8) - if the sub-object's active flag is set, run its
// update (func_802B58A4); otherwise do nothing.
void func_802B5148(CErrMesSub* self) {
    if (self->field_0 != 0) {
        func_802B58A4(self);
    }
}

void func_802B515C(CErrMesSub* self, int flag) {}

void func_802B5254(){}

void func_802B58A4(CErrMesSub* self) {}

void func_802B5970(){}

void func_802B5AC8(){}

extern "C" void __dt__10CMenuTitleFv() {}

// CMenuTitle::Init (us-802b8700) - attach the render callback, then initialize
// the +0x60 sub-object.
void CMenuTitle::Init() {
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->addRenderCB(render, 0xd, 1);
    func_802B73D4(&field_0x60[0]);
}

// CMenuTitle::Term (us-802b8750) - wait for draw completion, detach the render
// callback, release the +0x60 sub-object, and clear the global active flag.
void CMenuTitle::Term() {
    CDeviceVI::waitForDrawDone();
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->removeRenderCB(render);
    func_802B74F4(&field_0x60[0]);
    lbl_eu_80664C30 = 0;
}

// Dispatch the state ptmf table (lbl_eu_8053B040, indexed by the +0xE8 state
// byte), then advance the +0x60 sub-object (func_802B744C).
void CMenuTitle::Move() {
    (this->*lbl_eu_8053B040[field_0xE8])();
    func_802B744C(&field_0x60[0]);
}

extern "C" void cbRenderBefore__10CMenuTitleFv() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// CTTask<T> is declared in kyoshin/CTaskGameEff.hpp (via harness_catalog.hpp);
// specializations below emit the retail Move/Draw/dtor symbols.
// Test the ptmf hook at +0x3C / +0x48 and dispatch through __ptmf_scall if
// non-null (static_cast mirrors CTTask.hpp's inline body).
// CTTask<CUIErrMesWin>::~CTTask (us-802b7644) - empty body; MWCC emits the
// null check, CProcess base dtor call, and conditional operator delete.
template<>
CTTask<CUIErrMesWin>::~CTTask() {}

template<> void CTTask<CUIErrMesWin>::Move() {
    if (mMoveFunc) {
        (static_cast<CUIErrMesWin*>(this)->*mMoveFunc)();
    }
}

template<> void CTTask<CUIErrMesWin>::Draw() {
    if (mDrawFunc) {
        (static_cast<CUIErrMesWin*>(this)->*mDrawFunc)();
    }
}
class CMenuTitle;
template<> CTTask<CMenuTitle>::~CTTask() {}

// func_802B48A0 (us-802b7310) - reset an error-message record, then hand the
// embedded sub-object to func_802B5148.
void func_802B48A0(CErrMesEntry* self) {
    self->field_0 = 0;
    self->field_4 = 0;
    self->field_8 = 0;
    func_802B5148(&self->sub);
}

// func_802B48B8 (us-802b7328) - forward the record's sub-object to
// func_802B515C, passing 1 if the record is marked active (byte 8) in state 2
// (word 4), else 0.
void func_802B48B8(CErrMesEntry* self) {
    int flag = 0;
    if (self->field_8 != 0 && self->field_4 == 2) {
        flag = 1;
    }
    func_802B515C(&self->sub, flag);
}
extern "C" void func_802B48E4() {}
extern "C" void func_802B4968() {}
extern "C" void func_802B4A68() {}
