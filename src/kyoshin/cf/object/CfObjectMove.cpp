// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectMove
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__Q22cf12CfObjectMoveFv() {}

extern "C" void __dt__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc4__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc7__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc6__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectModel_UnkVirtualFunc1__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectModel_UnkVirtualFunc2__Q22cf12CfObjectMoveFv() {}

extern "C" void func_800BC2DC() {}

extern "C" void CfObjectMove_UnkVirtualFunc2__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectMove_UnkVirtualFunc1__Q22cf12CfObjectMoveFv() {}

extern "C" void func_800BC3AC() {}

extern "C" void func_800BC3B0() {}

extern "C" void func_800BC3D8(void* obj, float value)
{
    *(float*)((char*)obj + 0x6f0) = value;
    unsigned int flags = *(unsigned int*)((char*)obj + 0x68);
    flags = (flags & ~0x400u) | 0x800u;
    *(unsigned int*)((char*)obj + 0x68) = flags;
}

extern "C" void func_800BC3F0() {}

extern "C" void func_800BC458() {}

extern "C" f32 lbl_eu_80666A88;

extern "C" void func_800BC4A0(u8* arg0) {
    u32 flags = *(u32*)(arg0 + 0x68);
    f32 value = lbl_eu_80666A88;
    *(f32*)(arg0 + 0x6F0) = value;
    *(u32*)(arg0 + 0x68) = flags & ~0xC00;
}

extern "C" void func_800BC4B8() {}

extern "C" void func_800BC4CC() {}

extern "C" void func_800BC68C() {}

extern "C" void CfObject_UnkVirtualFunc5__Q22cf12CfObjectMoveFv() {}

extern "C" void func_800BC8D8() {}

extern "C" void func_800BC9EC() {}

extern "C" void func_800BCD04() {}

extern "C" void func_800BCFA0() {}

extern "C" void func_800BD638() {}

extern "C" void func_800BD644() {}

extern "C" void CfObject_UnkVirtualFunc46__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc47__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc49__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc64__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc65__Q22cf12CfObjectMoveFv() {}

extern "C" void func_804B0B54(void*, void*);

extern "C" void func_800BDB4C(unsigned int* param_1) {
    if ((param_1[0x19] & 8) != 0) {
        func_804B0B54((char*)param_1 + 0x60c, (char*)param_1 + 0x3c);
    }
}

extern "C" void CfObject_UnkVirtualFunc19__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc22__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc25__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc26__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc23__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc27__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc29__Q22cf13CfObjectModelFv(void*);
extern "C" void func_8004B4A4(void*, float);

