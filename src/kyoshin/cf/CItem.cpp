// Auto-scaffolded catalog TU for kyoshin/cf/CItem
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void func_80155660() {}

extern "C" int func_80155854(unsigned int param_1) {
    int result;
    if (param_1 < 0xC9) {
        if (param_1 < 0x65) {
            if (param_1 < 1) {
                result = 0;
            } else {
                result = 1;
            }
        } else {
            result = 2;
        }
    } else {
        if (param_1 < 0x191) {
            if (param_1 < 0x12D) {
                result = 3;
            } else {
                result = 4;
            }
        } else {
            result = 0;
        }
    }
    return result;
}

extern "C" void func_801558B4() {}

extern "C" void func_80155A00() {}

extern "C" void func_80155AA0() {}

extern "C" void CItem_initItemImplInstances() {}

extern "C" void func_80155CB4() {}

extern "C" void func_80155CC0(void* self, void* ptr, unsigned long val) {
    unsigned long v = *(unsigned long*)ptr;
    *(unsigned long*)ptr = __rlwimi(v, val, 2, 27, 29);
}

extern "C" void func_80155CD0() {}

extern "C" char* func_80155D28() {
    extern char lbl_eu_80501C58[];
    return lbl_eu_80501C58 + 0x63;
}

extern "C" char lbl_eu_806641B8[];

extern "C" void func_80155D38() {}

extern "C" void func_80155D48() {}

extern "C" void func_80155DBC() {}

extern "C" void func_80155E30() {}

extern "C" void func_80155EA4() {}

extern "C" void func_80155F34() {}

extern "C" int func_80156038(void* self) { return 0; }

extern "C" int func_80156040(void* self) { return 0; }

extern "C" int func_80156048(void* self) { return 0; }

extern "C" void func_80156050() {}

extern "C" void func_80156060() {}

extern "C" void func_80156164() {}

extern "C" void func_80156268() {}

extern "C" void func_801562DC() {}

extern "C" void func_80156350() {}

extern "C" void func_8015650C() {}

extern "C" void func_80156924() {}

extern "C" void func_80156928() {}

extern "C" u8 func_8015692C(u32 unused, void* obj) { return *(u8*)((char*)obj + 22); }

extern "C" void func_80156934() {}

extern "C" void func_80156BD8() {}

extern "C" u8 func_80156BDC(u32 unused, void* obj) { return *(u8*)((char*)obj + 22); }

extern "C" void func_80156BE4() {}

extern "C" u8 func_80156CF0(u32 unused, void* obj) { return *(u8*)((char*)obj + 22); }

extern "C" void func_80156CF8() {}

extern "C" void func_80156DAC() {}

extern "C" void func_80156ED4() {}

extern "C" void func_80156F0C() {}

extern "C" void func_80156F30() {}

extern "C" void func_80156F54() {}

extern "C" void func_80156FF8() {}

extern "C" void func_8015704C() {}

extern "C" void func_801570A0() {}

extern "C" void __dt__801570F0() {}

extern "C" void __dt__80157150() {}

extern "C" void func_80157184() {}

extern "C" void func_801571A8() {}

extern "C" void func_801571FC() {}

extern "C" void func_8015720C() {}

extern "C" void func_8015730C() {}

extern "C" void func_801575B0() {}

extern "C" void func_801576C8() {}

extern "C" unsigned short func_8015780C(int index) {
    return ((unsigned short*)(lbl_eu_806641B8 + 0x120EC))[index];
}

extern "C" void func_80157824(int index, short value) {
    ((short*)((char*)lbl_eu_806641B8 + 0x120EC))[index] = value;
}

extern "C" void func_8015783C() {}

extern "C" void func_80157948() {}

extern "C" void func_8015796C() {}

extern "C" void func_801579A4() {}

extern "C" void func_801579C4() {}

extern "C" void func_80157C20() {}

extern "C" void func_80157C4C() {}

extern "C" void func_80157CD0() {}

extern "C" void func_80157D6C() {}

extern "C" void func_80157F04() {}

extern "C" void func_80157FDC() {}

extern "C" void func_80158018() {}

extern "C" void func_80158068() {}

extern "C" void func_80158118() {}

extern "C" void func_801582FC() {}

extern "C" void func_80158300() {}

extern "C" void func_801583DC() {}

extern "C" void func_801583E0() {}

extern "C" void func_80158420() {}

extern "C" void func_801586CC() {}

extern "C" void func_801586D0() {}

extern "C" void func_801586D4() {}

extern "C" void func_80158700() {}

extern "C" void func_801587E8() {}

extern "C" void func_80158894() {}

extern "C" void func_801589A0() {}

extern "C" void __dt__801589BC() {}

extern "C" void func_80158AF4() {}

extern "C" void func_80158E74() {}

extern "C" void func_801591F4() {}

extern "C" void func_801592EC() {}

extern "C" void func_80159348() {}

extern "C" void func_80159524() {}

extern "C" void func_801599D4() {}

extern "C" void func_80159B40() {}

extern "C" void func_80159C04() {}

extern "C" void func_80159D74() {}

extern "C" void func_80159F6C() {}

extern "C" void func_8015A054() {}

extern "C" void func_8015A230() {}

extern "C" void func_8015A234() {}

extern "C" void func_8015A238() {}

extern "C" void func_8015A3CC() {}

extern "C" void func_8015A51C() {}

