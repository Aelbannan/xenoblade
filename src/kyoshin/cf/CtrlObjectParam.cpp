// Auto-scaffolded catalog TU for kyoshin/cf/CtrlObjectParam
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CtrlObjectParam.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include "kyoshin/cf/object/CObjectParam.hpp"

void __ct__8009D604(){}

void __dt__8009D72C(){}

extern "C" void* func_8009D764(void* self) {
    cf::CtrlObjectParamInit* p = static_cast<cf::CtrlObjectParamInit*>(self);
    p->field_00 = 0xFFFF;
    p->field_02 = 0xFFFF;
    p->field_04 = 0xFFFF;
    p->field_06 = 0xFFFF;
    p->field_08 = 0xFFFF;
    p->field_0A = 0xFFFF;
    extern void* memset(void*, int, unsigned long);
    return static_cast<char*>(memset(p->blob, 0, sizeof(p->blob)));
}

void func_8009D790(){}

extern "C" void* func_8009D7E4(void* self, unsigned long index) {
    cf::CtrlObjectParamData* p = static_cast<cf::CtrlObjectParamData*>(self);
    return &p->entries[index];
}

void func_8009D7F4(){}

extern "C" void* CActorParam_UnkVirtualFunc94__Q22cf11CActorParamFv(void* self) {
    return &static_cast<cf::CActorParam*>(self)->unk1650;
}

void func_8009DB1C(){}

void func_8009DB28(){}

void func_8009DBF4(){}

extern "C" void CActorParam_UnkVirtualFunc33__Q22cf11CActorParamFv(void* self, float val) {
    cf::CActorParam* p = static_cast<cf::CActorParam*>(self);
    reinterpret_cast<float&>(p->unk17E4.unk0[4]) = val;
}

void func_8009DFC8(){}

void func_8009E024(){}

void func_8009E030(){}

extern "C" void func_8009E03C(void* self, void* arg) {
    extern void func_8009DBF4(void*, unsigned long, void*);
    func_8009DBF4(self, 2, arg);
}

extern "C" void func_8009E048(void* self, void* arg) {
    extern void func_8009DBF4(void*, unsigned long, void*);
    func_8009DBF4(self, 3, arg);
}

void func_8009E054(){}

extern "C" void func_8009E0A8(void* self, void* arg) {
    extern void func_8009DBF4(void*, unsigned long, void*);
    func_8009DBF4(self, 5, arg);
}

extern "C" long func_8009E0B4(void* self, unsigned long index) {
    cf::CtrlObjectParamData* p = static_cast<cf::CtrlObjectParamData*>(self);
    return p->entries[0].shortArr[index];
}

void func_8009E0C4(){}

void func_8009E120(){}

void func_8009E168(){}

extern "C" int func_8009E20C(void* self, int firstType, int firstIndex,
                             int secondType, int secondIndex) {
    cf::CtrlObjectParamSwap* p = static_cast<cf::CtrlObjectParamSwap*>(self);
    int* first = 0;
    int* second = 0;
    if (firstType == 1)
        first = &p->intArr1[firstIndex];
    else if (firstType == 2)
        first = &p->intArr2[firstIndex];
    if (secondType == 1)
        second = &p->intArr1[secondIndex];
    else if (secondType == 2)
        second = &p->intArr2[secondIndex];
    int value = *first;
    *first = *second;
    *second = value;
    return 1;
}

void func_8009E284(){}

extern "C" int func_8009E344(const unsigned int* param_1, unsigned int param_2,
                              int* param_3, int* param_4) {
    for (int i = 0; i < 3; ++i) {
        if (param_1[i + 1] == param_2) {
            *param_3 = 1;
            *param_4 = i;
            return 1;
        }
    }
    for (int i = 0; i < 6; ++i) {
        if (param_1[i + 4] == param_2) {
            *param_3 = 2;
            *param_4 = i;
            return 1;
        }
    }
    return 0;
}

void func_8009E3C0(){}

void func_8009E474(){}

void func_8009E574(){}

void func_8009E56C(void* self){ func_8009E574(); }

void func_8009E740(){}

extern "C" int func_8009E7C8(unsigned char* self) {
    cf::CtrlObjectParamClear* p = reinterpret_cast<cf::CtrlObjectParamClear*>(self);
    p->firstByte = 0;
    for (int i = 0; i < 24; ++i) p->words[i] = 0;
    return 1;
}

void func_8009E838(){}

void func_8009E974(){}

void func_8009EABC(){}

void func_8009EB2C(){}

void func_8009EB94(){}