extern "C" void CfObject_UnkVirtualFunc29__Q22cf12CfObjectMoveFv(void* self, float value) {
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

extern "C" void CfObject_UnkVirtualFunc30__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc32__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc33__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc13__Q22cf12CfObjectMoveFv() {}

extern "C" void* func_800BE0B0(void* self) { return (void*)((u8*)self + 0x54); }

extern "C" void CfObject_UnkVirtualFunc57__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectMove_UnkVirtualFunc4__Q22cf12CfObjectMoveFv() {}

extern "C" void CObjectParam_UnkVirtualFunc2__Q22cf12CfObjectMoveFv(void) {}

extern "C" void func_800BE0F8() {}

extern "C" void func_800BE12C() {}

extern "C" void func_800BE1A4() {}

extern "C" void CfObject_UnkVirtualFunc14__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc15__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc16__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc17__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectMove_UnkVirtualFunc7__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectMove_UnkVirtualFunc8__Q22cf12CfObjectMoveFv() {}

extern "C" void func_800BE28C() {}

extern "C" void CfObjectMove_UnkVirtualFunc9__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectMove_UnkVirtualFunc10__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectMove_UnkVirtualFunc11__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectMove_UnkVirtualFunc12__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectMove_UnkVirtualFunc13__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectMove_UnkVirtualFunc14__Q22cf12CfObjectMoveFv() {}

extern "C" void func_800BE33C() {}

extern "C" void func_800BE3E8() {}

extern "C" void CfObjectMove_UnkVirtualFunc16__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectMove_UnkVirtualFunc17__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectMove_UnkVirtualFunc18__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectMove_UnkVirtualFunc21__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectMove_UnkVirtualFunc22__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectMove_UnkVirtualFunc19__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectMove_UnkVirtualFunc20__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectModel_UnkVirtualFunc18__Q22cf12CfObjectMoveFv() {}

extern "C" void func_800BE824() {}

extern "C" void CfObjectMove_UnkVirtualFunc23__Q22cf12CfObjectMoveFv() {}

extern "C" void func_800BE898__Q22cf8CfObjectFiUlff() {}

extern "C" void func_800BE8B4() {}

extern "C" int func_800BE8F4(void* this_ptr) { struct SubObj { int dummy0[11]; int field_2c; int field_30; }; SubObj* sub = *(SubObj**)((char*)this_ptr + 0x38); if (!sub) return -1; if (sub->field_2c < 0) return -1; return sub->field_30; }

extern "C" void func_800BE924(void* self)
{
    extern void func_802A1304(void*);
    char* sub = *reinterpret_cast<char**>(reinterpret_cast<char*>(self) + 0x38);
    if (sub != 0) {
        func_802A1304(sub + 0x28);
    }
}

extern "C" void func_800BE93C() {}

extern "C" void func_800BE948(void* self, unsigned short val) {
    void* ptr = *(void**)((char*)self + 0xb0);
    *(unsigned short*)((char*)ptr + 0xc) = val;
}

extern "C" int func_800BE954(void* self) { return *(s16*)((u8*)*(void**)((u8*)self + 176) + 10); }

extern "C" void func_800BE960(void* self, unsigned short val) {
    void* ptr = *(void**)((char*)self + 0xb0);
    *(unsigned short*)((char*)ptr + 0xa) = val;
}

extern "C" int func_800BE96C(void* self) { return *(s16*)((u8*)*(void**)((u8*)self + 176) + 14); }

extern "C" void func_800BE978(void* self, unsigned short val) {
    void* ptr = *(void**)((char*)self + 0xb0);
    *(unsigned short*)((char*)ptr + 0xe) = val;
}

extern "C" void CfObject_UnkVirtualFunc9__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc10__Q22cf12CfObjectMoveFv() {}

extern "C" void func_800BE9AC(void* self) {
    void** inner = *(void***)((char*)self + 0x10);
    void (*func)(void*) = (void (*)(void*))inner[0x14 / 4];
    func(self);
}

extern "C" void CfObject_UnkVirtualFunc61__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc62__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc12__Q22cf12CfObjectMoveFv() {}

extern "C" void func_800BEA34() {}

extern "C" int func_800BEA38(void* self) { return 0; }

extern "C" void CfObjectMove_UnkVirtualFunc3__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc66__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectModel_UnkVirtualFunc19__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObjectModel_UnkVirtualFunc6__Q22cf12CfObjectMoveFv() {}

extern "C" int func_800BEC44(void* self) { return 0; }

extern "C" void CfObject_UnkVirtualFunc37__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc38__Q22cf12CfObjectMoveFv() {}

extern "C" int func_800BED5C(void* self) { return 0; }

extern "C" int func_800BED64(void* self) { return 0; }

extern "C" void func_800BED6C() {}

extern "C" void func_800BED80() {}

extern "C" void func_800BEDC4() {}

extern "C" void func_800BEE08() {}

extern "C" void func_800BEE1C() {}

extern "C" void func_800BEE30() {}

extern "C" void CfObject_UnkVirtualFunc39__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc40__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc42__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc43__Q22cf12CfObjectMoveFv() {}

extern "C" void CfObject_UnkVirtualFunc45__Q22cf12CfObjectMoveFv() {}

extern "C" void func_800BF29C() {}

extern "C" void func_800BF2B0() {}

extern "C" void func_800BF2C4() {}

extern "C" void func_800BF2C8() {}

extern "C" void func_800BF2CC() {}

extern "C" void func_800BF2E0() {}

extern "C" void func_800BF2F4() {}

extern "C" void func_800BF2F8() {}

extern "C" void func_eu_800BFC78() {}

extern "C" void func_eu_800BFC7C() {}

extern "C" int func_800BF30C(void* self) { return 1; }

extern "C" void func_800BF314(void* self, unsigned long bit) {
    unsigned char val = *(unsigned char*)((char*)self + 0x6c9);
    *(unsigned char*)((char*)self + 0x6c9) = __rlwimi(val, bit, 0, 31, 31);
}

extern "C" void CfObject_UnkVirtualFunc70__Q22cf12CfObjectMoveFv() {}

extern "C" void* func_800BF324(void* param_1) {
    if (param_1 != NULL && (*reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(param_1) + 0x64) & 8) != 0) {
        return param_1;
    }
    return NULL;
}
