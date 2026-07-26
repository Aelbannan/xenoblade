// Auto-scaffolded catalog TU for kyoshin/cf/CtrlObjectParam
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-8009dfdc
extern "C" void __ct__8009D604() {}
// LLM-HARNESS-END: us-8009dfdc

// LLM-HARNESS-BEGIN: us-8009e104
extern "C" void __dt__8009D72C() {}
// LLM-HARNESS-END: us-8009e104

// LLM-HARNESS-BEGIN: us-8009e13c
extern "C" void* func_8009D764(void* self) {
    *(unsigned short*)((char*)self + 0) = 0xFFFF;
    *(unsigned short*)((char*)self + 2) = 0xFFFF;
    *(unsigned short*)((char*)self + 4) = 0xFFFF;
    *(unsigned short*)((char*)self + 6) = 0xFFFF;
    *(unsigned short*)((char*)self + 8) = 0xFFFF;
    *(unsigned short*)((char*)self + 10) = 0xFFFF;
    extern void* memset(void*, int, unsigned long);
    return memset((char*)self + 12, 0, 192);
}
// LLM-HARNESS-END: us-8009e13c

// LLM-HARNESS-BEGIN: us-8009e168
extern "C" void func_8009D790() {}
// LLM-HARNESS-END: us-8009e168

// LLM-HARNESS-BEGIN: us-8009e1bc
extern "C" void* func_8009D7E4(void* self, unsigned long index) {
    return (char*)self + (index << 5) + 0xc;
}
// LLM-HARNESS-END: us-8009e1bc

// LLM-HARNESS-BEGIN: us-8009e1cc
extern "C" void func_8009D7F4() {}
// LLM-HARNESS-END: us-8009e1cc

// LLM-HARNESS-BEGIN: us-8009e4ec
extern "C" void* CActorParam_UnkVirtualFunc94__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x1650); }
// LLM-HARNESS-END: us-8009e4ec

// LLM-HARNESS-BEGIN: us-8009e4f4
extern "C" void func_8009DB1C() {}
// LLM-HARNESS-END: us-8009e4f4

// LLM-HARNESS-BEGIN: us-8009e500
extern "C" void func_8009DB28() {}
// LLM-HARNESS-END: us-8009e500

// LLM-HARNESS-BEGIN: us-8009e5cc
extern "C" void func_8009DBF4() {}
// LLM-HARNESS-END: us-8009e5cc

// LLM-HARNESS-BEGIN: us-8009e998
extern "C" void CActorParam_UnkVirtualFunc33__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x17e8) = val; }
// LLM-HARNESS-END: us-8009e998

// LLM-HARNESS-BEGIN: us-8009e9a0
extern "C" void func_8009DFC8() {}
// LLM-HARNESS-END: us-8009e9a0

// LLM-HARNESS-BEGIN: us-8009e9fc
extern "C" void func_8009E024() {}
// LLM-HARNESS-END: us-8009e9fc

// LLM-HARNESS-BEGIN: us-8009ea08
extern "C" void func_8009E030() {}
// LLM-HARNESS-END: us-8009ea08

// LLM-HARNESS-BEGIN: us-8009ea14
extern "C" void func_8009E03C(void* self, void* arg) {
    extern void func_8009DBF4(void*, unsigned long, void*);
    func_8009DBF4(self, 2, arg);
}
// LLM-HARNESS-END: us-8009ea14

// LLM-HARNESS-BEGIN: us-8009ea20
extern "C" void func_8009E048(void* self, void* arg) {
    extern void func_8009DBF4(void*, unsigned long, void*);
    func_8009DBF4(self, 3, arg);
}
// LLM-HARNESS-END: us-8009ea20

// LLM-HARNESS-BEGIN: us-8009ea2c
extern "C" void func_8009E054() {}
// LLM-HARNESS-END: us-8009ea2c

// LLM-HARNESS-BEGIN: us-8009ea80
extern "C" void func_8009E0A8(void* self, void* arg) {
    extern void func_8009DBF4(void*, unsigned long, void*);
    func_8009DBF4(self, 5, arg);
}
// LLM-HARNESS-END: us-8009ea80

