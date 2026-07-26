// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMenuMakeCrystal
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/makecrystal/CMenuMakeCrystal.hpp"

extern "C" void __ct__CMenuMakeCrystal() {}

extern "C" void __dt__16CMenuMakeCrystalFv(void* self) {}

extern "C" void Init__16CMenuMakeCrystalFv() {}

extern "C" void func_80210E9C() {}

extern "C" void func_802116D4() {}

extern "C" void func_80211CB8() {}

extern "C" void func_80211CEC() {}

extern "C" void func_80212158() {}

extern "C" void Term__16CMenuMakeCrystalFv() {}

extern "C" void Move__16CMenuMakeCrystalFv() {}

extern "C" void cbRenderBefore__16CMenuMakeCrystalFv(void* self) {}

extern "C" void func_8021240C() {}

extern "C" void func_80212480() {}

extern "C" void* lbl_eu_806646C8;
extern "C" void* func_80212BE0(void*);

extern "C" void* func_80212490() {
    char* p = (char*)lbl_eu_806646C8;
    if (p != 0) {
        return func_80212BE0(p + 0x80);
    }
    return 0;
}

extern "C" void func_802124AC() {}

// IScnRender vtable this-adjusting thunk for cbRenderBefore.
// IScnRender is a non-primary base at offset 0x58 within CMenuMakeCrystal.
// The thunk converts the subobject pointer (IScnRender*) to the enclosing
// CMenuMakeCrystal* via integer-offset adjustment before forwarding to the
// real override. The incompatible cast to void(*)(CMenuMakeCrystal*)
// prevents MWCC from inlining the empty-stub body (which would collapse
// the thunk to blr).
extern "C" void func_802124F4(IScnRender* self) {
    ((void(*)(CMenuMakeCrystal*))cbRenderBefore__16CMenuMakeCrystalFv)(
        (CMenuMakeCrystal*)((u32)self - 0x58));
}

// IScnRender vtable this-adjusting thunk for destructor.
// Adjusts from IScnRender* (at offset 0x58) to full CMenuMakeCrystal* and
// forwards to ~CMenuMakeCrystal(). r4 (deletion flag) is preserved from
// the caller because the cast to void(*)(CMenuMakeCrystal*) declares only
// one parameter, leaving r4 untouched.
extern "C" void func_802124FC(IScnRender* self) {
    ((void(*)(CMenuMakeCrystal*))__dt__16CMenuMakeCrystalFv)(
        (CMenuMakeCrystal*)((u32)self - 0x58));
}
