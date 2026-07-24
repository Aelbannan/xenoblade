// Auto-scaffolded catalog TU for kyoshin/menu/CMenuPassiveSkill
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/menu/CMenuPassiveSkill.hpp"

// LLM-HARNESS-BEGIN: us-802655c8
extern "C" void __ct__CMenuPassiveSkill() {}
// LLM-HARNESS-END: us-802655c8

// LLM-HARNESS-BEGIN: us-80265698
extern "C" void __dt__17CMenuPassiveSkillFv(void* self) {}
// LLM-HARNESS-END: us-80265698

// LLM-HARNESS-BEGIN: us-80265710
extern "C" void Init__17CMenuPassiveSkillFv() {}
// LLM-HARNESS-END: us-80265710

// LLM-HARNESS-BEGIN: us-80265b7c
extern "C" void Term__17CMenuPassiveSkillFv() {}
// LLM-HARNESS-END: us-80265b7c

// LLM-HARNESS-BEGIN: us-80265be8
extern "C" void Move__17CMenuPassiveSkillFv() {}
// LLM-HARNESS-END: us-80265be8

// LLM-HARNESS-BEGIN: us-80265c9c
extern "C" void cbRenderBefore__17CMenuPassiveSkillFv(void* self) {}
// LLM-HARNESS-END: us-80265c9c

// LLM-HARNESS-BEGIN: us-80265d40
extern "C" void func_802638D0() {}
// LLM-HARNESS-END: us-80265d40

// LLM-HARNESS-BEGIN: us-80265db4
extern "C" void func_80263944() {}
// LLM-HARNESS-END: us-80265db4

// LLM-HARNESS-BEGIN: us-80265dc4
extern "C" void func_80263954() {}
// LLM-HARNESS-END: us-80265dc4

// LLM-HARNESS-BEGIN: us-80265e54
extern "C" void func_802639E4() {}
// LLM-HARNESS-END: us-80265e54

// LLM-HARNESS-BEGIN: us-80265ea4
extern "C" void func_80263A34() {}
// LLM-HARNESS-END: us-80265ea4

// LLM-HARNESS-BEGIN: us-802661ac
extern "C" void func_80263D3C() {}
// LLM-HARNESS-END: us-802661ac

// LLM-HARNESS-BEGIN: us-802661fc
extern "C" void func_80263D8C() {}
// LLM-HARNESS-END: us-802661fc

// LLM-HARNESS-BEGIN: us-80266258
extern "C" void func_80263DE8() {}
// LLM-HARNESS-END: us-80266258

// LLM-HARNESS-BEGIN: us-802662bc
extern "C" void func_80263E4C() {}
// LLM-HARNESS-END: us-802662bc

// LLM-HARNESS-BEGIN: us-8026631c
// Adjusted-this thunk: called through a secondary-base vtable entry
// (IScnRender at offset +0x58 within CMenuPassiveSkill).
// Upcasts to the full object and tail-calls cbRenderBefore.
extern "C" void func_80263EAC(CMenuPassiveSkill* self) {
    ((void(*)(CMenuPassiveSkill*))cbRenderBefore__17CMenuPassiveSkillFv)(
        reinterpret_cast<CMenuPassiveSkill*>(
            reinterpret_cast<char*>(self) - 0x58));
}
// LLM-HARNESS-END: us-8026631c

// LLM-HARNESS-BEGIN: us-80266324
// Adjusted-this thunk: called through a secondary-base vtable entry
// (IScnRender at offset +0x58 within CMenuPassiveSkill).
// Upcasts to the full object and tail-calls the destructor.
// r4 (dealloc flag) is left unchanged — the caller sets it via vtable.
extern "C" void func_80263EB4(CMenuPassiveSkill* self) {
    ((void(*)(CMenuPassiveSkill*))__dt__17CMenuPassiveSkillFv)(
        reinterpret_cast<CMenuPassiveSkill*>(
            reinterpret_cast<char*>(self) - 0x58));
}
// LLM-HARNESS-END: us-80266324
