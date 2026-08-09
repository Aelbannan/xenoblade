// Auto-scaffolded catalog TU for kyoshin/menu/CMenuSave
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/menu/CMenuSave.hpp"

void __ct__CMenuSave(){}

CMenuSave::~CMenuSave() {}

void CMenuSave::Init() {}

void CMenuSave::Term() {}

void CMenuSave::Move() {}

void CMenuSave::cbRenderBefore() {}

void func_8028E3B4(){}

void stub_us_802908bc() {}

void func_8028E450(){}

int func_8028F664(u8*);

int isIdle__11CTitleAHelpFv(u8*);

void func_8028E4E0(u8* this_) {
    if (isIdle__11CTitleAHelpFv(this_ + 0x80) != 0 &&
        func_8028F664(this_ + 0xb8) != 0) {
        this_[0x20b] = 2;
    }
}

void func_8028E530(){}

void func_8028E768(){}

/// Adjusting thunk: called when IScnRender subobject (at CMenuSave+0x58)
/// receives cbRenderBefore.
void CMenuSave::func_8028E7B8() {
    cbRenderBefore();
}

/// Adjusting destructor thunk: called when IScnRender subobject (at CMenuSave+0x58)
/// receives ~CMenuSave.
void CMenuSave::func_8028E7C0() {
    this->~CMenuSave();
}

extern unsigned long lbl_eu_806649E8;
extern "C" unsigned long func_8028E440(void) { return lbl_eu_806649E8 != 0; }
