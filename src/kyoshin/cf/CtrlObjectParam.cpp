// Auto-scaffolded catalog TU for kyoshin/cf/CtrlObjectParam
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CtrlObjectParam.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include "kyoshin/cf/object/CObjectParam.hpp"

void __ct__8009D604(){}

void __dt__8009D72C(){}

extern "C" void* func_8009D764(cf::CtrlObjectParamInit* p) {
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

cf::CtrlObjectParamSubEntry* cf::CtrlObjectParamData::getSubStruct(unsigned long index) {
    return &entries[index];
}

void func_8009D7F4(){}

void* cf::CActorParam::CActorParam_UnkVirtualFunc94() {
    return &unk1650;
}

void func_8009DB1C(){}

void func_8009DB28(){}

void func_8009DBF4(){}

void cf::CActorParam::CActorParam_UnkVirtualFunc33(float val) {
    reinterpret_cast<float&>(unk17E4.unk0[4]) = val;
}

void func_8009DFC8(){}

void func_8009E024(){}

void func_8009E030(){}

void cf::CtrlObjectParamData::setArgType2(void* arg) {
    extern void func_8009DBF4(cf::CtrlObjectParamData*, unsigned long, void*);
    func_8009DBF4(this, 2, arg);
}

void cf::CtrlObjectParamData::setArgType3(void* arg) {
    extern void func_8009DBF4(cf::CtrlObjectParamData*, unsigned long, void*);
    func_8009DBF4(this, 3, arg);
}

void func_8009E054(){}

void cf::CtrlObjectParamData::setArgType5(void* arg) {
    extern void func_8009DBF4(cf::CtrlObjectParamData*, unsigned long, void*);
    func_8009DBF4(this, 5, arg);
}

long cf::CtrlObjectParamData::getShortAt1C(unsigned long index) {
    return entries[0].shortArr[index];
}

void func_8009E0C4(){}

void func_8009E120(){}

void func_8009E168(){}

extern "C" int func_8009E20C(cf::CtrlObjectParamSwap* self, int firstType, int firstIndex,
                             int secondType, int secondIndex) {
    int* first = 0;
    int* second = 0;
    if (firstType == 1)
        first = &self->intArr1[firstIndex];
    else if (firstType == 2)
        first = &self->intArr2[firstIndex];
    if (secondType == 1)
        second = &self->intArr1[secondIndex];
    else if (secondType == 2)
        second = &self->intArr2[secondIndex];
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

int cf::CtrlObjectParamClear::clearStruct() {
    firstByte = 0;
    for (int i = 0; i < 24; ++i) words[i] = 0;
    return 1;
}

extern "C" void func_8009E838(void* self) {
    for (int a = 0; a < 3; a++) {
        void* cur = (u8*)self + a * 4;
        if (*(u32*)((u8*)cur + 4) == 0) {
            int found = 0;
            for (int i = a + 1; i < 3; i++) {
                if (*(u32*)((u8*)self + (i * 4) + 4) != 0) {
                    *(u32*)((u8*)cur + 4) = *(u32*)((u8*)self + (i * 4) + 4);
                    *(u32*)((u8*)self + (i * 4) + 4) = 0;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                int done = 0;
                for (int j = 0; j < 6; j++) {
                    void* cur2 = (u8*)self + j * 4;
                    if (*(u32*)((u8*)cur2 + 0x10) == 0) {
                        for (int i = j + 1; i < 6; i++) {
                            if (*(u32*)((u8*)self + (i * 4) + 0x10) != 0) {
                                *(u32*)((u8*)cur2 + 0x10) = *(u32*)((u8*)self + (i * 4) + 0x10);
                                *(u32*)((u8*)self + (i * 4) + 0x10) = 0;
                                done = 1;
                                break;
                            }
                        }
                    }
                    if (done) break;
                }
                return;
            }
        }
    }
}

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

void cf::CActorParam::CActorParam_UnkVirtualFunc16(float val) {
    unk15F8 = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc55(u16 val) {
    unk160E = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc61(u16 val) {
    unk1616 = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc65(float val) {
    unk1610 = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc68(float val) {
    unk1618 = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc74(float val) {
    unk17E4.unk4C = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc14(u8 val) {
    unk15F4[0] = val;
}

void* cf::CActorParam::CActorParam_UnkVirtualFunc165() {
    return &unk164C;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc116(float val) {
    unk1620 = val;
}

void func_8009F6D4(){}

void* cf::CActorParam::CActorParam_UnkVirtualFunc152() {
    return &unk3358;
}

void __ct__8009F8B8(){}

void func_800A03F4(){}

void* cf::CActorParam::CActorParam_UnkVirtualFunc125() {
    return unk2740;
}

void* cf::CActorParam::CActorParam_UnkVirtualFunc76() {
    return &unk17E4.unk4C;
}

void func_800A082C(){}

void func_800A0860(){}

void* cf::CActorParam::CActorParam_UnkVirtualFunc100() {
    return &unk17E4;
}

void func_800A0E64(){}

void func_800A11A4(){}

void func_800A1370(){}

void CActorParam_UnkVirtualFunc126__Q22cf11CActorParamFv() {}

void func_800A13C4(){}

void func_800A145C(){}

u32 cf::CActorParam::CActorParam_UnkVirtualFunc26() {
    return reinterpret_cast<u32&>(unk1650);
}

void CActorParam_UnkVirtualFunc166__Q22cf11CActorParamFv() {}

void func_800A18A4(){}

void func_800A1B08(){}

void func_800A1CA0(){}

void func_800A1E3C(){}

void func_800A21F8(){}

void cf::CActorParam::CActorParam_UnkVirtualFunc82(u32 addend) {
    u32 cap = 0x05F60000 - 7937;
    u32 val = unk1604 + addend;
    unk1604 = val;
    if (val > cap) {
        unk1604 = cap;
    }
}

void cf::CActorParam::CActorParam_UnkVirtualFunc90(u32 addend) {
    u32 cap = 0x05F60000 - 7937;
    u32 val = unk1608 + addend;
    unk1608 = val;
    if (val > cap) {
        unk1608 = cap;
    }
}

void func_800A26A4(){}

void func_800A282C(){}

void func_800A2974(){}

u32 cf::CActorParam::CActorParam_UnkVirtualFunc85() {
    return unk1604;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc83(u32 addend) {
    u32 cap = 0x05F60000 - 7937;
    u32 val = unk1600 + addend;
    unk1600 = val;
    if (val > cap) {
        unk1600 = cap;
    }
}

void cf::CActorParam::CActorParam_UnkVirtualFunc81(u32 val) {
    unk1604 = val;
}

void func_800A2AF0(){}

void func_800A2DE8(){}

void func_800A30E4(){}

u8 cf::CtrlObjectParamByteE4::getByteE4() {
    return field_E4;
}

void func_800A32C4(){}

void func_800A3304(){}

void func_800A33C8(){}

void cf::CObjectState::CObjectState_UnkVirtualFunc13() {
    unkC = unk8;
}

void* cf::CObjectState::CObjectState_UnkVirtualFunc12() {
    return &unkC;
}

void cf::CObjectState::CObjectState_UnkVirtualFunc6() {
    unk8 = 0;
}

void cf::CObjectState::CObjectState_UnkVirtualFunc4() {
    unk4 = 0;
}

int cf::CObjectParam::CObjectParam_UnkVirtualFunc3() {
    return field_30 != 0 ? 1 : 0;
}

void cf::CtrlObjectParamClear16::clear16Bytes() {
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
    words[3] = 0;
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
