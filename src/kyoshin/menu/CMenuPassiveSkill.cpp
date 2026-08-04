// Auto-scaffolded catalog TU for kyoshin/menu/CMenuPassiveSkill
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/menu/CMenuPassiveSkill.hpp"

void __ct__CMenuPassiveSkill(){}

CMenuPassiveSkill::~CMenuPassiveSkill() {}

void CMenuPassiveSkill::Init() {}

void CMenuPassiveSkill::Term() {}

void CMenuPassiveSkill::Move() {}

void CMenuPassiveSkill::cbRenderBefore() {}

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
// (IScnRender at offset +0x58 within CMenuPassiveSkill).
void CMenuPassiveSkill::func_80263EAC() {
    cbRenderBefore();
}

// Adjusted-this thunk: called through a secondary-base vtable entry
// (IScnRender at offset +0x58 within CMenuPassiveSkill).
void CMenuPassiveSkill::func_80263EB4() {
    this->~CMenuPassiveSkill();
}

extern unsigned long lbl_eu_80664878;
extern "C" unsigned long func_80263944(void) { return lbl_eu_80664878 != 0; }