extern "C" int func_8009EBE8(unsigned int idx) {
    extern u32 lbl_eu_80663E88;
    if (idx >= 50000) return 0;
    const u8* bitmap = reinterpret_cast<const u8*>(lbl_eu_80663E88 + 8188);
    return (bitmap[idx >> 3] >> (idx & 7)) & 1;
}

void func_8009EC18(){}

void func_8009EC6C(){}

extern "C" void* func_8009EC9C(int idx) {
    extern u32 lbl_eu_80663E88;
    return reinterpret_cast<void*>(lbl_eu_80663E88 + idx * 15828 + 16880);
}

extern "C" void* func_8009ECB0() {
    extern unsigned long lbl_eu_80663E88;
    return reinterpret_cast<void*>(lbl_eu_80663E88 + 0x1f98);
}

extern "C" void* func_8009ECBC(int idx) {
    extern u32 lbl_eu_80663E88;
    return reinterpret_cast<void*>(lbl_eu_80663E88 + (idx * 8) + 88);
}

extern "C" void func_8009ECD0(unsigned long val) {
    extern unsigned long lbl_eu_80663E88;
    unsigned long addr = lbl_eu_80663E88;
    *reinterpret_cast<unsigned long*>(addr + 0x3A388) = val;
}

extern "C" u32 func_8009ECE0() {
    extern u32 lbl_eu_80663E88;
    return *reinterpret_cast<u32*>(lbl_eu_80663E88 + 0x3A388);
}

extern "C" unsigned long func_8009ECF0() {
    extern unsigned long lbl_eu_80663E88;
    return *reinterpret_cast<unsigned short*>(lbl_eu_80663E88 + 0x50);
}

extern "C" void func_8009ECFC(unsigned short value) {
    extern unsigned long lbl_eu_80663E88;
    *reinterpret_cast<unsigned short*>(lbl_eu_80663E88 + 0x50) = value;
}

void __ct__8009ED08(){}

void func_8009EF9C(){}

extern "C" void CActorParam_UnkVirtualFunc16__Q22cf11CActorParamFv(void* self, float val) {
    static_cast<cf::CActorParam*>(self)->unk15F8 = val;
}

extern "C" void CActorParam_UnkVirtualFunc55__Q22cf11CActorParamFv(void* self, u16 val) {
    static_cast<cf::CActorParam*>(self)->unk160E = val;
}

extern "C" void CActorParam_UnkVirtualFunc61__Q22cf11CActorParamFv(void* self, u16 val) {
    static_cast<cf::CActorParam*>(self)->unk1616 = val;
}

extern "C" void CActorParam_UnkVirtualFunc65__Q22cf11CActorParamFv(void* self, float val) {
    static_cast<cf::CActorParam*>(self)->unk1610 = val;
}

extern "C" void CActorParam_UnkVirtualFunc68__Q22cf11CActorParamFv(void* self, float val) {
    static_cast<cf::CActorParam*>(self)->unk1618 = val;
}

extern "C" void CActorParam_UnkVirtualFunc74__Q22cf11CActorParamFv(void* self, float val) {
    static_cast<cf::CActorParam*>(self)->unk17E4.unk4C = val;
}

extern "C" void CActorParam_UnkVirtualFunc14__Q22cf11CActorParamFv(void* self, u8 val) {
    static_cast<cf::CActorParam*>(self)->unk15F4[0] = val;
}

extern "C" void* CActorParam_UnkVirtualFunc165__Q22cf11CActorParamFv(void* self) {
    return &static_cast<cf::CActorParam*>(self)->unk164C;
}

extern "C" void CActorParam_UnkVirtualFunc116__Q22cf11CActorParamFv(void* self, float val) {
    static_cast<cf::CActorParam*>(self)->unk1620 = val;
}

void func_8009F6D4(){}

extern "C" void* CActorParam_UnkVirtualFunc152__Q22cf11CActorParamFv(void* self) {
    return &static_cast<cf::CActorParam*>(self)->unk3358;
}

void __ct__8009F8B8(){}

void func_800A03F4(){}

extern "C" void* CActorParam_UnkVirtualFunc125__Q22cf11CActorParamFv(void* self) {
    return static_cast<cf::CActorParam*>(self)->unk2740;
}

extern "C" void* CActorParam_UnkVirtualFunc76__Q22cf11CActorParamFv(void* self) {
    return &static_cast<cf::CActorParam*>(self)->unk17E4.unk4C;
}

void func_800A082C(){}

void func_800A0860(){}