// LLM-HARNESS-BEGIN: us-8009ea8c
extern "C" long func_8009E0B4(void* self, unsigned long index) {
    return *(short*)((char*)self + (index << 1) + 0x1c);
}
// LLM-HARNESS-END: us-8009ea8c

// LLM-HARNESS-BEGIN: us-8009ea9c
extern "C" void func_8009E0C4() {}
// LLM-HARNESS-END: us-8009ea9c

// LLM-HARNESS-BEGIN: us-8009eaf8
extern "C" void func_8009E120() {}
// LLM-HARNESS-END: us-8009eaf8

// LLM-HARNESS-BEGIN: us-8009eb40
extern "C" void func_8009E168() {}
// LLM-HARNESS-END: us-8009eb40

// LLM-HARNESS-BEGIN: us-8009ebe4
extern "C" int func_8009E20C(void* self, int firstType, int firstIndex, int secondType, int secondIndex) {
    int* first = 0;
    int* second = 0;
    if (firstType == 1)
        first = reinterpret_cast<int*>(reinterpret_cast<char*>(self) + firstIndex * 4 + 4);
    else if (firstType == 2)
        first = reinterpret_cast<int*>(reinterpret_cast<char*>(self) + firstIndex * 4 + 16);
    if (secondType == 1)
        second = reinterpret_cast<int*>(reinterpret_cast<char*>(self) + secondIndex * 4 + 4);
    else if (secondType == 2)
        second = reinterpret_cast<int*>(reinterpret_cast<char*>(self) + secondIndex * 4 + 16);
    int value = *first;
    *first = *second;
    *second = value;
    return 1;
}
// LLM-HARNESS-END: us-8009ebe4

// LLM-HARNESS-BEGIN: us-8009ec5c
extern "C" void func_8009E284() {}
// LLM-HARNESS-END: us-8009ec5c