extern "C" void func_8015A6AC() {}

extern "C" void func_8015A7FC() {}

extern "C" void func_8015A930() {}

extern "C" void func_8015AAB4() {}

extern "C" void func_8015ACAC() {}

extern "C" void func_8015AE9C() {}

extern "C" void func_8015AFA4() {}

extern "C" {
extern u8 lbl_eu_80573EEC[];
}

extern "C" void func_8015B11C() {
    *(u32*)(lbl_eu_80573EEC + 0xd0) = 0;
}

extern "C" void func_8015B130() {}

extern "C" void func_8015B25C() {}

extern "C" int func_8015B3DC(void* self) { return 0; }

extern "C" int func_8015B3E4(void* self) { return 1; }

extern "C" int func_8015B3EC(void* self) { return 0; }

extern "C" int func_8015B3F4(void* self) { return 0; }

extern "C" int func_8015B3FC(void* self) { return 0; }

extern "C" void func_8015B404() {}

extern "C" void func_8015B414() {}

extern "C" void func_8015B420() {}

extern "C" void func_8015B430() {}

extern "C" u32 func_8015B440(u32 unused, void* obj) { return (*(u16*)((char*)obj + 12) >> 1) & 7; }

extern "C" u32 func_8015B44C(u32 unused, void* obj) { return (*(u16*)((char*)obj + 12) >> 4) & 0xFFF; }

extern "C" void* func_8015B458(u32 unused, void* obj) { return (void*)((char*)obj + 8); }

extern "C" u32 func_8015B460(u32 unused, void* obj) { return 1 - *(u8*)((char*)obj + 6); }

extern "C" void func_8015B46C() {}

extern "C" int func_8015B4B4(void* self) { return 16; }

extern "C" u32 func_8015B4BC(u32 unused, void* obj) { return (*(u32*)((char*)obj + 8) >> 7) & 7; }

extern "C" void func_8015B4C8() {}

extern "C" u32 func_8015B4D8(u32 unused, void* obj) { return *(u8*)((char*)obj + 7) & 3; }

extern "C" void func_8015B4E4() {}

extern "C" int func_8015B4E8(void* self) { return 0; }

extern "C" void func_8015B4F0() {}

extern "C" void func_8015B538() {}

extern "C" int func_8015B53C(void* self) { return 0; }

extern "C" int func_8015B544(void* self) { return 0; }

extern "C" int func_8015B54C(void* self) { return 0; }

extern "C" int func_8015B554(void* self) { return 0; }

extern "C" u32 func_8015B55C(u32 unused, void* obj, u16 val) { *(u16*)((char*)obj + 16) = val; return unused; }

extern "C" u16 func_8015B564(u32 unused, void* obj) { return *(u16*)((char*)obj + 16); }

extern "C" void func_8015B56C() {}

extern "C" u32 func_8015B57C(u32 unused, void* obj) { return (*(u16*)((char*)obj + 24) >> 15) & 1; }

extern "C" void func_8015B588() {}

extern "C" u32 func_8015B598(u32 unused, void* obj) { return *(u16*)((char*)obj + 24) & 0x7FFF; }

extern "C" void func_8015B5A4() {}

extern "C" u32 func_8015B5B4(u32 unused, void* obj) { return *(u8*)((char*)obj + 7) & 3; }

extern "C" u32 func_8015B5C0(u32 unused, void* obj) { return 1 - *(u8*)((char*)obj + 6); }

extern "C" u8* func_8015B5CC(u32 unused, u8* obj, u32 idx, u8 val) {
    obj[idx + 18] = val;
    return obj + idx;
}

extern "C" u8 func_8015B5D8(u32 unused, u8* obj, u32 idx) { return obj[idx + 0x12]; }

extern "C" void func_8015B5E4() {}

extern "C" void func_8015B5F8() {}

extern "C" void func_8015B60C() {}

extern "C" int func_8015B654(void* self) { return 28; }

extern "C" void func_8015B65C() {}

extern "C" u32 func_8015B6A8(u32 unused, void* obj) { return 1 - *(u8*)((char*)obj + 6); }

extern "C" void func_8015B6B4() {}

extern "C" void func_8015B75C() {}

extern "C" void func_8015B86C() {}

extern "C" void func_8015B88C() {}

extern "C" u32 func_8015B8A0(u32 unused, void* obj, u8 val) { *(u8*)((char*)obj + 49) = val; return unused; }

extern "C" u8 func_8015B8A8(u32 unused, void* obj) { return *(u8*)((char*)obj + 49); }

extern "C" u32 func_8015B8B0(u32 unused, void* obj, u8 val) { *(u8*)((char*)obj + 48) = val; return unused; }

extern "C" u8 func_8015B8B8(u32 unused, void* obj) { return *(u8*)((char*)obj + 48); }

extern "C" void func_8015B8C0() {}

extern "C" void func_8015B8D0() {}

extern "C" int func_8015B918(void* self) { return 52; }

extern "C" u32 func_8015B920(u32 unused, void* obj) { return 99 - *(u8*)((char*)obj + 6); }

extern "C" void func_8015B92C() {}

extern "C" int func_8015B974(void* self) { return 8; }

extern "C" u32 func_8015B97C(u32 unused, void* obj) { return 1 - *(u8*)((char*)obj + 6); }

extern "C" void func_8015B988() {}

extern "C" int func_8015B9D0(void* self) { return 8; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8015B9D8() {}
