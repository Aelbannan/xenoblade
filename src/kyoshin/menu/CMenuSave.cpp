// Auto-scaffolded catalog TU for kyoshin/menu/CMenuSave
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/menu/CMenuSave.hpp"

// LLM-HARNESS-BEGIN: us-80290128
extern "C" void __ct__CMenuSave() {}
// LLM-HARNESS-END: us-80290128

// LLM-HARNESS-BEGIN: us-80290218
extern "C" void __dt__9CMenuSaveFv(CMenuSave* self, int dtorFlag) {}
// LLM-HARNESS-END: us-80290218

// LLM-HARNESS-BEGIN: us-80290290
extern "C" void Init__9CMenuSaveFv() {}
// LLM-HARNESS-END: us-80290290

// LLM-HARNESS-BEGIN: us-80290650
extern "C" void Term__9CMenuSaveFv() {}
// LLM-HARNESS-END: us-80290650

// LLM-HARNESS-BEGIN: us-802906d8
extern "C" void Move__9CMenuSaveFv() {}
// LLM-HARNESS-END: us-802906d8

// LLM-HARNESS-BEGIN: us-8029078c
extern "C" void cbRenderBefore__9CMenuSaveFv(CMenuSave* self) {}
// LLM-HARNESS-END: us-8029078c

// LLM-HARNESS-BEGIN: us-80290830
extern "C" void func_8028E3B4() {}
// LLM-HARNESS-END: us-80290830

// LLM-HARNESS-BEGIN: us-802908bc
extern "C" void func_8028E440() {}
// LLM-HARNESS-END: us-802908bc

// LLM-HARNESS-BEGIN: us-802908cc
extern "C" void func_8028E450() {}
// LLM-HARNESS-END: us-802908cc

// LLM-HARNESS-BEGIN: us-8029095c
extern "C" void func_8028E4E0() {}
// LLM-HARNESS-END: us-8029095c

// LLM-HARNESS-BEGIN: us-802909ac
extern "C" void func_8028E530() {}
// LLM-HARNESS-END: us-802909ac

// LLM-HARNESS-BEGIN: us-80290be4
extern "C" void func_8028E768() {}
// LLM-HARNESS-END: us-80290be4

// LLM-HARNESS-BEGIN: us-80290c34
/// Adjusting thunk: called when IScnRender subobject (at CMenuSave+0x58)
/// receives cbRenderBefore. Adjusts this back to full CMenuSave*.
extern "C" void func_8028E7B8(CMenuSave* self) {
    ((void(*)(CMenuSave*))cbRenderBefore__9CMenuSaveFv)(
        reinterpret_cast<CMenuSave*>(reinterpret_cast<char*>(self) - 0x58));
}
// LLM-HARNESS-END: us-80290c34

// LLM-HARNESS-BEGIN: us-80290c3c
/// Adjusting destructor thunk: called when IScnRender subobject (at CMenuSave+0x58)
/// receives ~CMenuSave. The delete flag is passed through unchanged.
extern "C" void func_8028E7C0(CMenuSave* self, int dtorFlag) {
    ((void(*)(CMenuSave*, int))__dt__9CMenuSaveFv)(
        reinterpret_cast<CMenuSave*>(reinterpret_cast<char*>(self) - 0x58),
        dtorFlag);
}
// LLM-HARNESS-END: us-80290c3c
