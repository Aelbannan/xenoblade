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

u32 cf::CtrlPc::testBit20() { return (mPadFlags >> 20) & 0x1u; }
u32 cf::CtrlPc::testBit19() { return (mPadFlags >> 19) & 0x1u; }
u32 cf::CtrlPc::testBit18() { return (mPadFlags >> 18) & 0x1u; }
u32 cf::CtrlPc::testBit17() { return (mPadFlags >> 17) & 0x1u; }
u32 cf::CtrlPc::testBit16() { return (mPadFlags >> 16) & 0x1u; }
u32 cf::CtrlPc::testBit15() { return (mPadFlags >> 15) & 0x1u; }
u32 cf::CtrlPc::testBit14() { return (mPadFlags >> 14) & 0x1u; }
u32 cf::CtrlPc::testBit13() { return (mPadFlags >> 13) & 0x1u; }
u32 cf::CtrlPc::testBit12() { return (mPadFlags >> 12) & 0x1u; }
u32 cf::CtrlPc::testBit11() { return (mPadFlags >> 11) & 0x1u; }
u32 cf::CtrlPc::testBit10() { return (mPadFlags >> 10) & 0x1u; }
u32 cf::CtrlPc::testBit9() { return (mPadFlags >> 9) & 0x1u; }
u32 cf::CtrlPc::testBit8() { return (mPadFlags >> 8) & 0x1u; }

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



void func_800983B8(){}

void cf::CtrlPc::setField380(u32 val) { mField380 = val; }

void func_80098694(){}

void func_800987A0(){}

void cf::CtrlPc::delegateTo899C0() {
    extern void func_800899C0(void*, float);
    extern float lbl_eu_80666728;
    func_800899C0(mSubObj8C, lbl_eu_80666728);
}

void func_80089B24(void* self);
void cf::CtrlPc::delegateTo89B24() { func_80089B24(mSubObj8C); }

void func_80089E88(void* self);
void cf::CtrlPc::delegateTo89E88() { func_80089E88(mSubObj8C); }

extern "C" void func_80089F68(void* self);
void cf::CtrlPc::delegateTo89F68() { func_80089F68(mSubObj8C); }

void func_80098810(){}

void cf::CAttackParam::CAttackParam_UnkVirtualFunc3(u8 val) { unk2A = val; }
u8 cf::CAttackParam::CAttackParam_UnkVirtualFunc2() { return unk2A; }

void func_80098A04(){}

void func_80098AF0(){}

u32 cf::CtrlPc::getField1D4() { return mField1D4; }
int cf::CtrlPc::returnTrue() { return 1; }

void func_80098B74(){}

extern "C" int lbl_eu_80527E98[];

extern "C" void CtrlPc_setArrayVal(int idx, int val)
{
    lbl_eu_80527E98[idx] = val;
}