// LLM-HARNESS-BEGIN: us-8009ed1c
extern "C" int func_8009E344(const unsigned int* param_1, unsigned int param_2, int* param_3, int* param_4) {
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
// LLM-HARNESS-END: us-8009ed1c

// LLM-HARNESS-BEGIN: us-8009ed98
extern "C" void func_8009E3C0() {}
// LLM-HARNESS-END: us-8009ed98

// LLM-HARNESS-BEGIN: us-8009ee4c
extern "C" void func_8009E474() {}
// LLM-HARNESS-END: us-8009ee4c

// LLM-HARNESS-BEGIN: us-8009ef4c
extern "C" void func_8009E574() {}
// LLM-HARNESS-END: us-8009ef4c

// LLM-HARNESS-BEGIN: us-8009ef44
extern "C" void func_8009E56C(void* self) { func_8009E574(); }
// LLM-HARNESS-END: us-8009ef44
// LLM-HARNESS-END: us-8009ef4c

// LLM-HARNESS-BEGIN: us-8009f118
extern "C" void func_8009E740() {}
// LLM-HARNESS-END: us-8009f118

// LLM-HARNESS-BEGIN: us-8009f1a0
extern "C" int func_8009E7C8(unsigned char* self) { self[0] = 0; unsigned int* words = reinterpret_cast<unsigned int*>(self + 4); words[0] = 0; words[1] = 0; words[2] = 0; words[3] = 0; words[4] = 0; words[5] = 0; words[6] = 0; words[7] = 0; words[8] = 0; words[9] = 0; words[10] = 0; words[11] = 0; words[12] = 0; words[13] = 0; words[14] = 0; words[15] = 0; words[16] = 0; words[17] = 0; words[18] = 0; words[19] = 0; words[20] = 0; words[21] = 0; words[22] = 0; words[23] = 0; return 1; }
// LLM-HARNESS-END: us-8009f1a0

// LLM-HARNESS-BEGIN: us-8009f210
extern "C" void func_8009E838() {}
// LLM-HARNESS-END: us-8009f210

// LLM-HARNESS-BEGIN: us-8009f34c
extern "C" void func_8009E974() {}
// LLM-HARNESS-END: us-8009f34c

// LLM-HARNESS-BEGIN: us-8009f494
extern "C" void func_8009EABC() {}
// LLM-HARNESS-END: us-8009f494

// LLM-HARNESS-BEGIN: us-8009f504
extern "C" void func_8009EB2C() {}
// LLM-HARNESS-END: us-8009f504

// LLM-HARNESS-BEGIN: us-8009f56c
extern "C" void func_8009EB94() {}
// LLM-HARNESS-END: us-8009f56c

// LLM-HARNESS-BEGIN: us-8009f5c0
extern "C" int func_8009EBE8(unsigned int idx) {
    extern u32 lbl_eu_80663E88;
    if (idx >= 50000) return 0;
    return (*(unsigned char*)(lbl_eu_80663E88 + (idx >> 3) + 8188) >> (idx & 7)) & 1;
}
// LLM-HARNESS-END: us-8009f5c0

// LLM-HARNESS-BEGIN: us-8009f5f0
extern "C" void func_8009EC18() {}
// LLM-HARNESS-END: us-8009f5f0

// LLM-HARNESS-BEGIN: us-8009f644
extern "C" void func_8009EC6C() {}
// LLM-HARNESS-END: us-8009f644

// LLM-HARNESS-BEGIN: us-8009f674
extern "C" void* func_8009EC9C(int idx) {
    extern u32 lbl_eu_80663E88;
    return (void*)(lbl_eu_80663E88 + idx * 15828 + 16880);
}
// LLM-HARNESS-END: us-8009f674

// LLM-HARNESS-BEGIN: us-8009f688
extern "C" void* func_8009ECB0() {
    extern unsigned long lbl_eu_80663E88;
    return (void*)(lbl_eu_80663E88 + 0x1f98);
}
// LLM-HARNESS-END: us-8009f688

// LLM-HARNESS-BEGIN: us-8009f694
extern "C" void* func_8009ECBC(int idx) {
    extern u32 lbl_eu_80663E88;
    return (void*)(lbl_eu_80663E88 + (idx * 8) + 88);
}
// LLM-HARNESS-END: us-8009f694

// LLM-HARNESS-BEGIN: us-8009f6a8
extern "C" void func_8009ECD0(unsigned long val) {
    extern unsigned long lbl_eu_80663E88;
    unsigned long addr = lbl_eu_80663E88;
    *(unsigned long*)(addr + 0x3A388) = val;
}
// LLM-HARNESS-END: us-8009f6a8

// LLM-HARNESS-BEGIN: us-8009f6b8
extern "C" u32 func_8009ECE0() {
    extern u32 lbl_eu_80663E88;
    return *(u32*)(lbl_eu_80663E88 + 0x40000 - 0x5C78);
}
// LLM-HARNESS-END: us-8009f6b8

// LLM-HARNESS-BEGIN: us-8009f6c8
extern "C" unsigned long func_8009ECF0() {
    extern unsigned long lbl_eu_80663E88;
    return *(unsigned short*)(lbl_eu_80663E88 + 0x50);
}
// LLM-HARNESS-END: us-8009f6c8

// LLM-HARNESS-BEGIN: us-8009f6d4
extern "C" void func_8009ECFC(unsigned short value) {
    extern unsigned long lbl_eu_80663E88;
    *(unsigned short*)(lbl_eu_80663E88 + 0x50) = value;
}
// LLM-HARNESS-END: us-8009f6d4

// LLM-HARNESS-BEGIN: us-8009f6e0
extern "C" void __ct__8009ED08() {}
// LLM-HARNESS-END: us-8009f6e0

// LLM-HARNESS-BEGIN: us-8009f974
extern "C" void func_8009EF9C() {}
// LLM-HARNESS-END: us-8009f974

// LLM-HARNESS-BEGIN: us-800a0064
extern "C" void CActorParam_UnkVirtualFunc16__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x15f8) = val; }
// LLM-HARNESS-END: us-800a0064

// LLM-HARNESS-BEGIN: us-800a006c
extern "C" void CActorParam_UnkVirtualFunc55__Q22cf11CActorParamFv(void* self, u16 val) { *(u16*)((u8*)self + 0x160E) = val; }
// LLM-HARNESS-END: us-800a006c

