// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectMove
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"

namespace cf {

CfObjectMove::CfObjectMove() {}

cf::CfObjectMove::~CfObjectMove() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc4() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc7() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc6() {}

void cf::CfObjectMove::CfObjectModel_UnkVirtualFunc1() {}

void cf::CfObjectMove::CfObjectModel_UnkVirtualFunc2() {}

void CfObjectMove_nullsub_1() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc2() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc1() {}

void CfObjectMove_nullsub_2() {}

void CfObjectMove_nullsub_3(){}

void cf::CfObjectMove::setMoveSpeed(float value)
{
    mMoveSpeed = value;
    unsigned int flags = mFlags68;
    flags = (flags & ~0x400u) | 0x800u;
    mFlags68 = flags;
}

void CfObjectMove_nullsub_4(){}

void CfObjectMove_nullsub_5(){}

f32 lbl_eu_80666A88;

void cf::CfObjectMove::resetMoveSpeed() {
    u32 flags = mFlags68;
    f32 value = lbl_eu_80666A88;
    mMoveSpeed = value;
    mFlags68 = flags & ~0xC00;
}

void CfObjectMove_nullsub_6(){}

void CfObjectMove_nullsub_7(){}

void CfObjectMove_nullsub_8(){}

void cf::CfObjectMove::CfObject_UnkVirtualFunc5() {}

void CfObjectMove_nullsub_9(){}

void CfObjectMove_nullsub_10(){}

void CfObjectMove_nullsub_11(){}

void CfObjectMove_nullsub_12(){}

void CfObjectMove_nullsub_13(){}

void CfObjectMove_nullsub_14(){}

void cf::CfObjectMove::CfObject_UnkVirtualFunc46() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc47() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc49() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc64() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc65() {}

void func_804B0B54(void*, void*);

void cf::CfObjectMove::updatePos() {
    if ((unk64 & 8) != 0) {
        func_804B0B54(_60C_region, _pad3C);
    }
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc19() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc22() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc25() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc26() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc23() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc27() {}

void CfObject_UnkVirtualFunc29__Q22cf13CfObjectModelFv(CfObjectModel*);
void func_8004B4A4(void*, float);

void cf::CfObjectMove::CfObject_UnkVirtualFunc29(float value) {
    CfObject_UnkVirtualFunc29__Q22cf13CfObjectModelFv(this);
    void* target = mTargetC4;
    if (target != 0 && ((mFlags68 & 0x4) != 0)) {
        func_8004B4A4(target, value);
    }
    target = mTarget6C0;
    if (target != 0) {
        *(float*)((char*)target + 0xC) = value;
    }
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc30() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc32() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc33() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc13() {}

void* cf::CfObjectMove::getUnk54() { return (void*)mSubObj54; }

void cf::CfObjectMove::CfObject_UnkVirtualFunc57() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc4() {}

void cf::CfObjectMove::CObjectParam_UnkVirtualFunc2(void) {}

void CfObjectMove_nullsub_15(){}

void CfObjectMove_nullsub_16(){}

void CfObjectMove_nullsub_17(){}

void cf::CfObjectMove::CfObject_UnkVirtualFunc14() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc15() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc16() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc17() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc7() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc8() {}

void CfObjectMove_nullsub_18(){}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc9() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc10() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc11() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc12() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc13() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc14() {}

void CfObjectMove_nullsub_19(){}

void CfObjectMove_nullsub_20(){}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc16() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc17() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc18() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc21() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc22() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc19() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc20() {}

void cf::CfObjectMove::CfObjectModel_UnkVirtualFunc18() {}

void CfObjectMove_nullsub_21(){}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc23() {}

void cf::CfObject::func_800BE898() {}

void CfObjectMove_nullsub_22(){}

int cf::CfObjectMove::getSubState() {
    struct SubObj { int dummy0[11]; int field_2c; int field_30; };
    SubObj* sub = (SubObj*)mSubObj38;
    if (!sub) return -1;
    if (sub->field_2c < 0) return -1;
    return sub->field_30;
}

void cf::CfObjectMove::freeSub()
{
    extern void func_802A1304(void*);
    char* sub = (char*)mSubObj38;
    if (sub != 0) {
        func_802A1304(sub + 0x28);
    }
}

void CfObjectMove_nullsub_23(){}

void cf::CfObjectMove::setSubFieldC(unsigned short val) {
    void* ptr = mSubObjB0;
    *(unsigned short*)((char*)ptr + 0xc) = val;
}

int cf::CfObjectMove::getSubFieldA() { return *(s16*)((char*)mSubObjB0 + 10); }

void cf::CfObjectMove::setSubFieldA(unsigned short val) {
    void* ptr = mSubObjB0;
    *(unsigned short*)((char*)ptr + 0xa) = val;
}

int cf::CfObjectMove::getSubFieldE() { return *(s16*)((char*)mSubObjB0 + 14); }

void cf::CfObjectMove::setSubFieldE(unsigned short val) {
    void* ptr = mSubObjB0;
    *(unsigned short*)((char*)ptr + 0xe) = val;
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc9() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc10() {}

void cf::CfObjectMove::virtCall10(){
    void** inner = (void**)mPtr10;
    void (*func)(void*) = (void (*)(void*))inner[0x14 / 4];
    func(this);
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc61() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc62() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc12() {}

void CfObjectMove_nullsub_24() {}

int cf::CfObjectMove::nullsub_25() { return 0; }

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc3() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc66() {}

void cf::CfObjectMove::CfObjectModel_UnkVirtualFunc19() {}

void cf::CfObjectMove::CfObjectModel_UnkVirtualFunc6() {}

int cf::CfObjectMove::nullsub_26() { return 0; }

void cf::CfObjectMove::CfObject_UnkVirtualFunc37() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc38() {}

int cf::CfObjectMove::nullsub_27() { return 0; }

int cf::CfObjectMove::nullsub_28() { return 0; }

void CfObjectMove_nullsub_29(){}

void CfObjectMove_nullsub_30(){}

void CfObjectMove_nullsub_31(){}

void CfObjectMove_nullsub_32(){}

void CfObjectMove_nullsub_33(){}

void CfObjectMove_nullsub_34() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc39() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc40() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc42() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc43() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc45() {}

void CfObjectMove_nullsub_35(){}

void CfObjectMove_nullsub_36(){}

void CfObjectMove_nullsub_37() {}

void CfObjectMove_nullsub_38() {}

void CfObjectMove_nullsub_39(){}

void CfObjectMove_nullsub_40(){}

void CfObjectMove_nullsub_41() {}

void CfObjectMove_nullsub_42(){}

void func_eu_800BFC78() {}

void func_eu_800BFC7C(){}

int cf::CfObjectMove::isActive() { return 1; }

void cf::CfObjectMove::setBit6c9(unsigned long bit) {
    unsigned char val = mFlags6C9;
    mFlags6C9 = __rlwimi(val, bit, 0, 31, 31);
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc70() {}

cf::CfObjectMove* cf::CfObjectMove::testFlag8() {
    if (this != NULL && (unk64 & 8) != 0) {
        return this;
    }
    return NULL;
}

} // namespace cf
