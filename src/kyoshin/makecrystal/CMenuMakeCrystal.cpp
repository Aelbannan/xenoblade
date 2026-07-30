// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMenuMakeCrystal
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/makecrystal/CMenuMakeCrystal.hpp"


void __ct__CMenuMakeCrystal(){}

CMenuMakeCrystal::~CMenuMakeCrystal() {}

void CMenuMakeCrystal::Init() {}

void func_80210E9C(){}

void func_802116D4(){}

void func_80211CB8(){}

void func_80211CEC(){}

void func_80212158(){}

void CMenuMakeCrystal::Term() {}

void CMenuMakeCrystal::Move() {}

void CMenuMakeCrystal::cbRenderBefore() {}

void func_8021240C(){}

void func_80212480(){}

void* lbl_eu_806646C8;
void* func_80212BE0(void*);

void* func_80212490() {
    unsigned char* p = static_cast<unsigned char*>(lbl_eu_806646C8);
    if (p != 0) {
        return func_80212BE0(p + 0x80);
    }
    return 0;
}

void func_802124AC(){}

// IScnRender vtable this-adjusting thunk for cbRenderBefore.
// IScnRender is a non-primary base at offset 0x58 within CMenuMakeCrystal.
void CMenuMakeCrystal::func_802124F4() {
    cbRenderBefore();
}

// IScnRender vtable this-adjusting thunk for destructor.
void CMenuMakeCrystal::func_802124FC() {
    this->~CMenuMakeCrystal();
}