extern "C" void* CActorParam_UnkVirtualFunc100__Q22cf11CActorParamFv(void* self) {
    return &static_cast<cf::CActorParam*>(self)->unk17E4;
}

void func_800A0E64(){}

void func_800A11A4(){}

void func_800A1370(){}

void CActorParam_UnkVirtualFunc126__Q22cf11CActorParamFv() {}

void func_800A13C4(){}

void func_800A145C(){}

extern "C" u32 CActorParam_UnkVirtualFunc26__Q22cf11CActorParamFv(void* self) {
    return reinterpret_cast<u32&>(static_cast<cf::CActorParam*>(self)->unk1650);
}

void CActorParam_UnkVirtualFunc166__Q22cf11CActorParamFv() {}

void func_800A18A4(){}

void func_800A1B08(){}

void func_800A1CA0(){}

void func_800A1E3C(){}

void func_800A21F8(){}

extern "C" void CActorParam_UnkVirtualFunc82__Q22cf11CActorParamFv(void* self, u32 addend) {
    cf::CActorParam* p = static_cast<cf::CActorParam*>(self);
    u32 cap = 0x05F60000 - 7937;
    u32 val = p->unk1604 + addend;
    p->unk1604 = val;
    if (val > cap) {
        p->unk1604 = cap;
    }
}

extern "C" void CActorParam_UnkVirtualFunc90__Q22cf11CActorParamFv(void* self, u32 addend) {
    cf::CActorParam* p = static_cast<cf::CActorParam*>(self);
    u32 cap = 0x05F60000 - 7937;
    u32 val = p->unk1608 + addend;
    p->unk1608 = val;
    if (val > cap) {
        p->unk1608 = cap;
    }
}

void func_800A26A4(){}

void func_800A282C(){}

void func_800A2974(){}

extern "C" u32 CActorParam_UnkVirtualFunc85__Q22cf11CActorParamFv(void* self) {
    return static_cast<cf::CActorParam*>(self)->unk1604;
}

extern "C" void CActorParam_UnkVirtualFunc83__Q22cf11CActorParamFv(void* self, u32 addend) {
    cf::CActorParam* p = static_cast<cf::CActorParam*>(self);
    u32 cap = 0x05F60000 - 7937;
    u32 val = p->unk1600 + addend;
    p->unk1600 = val;
    if (val > cap) {
        p->unk1600 = cap;
    }
}

extern "C" void CActorParam_UnkVirtualFunc81__Q22cf11CActorParamFv(void* self, u32 val) {
    static_cast<cf::CActorParam*>(self)->unk1604 = val;
}

void func_800A2AF0(){}

void func_800A2DE8(){}

void func_800A30E4(){}

extern "C" u8 func_800A32BC(void* self) {
    cf::CtrlObjectParamByteE4* p = reinterpret_cast<cf::CtrlObjectParamByteE4*>(self);
    return p->field_E4;
}

void func_800A32C4(){}

void func_800A3304(){}

void func_800A33C8(){}

extern "C" void CObjectState_UnkVirtualFunc13__Q22cf12CObjectStateFv(void* self) {
    cf::CObjectState* p = static_cast<cf::CObjectState*>(self);
    p->unkC = p->unk8;
}

extern "C" void* CObjectState_UnkVirtualFunc12__Q22cf12CObjectStateFv(void* self) {
    return &static_cast<cf::CObjectState*>(self)->unkC;
}

extern "C" void CObjectState_UnkVirtualFunc6__Q22cf12CObjectStateFv(void* self) {
    static_cast<cf::CObjectState*>(self)->unk8 = 0;
}

extern "C" void CObjectState_UnkVirtualFunc4__Q22cf12CObjectStateFv(void* self) {
    static_cast<cf::CObjectState*>(self)->unk4 = 0;
}

extern "C" int CObjectParam_UnkVirtualFunc3__Q22cf12CObjectParamFv(void* self) {
    cf::CObjectParam* p = static_cast<cf::CObjectParam*>(self);
    return p->field_30 != 0 ? 1 : 0;
}

extern "C" void func_800A34C8(void* self) {
    cf::CtrlObjectParamClear16* p = reinterpret_cast<cf::CtrlObjectParamClear16*>(self);
    p->words[0] = 0;
    p->words[1] = 0;
    p->words[2] = 0;
    p->words[3] = 0;
}

void __dt__800A34E0(){}

void func_800A3520(){}

void func_800A3594(){}

void func_800A36A4(){}

void func_800A37CC(){}

void func_800A3940(){}

void func_800A3998(){}

void func_800A39E8(){}

void func_800A3A6C(){}
