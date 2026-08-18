// Auto-scaffolded catalog TU for monolib/src/scn/code_804BD8E8
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// Cross-TU imports (monolib/coli code_804B59C8). C linkage keeps the retail
// plain symbol names (MWCC would mangle C++-linkage declarations).
extern "C" u32 func_804BADA0(const char* self);
extern "C" void func_804BA7BC(const char* self, u32 a, u32 b);
extern "C" void func_804BA26C(const char* self, u32 a, u32 b, u32 c, u32 d);

extern char lbl_eu_8065F32C[];

void __dt__804BD8E8(){}

void func_804BD94C(){}

void func_804BE2E8(){}

void func_804BE348(){}

void func_804BE398(){}

void func_804BE3E0(u32 a1, u32 a2, u32 a3, u32 a4) {
    func_804BA26C(lbl_eu_8065F32C, a1, a2, a3, a4);
}

void func_804BE408(){}

// 12-byte header block copied out of a ScnResourceEntry.
struct ScnResHead {
    u32 field_0x00;  // 0x00
    u32 field_0x04;  // 0x04
    u32 field_0x08;  // 0x08
};

struct ScnResourceEntry {
    ScnResHead head;             // 0x00
    u8 field_0x0C[0x20 - 0x0C];  // 0x0C
    int value;                   // 0x20
};

void func_804BE458(u32 a1, u32 a2) {
    func_804BA7BC(lbl_eu_8065F32C, a1, a2);
}

void func_804BE470(void* a1, void* a2, void* a3, void* a4, void* a5) {
    extern void func_804BA8B4(void*, void*, void*, void*, void*, void*);
    extern char lbl_eu_8065F32C[];
    func_804BA8B4(lbl_eu_8065F32C, a1, a2, a3, a4, a5);
}

u32 func_804BE4A0() {
    return func_804BADA0(lbl_eu_8065F32C);
}

u32 func_804BE4AC(void) {
    extern u32 lbl_eu_80665988;
    return lbl_eu_80665988;
}

// Copy the 12-byte header of resource entry [index] into dst.
void func_804BE4B4(ScnResHead* dst, int index) {
    extern unsigned char lbl_eu_8065F428[];
    ScnResourceEntry* entries = (ScnResourceEntry*)lbl_eu_8065F428;
    dst->field_0x04 = entries[index].head.field_0x04;
    dst->field_0x00 = entries[index].head.field_0x00;
    dst->field_0x08 = entries[index].head.field_0x08;
}

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

extern "C" int func_804BEE54(u32 flags);
extern "C" int func_804BE5A0(u32 flags) { return func_804BEE54(flags); }

extern "C" void func_804BEEAC();
extern "C" void func_804BE5A4(void) { func_804BEEAC(); }

extern "C" void func_804BEDFC();
extern "C" void func_804BE5A8(void) { func_804BEDFC(); }

int func_804BE5AC() {
    return func_804BEE54(0x8000);
}

extern u8 lbl_eu_8066597C;
extern u8 lbl_eu_8066597D;
u8 func_804BE5B8() { return lbl_eu_8066597C; }

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
extern "C" int func_804BEE54(u32 flags) { return 0; }
#pragma pop

#pragma push
#pragma auto_inline off
extern "C" void func_804BEEAC(){}
#pragma pop

// Dissolved monolibdata2 sdata slot: word 0 is the func_804BF274 pointer
// called by func_804BEEEC (retail loads the data word, not the address).
extern "C" u32 lbl_eu_80663AD8[2];
extern "C" u32 lbl_eu_80663AE0[2];

void func_804BEEEC() {
    ((void (*)())*(u32*)&lbl_eu_80663AD8)();
}

void func_804BEEF8(){}

extern "C" void func_804BF274(){}

#pragma push
#pragma auto_inline off
extern "C" void func_804BF3B4(){}
#pragma pop

void func_804BF3EC(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_804BF540(){}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// func_804BF274 is defined in this TU above (plain C++ linkage, unmangled name)
extern "C" u32 lbl_eu_8066AFC4;   // .sdata2 string

// [.sdata] 0x80663AD8-0x80663AE8 (16B)
extern "C" u32 lbl_eu_80663AD8[2] = { (u32)&func_804BF274, 0x00000000 };
extern "C" u32 lbl_eu_80663AE0[2] = { (u32)&lbl_eu_8066AFC4, 0x00000000 };

// [.bss] 0x8065F418-0x8065FA40 (0x628 = 1576B) zero-fill
u8 lbl_eu_8065F418[16];
u8 lbl_eu_8065F428[1152];
u8 lbl_eu_8065F8A8[24];
u8 lbl_eu_8065F8C0[384];

// [.sbss] 0x8066597C-0x806659A0 (36B) zero-fill. Each global <=8B stays in
// .sbss; all 4-aligned. (Retail .sbss sh_addralign is 4; MWCC emits 8 here --
// needs UNIT_RULES set_data_align=(('.sbss',4)) to fix in postprocess.)
u8 lbl_eu_8066597C;
u8 lbl_eu_8066597D;
u8 lbl_eu_8066597E;
u8 lbl_eu_8066597F;
u32 lbl_eu_80665980;
u32 lbl_eu_80665984;
u32 lbl_eu_80665988;
u32 lbl_eu_8066598C;
u32 lbl_eu_80665990;
u32 lbl_eu_80665994;
u32 lbl_eu_80665998;
u32 lbl_eu_8066599C;
