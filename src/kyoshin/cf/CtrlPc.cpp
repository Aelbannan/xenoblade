// Auto-scaffolded catalog TU for kyoshin/cf/CtrlPc
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CtrlPc.hpp"
#include "kyoshin/cf/CArtsSet.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
void __ct__cf_CtrlPad(){}

void cf::CtrlRemote::~CtrlRemote() {}

void func_80096974(){}

void func_80096EE8(){}

void func_80096EF4(){}

void func_80096F00(){}

u32 CtrlPc_testBit20(cf::CtrlPc* self) { return (self->mPadFlags >> 20) & 0x1u; }

u32 CtrlPc_testBit19(cf::CtrlPc* self) { return (self->mPadFlags >> 19) & 0x1u; }

u32 CtrlPc_testBit18(cf::CtrlPc* self) { return (self->mPadFlags >> 18) & 0x1u; }

u32 CtrlPc_testBit17(cf::CtrlPc* self) { return (self->mPadFlags >> 17) & 0x1u; }

u32 CtrlPc_testBit16(cf::CtrlPc* self) { return (self->mPadFlags >> 16) & 0x1u; }

u32 CtrlPc_testBit15(cf::CtrlPc* self) { return (self->mPadFlags >> 15) & 0x1u; }

u32 CtrlPc_testBit14(cf::CtrlPc* self) { return (self->mPadFlags >> 14) & 0x1u; }

u32 CtrlPc_testBit13(cf::CtrlPc* self) { return (self->mPadFlags >> 13) & 0x1u; }

u32 CtrlPc_testBit12(cf::CtrlPc* self) { return (self->mPadFlags >> 12) & 0x1u; }

u32 CtrlPc_testBit11(cf::CtrlPc* self) { return (self->mPadFlags >> 11) & 0x1u; }

u32 CtrlPc_testBit10(cf::CtrlPc* self) { return (self->mPadFlags >> 10) & 0x1u; }

u32 CtrlPc_testBit9(cf::CtrlPc* self) { return (self->mPadFlags >> 9) & 0x1u; }

u32 CtrlPc_testBit8(cf::CtrlPc* self) { return (self->mPadFlags >> 8) & 0x1u; }

void cf::CtrlPad::~CtrlPad() {}

void __ct__cf_CtrlPc(){}

void cf::CtrlPc::~CtrlPc() {}

void func_80097134(){}

void func_80097598(){}

void func_80097A5C(){}

void func_80097C74(){}

void CtrlPc_emptyStub_97DFC() {}

void func_80097E00(){}

void func_80098194(){}

void* CActorParam_UnkVirtualFunc122__Q22cf11CActorParamFv(cf::CActorParam* self) { return &self->mArtsSet; }

void func_800983B8(){}

void CtrlPc_setField380(cf::CtrlPc* self, u32 val) { self->mField380 = val; }

void func_80098694(){}

void func_800987A0(){}

void CtrlPc_delegateTo899C0(cf::CtrlPc* self) {
    extern void func_800899C0(void*, float);
    extern float lbl_eu_80666728;
    func_800899C0(self->mSubObj8C, lbl_eu_80666728);
}

void func_80089B24(void* self);
void CtrlPc_delegateTo89B24(cf::CtrlPc* self) { ((void(*)(void*))func_80089B24)(self->mSubObj8C); }

void func_80089E88(void* self);
extern "C" void CtrlPc_delegateTo89E88(cf::CtrlPc* self) { ((void(*)(void*))func_80089E88)(self->mSubObj8C); }

extern "C" void func_80089F68(void* self);
extern "C" void CtrlPc_delegateTo89F68(cf::CtrlPc* self) { ((void(*)(void*))func_80089F68)(self->mSubObj8C); }

void func_80098810(){}

extern "C" void CAttackParam_UnkVirtualFunc3__Q22cf12CAttackParamFv(cf::CAttackParam* self, u8 val) { self->unk2A = val; }

extern "C" u8 CAttackParam_UnkVirtualFunc2__Q22cf12CAttackParamFv(cf::CAttackParam* self) { return self->unk2A; }

void func_80098A04(){}

void func_80098AF0(){}

extern "C" u32 CtrlPc_getField1D4(cf::CtrlPc* self) { return self->mField1D4; }

extern "C" int CtrlPc_returnTrue(cf::CtrlPc* self) { return 1; }

void func_80098B74(){}

extern "C" int lbl_eu_80527E98[];

extern "C" void CtrlPc_setArrayVal(int idx, int val)
{
    lbl_eu_80527E98[idx] = val;
}
