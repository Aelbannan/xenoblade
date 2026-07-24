// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMenuMakeCrystal
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/makecrystal/CMenuMakeCrystal.hpp"

// LLM-HARNESS-BEGIN: us-80212a78
extern "C" void __ct__CMenuMakeCrystal() {}
// LLM-HARNESS-END: us-80212a78

// LLM-HARNESS-BEGIN: us-80212b38
extern "C" void __dt__16CMenuMakeCrystalFv(void* self) {}
// LLM-HARNESS-END: us-80212b38

// LLM-HARNESS-BEGIN: us-80212ba4
extern "C" void Init__16CMenuMakeCrystalFv() {}
// LLM-HARNESS-END: us-80212ba4

// LLM-HARNESS-BEGIN: us-80212cf4
extern "C" void func_80210E9C() {}
// LLM-HARNESS-END: us-80212cf4

// LLM-HARNESS-BEGIN: us-8021352c
extern "C" void func_802116D4() {}
// LLM-HARNESS-END: us-8021352c

// LLM-HARNESS-BEGIN: us-80213b10
extern "C" void func_80211CB8() {}
// LLM-HARNESS-END: us-80213b10

// LLM-HARNESS-BEGIN: us-80213b44
extern "C" void func_80211CEC() {}
// LLM-HARNESS-END: us-80213b44

// LLM-HARNESS-BEGIN: us-80213fb0
extern "C" void func_80212158() {}
// LLM-HARNESS-END: us-80213fb0

// LLM-HARNESS-BEGIN: us-80214044
extern "C" void Term__16CMenuMakeCrystalFv() {}
// LLM-HARNESS-END: us-80214044

// LLM-HARNESS-BEGIN: us-802140b8
extern "C" void Move__16CMenuMakeCrystalFv() {}
// LLM-HARNESS-END: us-802140b8

// LLM-HARNESS-BEGIN: us-802141d8
extern "C" void cbRenderBefore__16CMenuMakeCrystalFv(void* self) {}
// LLM-HARNESS-END: us-802141d8

// LLM-HARNESS-BEGIN: us-80214264
extern "C" void func_8021240C() {}
// LLM-HARNESS-END: us-80214264

// LLM-HARNESS-BEGIN: us-802142d8
extern "C" void func_80212480() {}
// LLM-HARNESS-END: us-802142d8

// LLM-HARNESS-BEGIN: us-802142e8
extern "C" void func_80212490() {}
// LLM-HARNESS-END: us-802142e8

// LLM-HARNESS-BEGIN: us-80214304
extern "C" void func_802124AC() {}
// LLM-HARNESS-END: us-80214304

// IScnRender vtable this-adjusting thunk for cbRenderBefore.
// IScnRender is a non-primary base at offset 0x58 within CMenuMakeCrystal.
// The thunk converts the subobject pointer (IScnRender*) to the enclosing
// CMenuMakeCrystal* via integer-offset adjustment before forwarding to the
// real override. The incompatible cast to void(*)(CMenuMakeCrystal*)
// prevents MWCC from inlining the empty-stub body (which would collapse
// the thunk to blr).
// LLM-HARNESS-BEGIN: us-8021434c
extern "C" void func_802124F4(IScnRender* self) {
    ((void(*)(CMenuMakeCrystal*))cbRenderBefore__16CMenuMakeCrystalFv)(
        (CMenuMakeCrystal*)((u32)self - 0x58));
}
// LLM-HARNESS-END: us-8021434c

// IScnRender vtable this-adjusting thunk for destructor.
// Adjusts from IScnRender* (at offset 0x58) to full CMenuMakeCrystal* and
// forwards to ~CMenuMakeCrystal(). r4 (deletion flag) is preserved from
// the caller because the cast to void(*)(CMenuMakeCrystal*) declares only
// one parameter, leaving r4 untouched.
// LLM-HARNESS-BEGIN: us-80214354
extern "C" void func_802124FC(IScnRender* self) {
    ((void(*)(CMenuMakeCrystal*))__dt__16CMenuMakeCrystalFv)(
        (CMenuMakeCrystal*)((u32)self - 0x58));
}
// LLM-HARNESS-END: us-80214354
