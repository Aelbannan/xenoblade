// Auto-scaffolded catalog TU for monolib/src/scn/CScnBlend
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern "C" void __ct__CScnBlend() {}

extern "C" void __dt__9CScnBlendFv() {}

extern "C" void func_80498D98(void* r3, int r4) {
    uint8_t v = *(uint8_t*)((uintptr_t)r3 + 0x54);
    if (r4 != 0) {
        *(uint8_t*)((uintptr_t)r3 + 0x54) = v | 1;
    } else {
        *(uint8_t*)((uintptr_t)r3 + 0x54) = v & 0xFE;
    }
}

extern "C" void func_80498DC0() {}

extern "C" void func_80498DE8() {}
