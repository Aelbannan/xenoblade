// Auto-scaffolded catalog TU for kyoshin/cf/CtrlAct
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__800D10DC() {}

extern "C" void func_800D11B0() {}

extern "C" void func_800D1CFC() {}

extern "C" void func_800D1F0C() {}

extern "C" void CAttackParam_UnkVirtualFunc4__Q22cf12CAttackParamFv() {}

extern "C" void func_800D2A5C() {}

extern "C" void func_800D2D64() {}

extern "C" void func_800D34D4() {}

extern "C" void func_800D3998() {}

extern "C" void func_800D3D34() {}

extern "C" void func_800D3FFC() {}

extern "C" void func_800D4834() {}

extern "C" int func_800D49E4(void* self) { return 0; }

extern "C" int func_800D49EC(void* self) { return 0; }

extern "C" void func_800D49F4() {}

extern "C" void func_800D4F30() {}

extern "C" void func_800D5308() {}

extern "C" void func_800D56F0() {}

extern "C" int func_800D5814(void* self) { return 0; }

extern "C" void func_800D581C() {}

extern "C" unsigned long func_800D5860(void* self) {
    unsigned long v = *(unsigned long*)((char*)self + 0x58);
    return !((v >> 15) & 1);
}

extern "C" void func_800D5874() {}

extern "C" const float lbl_eu_80666CFC;

extern "C" void func_800D59FC(void* obj) {
    struct __attribute__((packed)) Data70 {
        float f70;
        unsigned short u74;
        unsigned short u76;
        unsigned char u78;
        unsigned char u79;
        unsigned char u7a;
        unsigned char u7b;
    };
    Data70* data = (Data70*)((char*)obj + 0x70);
    unsigned short temp = data->u74;
    temp &= 0x2000;
    data->u78 = 0;
    data->u7b = 0;
    data->f70 = lbl_eu_80666CFC;
    data->u7a = 0;
    data->u79 = 0;
    data->u76 = 0;
    data->u74 = temp;
}

extern "C" void func_800D5A2C() {}

extern "C" void func_800D5D68() {}

extern "C" void func_800D5F98() {}

extern "C" int func_800D64D8(void* self) { return 0; }

extern "C" int func_800D64E0(void* self) { return 0; }

extern "C" void func_800D64E8() {}

extern "C" void func_800D6720() {}

extern "C" void func_800D69D8() {}

extern "C" void func_800D755C() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_800D79B4() {}
