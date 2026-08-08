// Auto-scaffolded catalog TU for kyoshin/cf/CtrlEnemy
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CtrlEnemy.hpp"
#include "kyoshin/cf/CtrlAct.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"

namespace cf {
class CtrlBase;
class CBattleManager;
class CfObjectMove;
class CActorParam;
}
void __ct__cf_CtrlEnemy(){}

cf::CtrlAct::~CtrlAct() {}

cf::CtrlBase::~CtrlBase() {}

void __dt__80087688(){}

cf::CtrlEnemy::~CtrlEnemy() {}

void CtrlEnemy_UnkFunc_8008772C(){}

void CtrlEnemy_UnkFunc_800877A8(){}

void cf::CfObjectMove::CfObject_UnkVirtualFunc60() { *(float*)((u8*)this + 0x6ec); }

void CtrlEnemy_UnkFunc_80087EEC(){}

bool CBattleManager_CheckUnk84Flag(cf::CBattleManager* obj, unsigned int mask)
{
    return (*(unsigned int*)((char*)obj + 0x84) & mask) != 0;
}

void CtrlEnemy_UnkFunc_80088620(){}

void CtrlEnemy_UnkFunc_80088800(){}

void CtrlEnemy_UnkFunc_8008884C(){}

void cf::CtrlEnemy::delegateTo80089B24() { func_80089B24((char*)this + 0x84); }

void cf::CtrlEnemy::delegateTo80089E88() { func_80089E88((char*)this + 0x84); }

void cf::CtrlEnemy::delegateTo80089F68() { func_80089F68((char*)this + 0x84); }

int cf::CtrlEnemy::ret0_80088870() { return 0; }
int cf::CtrlEnemy::ret0_80088878() { return 0; }
int cf::CtrlEnemy::ret0_80088880() { return 0; }
int cf::CtrlEnemy::ret0_80088888() { return 0; }
int cf::CtrlEnemy::ret0_80088890() { return 0; }
int cf::CtrlEnemy::ret0_80088898() { return 0; }
int cf::CtrlEnemy::ret0_800888A0() { return 0; }
int cf::CtrlEnemy::ret0_800888A8() { return 0; }
int cf::CtrlEnemy::ret0_800888B0() { return 0; }
int cf::CtrlEnemy::ret0_800888B8() { return 0; }
int cf::CtrlEnemy::ret0_800888C0() { return 0; }
int cf::CtrlEnemy::ret0_800888C8() { return 0; }
int cf::CtrlEnemy::ret0_800888D0() { return 0; }
int cf::CtrlEnemy::ret0_800888D8() { return 0; }
int cf::CtrlEnemy::ret0_800888E0() { return 0; }
int cf::CtrlEnemy::ret0_800888E8() { return 0; }

extern "C" void CtrlEnemy_emptyStub_800888F0() {}

int cf::CtrlEnemy::ret1_800888F4() { return 1; }
int cf::CtrlEnemy::ret1_800888FC() { return 1; }

// Free-function aliases (C linkage) at 0x800892xx
extern "C" int CtrlEnemy_ret0_80088870() { return 0; }
extern "C" int CtrlEnemy_ret0_80088878() { return 0; }
extern "C" int CtrlEnemy_ret0_80088880() { return 0; }
extern "C" int CtrlEnemy_ret0_80088888() { return 0; }
extern "C" int CtrlEnemy_ret0_80088890() { return 0; }
extern "C" int CtrlEnemy_ret0_80088898() { return 0; }
extern "C" int CtrlEnemy_ret0_800888A0() { return 0; }
extern "C" int CtrlEnemy_ret0_800888A8() { return 0; }
extern "C" int CtrlEnemy_ret0_800888B0() { return 0; }
extern "C" int CtrlEnemy_ret0_800888B8() { return 0; }
extern "C" int CtrlEnemy_ret0_800888C0() { return 0; }
extern "C" int CtrlEnemy_ret0_800888C8() { return 0; }
extern "C" int CtrlEnemy_ret0_800888D0() { return 0; }
extern "C" int CtrlEnemy_ret0_800888D8() { return 0; }
extern "C" int CtrlEnemy_ret0_800888E0() { return 0; }
extern "C" int CtrlEnemy_ret0_800888E8() { return 0; }
extern "C" int CtrlEnemy_ret1_800888F4() { return 1; }
extern "C" int CtrlEnemy_ret1_800888FC() { return 1; }

extern "C" void CtrlEnemy_delegateTo80089B24(void* self) { func_80089B24((char*)self + 0x84); }
extern "C" void CtrlEnemy_delegateTo80089E88(void* self) { func_80089E88((char*)self + 0x84); }
extern "C" void CtrlEnemy_delegateTo80089F68(void* self) { func_80089F68((char*)self + 0x84); }

extern "C" int CBattleManager_CheckUnk84Flag(void* self, u32 mask) {
    return (*(u32*)((char*)self + 0x84) & mask) != 0;
}