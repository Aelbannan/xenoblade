// Auto-scaffolded catalog TU for kyoshin/menu/CMenuPassiveSkill
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/menu/CMenuPassiveSkill.hpp"

void __ct__CMenuPassiveSkill(){}

CMenuPassiveSkill::~CMenuPassiveSkill() {}

void CMenuPassiveSkill::Init() {}

void CMenuPassiveSkill::Term() {}

void CMenuPassiveSkill::Move() {}

// noinline: retail keeps the virtual dispatch as a real call (the thunks
// tail-branch to this symbol; an inline would fold the thunks to empty).
__declspec(noinline) void CMenuPassiveSkill::cbRenderBefore() {}

void func_802638D0(){}

void stub_us_80265db4() {}

void func_80263954(){}

void func_802639E4(){}

void func_80263A34(){}

void func_80263D3C(){}

void func_80263D8C(){}

void func_80263DE8(){}

void func_80263E4C(){}

// Adjusted-this thunk: called through a secondary-base vtable entry
// (IScnRender at offset +0x58 within CMenuPassiveSkill). Retail is
// subi r3,r3,0x58 + tail-branch, so call through the adjusted pointer.
void CMenuPassiveSkill::func_80263EAC() {
    ((CMenuPassiveSkill*)((u8*)this - 0x58))->cbRenderBefore();
}

// Adjusted-this thunk for the destructor (same +0x58 adjustment). Direct
// extern-C call keeps the retail subi + tail-branch (no virtual dispatch,
// no delete flag).
void CMenuPassiveSkill::func_80263EB4() {
    __dt__17CMenuPassiveSkillFv((u8*)this - 0x58);
}

extern unsigned long lbl_eu_80664878;
extern "C" unsigned long func_80263944(void) { return lbl_eu_80664878 != 0; }
