// Auto-scaffolded catalog TU for kyoshin/CUIErrMesWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/CUIErrMesWin.hpp"

// Forward declarations (retail functions defined later in this TU).
void func_802B5148(CErrMesSub* self);
void func_802B515C(CErrMesSub* self, int flag);
void func_802B58A4(CErrMesSub* self);

void func_802B5130(void* self) { ((void(*)(void*))__dt__12CUIErrMesWinFv)((char*)self - 0x54); }


void CUIErrMesWin::Move() {}

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

void func_802B5054(){}

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

extern "C" void Init__10CMenuTitleFv() {}

extern "C" void Term__10CMenuTitleFv() {}

extern "C" void Move__10CMenuTitleFv() {}

extern "C" void cbRenderBefore__10CMenuTitleFv() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// CTTask<T> is declared in kyoshin/CTaskGameEff.hpp (via harness_catalog.hpp);
// specializations below emit the retail Move/Draw/dtor symbols.
template<> void CTTask<CUIErrMesWin>::Move() {}
template<> void CTTask<CUIErrMesWin>::Draw() {}
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
extern "C" void func_802B4B84() {}
