// Auto-scaffolded catalog TU for kyoshin/cf/CItem
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// Forward declarations for item-related classes
struct CItemInfo;
struct CItemData;
struct CItemParam;
struct CItemExt;

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

void func_80155CC0(CItemData*, unsigned long* ptr, unsigned long val) {
    unsigned long v = *ptr;
    *ptr = __rlwimi(v, val, 2, 27, 29);
}

void func_80155CD0(){}

char* func_80155D28() {
    extern char lbl_eu_80501C58[];
    return lbl_eu_80501C58 + 0x63;
}

extern char* lbl_eu_806641B8;

extern char lbl_eu_80501C58[];

// Recovered CItemData bitfield area (offsets 0x7/0x8/0x18)
struct ItemByte7 { u8 pad0 : 6; u8 b01 : 2; };
struct ItemWord8 { u32 pad0 : 11; u32 f11 : 11; u32 pad1 : 3; u32 f7 : 7; };
struct ItemHalf18 { u16 bit15 : 1; u16 low15 : 15; };

struct If20 { virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void vf20(); };

extern "C" const char* func_80155D38(void) { return (const char*)lbl_eu_80501C58 + 0x63; }

void func_80155D48(){}

void func_80155DBC(){}

void func_80155E30(){}

void func_80155EA4(){}

void func_80155F34(){}

int CItemInfo_getSize0(CItemInfo*) { return 0; }

int CItemInfo_isEnabled0(CItemInfo*) { return 0; }

int CItemInfo_getFlag0(CItemInfo*) { return 0; }

extern "C" void func_80156050(void* self) { reinterpret_cast<If20*>(self)->vf20(); }

void func_80156060(){}

void func_80156164(){}

void func_80156268(){}

void func_801562DC(){}

void func_80156350(){}

void func_8015650C(){}

void func_80156924() {}

void func_80156928() {}

u8 CItemInfo_getByte22(u32, CItemInfo* obj) { return *(u8*)((char*)obj + 22); }

void func_80156934(){}

void func_80156BD8() {}

u8 CItemInfo_getByte22_dup1(u32, CItemInfo* obj) { return *(u8*)((char*)obj + 22); }

void func_80156BE4(){}

u8 CItemInfo_getByte22_dup2(u32, CItemInfo* obj) { return *(u8*)((char*)obj + 22); }

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

extern "C" void* func_801571FC(void) { return *(void**)((u8*)lbl_eu_806641B8 + 0x10000 + 0x20e8); }

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

extern "C" u32 func_8009CF8C(u32 resourceId);

extern "C" void* func_801579C4(u8 arg, u32* out1, u32* out2) {
    void* result = 0;
    u32* ptr;
    u32 v;

    func_8009CF8C(0x80c);

    *out1 = 0;

    for (u32 i = 1; i <= 11; i++) {
        if (!!(((u32*)lbl_eu_806641B8)[0x4842] & (1 << i))) {
            *out1 += 10;
        }
    }

    ptr = (u32*)lbl_eu_806641B8;
    v = ptr[0x4844];
    if (v < 30) {
        v = 30;
    }
    ptr[0x4844] = v;

    if (arg > 13) {
        *out1 = 0;
        *out2 = 0;
        return 0;
    }

    switch (arg) {
        case 0:
        case 1:
            *out2 = 0;
            *out1 = 0;
            return 0;
        case 2:
            *out2 = 52;
            result = (char*)ptr + 0xA58C;
            *out1 += v + 60;
            break;
        case 3:
            *out2 = 16;
            *out1 = 300;
            result = (char*)ptr + 0xE778;
            break;
        case 4:
            *out2 = 52;
            result = ptr;
            *out1 += v + 60;
            break;
        case 5:
            *out2 = 52;
            result = (char*)ptr + 0x211C;
            *out1 += v + 60;
            break;
        case 6:
            *out2 = 52;
            result = (char*)ptr + 0x4238;
            *out1 += v + 60;
            break;
        case 7:
            *out2 = 52;
            result = (char*)ptr + 0x6354;
            *out1 += v + 60;
            break;
        case 8:
            *out2 = 52;
            result = (char*)ptr + 0x8470;
            *out1 += v + 60;
            break;
        case 9:
            *out2 = 28;
            *out1 = 300;
            result = (char*)ptr + 0xE778;
            break;
        case 10:
            *out2 = 8;
            *out1 = 300;
            result = (char*)ptr + 0x101B8;
            break;
        case 11:
            *out2 = 8;
            result = (char*)ptr + 0x10B18;
            *out1 += v + 60;
            break;
        case 12:
            *out2 = 8;
            *out1 = 200;
            result = (char*)ptr + 0x11478;
            break;
        case 13:
            *out2 = 8;
            *out1 = 240;
            result = (char*)ptr + 0xFA38;
            break;
    }

    return result;
}

extern "C" u32 func_80157C20(u8 arg) {
    u32 a, b;
    func_801579C4(arg, &a, &b);
    return a;
}

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

extern u8 lbl_eu_80573EEC[];

void func_8015B11C() {
    *(u32*)(lbl_eu_80573EEC + 0xd0) = 0;
}

void func_8015B130(){}

void func_8015B25C(){}

int CItemData_isFalse0(CItemData*) { return 0; }

int CItemData_isTrue(CItemData*) { return 1; }

int CItemData_isFalse1(CItemData*) { return 0; }

int CItemData_isFalse2(CItemData*) { return 0; }

int CItemData_isFalse3(CItemData*) { return 0; }

extern "C" void func_8015B404(void* u, void* p, u32 val) { ((ItemWord8*)((u8*)p + 8))->f7 = val; }

