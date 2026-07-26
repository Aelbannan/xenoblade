// Auto-scaffolded catalog TU for monolib/src/core/code_804DEDA8
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern "C" void func_804DEDA8(void* r3) {
    char* base = (char*)r3;
    base[0] = 0;
    base[2] = 0;
    base[3] = 0;
    *(int*)(base + 4) = 0;
    *(int*)(base + 8) = 0;
    *(int*)(base + 0x10) = 0;
    *(int*)(base + 0xc) = 0;
}

extern "C" void __dt__804DEDCC() {}

extern "C" void __dt__804DF068() {}

extern "C" void func_804DF118() {}

extern "C" void func_804DF150() {}

extern "C" void func_804DF164() {}

extern "C" void* func_804DF2A8(void* self, int index) {
    uint8_t* base = *(uint8_t**)((uint8_t*)self + 0x10);
    uint8_t* arrayBase = base + *(uint32_t*)(base + 0x8);
    uint32_t offset = *(uint32_t*)(arrayBase + (index << 3));
    return base + offset;
}

extern "C" void func_804DF2C4() {}

extern "C" void func_804DF2F0() {}

extern "C" void func_804DF344() {}

extern "C" void func_804DF3D0() {}

extern "C" void func_804DF4BC() {}

extern "C" void func_804DF5F8() {}

extern "C" void func_804DF690() {}

extern "C" void __dla__FPv(void*);
extern "C" void* lbl_eu_80665A30;
extern "C" void* lbl_eu_80665A34;
extern "C" void* lbl_eu_80665A38;
extern "C" void* lbl_eu_80665A3C;

extern "C" void __dt__804DF744() {
    void* p1 = lbl_eu_80665A34;
    lbl_eu_80665A30 = 0;
    if (p1) {
        __dla__FPv(p1);
        lbl_eu_80665A34 = 0;
    }
    void* p2 = lbl_eu_80665A38;
    if (p2) {
        __dla__FPv(p2);
        lbl_eu_80665A38 = 0;
    }
    lbl_eu_80665A3C = 0;
}

extern "C" void func_804DF7A4() {}

extern "C" void func_804DF7FC(void) {}

extern "C" void func_804DF808() {}

extern "C" void func_804DFA08() {}

extern "C" void func_804DFA84() {}

extern "C" void func_804DFB88() {}

extern "C" void func_804DFBF4() {}

extern "C" void func_804DFC48() {}

extern "C" void func_804DFCC4() {}

extern "C" void func_804DFE20() {}

extern "C" void func_804DFE8C() {}

extern "C" void func_804DFE9C() {}

extern "C" void func_804DFEAC() {}

extern "C" void func_804DFF00() {}

extern "C" void func_804DFFA8() {}

extern "C" void func_804E0098() {}

extern "C" void func_804E0104() {}

extern "C" void func_804E0114() {}

extern "C" void func_804E0168() {}

extern "C" void func_804E0188() {}

extern "C" void func_804E0248() {}

extern "C" void func_804E04D4() {}

extern "C" void func_804E0580() {}

extern "C" void func_804E06B4() {}

extern "C" void func_804E0788() {}

extern "C" void func_804E08BC() {}

extern "C" void func_804E0990() {}

extern "C" void func_804E0B94() {}

extern "C" void func_804E0CF0() {}

extern "C" void func_804E0E48() {}

extern "C" void func_804E1044() {}

extern "C" void func_804E1294() {}

extern "C" void func_804E17A4() {}

extern "C" void func_804E18CC() {}

extern "C" void func_804E196C() {}

extern "C" void func_804E1A44() {}

extern "C" void func_804E1AA8() {}

extern "C" void func_804E1C1C() {}

extern "C" void func_804E1D50() {}

extern "C" void func_804E2088() {}

extern "C" void func_804E214C() {}

extern "C" void func_804E24A8() {}

extern "C" void func_804E26D8() {}

extern "C" void func_804E2A5C() {}

extern "C" void func_804E2B54() {}

extern "C" void func_804E2D8C() {}

extern "C" void func_804E2EAC() {}

extern "C" void func_804E2F7C() {}

extern "C" void func_804E30F0() {}
