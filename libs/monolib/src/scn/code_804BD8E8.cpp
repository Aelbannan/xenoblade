// Auto-scaffolded catalog TU for monolib/src/scn/code_804BD8E8
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"
#include "monolib/scn/code_804BF59C.hpp"
#include "monolib/effect/code_804CC2B8.hpp"

// Cross-TU imports (monolib/coli code_804B59C8). C linkage keeps the retail
// plain symbol names (MWCC would mangle C++-linkage declarations).
extern "C" u32 func_804BADA0(const char* self);
extern "C" void func_804BA7BC(const char* self, u32 a, u32 b);
extern "C" void func_804BA26C(const char* self, u32 a, u32 b, u32 c, u32 d);

extern char lbl_eu_8065F32C[];
extern u32 lbl_eu_80665988;
extern u8 lbl_eu_8065F8C0[0x180];

// Resource-manager methods (monolib/coli code_804B59C8). C linkage keeps
// the retail plain symbol names (MWCC would mangle C++-linkage
// declarations).
extern "C" void func_804B8C2C(const char* self, void* a1, void* a2, u32 a3, u32 a4, u32 a5);
extern "C" void func_804B9818(const char* self, u32 a, u32 b, u32 c);

// ScnResourceEntry ctor callback and the flag-data ctor callback for the
// two .ctors array constructions are declared in code_804BF59C.hpp and
// code_804CC2B8.hpp (included above).

extern "C" u32 func_804BE62C(u32 self);

void __dt__804BD8E8(){}

void func_804BD94C(){}

void func_804BE2E8(){}

u32 func_804BE348(void* a1, void* a2, u32 a3, u32 a4, u32 a5) {
    func_804B8C2C(lbl_eu_8065F32C, a1, a2, a3, a4, a5);
    return lbl_eu_80665988;
}

void func_804BE398(){}

void func_804BE3E0(u32 a1, u32 a2, u32 a3, u32 a4) {
    func_804BA26C(lbl_eu_8065F32C, a1, a2, a3, a4);
}

u32 func_804BE408(u32 self, u32 a, u32 b, u32 c) {
    func_804B9818(lbl_eu_8065F32C, a, b, c);
    u32 count = lbl_eu_80665988;
    if (count != 0) {
        return func_804BE62C(self);
    }
    return 0;
}

// 12-byte header block copied out of a ScnResourceEntry.
struct ScnResHead {
    u32 field_0x00;  // 0x00
    u32 field_0x04;  // 0x04
    u32 field_0x08;  // 0x08
};

// Resource info block referenced by ScnResourceEntry::field_0x18; a u16
// table index lives at +0x12.
struct ScnResInfo {
    u8 field_0x00[0x12];  // 0x00
    u16 field_0x12;       // 0x12
};

// Scene resource manager object at lbl_eu_8065F32C. The byte table at +0x28
// holds u32 flag words indexed by ScnResInfo::field_0x12 and doubles as a
// byte-offset base for the matching flag data.
struct ScnResMgr {
    u8 field_0x00[0x28];  // 0x00
    u8* field_0x28;       // 0x28
};

struct ScnResourceEntry {
    ScnResHead head;             // 0x00
    u8 field_0x0C[0x18 - 0x0C];  // 0x0C
    ScnResInfo* field_0x18;      // 0x18
    u32 field_0x1C;              // 0x1C
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

u32 func_804BE538(u32 self) { return func_804BE62C(self); }

void func_804BE53C(){}

extern "C" int func_804BEE54(u32 flags);
extern "C" int func_804BE5A0(u32 flags) { return func_804BEE54(flags); }

extern "C" void func_804BEEAC();
extern "C" void func_804BE5A4(void) { func_804BEEAC(); }

extern "C" int func_804BEDFC(u32* out, u32 flags, int index, u8* base);
int func_804BE5A8(u32* a1, u32 a2, int a3, u8* a4) { return func_804BEDFC(a1, a2, a3, a4); }

int func_804BE5AC() {
    return func_804BEE54(0x8000);
}

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
u32 func_804BE62C(u32 self) { return 0; }
#pragma pop

#pragma push
#pragma auto_inline off
int func_804BEDFC(u32* out, u32 flags, int index, u8* base) {
    ScnResourceEntry* entry = &((ScnResourceEntry*)lbl_eu_8065F428)[index];
    ScnResInfo* info = entry->field_0x18;
    u8* tbl = ((ScnResMgr*)lbl_eu_8065F32C)->field_0x28;
    u16 id = info->field_0x12;
    if (flags & ((u32*)tbl)[id]) {
        *out = *(u8*)(base + (u32)tbl + (id + 1) * 4);
        return 1;
    }
    return 0;
}
#pragma pop

#pragma push
#pragma auto_inline off
int func_804BEE54(u32 flags) {
    int count = (int)lbl_eu_80665988;
    if (count != 0) {
        ScnResourceEntry* entry = (ScnResourceEntry*)lbl_eu_8065F428;
        u8* tbl = ((ScnResMgr*)lbl_eu_8065F32C)->field_0x28;
        for (int i = 0; i < count; i++) {
            u16 id = entry->field_0x18->field_0x12;
            if (flags & ((u32*)tbl)[id]) {
                return 1;
            }
            entry++;
        }
    }
    return 0;
}
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
void sinit_804BF540() {
    __construct_array(lbl_eu_8065F428, (ConstructorDestructor)func_804BF59C, NULL, 0x24, 0x20);
    __construct_array(lbl_eu_8065F8C0, (ConstructorDestructor)func_8004B0B0, NULL, 0xc, 0x20);
}