// LLM-HARNESS-BEGIN: us-800a0074
extern "C" void CActorParam_UnkVirtualFunc61__Q22cf11CActorParamFv(void* self, u16 val) { *(u16*)((u8*)self + 0x1616) = val; }
// LLM-HARNESS-END: us-800a0074

// LLM-HARNESS-BEGIN: us-800a007c
extern "C" void CActorParam_UnkVirtualFunc65__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x1610) = val; }
// LLM-HARNESS-END: us-800a007c

// LLM-HARNESS-BEGIN: us-800a0084
extern "C" void CActorParam_UnkVirtualFunc68__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x1618) = val; }
// LLM-HARNESS-END: us-800a0084

// LLM-HARNESS-BEGIN: us-800a008c
extern "C" void CActorParam_UnkVirtualFunc74__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x1830) = val; }
// LLM-HARNESS-END: us-800a008c

// LLM-HARNESS-BEGIN: us-800a0094
extern "C" void CActorParam_UnkVirtualFunc14__Q22cf11CActorParamFv(void* self, u8 val) { ((u8*)self)[0x15F4] = val; }
// LLM-HARNESS-END: us-800a0094

// LLM-HARNESS-BEGIN: us-800a009c
extern "C" void* CActorParam_UnkVirtualFunc165__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x164c); }
// LLM-HARNESS-END: us-800a009c

// LLM-HARNESS-BEGIN: us-800a00a4
extern "C" void CActorParam_UnkVirtualFunc116__Q22cf11CActorParamFv(void* self, float val) { *(float*)((u8*)self + 0x1620) = val; }
// LLM-HARNESS-END: us-800a00a4

// LLM-HARNESS-BEGIN: us-800a00ac
extern "C" void func_8009F6D4() {}
// LLM-HARNESS-END: us-800a00ac

// LLM-HARNESS-BEGIN: us-800a0288
extern "C" void* CActorParam_UnkVirtualFunc152__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x3358); }
// LLM-HARNESS-END: us-800a0288

// LLM-HARNESS-BEGIN: us-800a0290
extern "C" void __ct__8009F8B8() {}
// LLM-HARNESS-END: us-800a0290

// LLM-HARNESS-BEGIN: us-800a0dcc
extern "C" void func_800A03F4() {}
// LLM-HARNESS-END: us-800a0dcc

// LLM-HARNESS-BEGIN: us-800a11f4
extern "C" void* CActorParam_UnkVirtualFunc125__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x2740); }
// LLM-HARNESS-END: us-800a11f4

// LLM-HARNESS-BEGIN: us-800a11fc
extern "C" void* CActorParam_UnkVirtualFunc76__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x1830); }
// LLM-HARNESS-END: us-800a11fc

// LLM-HARNESS-BEGIN: us-800a1204
extern "C" void func_800A082C() {}
// LLM-HARNESS-END: us-800a1204

// LLM-HARNESS-BEGIN: us-800a1238
extern "C" void func_800A0860() {}
// LLM-HARNESS-END: us-800a1238

// LLM-HARNESS-BEGIN: us-800a1834
extern "C" void* CActorParam_UnkVirtualFunc100__Q22cf11CActorParamFv(void* self) { return (void*)((u8*)self + 0x17e4); }
// LLM-HARNESS-END: us-800a1834

// LLM-HARNESS-BEGIN: us-800a183c
extern "C" void func_800A0E64() {}
// LLM-HARNESS-END: us-800a183c

// LLM-HARNESS-BEGIN: us-800a1b7c
extern "C" void func_800A11A4() {}
// LLM-HARNESS-END: us-800a1b7c

// LLM-HARNESS-BEGIN: us-800a1d48
extern "C" void func_800A1370() {}
// LLM-HARNESS-END: us-800a1d48

// LLM-HARNESS-BEGIN: us-800a1d98
extern "C" void CActorParam_UnkVirtualFunc126__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-800a1d98

// LLM-HARNESS-BEGIN: us-800a1d9c
extern "C" void func_800A13C4() {}
// LLM-HARNESS-END: us-800a1d9c

// LLM-HARNESS-BEGIN: us-800a1e34
extern "C" void func_800A145C() {}
// LLM-HARNESS-END: us-800a1e34

