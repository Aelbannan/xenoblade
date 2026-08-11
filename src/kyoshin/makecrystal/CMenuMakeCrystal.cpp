// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMenuMakeCrystal
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/makecrystal/CMenuMakeCrystal.hpp"

extern "C" void __dt__16CMenuMakeCrystalFv(void*, int);
extern "C" void cbRenderBefore__16CMenuMakeCrystalFv(void*);


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

void* lbl_eu_806646C8;

// (lbl_eu_806646C8 != 0)
extern "C" bool func_80212480() { return lbl_eu_806646C8 != 0; }
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
extern "C" void func_802124F4(void* self) {
    ((void(*)(void*))cbRenderBefore__16CMenuMakeCrystalFv)((char*)self - 0x58);
}

// IScnRender vtable this-adjusting thunk for destructor.
extern "C" void func_802124FC(void* self) {
    ((void(*)(void*))__dt__16CMenuMakeCrystalFv)((char*)self - 0x58);
}
