// Auto-scaffolded catalog TU for kyoshin/cf/CtrlEnemy
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CtrlEnemy.hpp"

namespace cf {
class CtrlBase;
class CBattleManager;
class CfObjectMove;
class CActorParam;
}
void __ct__cf_CtrlEnemy(){}

cf::CtrlAct::~CtrlAct() {}

void* __dt__Q22cf8CtrlBaseFv(cf::CtrlBase* self, int deleteFlag) {
    if (self != nullptr) {
        if (deleteFlag > 0) {
            operator delete(self);
        }
    }
    return self;
}

void __dt__80087688(){}

cf::CtrlEnemy::~CtrlEnemy() {}

void CtrlEnemy_UnkFunc_8008772C(){}

void CtrlEnemy_UnkFunc_800877A8(){}

float CfObject_UnkVirtualFunc60__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self) { return *(float*)((u8*)self + 0x6ec); }

void CtrlEnemy_UnkFunc_80087EEC(){}

u32* CActorParam_UnkVirtualFunc113__Q22cf11CActorParamFv(cf::CActorParam* self) { return (u32*)((u8*)self + 0x161c); }

bool CBattleManager_CheckUnk84Flag(cf::CBattleManager* obj, unsigned int mask)
{
    return (*(unsigned int*)((char*)obj + 0x84) & mask) != 0;
}

float* CActorParam_UnkVirtualFunc117__Q22cf11CActorParamFv(cf::CActorParam* self) { return (float*)((u8*)self + 0x1620); }

float* CActorParam_UnkVirtualFunc119__Q22cf11CActorParamFv(cf::CActorParam* self) { return (float*)((u8*)self + 0x1624); }

u32 CActorParam_UnkVirtualFunc29__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(u32*)((u8*)self + 0x17E4); }

void CtrlEnemy_UnkFunc_80088620(){}

bool CActorParam_UnkVirtualFunc115__Q22cf11CActorParamFv(cf::CActorParam* self) {
    return !!((unsigned char*)self)[0x1628];
}

extern "C" u32 CActorParam_UnkVirtualFunc19__Q22cf11CActorParamFv(cf::CActorParam* self) { return *(u32*)((u8*)self + 0x15EC); }

void CtrlEnemy_UnkFunc_80088800(){}

void CtrlEnemy_UnkFunc_8008884C(){}

extern "C" void func_80089B24(void* self);
extern "C" void CtrlEnemy_delegateTo80089B24(cf::CtrlEnemy* self) { ((void(*)(void*))func_80089B24)((char*)self + 0x84); }

extern "C" void func_80089E88(void* self);
extern "C" void CtrlEnemy_delegateTo80089E88(cf::CtrlEnemy* self) { ((void(*)(void*))func_80089E88)((char*)self + 0x84); }

extern "C" void func_80089F68(void* self);
extern "C" void CtrlEnemy_delegateTo80089F68(cf::CtrlEnemy* self) { ((void(*)(void*))func_80089F68)((char*)self + 0x84); }

extern "C" int CtrlEnemy_ret0_80088870(cf::CtrlEnemy* self) { return 0; }

extern "C" int CtrlEnemy_ret0_80088878(cf::CtrlEnemy* self) { return 0; }

extern "C" int CtrlEnemy_ret0_80088880(cf::CtrlEnemy* self) { return 0; }

extern "C" int CtrlEnemy_ret0_80088888(cf::CtrlEnemy* self) { return 0; }

extern "C" int CtrlEnemy_ret0_80088890(cf::CtrlEnemy* self) { return 0; }

extern "C" int CtrlEnemy_ret0_80088898(cf::CtrlEnemy* self) { return 0; }

extern "C" int CtrlEnemy_ret0_800888A0(cf::CtrlEnemy* self) { return 0; }

extern "C" int CtrlEnemy_ret0_800888A8(cf::CtrlEnemy* self) { return 0; }

extern "C" int CtrlEnemy_ret0_800888B0(cf::CtrlEnemy* self) { return 0; }

extern "C" int CtrlEnemy_ret0_800888B8(cf::CtrlEnemy* self) { return 0; }

extern "C" int CtrlEnemy_ret0_800888C0(cf::CtrlEnemy* self) { return 0; }

extern "C" int CtrlEnemy_ret0_800888C8(cf::CtrlEnemy* self) { return 0; }

extern "C" int CtrlEnemy_ret0_800888D0(cf::CtrlEnemy* self) { return 0; }

extern "C" int CtrlEnemy_ret0_800888D8(cf::CtrlEnemy* self) { return 0; }

extern "C" int CtrlEnemy_ret0_800888E0(cf::CtrlEnemy* self) { return 0; }

extern "C" int CtrlEnemy_ret0_800888E8(cf::CtrlEnemy* self) { return 0; }

extern "C" void CtrlEnemy_emptyStub_800888F0() {}

extern "C" int CtrlEnemy_ret1_800888F4(cf::CtrlEnemy* self) { return 1; }

extern "C" int CtrlEnemy_ret1_800888FC(cf::CtrlEnemy* self) { return 1; }