// Auto-scaffolded catalog TU for monolib/src/scn/code_804BD8E8
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void __dt__804BD8E8(){}

void func_804BD94C(){}

void func_804BE2E8(){}

void func_804BE348(){}

void func_804BE398(){}

void func_804BE3E0(){}

void func_804BE408(){}

struct ScnResourceEntry {
    u8 _00[0x20];
    int value;
};

void func_804BE458(){}

void func_804BE470(void* a1, void* a2, void* a3, void* a4, void* a5) {
    extern void func_804BA8B4(void*, void*, void*, void*, void*, void*);
    extern char lbl_eu_8065F32C[];
    func_804BA8B4(lbl_eu_8065F32C, a1, a2, a3, a4, a5);
}

void func_804BE4A0(){}

u32 func_804BE4AC(void) {
    extern u32 lbl_eu_80665988;
    return lbl_eu_80665988;
}

void func_804BE4B4(){}

void func_804BE4E0(){}

extern "C" { extern unsigned char lbl_eu_8065F428[]; }
extern "C" void* func_804BE50C(u32 idx) { return (void*)((char*)lbl_eu_8065F428 + idx * 0x24); }

void* func_804BE520(int index) {
    extern unsigned char lbl_eu_8065F428[];
    return (void*)(lbl_eu_8065F428 + index * sizeof(ScnResourceEntry) + 0x0c);
}

extern "C" void func_804BE62C();
extern "C" void func_804BE538(void) { func_804BE62C(); }

void func_804BE53C(){}

extern "C" void func_804BEE54();
extern "C" void func_804BE5A0(void) { func_804BEE54(); }

extern "C" void func_804BEEAC();
extern "C" void func_804BE5A4(void) { func_804BEEAC(); }

extern "C" void func_804BEDFC();
extern "C" void func_804BE5A8(void) { func_804BEDFC(); }

void func_804BE5AC(){}

extern u8 lbl_eu_8066597C;
u8 func_804BE5B8() { return lbl_eu_8066597C; }

extern u8 lbl_eu_8066597D;
u8 func_804BE5C0() { return lbl_eu_8066597D; }

void func_804BE5C8(){}

int func_804BE604(int index) {
    extern unsigned char lbl_eu_8065F428[];
    ScnResourceEntry* entries = (ScnResourceEntry*)lbl_eu_8065F428;
    return entries[index].value != 0;
}

extern "C" void func_804BF3B4();
extern "C" void func_804BE628(void) { func_804BF3B4(); }

#pragma push
#pragma auto_inline off
extern "C" void func_804BE62C(){}
#pragma pop

#pragma push
#pragma auto_inline off
extern "C" void func_804BEDFC(){}
#pragma pop

#pragma push
#pragma auto_inline off
extern "C" void func_804BEE54(){}
#pragma pop

#pragma push
#pragma auto_inline off
extern "C" void func_804BEEAC(){}
#pragma pop

void func_804BEEEC() {
    extern void *lbl_eu_80663AD8;
    ((void (*)())lbl_eu_80663AD8)();
}

void func_804BEEF8(){}

void func_804BF274(){}

#pragma push
#pragma auto_inline off
extern "C" void func_804BF3B4(){}
#pragma pop

void func_804BF3EC(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_804BF540(){}
