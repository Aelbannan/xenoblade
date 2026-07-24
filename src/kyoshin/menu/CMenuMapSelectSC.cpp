// Auto-scaffolded catalog TU for kyoshin/menu/CMenuMapSelectSC
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuMapSelectSC.hpp"
#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-80252f5c
extern "C" void __ct__CMenuMapSelectSC() {}
// LLM-HARNESS-END: us-80252f5c

// LLM-HARNESS-BEGIN: us-80253028
extern "C" void __dt__16CMenuMapSelectSCFv(void* self, int deletionFlag) {}
// LLM-HARNESS-END: us-80253028

// LLM-HARNESS-BEGIN: us-802530a0
extern "C" void Init__16CMenuMapSelectSCFv() {}
// LLM-HARNESS-END: us-802530a0

// LLM-HARNESS-BEGIN: us-802534cc
extern "C" void func_80251294() {}
// LLM-HARNESS-END: us-802534cc

// LLM-HARNESS-BEGIN: us-80253500
extern "C" void Term__16CMenuMapSelectSCFv() {}
// LLM-HARNESS-END: us-80253500

// LLM-HARNESS-BEGIN: us-80253578
extern "C" void Move__16CMenuMapSelectSCFv() {}
// LLM-HARNESS-END: us-80253578

// LLM-HARNESS-BEGIN: us-80253668
extern "C" void cbRenderBefore__16CMenuMapSelectSCFv(void* self) {}
// LLM-HARNESS-END: us-80253668

// LLM-HARNESS-BEGIN: us-8025370c
extern "C" void func_802514D4() {}
// LLM-HARNESS-END: us-8025370c

// LLM-HARNESS-BEGIN: us-80253788
extern "C" void func_80251550() {}
// LLM-HARNESS-END: us-80253788

// LLM-HARNESS-BEGIN: us-80253798
extern "C" void func_80251560() {}
// LLM-HARNESS-END: us-80253798

// LLM-HARNESS-BEGIN: us-802537f0
extern "C" void func_802515B8() {}
// LLM-HARNESS-END: us-802537f0

// LLM-HARNESS-BEGIN: us-80253860
extern "C" void func_80251628() {}
// LLM-HARNESS-END: us-80253860

// LLM-HARNESS-BEGIN: us-80253914
extern "C" void func_802516DC() {}
// LLM-HARNESS-END: us-80253914

// LLM-HARNESS-BEGIN: us-80253f84
extern "C" void func_80251D4C() {}
// LLM-HARNESS-END: us-80253f84

// LLM-HARNESS-BEGIN: us-80253fcc
extern "C" void func_80251D94() {}
// LLM-HARNESS-END: us-80253fcc

// IScnRender vtable this-adjusting thunk for cbRenderBefore.
// IScnRender is a non-primary base at offset 0x58 within CMenuMapSelectSC.
// The thunk converts the subobject pointer (IScnRender*) to the enclosing
// CMenuMapSelectSC* via integer-offset adjustment before forwarding to the
// real override. The function-pointer cast prevents MWCC from null-checking
// the static_cast chain (which would add a cmpwi/beq that the retail thunk
// does not have).
// LLM-HARNESS-BEGIN: us-80254020
extern "C" void func_80251DE8(IScnRender* self) {
    ((void(*)(CMenuMapSelectSC*))cbRenderBefore__16CMenuMapSelectSCFv)(
        (CMenuMapSelectSC*)((u32)self - 0x58));
}
// LLM-HARNESS-END: us-80254020

// IScnRender vtable this-adjusting thunk for destructor.
// Adjusts from IScnRender* (at offset 0x58) to full CMenuMapSelectSC* and
// forwards to ~CMenuMapSelectSC(). r4 (deletion flag) is preserved from the
// caller because the cast to void(*)(CMenuMapSelectSC*) declares only one
// parameter, leaving r4 untouched.
// LLM-HARNESS-BEGIN: us-80254028
extern "C" void func_80251DF0(IScnRender* self) {
    ((void(*)(CMenuMapSelectSC*))__dt__16CMenuMapSelectSCFv)(
        (CMenuMapSelectSC*)((u32)self - 0x58));
}
// LLM-HARNESS-END: us-80254028
