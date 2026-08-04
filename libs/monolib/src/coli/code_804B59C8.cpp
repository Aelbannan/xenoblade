// Auto-scaffolded catalog TU for monolib/src/coli/code_804B59C8
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void func_804B59C8(){}

u32 func_804B5A68(void) {
    extern u32 lbl_eu_80663AC8;
    return lbl_eu_80663AC8;
}

void* func_804B5A70(void){ return 0; }

void __dt__804B5A7C(){}

void __dt__804B5AD0(){}

void func_804B5B34(void) {}

void func_804B5B38(){}

void func_804B5BB0(){}

void func_804B5C58(){}

void func_804B5CEC(){}

void func_804B5DE8(){}

void func_804B5E68(){}

void func_804B61F8(void) {}

void func_804B61FC(){}

void func_804B62B0(){}

void func_804B6364(){}

void func_804B6418(){}

void func_804B64CC(){}

void func_804B6580(){}

void func_804B6634(void) {}

void func_804B6638(){}

void func_804B66B0(){}

void func_804B6728(){}

void func_804B684C(){}

extern "C" { extern char lbl_eu_8065F1C8[]; }
extern "C" void func_804B6954(void* a, void* b, void* c) {
    void (*fn)(void*, void*, void*) = *(void (**)(void*, void*, void*))(lbl_eu_8065F1C8 + 0xA0);
    void* p = *(void**)(lbl_eu_8065F1C8 + 0x9C);
    fn(b, c, p);
}

void func_804B6974(){}

void func_804B6A3C(){}

void func_804B6C48(){}

void func_804B6D30(){}

void func_804B6F58(){}

extern "C" u32 func_804B7044(void* self, float f) {
    u32 r = 0;
    if (f <= *(float*)((u8*)self + 0x64) && f >= *(float*)((u8*)self + 0x68)) r = 1;
    return r;
}

extern "C" u32 func_804B7074(void* self, float f) { return f <= *(float*)((u8*)self + 0x64); }

void func_804B708C(){}

void func_804B71CC(){}

void func_804B74F0(){}

void func_804B7540(){}

void func_804B7804(){}

void func_804B791C(){}

void func_804B7944(){}

void func_804B7A00(){}

void func_804B7AC8(void) {}

void func_804B7ACC(){}

void func_804B7B6C(){}

u32 lbl_eu_8065F1A0;
void func_804B7D9C(int, int bit_index) {
    if (bit_index > 0x140) bit_index = 0x140;
    int word_idx = bit_index >> 5;
    int bit_off = bit_index & 0x1F;
    int mask = 1 << bit_off;
    int* bits = (int*)&lbl_eu_8065F1A0;
    bits[word_idx] &= ~mask;
}

void func_804B7DD4(){}

void func_804B7E0C(){}

void func_804B8078(){}

void func_804B80A4(){}

void func_804B80CC(){}

void func_804B8108(){}

void func_804B85C8(){}

void func_804B877C(){}

void func_804B8C2C(){}

void func_804B91E0(){}

void func_804B9818(){}

void func_804B9E14(){}

void func_804BA26C(){}

void func_804BA7BC(){}

void func_804BA8B4(){}

extern "C" u32 func_804BADA0(void* self) { return *(u32*)((u8*)self + 0x5C) != 0; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_804BADB4(){}
