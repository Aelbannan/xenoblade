// Auto-scaffolded catalog TU for monolib/src/coli/code_804B59C8
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern "C" void func_804B59C8() {}

extern "C" u32 func_804B5A68(void) {
    extern u32 lbl_eu_80663AC8;
    return lbl_eu_80663AC8;
}

extern "C" void* func_804B5A70(void) { return 0; }

extern "C" void __dt__804B5A7C() {}

extern "C" void __dt__804B5AD0() {}

extern "C" void func_804B5B34(void) {}

extern "C" void func_804B5B38() {}

extern "C" void func_804B5BB0() {}

extern "C" void func_804B5C58() {}

extern "C" void func_804B5CEC() {}

extern "C" void func_804B5DE8() {}

extern "C" void func_804B5E68() {}

extern "C" void func_804B61F8(void) {}

extern "C" void func_804B61FC() {}

extern "C" void func_804B62B0() {}

extern "C" void func_804B6364() {}

extern "C" void func_804B6418() {}

extern "C" void func_804B64CC() {}

extern "C" void func_804B6580() {}

extern "C" void func_804B6634(void) {}

extern "C" void func_804B6638() {}

extern "C" void func_804B66B0() {}

extern "C" void func_804B6728() {}

extern "C" void func_804B684C() {}

extern "C" void func_804B6954() {}

extern "C" void func_804B6974() {}

extern "C" void func_804B6A3C() {}

extern "C" void func_804B6C48() {}

extern "C" void func_804B6D30() {}

extern "C" void func_804B6F58() {}

extern "C" void func_804B7044() {}

extern "C" void func_804B7074() {}

extern "C" void func_804B708C() {}

extern "C" void func_804B71CC() {}

extern "C" void func_804B74F0() {}

extern "C" void func_804B7540() {}

extern "C" void func_804B7804() {}

extern "C" void func_804B791C() {}

extern "C" void func_804B7944() {}

extern "C" void func_804B7A00() {}

extern "C" void func_804B7AC8(void) {}

extern "C" void func_804B7ACC() {}

extern "C" void func_804B7B6C() {}

extern "C" u32 lbl_eu_8065F1A0;
extern "C" void func_804B7D9C(int, int bit_index) {
    if (bit_index > 0x140) bit_index = 0x140;
    int word_idx = bit_index >> 5;
    int bit_off = bit_index & 0x1F;
    int mask = 1 << bit_off;
    ((int*)&lbl_eu_8065F1A0)[word_idx] &= ~mask;
}

extern "C" void func_804B7DD4() {}

extern "C" void func_804B7E0C() {}

extern "C" void func_804B8078() {}

extern "C" void func_804B80A4() {}

extern "C" void func_804B80CC() {}

extern "C" void func_804B8108() {}

extern "C" void func_804B85C8() {}

extern "C" void func_804B877C() {}

extern "C" void func_804B8C2C() {}

extern "C" void func_804B91E0() {}

extern "C" void func_804B9818() {}

extern "C" void func_804B9E14() {}

extern "C" void func_804BA26C() {}

extern "C" void func_804BA7BC() {}

extern "C" void func_804BA8B4() {}

extern "C" void func_804BADA0() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_804BADB4() {}