// LLM-HARNESS-BEGIN: us-800a2270
extern "C" u32 CActorParam_UnkVirtualFunc26__Q22cf11CActorParamFv(void* self) { return *(u32*)((u8*)self + 0x1650); }
// LLM-HARNESS-END: us-800a2270

// LLM-HARNESS-BEGIN: us-800a2278
extern "C" void CActorParam_UnkVirtualFunc166__Q22cf11CActorParamFv() {}
// LLM-HARNESS-END: us-800a2278

// LLM-HARNESS-BEGIN: us-800a227c
extern "C" void func_800A18A4() {}
// LLM-HARNESS-END: us-800a227c

// LLM-HARNESS-BEGIN: us-800a24e0
extern "C" void func_800A1B08() {}
// LLM-HARNESS-END: us-800a24e0

// LLM-HARNESS-BEGIN: us-800a2678
extern "C" void func_800A1CA0() {}
// LLM-HARNESS-END: us-800a2678

// LLM-HARNESS-BEGIN: us-800a2814
extern "C" void func_800A1E3C() {}
// LLM-HARNESS-END: us-800a2814

// LLM-HARNESS-BEGIN: us-800a2bd0
extern "C" void func_800A21F8() {}
// LLM-HARNESS-END: us-800a2bd0

// LLM-HARNESS-BEGIN: us-800a3034
extern "C" void CActorParam_UnkVirtualFunc82__Q22cf11CActorParamFv(void* self, u32 addend) {
    u32 cap = 0x05F60000 - 7937;
    u32 val = *(u32*)((u8*)self + 0x1604) + addend;
    *(u32*)((u8*)self + 0x1604) = val;
    if (val > cap) {
        *(u32*)((u8*)self + 0x1604) = cap;
    }
}
// LLM-HARNESS-END: us-800a3034

// LLM-HARNESS-BEGIN: us-800a3058
extern "C" void CActorParam_UnkVirtualFunc90__Q22cf11CActorParamFv(void* self, u32 addend) {
    u32 cap = 0x05F60000 - 7937;
    u32 val = *(u32*)((u8*)self + 0x1608) + addend;
    *(u32*)((u8*)self + 0x1608) = val;
    if (val > cap) {
        *(u32*)((u8*)self + 0x1608) = cap;
    }
}
// LLM-HARNESS-END: us-800a3058

// LLM-HARNESS-BEGIN: us-800a307c
extern "C" void func_800A26A4() {}
// LLM-HARNESS-END: us-800a307c

// LLM-HARNESS-BEGIN: us-800a3204
extern "C" void func_800A282C() {}
// LLM-HARNESS-END: us-800a3204

// LLM-HARNESS-BEGIN: us-800a334c
extern "C" void func_800A2974() {}
// LLM-HARNESS-END: us-800a334c

// LLM-HARNESS-BEGIN: us-800a3494
extern "C" u32 CActorParam_UnkVirtualFunc85__Q22cf11CActorParamFv(void* self) { return *(u32*)((u8*)self + 0x1604); }
// LLM-HARNESS-END: us-800a3494

// LLM-HARNESS-BEGIN: us-800a349c
extern "C" void CActorParam_UnkVirtualFunc83__Q22cf11CActorParamFv(void* self, u32 addend) {
    u32 cap = 0x05F60000 - 7937;
    u32 val = *(u32*)((u8*)self + 0x1600) + addend;
    *(u32*)((u8*)self + 0x1600) = val;
    if (val > cap) {
        *(u32*)((u8*)self + 0x1600) = cap;
    }
}
// LLM-HARNESS-END: us-800a349c

// LLM-HARNESS-BEGIN: us-800a34c0
extern "C" void CActorParam_UnkVirtualFunc81__Q22cf11CActorParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x1604) = val; }
// LLM-HARNESS-END: us-800a34c0

// LLM-HARNESS-BEGIN: us-800a34c8
extern "C" void func_800A2AF0() {}
// LLM-HARNESS-END: us-800a34c8

// LLM-HARNESS-BEGIN: us-800a36b0
extern "C" void func_800A2DE8() {}
// LLM-HARNESS-END: us-800a36b0