void func_8015B414(){}

extern "C" void func_8015B420(void* u, void* p, u32 val) { ((ItemWord8*)((u8*)p + 8))->f11 = val; }

extern "C" s16 func_8015B430(void* u, void* p) {
    u32 x = *(u32*)((u8*)p + 8);
    return (s16)((x >> 10) & 0x7FF);
}

u32 CItemParam_getCategory(u32, CItemParam* obj) { return (*(u16*)((char*)obj + 12) >> 1) & 7; }

u32 CItemParam_getId(u32, CItemParam* obj) { return (*(u16*)((char*)obj + 12) >> 4) & 0xFFF; }

void* CItemData_getBuffer(u32, CItemData* obj) { return (void*)((char*)obj + 8); }

u32 CItemData_getInvByte6(u32, CItemData* obj) { return 1 - *(u8*)((char*)obj + 6); }

void func_8015B46C(){}

int CItemData_getSize16(CItemData*) { return 16; }

u32 CItemData_getBits7to9(u32, CItemData* obj) { return (*(u32*)((char*)obj + 8) >> 7) & 7; }

extern "C" void func_8015B4C8(void* u, void* p, u32 val) { ((ItemByte7*)((u8*)p + 7))->b01 = (u8)val; }

u32 CItemData_getByte7Bits01(u32, CItemData* obj) { return *(u8*)((char*)obj + 7) & 3; }

void func_8015B4E4() {}

int CItemData_isFalse4(CItemData*) { return 0; }

void func_8015B4F0(){}

void func_8015B538() {}

int CItemParam_isFalse0(CItemParam*) { return 0; }

int CItemParam_isFalse1(CItemParam*) { return 0; }

int CItemParam_isFalse2(CItemParam*) { return 0; }

int CItemParam_isFalse3(CItemParam*) { return 0; }

u32 CItemParam_setField16(u32 unused, CItemParam* obj, u16 val) { *(u16*)((char*)obj + 16) = val; return unused; }

u16 CItemParam_getField16(u32, CItemParam* obj) { return *(u16*)((char*)obj + 16); }

extern "C" void func_8015B56C(void* u, void* p, u32 val) { ((ItemHalf18*)((u8*)p + 0x18))->bit15 = (u16)val; }

u32 CItemParam_getField24Bit15(u32, CItemParam* obj) { return (*(u16*)((char*)obj + 24) >> 15) & 1; }

extern "C" void func_8015B588(void* u, void* p, u32 val) { ((ItemHalf18*)((u8*)p + 0x18))->low15 = (u16)val; }

u32 CItemParam_getField24Mask(u32, CItemParam* obj) { return *(u16*)((char*)obj + 24) & 0x7FFF; }

extern "C" void func_8015B5A4(void* u, void* p, u32 val) { ((ItemByte7*)((u8*)p + 7))->b01 = (u8)val; }

u32 CItemData_getByte7Bits01_dup(u32, CItemData* obj) { return *(u8*)((char*)obj + 7) & 3; }

u32 CItemData_getInvByte6_dup(u32, CItemData* obj) { return 1 - *(u8*)((char*)obj + 6); }

extern "C" u8* func_8015B5CC(u32 unused, u8* obj, u32 idx, u8 val) {
    obj[idx + 18] = val;
    return obj + idx;
}

extern "C" u8 func_8015B5D8(u32 unused, u8* obj, u32 idx) { return obj[idx + 0x12]; }

extern "C" u32 func_8015B5E4(void* u, void* p, u32 idx) { return (*(u16*)((u8*)p + idx * 2 + 8) >> 1) & 7; }

extern "C" u32 func_8015B5F8(void* u, void* p, u32 idx) { return (*(u16*)((u8*)p + idx * 2 + 8) >> 4) & 0xFFF; }

void func_8015B60C(){}

int CItemExt_getSize28(CItemExt*) { return 28; }

void func_8015B65C(){}

u32 CItemData_getInvByte6_dup2(u32, CItemData* obj) { return 1 - *(u8*)((char*)obj + 6); }

void func_8015B6B4(){}

void func_8015B75C(){}

void func_8015B86C(){}

extern "C" u32 func_8015B88C(void* u, void* p, u32 idx) { return (*(u16*)((u8*)p + idx * 8 + 0xc) >> 4) & 0xFFF; }

u32 CItemExt_setByte49(u32 unused, CItemExt* obj, u8 val) { *(u8*)((char*)obj + 49) = val; return unused; }

u8 CItemExt_getByte49(u32, CItemExt* obj) { return *(u8*)((char*)obj + 49); }

u32 CItemExt_setByte48(u32 unused, CItemExt* obj, u8 val) { *(u8*)((char*)obj + 48) = val; return unused; }

u8 CItemExt_getByte48(u32, CItemExt* obj) { return *(u8*)((char*)obj + 48); }

extern "C" void* func_8015B8C0(void* u, void* p, u32 idx) { return (u8*)p + idx * 8 + 8; }

void func_8015B8D0(){}

int CItemExt_getSize52(CItemExt*) { return 52; }

u32 CItemExt_get99minusByte6(u32, CItemExt* obj) { return 99 - *(u8*)((char*)obj + 6); }

void func_8015B92C(){}

int CItemExt_getSize8(CItemExt*) { return 8; }

u32 CItemExt_getInvByte6(u32, CItemExt* obj) { return 1 - *(u8*)((char*)obj + 6); }

void func_8015B988(){}

int CItemExt_getSize8_dup(CItemExt*) { return 8; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_8015B9D8(){}
