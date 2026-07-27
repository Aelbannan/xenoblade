// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectMove
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

cf::CfObjectMove::CfObjectMove() {}

void cf::CfObjectMove::~CfObjectMove() {}

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

void CfObjectMove_setMoveSpeed(CfObjectMove* obj, float value)
{
    *(float*)((char*)obj + 0x6f0) = value;
    unsigned int flags = *(unsigned int*)((char*)obj + 0x68);
    flags = (flags & ~0x400u) | 0x800u;
    *(unsigned int*)((char*)obj + 0x68) = flags;
}

void CfObjectMove_nullsub_4(){}

void CfObjectMove_nullsub_5(){}

f32 lbl_eu_80666A88;

void CfObjectMove_resetMoveSpeed(u8* arg0) {
    u32 flags = *(u32*)(arg0 + 0x68);
    f32 value = lbl_eu_80666A88;
    *(f32*)(arg0 + 0x6F0) = value;
    *(u32*)(arg0 + 0x68) = flags & ~0xC00;
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

void CfObjectMove_updatePos(unsigned int* param_1) {
    if ((param_1[0x19] & 8) != 0) {
        func_804B0B54((char*)param_1 + 0x60c, (char*)param_1 + 0x3c);
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

void CfObject_UnkVirtualFunc29__Q22cf12CfObjectMoveFv(CfObjectMove* self, float value) {
    unsigned char* object = static_cast<unsigned char*>(self);
    CfObject_UnkVirtualFunc29__Q22cf13CfObjectModelFv(self);
    void* target = *reinterpret_cast<void**>(object + 0xC4);
    if (target != 0 && ((*reinterpret_cast<unsigned int*>(object + 0x68) & 0x4) != 0)) {
        func_8004B4A4(target, value);
    }
    target = *reinterpret_cast<void**>(object + 0x6C0);
    if (target != 0) {
        *reinterpret_cast<float*>(static_cast<unsigned char*>(target) + 0xC) = value;
    }
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc30() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc32() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc33() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc13() {}

void* CfObjectMove_getUnk54(CfObjectMove* self) { return (void*)((u8*)self + 0x54); }

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

int CfObjectMove_getSubState(CfObjectMove* this_ptr) { struct SubObj { int dummy0[11]; int field_2c; int field_30; }; SubObj* sub = *(SubObj**)((char*)this_ptr + 0x38); if (!sub) return -1; if (sub->field_2c < 0) return -1; return sub->field_30; }

void CfObjectMove_freeSub(CfObjectMove* self)
{
    extern void func_802A1304(void*);
    char* sub = *reinterpret_cast<char**>(reinterpret_cast<char*>(self) + 0x38);
    if (sub != 0) {
        func_802A1304(sub + 0x28);
    }
}

void CfObjectMove_nullsub_23(){}

void CfObjectMove_setSubFieldC(CfObjectMove* self, unsigned short val) {
    void* ptr = *(void**)((char*)self + 0xb0);
    *(unsigned short*)((char*)ptr + 0xc) = val;
}

int CfObjectMove_getSubFieldA(CfObjectMove* self) { return *(s16*)((u8*)*(void**)((u8*)self + 176) + 10); }

void CfObjectMove_setSubFieldA(CfObjectMove* self, unsigned short val) {
    void* ptr = *(void**)((char*)self + 0xb0);
    *(unsigned short*)((char*)ptr + 0xa) = val;
}

int CfObjectMove_getSubFieldE(CfObjectMove* self) { return *(s16*)((u8*)*(void**)((u8*)self + 176) + 14); }

void CfObjectMove_setSubFieldE(CfObjectMove* self, unsigned short val) {
    void* ptr = *(void**)((char*)self + 0xb0);
    *(unsigned short*)((char*)ptr + 0xe) = val;
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc9() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc10() {}

void CfObjectMove_virtCall10(CfObjectMove* self){
    void** inner = *(void***)((char*)self + 0x10);
    void (*func)(void*) = (void (*)(void*))inner[0x14 / 4];
    func(self);
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc61() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc62() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc12() {}

void CfObjectMove_nullsub_24() {}

int CfObjectMove_nullsub_25(CfObjectMove* self) { return 0; }

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc3() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc66() {}

void cf::CfObjectMove::CfObjectModel_UnkVirtualFunc19() {}

void cf::CfObjectMove::CfObjectModel_UnkVirtualFunc6() {}

int CfObjectMove_nullsub_26(CfObjectMove* self) { return 0; }

void cf::CfObjectMove::CfObject_UnkVirtualFunc37() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc38() {}

int CfObjectMove_nullsub_27(CfObjectMove* self) { return 0; }

int CfObjectMove_nullsub_28(CfObjectMove* self) { return 0; }

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

extern "C" int CfObjectMove_isActive(CfObjectMove* self) { return 1; }

extern "C" void CfObjectMove_setBit6c9(CfObjectMove* self, unsigned long bit) {
    unsigned char val = *(unsigned char*)((char*)self + 0x6c9);
    *(unsigned char*)((char*)self + 0x6c9) = __rlwimi(val, bit, 0, 31, 31);
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc70() {}

extern "C" CfObjectMove* CfObjectMove_testFlag8(CfObjectMove* param_1) {
    if (param_1 != NULL && (*reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(param_1) + 0x64) & 8) != 0) {
        return param_1;
    }
    return NULL;
}