// LLM-HARNESS-BEGIN: us-800a39ac
extern "C" void func_800A30E4() {}
// LLM-HARNESS-END: us-800a39ac

// LLM-HARNESS-BEGIN: us-800a3b84
extern "C" u8 func_800A32BC(void* self) { return ((u8*)self)[0xE4]; }
// LLM-HARNESS-END: us-800a3b84

// LLM-HARNESS-BEGIN: us-800a3b8c
extern "C" void func_800A32C4() {}
// LLM-HARNESS-END: us-800a3b8c

// LLM-HARNESS-BEGIN: us-800a3bcc
extern "C" void func_800A3304() {}
// LLM-HARNESS-END: us-800a3bcc

// LLM-HARNESS-BEGIN: us-800a3c90
extern "C" void func_800A33C8() {}
// LLM-HARNESS-END: us-800a3c90

// LLM-HARNESS-BEGIN: us-800a3d50
extern "C" void CObjectState_UnkVirtualFunc13__Q22cf12CObjectStateFv(void* self) { *(u32*)((u8*)self + 12) = *(u32*)((u8*)self + 8); }
// LLM-HARNESS-END: us-800a3d50

// LLM-HARNESS-BEGIN: us-800a3d5c
extern "C" void* CObjectState_UnkVirtualFunc12__Q22cf12CObjectStateFv(void* self) { return (void*)((u8*)self + 0xc); }
// LLM-HARNESS-END: us-800a3d5c

// LLM-HARNESS-BEGIN: us-800a3d64
extern "C" void CObjectState_UnkVirtualFunc6__Q22cf12CObjectStateFv(void* self) { *(u32*)((u8*)self + 8) = 0; }
// LLM-HARNESS-END: us-800a3d64

// LLM-HARNESS-BEGIN: us-800a3d70
extern "C" void CObjectState_UnkVirtualFunc4__Q22cf12CObjectStateFv(void* self) { *(u32*)((u8*)self + 4) = 0; }
// LLM-HARNESS-END: us-800a3d70

// LLM-HARNESS-BEGIN: us-800a3d7c
extern "C" int CObjectParam_UnkVirtualFunc3__Q22cf12CObjectParamFv(void* self) {
    return *(int*)((char*)self + 48) != 0 ? 1 : 0;
}
// LLM-HARNESS-END: us-800a3d7c

// LLM-HARNESS-BEGIN: us-800a3d90
extern "C" void func_800A34C8(void* self) {
    *(u32*)((u8*)self + 0) = 0;
    *(u32*)((u8*)self + 4) = 0;
    *(u32*)((u8*)self + 8) = 0;
    *(u32*)((u8*)self + 12) = 0;
}
// LLM-HARNESS-END: us-800a3d90

// LLM-HARNESS-BEGIN: us-800a3da8
extern "C" void __dt__800A34E0() {}
// LLM-HARNESS-END: us-800a3da8

// LLM-HARNESS-BEGIN: us-800a3de8
extern "C" void func_800A3520() {}
// LLM-HARNESS-END: us-800a3de8

// LLM-HARNESS-BEGIN: us-800a3e5c
extern "C" void func_800A3594() {}
// LLM-HARNESS-END: us-800a3e5c

// LLM-HARNESS-BEGIN: us-800a3f6c
extern "C" void func_800A36A4() {}
// LLM-HARNESS-END: us-800a3f6c

// LLM-HARNESS-BEGIN: us-800a4094
extern "C" void func_800A37CC() {}
// LLM-HARNESS-END: us-800a4094

// LLM-HARNESS-BEGIN: us-800a4208
extern "C" void func_800A3940() {}
// LLM-HARNESS-END: us-800a4208

// LLM-HARNESS-BEGIN: us-800a4260
extern "C" void func_800A3998() {}
// LLM-HARNESS-END: us-800a4260

// LLM-HARNESS-BEGIN: us-800a42b0
extern "C" void func_800A39E8() {}
// LLM-HARNESS-END: us-800a42b0

// LLM-HARNESS-BEGIN: us-800a4334
extern "C" void func_800A3A6C() {}
// LLM-HARNESS-END: us-800a4334
