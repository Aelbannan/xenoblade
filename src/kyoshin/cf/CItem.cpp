// Auto-scaffolded catalog TU for kyoshin/cf/CItem
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void func_80155660(){}

int func_80155854(unsigned int param_1) {
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

void func_801558B4(){}

void func_80155A00(){}

void func_80155AA0(){}

void CItem_initItemImplInstances(){}

void func_80155CB4(){}

void func_80155CC0(void* self, void* ptr, unsigned long val) {
    unsigned long v = *(unsigned long*)ptr;
    *(unsigned long*)ptr = __rlwimi(v, val, 2, 27, 29);
}

void func_80155CD0(){}

char* func_80155D28() {
    extern char lbl_eu_80501C58[];
    return lbl_eu_80501C58 + 0x63;
}

char lbl_eu_806641B8[];

void func_80155D38(){}

void func_80155D48(){}

void func_80155DBC(){}

void func_80155E30(){}

void func_80155EA4(){}

void func_80155F34(){}

int func_80156038(void* self) { return 0; }

int func_80156040(void* self) { return 0; }

int func_80156048(void* self) { return 0; }

void func_80156050(){}

void func_80156060(){}

void func_80156164(){}

void func_80156268(){}

void func_801562DC(){}

void func_80156350(){}

void func_8015650C(){}

void func_80156924() {}

void func_80156928() {}

u8 func_8015692C(u32 unused, void* obj) { return *(u8*)((char*)obj + 22); }

void func_80156934(){}

void func_80156BD8() {}

u8 func_80156BDC(u32 unused, void* obj) { return *(u8*)((char*)obj + 22); }

void func_80156BE4(){}

u8 func_80156CF0(u32 unused, void* obj) { return *(u8*)((char*)obj + 22); }

void func_80156CF8(){}

void func_80156DAC(){}

void func_80156ED4(){}

void func_80156F0C(){}

void func_80156F30(){}

void func_80156F54(){}

void func_80156FF8(){}

void func_8015704C(){}

void func_801570A0(){}

void __dt__801570F0(){}

void __dt__80157150(){}

void func_80157184(){}

void func_801571A8(){}

void func_801571FC(){}

void func_8015720C(){}

void func_8015730C(){}

void func_801575B0(){}

void func_801576C8(){}

unsigned short func_8015780C(int index) {
    return ((unsigned short*)(lbl_eu_806641B8 + 0x120EC))[index];
}

void func_80157824(int index, short value) {
    ((short*)((char*)lbl_eu_806641B8 + 0x120EC))[index] = value;
}

void func_8015783C(){}

void func_80157948(){}

void func_8015796C(){}

void func_801579A4(){}

void func_801579C4(){}

void func_80157C20(){}

void func_80157C4C(){}

void func_80157CD0(){}

void func_80157D6C(){}

void func_80157F04(){}

void func_80157FDC(){}

void func_80158018(){}

void func_80158068(){}

void func_80158118(){}

void func_801582FC() {}

void func_80158300(){}

void func_801583DC() {}

void func_801583E0(){}

void func_80158420(){}

void func_801586CC() {}

void func_801586D0() {}

void func_801586D4(){}

void func_80158700(){}

void func_801587E8(){}

void func_80158894(){}

void func_801589A0(){}

void __dt__801589BC(){}

void func_80158AF4(){}

void func_80158E74(){}

void func_801591F4(){}

void func_801592EC(){}

void func_80159348(){}

void func_80159524(){}

void func_801599D4(){}

void func_80159B40(){}

void func_80159C04(){}

void func_80159D74(){}

void func_80159F6C(){}

void func_8015A054(){}

void func_8015A230() {}

void func_8015A234() {}

void func_8015A238(){}

void func_8015A3CC(){}

void func_8015A51C(){}

void func_8015A6AC(){}

void func_8015A7FC(){}

void func_8015A930(){}

void func_8015AAB4(){}

void func_8015ACAC(){}

void func_8015AE9C(){}

void func_8015AFA4(){}

{
extern u8 lbl_eu_80573EEC[];
}

void func_8015B11C() {
    *(u32*)(lbl_eu_80573EEC + 0xd0) = 0;
}

void func_8015B130(){}

void func_8015B25C(){}

int func_8015B3DC(void* self) { return 0; }

int func_8015B3E4(void* self) { return 1; }

int func_8015B3EC(void* self) { return 0; }

int func_8015B3F4(void* self) { return 0; }

int func_8015B3FC(void* self) { return 0; }

void func_8015B404(){}

void func_8015B414(){}

void func_8015B420(){}

void func_8015B430(){}

u32 func_8015B440(u32 unused, void* obj) { return (*(u16*)((char*)obj + 12) >> 1) & 7; }

u32 func_8015B44C(u32 unused, void* obj) { return (*(u16*)((char*)obj + 12) >> 4) & 0xFFF; }

void* func_8015B458(u32 unused, void* obj) { return (void*)((char*)obj + 8); }

u32 func_8015B460(u32 unused, void* obj) { return 1 - *(u8*)((char*)obj + 6); }

void func_8015B46C(){}

int func_8015B4B4(void* self) { return 16; }

u32 func_8015B4BC(u32 unused, void* obj) { return (*(u32*)((char*)obj + 8) >> 7) & 7; }

void func_8015B4C8(){}

u32 func_8015B4D8(u32 unused, void* obj) { return *(u8*)((char*)obj + 7) & 3; }

void func_8015B4E4() {}

int func_8015B4E8(void* self) { return 0; }

void func_8015B4F0(){}

void func_8015B538() {}

int func_8015B53C(void* self) { return 0; }

int func_8015B544(void* self) { return 0; }

int func_8015B54C(void* self) { return 0; }

int func_8015B554(void* self) { return 0; }

u32 func_8015B55C(u32 unused, void* obj, u16 val) { *(u16*)((char*)obj + 16) = val; return unused; }

u16 func_8015B564(u32 unused, void* obj) { return *(u16*)((char*)obj + 16); }

void func_8015B56C(){}

u32 func_8015B57C(u32 unused, void* obj) { return (*(u16*)((char*)obj + 24) >> 15) & 1; }

void func_8015B588(){}

u32 func_8015B598(u32 unused, void* obj) { return *(u16*)((char*)obj + 24) & 0x7FFF; }

void func_8015B5A4(){}

u32 func_8015B5B4(u32 unused, void* obj) { return *(u8*)((char*)obj + 7) & 3; }

extern "C" u32 func_8015B5C0(u32 unused, void* obj) { return 1 - *(u8*)((char*)obj + 6); }

extern "C" u8* func_8015B5CC(u32 unused, u8* obj, u32 idx, u8 val) {
    obj[idx + 18] = val;
    return obj + idx;
}

extern "C" u8 func_8015B5D8(u32 unused, u8* obj, u32 idx) { return obj[idx + 0x12]; }

void func_8015B5E4(){}

void func_8015B5F8(){}

void func_8015B60C(){}

extern "C" int func_8015B654(void* self) { return 28; }

void func_8015B65C(){}

extern "C" u32 func_8015B6A8(u32 unused, void* obj) { return 1 - *(u8*)((char*)obj + 6); }

void func_8015B6B4(){}

void func_8015B75C(){}

void func_8015B86C(){}

void func_8015B88C(){}

extern "C" u32 func_8015B8A0(u32 unused, void* obj, u8 val) { *(u8*)((char*)obj + 49) = val; return unused; }

extern "C" u8 func_8015B8A8(u32 unused, void* obj) { return *(u8*)((char*)obj + 49); }

extern "C" u32 func_8015B8B0(u32 unused, void* obj, u8 val) { *(u8*)((char*)obj + 48) = val; return unused; }

extern "C" u8 func_8015B8B8(u32 unused, void* obj) { return *(u8*)((char*)obj + 48); }

void func_8015B8C0(){}

void func_8015B8D0(){}

extern "C" int func_8015B918(void* self) { return 52; }

extern "C" u32 func_8015B920(u32 unused, void* obj) { return 99 - *(u8*)((char*)obj + 6); }

void func_8015B92C(){}

extern "C" int func_8015B974(void* self) { return 8; }

extern "C" u32 func_8015B97C(u32 unused, void* obj) { return 1 - *(u8*)((char*)obj + 6); }

void func_8015B988(){}

extern "C" int func_8015B9D0(void* self) { return 8; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_8015B9D8(){}
