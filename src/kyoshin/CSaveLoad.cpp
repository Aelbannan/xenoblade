// Auto-scaffolded catalog TU for kyoshin/CSaveLoad
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" u8 func_8028F664(void* self) { return ((u8*)self)[0x123]; }










extern "C" void func_8028EA74__6CSLCurFv() {}

extern "C" void func_8028EAF8() {}

extern "C" void func_8028EB70() {}

extern "C" void func_8028EB9C() {}

extern "C" void func_8028EC04(void* arg1, const void* arg2) {
    void* ptr1 = *(void**)((char*)arg1 + 8);
    void* ptr2 = *(void**)((char*)ptr1 + 0x10);
    const float* src = (const float*)arg2;
    float* dst = (float*)((char*)ptr2 + 0x2c);
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

extern "C" void func_8028EC28() {}

extern "C" void func_8028EC74() {}

extern "C" void func_8028ED0C(void* r3, int r4) {
    struct InitData {
        int field0;
        int field4;
        int field8;
        unsigned char field12;
        unsigned char field13;
        unsigned char field14;
    };
    InitData* data = static_cast<InitData*>(r3);
    data->field0 = r4;
    data->field4 = 0;
    data->field8 = 0;
    data->field12 = 0;
    data->field13 = 0;
    data->field14 = 1;
}

extern "C" void __dt__8028ED30() {}

extern "C" void func_8028ED70() {}

extern "C" void func_8028EDF8() {}

extern "C" void func_8028EE68() {}

extern "C" void func_8028EEC0(void* self) {
    *(u8*)((u8*)self + 0xC) = 1;
    *(u8*)((u8*)self + 0xD) = 1;
    *(u8*)((u8*)self + 0xE) = 0;
}

extern "C" void func_8028EED8() {}

extern "C" void func_8028EF24() {}

extern "C" void func_8028EF74() {}

extern "C" void __ct__CSaveLoad() {}

extern "C" void __dt__9CSaveLoadFv() {}

extern "C" void func_8028F23C__9CSaveLoadFv() {}

extern "C" void func_8028F2CC() {}

extern "C" void func_8028F3D4() {}

extern "C" void func_8028F4AC() {}

extern "C" void func_8028F5C4() {}


extern "C" void func_8028F66C() {}

extern "C" void func_8028F6DC() {}

extern "C" void func_8028F774() {}

extern "C" void func_8028F7D0() {}

extern "C" void func_8028F904() {}

extern "C" void func_8028FA54() {}

extern "C" void func_8028FB20() {}

extern "C" void func_8028FC18() {}

extern "C" void func_8028FE50() {}

extern "C" u8 func_8028FEC4(void* self) { return ((u8*)self)[0x12A]; }

extern "C" void func_8028FECC() {}

extern "C" void func_8028FFD4() {}

extern "C" void func_80290094() {}

extern "C" void func_802900E0() {}

extern "C" void func_80290140() {}

extern "C" void func_8029018C() {}

extern "C" void func_802901D8() {}

extern "C" void func_8029022C() {}

extern "C" void func_8029040C() {}

extern "C" void func_8029049C(void* p) {
    unsigned char* base = reinterpret_cast<unsigned char*>(p);
    if (base[0x11e] != 0) {
        base[0x121] = 3;
    }
}

extern "C" void func_802904B4() {}

extern "C" void func_8029078C() {}

extern "C" void func_802907E4() {}

extern "C" void func_80290844() {}

extern "C" void func_802908A4() {}

extern "C" void func_80290994() {}

extern "C" void func_802910D4() {}

extern "C" void func_80291204() {}

extern "C" void OnFileEvent__9CSaveLoadFv() {}

extern "C" u32 func_8029183C(void) {
    extern u32 lbl_eu_80662AD0;
    return lbl_eu_80662AD0;
}

extern "C" void __dt__Q22cf7CfAwardFv() {}

extern "C" void func_802918AC() {}

extern "C" void func_8029194C() {}

extern "C" void func_802919A0() {}

extern "C" void func_80291A04() {}

extern "C" void func_80291B18__Q22cf7CfAwardFv() {}

extern "C" void func_80291B30() {}

extern "C" void func_80291BF8() {}

extern "C" void func_80291C60() {}

extern "C" void func_80291D98() {}

extern "C" void func_80291EF0() {}

extern "C" void func_80292000() {}

extern "C" void func_80292418() {}

extern "C" void func_802929C8() {}

extern "C" void func_80292EC0() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_802930E0() {}
