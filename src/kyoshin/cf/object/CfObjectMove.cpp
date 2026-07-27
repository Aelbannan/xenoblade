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

void func_800BC2DC() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc2() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc1() {}

void func_800BC3AC() {}

void func_800BC3B0(){}

void func_800BC3D8(void* obj, float value)
{
    *(float*)((char*)obj + 0x6f0) = value;
    unsigned int flags = *(unsigned int*)((char*)obj + 0x68);
    flags = (flags & ~0x400u) | 0x800u;
    *(unsigned int*)((char*)obj + 0x68) = flags;
}

void func_800BC3F0(){}

void func_800BC458(){}

f32 lbl_eu_80666A88;

void func_800BC4A0(u8* arg0) {
    u32 flags = *(u32*)(arg0 + 0x68);
    f32 value = lbl_eu_80666A88;
    *(f32*)(arg0 + 0x6F0) = value;
    *(u32*)(arg0 + 0x68) = flags & ~0xC00;
}

void func_800BC4B8(){}

void func_800BC4CC(){}

void func_800BC68C(){}

void cf::CfObjectMove::CfObject_UnkVirtualFunc5() {}

void func_800BC8D8(){}

void func_800BC9EC(){}

void func_800BCD04(){}

void func_800BCFA0(){}

void func_800BD638(){}

void func_800BD644(){}

void cf::CfObjectMove::CfObject_UnkVirtualFunc46() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc47() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc49() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc64() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc65() {}

void func_804B0B54(void*, void*);

void func_800BDB4C(unsigned int* param_1) {
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

void CfObject_UnkVirtualFunc29__Q22cf13CfObjectModelFv(void*);
void func_8004B4A4(void*, float);

void CfObject_UnkVirtualFunc29__Q22cf12CfObjectMoveFv(void* self, float value) {
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

void* func_800BE0B0(void* self) { return (void*)((u8*)self + 0x54); }

void cf::CfObjectMove::CfObject_UnkVirtualFunc57() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc4() {}

void cf::CfObjectMove::CObjectParam_UnkVirtualFunc2(void) {}

void func_800BE0F8(){}

void func_800BE12C(){}

void func_800BE1A4(){}

void cf::CfObjectMove::CfObject_UnkVirtualFunc14() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc15() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc16() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc17() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc7() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc8() {}

void func_800BE28C(){}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc9() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc10() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc11() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc12() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc13() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc14() {}

void func_800BE33C(){}

void func_800BE3E8(){}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc16() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc17() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc18() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc21() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc22() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc19() {}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc20() {}

void cf::CfObjectMove::CfObjectModel_UnkVirtualFunc18() {}

void func_800BE824(){}

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc23() {}

void cf::CfObject::func_800BE898() {}

void func_800BE8B4(){}

int func_800BE8F4(void* this_ptr) { struct SubObj { int dummy0[11]; int field_2c; int field_30; }; SubObj* sub = *(SubObj**)((char*)this_ptr + 0x38); if (!sub) return -1; if (sub->field_2c < 0) return -1; return sub->field_30; }

void func_800BE924(void* self)
{
    extern void func_802A1304(void*);
    char* sub = *reinterpret_cast<char**>(reinterpret_cast<char*>(self) + 0x38);
    if (sub != 0) {
        func_802A1304(sub + 0x28);
    }
}

void func_800BE93C(){}

void func_800BE948(void* self, unsigned short val) {
    void* ptr = *(void**)((char*)self + 0xb0);
    *(unsigned short*)((char*)ptr + 0xc) = val;
}

int func_800BE954(void* self) { return *(s16*)((u8*)*(void**)((u8*)self + 176) + 10); }

void func_800BE960(void* self, unsigned short val) {
    void* ptr = *(void**)((char*)self + 0xb0);
    *(unsigned short*)((char*)ptr + 0xa) = val;
}

int func_800BE96C(void* self) { return *(s16*)((u8*)*(void**)((u8*)self + 176) + 14); }

void func_800BE978(void* self, unsigned short val) {
    void* ptr = *(void**)((char*)self + 0xb0);
    *(unsigned short*)((char*)ptr + 0xe) = val;
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc9() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc10() {}

void func_800BE9AC(void* self){
    void** inner = *(void***)((char*)self + 0x10);
    void (*func)(void*) = (void (*)(void*))inner[0x14 / 4];
    func(self);
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc61() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc62() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc12() {}

void func_800BEA34() {}

int func_800BEA38(void* self) { return 0; }

void cf::CfObjectMove::CfObjectMove_UnkVirtualFunc3() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc66() {}

void cf::CfObjectMove::CfObjectModel_UnkVirtualFunc19() {}

void cf::CfObjectMove::CfObjectModel_UnkVirtualFunc6() {}

int func_800BEC44(void* self) { return 0; }

void cf::CfObjectMove::CfObject_UnkVirtualFunc37() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc38() {}

int func_800BED5C(void* self) { return 0; }

int func_800BED64(void* self) { return 0; }

void func_800BED6C(){}

void func_800BED80(){}

void func_800BEDC4(){}

void func_800BEE08(){}

void func_800BEE1C(){}

void func_800BEE30() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc39() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc40() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc42() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc43() {}

void cf::CfObjectMove::CfObject_UnkVirtualFunc45() {}

void func_800BF29C(){}

void func_800BF2B0(){}

void func_800BF2C4() {}

void func_800BF2C8() {}

void func_800BF2CC(){}

void func_800BF2E0(){}

void func_800BF2F4() {}

void func_800BF2F8(){}

void func_eu_800BFC78() {}

void func_eu_800BFC7C(){}

extern "C" int func_800BF30C(void* self) { return 1; }

extern "C" void func_800BF314(void* self, unsigned long bit) {
    unsigned char val = *(unsigned char*)((char*)self + 0x6c9);
    *(unsigned char*)((char*)self + 0x6c9) = __rlwimi(val, bit, 0, 31, 31);
}

void cf::CfObjectMove::CfObject_UnkVirtualFunc70() {}

extern "C" void* func_800BF324(void* param_1) {
    if (param_1 != NULL && (*reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(param_1) + 0x64) & 8) != 0) {
        return param_1;
    }
    return NULL;
}
