// Auto-scaffolded catalog TU for kyoshin/menu/CMenuMapSelect
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuMapSelect.hpp"

// LLM-HARNESS-BEGIN: us-802443d4
extern "C" void __ct__CMenuMapSelect() {}
// LLM-HARNESS-END: us-802443d4

// LLM-HARNESS-BEGIN: us-8024450c
extern "C" void func_80242354() {}
// LLM-HARNESS-END: us-8024450c

// LLM-HARNESS-BEGIN: us-80244520
extern "C" void func_80242368() {}
// LLM-HARNESS-END: us-80244520

// LLM-HARNESS-BEGIN: us-802446dc
extern "C" void func_80242524() {}
// LLM-HARNESS-END: us-802446dc

// LLM-HARNESS-BEGIN: us-80244be0
extern "C" void func_80242A28() {}
// LLM-HARNESS-END: us-80244be0

// IScnRender vtable this-adjusting thunk for cbRenderBefore.
// IScnRender is a non-primary base at offset 0x58 within CMenuMapSelect.
// The thunk converts the subobject pointer (IScnRender*) to the enclosing
// CMenuMapSelect* via integer-offset adjustment before forwarding to the
// real override. The function-pointer cast prevents MWCC from null-checking
// the static_cast chain (which would add a cmpwi/beq that the retail thunk
// does not have).
extern "C" void cbRenderBefore__14CMenuMapSelectFv(CMenuMapSelect* self);
// LLM-HARNESS-BEGIN: us-80245254
extern "C" void func_8024309C(IScnRender* self) {
    ((void(*)(CMenuMapSelect*))cbRenderBefore__14CMenuMapSelectFv)(
        (CMenuMapSelect*)((u32)self - 0x58));
}
// LLM-HARNESS-END: us-80245254

// IScnRender vtable this-adjusting thunk for destructor.
// Adjusts from IScnRender* (at offset 0x58) to full CMenuMapSelect* and
// forwards to ~CMenuMapSelect(). r4 (deletion flag) is preserved from the
// caller because the cast to void(*)(CMenuMapSelect*) declares only one
// parameter, leaving r4 untouched.
extern "C" void __dt__14CMenuMapSelectFv(CMenuMapSelect* self, int deletionFlag);
// LLM-HARNESS-BEGIN: us-8024525c
extern "C" void func_802430A4(IScnRender* self) {
    ((void(*)(CMenuMapSelect*))__dt__14CMenuMapSelectFv)(
        (CMenuMapSelect*)((u32)self - 0x58));
}
// LLM-HARNESS-END: us-8024525c
