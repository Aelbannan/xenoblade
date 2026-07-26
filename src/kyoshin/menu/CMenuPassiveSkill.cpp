// Auto-scaffolded catalog TU for kyoshin/menu/CMenuPassiveSkill
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/menu/CMenuPassiveSkill.hpp"

extern "C" void __ct__CMenuPassiveSkill() {}

extern "C" void __dt__17CMenuPassiveSkillFv(void* self) {}

extern "C" void Init__17CMenuPassiveSkillFv() {}

extern "C" void Term__17CMenuPassiveSkillFv() {}

extern "C" void Move__17CMenuPassiveSkillFv() {}

extern "C" void cbRenderBefore__17CMenuPassiveSkillFv(void* self) {}

extern "C" void func_802638D0() {}

extern "C" void stub_us_80265db4() {}

extern "C" void func_80263954() {}

extern "C" void func_802639E4() {}

extern "C" void func_80263A34() {}

extern "C" void func_80263D3C() {}

extern "C" void func_80263D8C() {}

extern "C" void func_80263DE8() {}

extern "C" void func_80263E4C() {}

// Adjusted-this thunk: called through a secondary-base vtable entry
// (IScnRender at offset +0x58 within CMenuPassiveSkill).
// Upcasts to the full object and tail-calls cbRenderBefore.
extern "C" void func_80263EAC(CMenuPassiveSkill* self) {
    ((void(*)(CMenuPassiveSkill*))cbRenderBefore__17CMenuPassiveSkillFv)(
        reinterpret_cast<CMenuPassiveSkill*>(
            reinterpret_cast<char*>(self) - 0x58));
}

// Adjusted-this thunk: called through a secondary-base vtable entry
// (IScnRender at offset +0x58 within CMenuPassiveSkill).
// Upcasts to the full object and tail-calls the destructor.
// r4 (dealloc flag) is left unchanged — the caller sets it via vtable.
extern "C" void func_80263EB4(CMenuPassiveSkill* self) {
    ((void(*)(CMenuPassiveSkill*))__dt__17CMenuPassiveSkillFv)(
        reinterpret_cast<CMenuPassiveSkill*>(
            reinterpret_cast<char*>(self) - 0x58));
}
