// Auto-scaffolded catalog TU for kyoshin/CFloorMap
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CFloorMap.hpp"

u8 func_8024CE60(void* self) { return static_cast<CFloorMapFull*>(self)->field_40; }










void func_80245450(){}

void func_802455F0(){}

void func_8024577C(){}

void func_80245950(){}

void func_80245DF8(){}

void func_80246200(){}

void __dt__802462F0(){}

void func_80246330(){}

void __dt__802468C8(){}

void func_80246908(){}

void func_80247490(){}

void func_8024808C(){}

void func_8024830C(){}

void func_80248558(){}

void func_80248920(){}

void func_80248A6C(){}

void func_80248ED8(){}

void func_80249344(){}

void func_802497B0(){}

void func_80249C1C(){}

void func_8024A448(){}

void func_8024A748(){}

void func_8024AEEC(){}

void func_8024B234(){}

void func_8024B4CC(){}

void __dt__8024B6B8(){}

void func_8024B6F8(){}

void __dt__8024B894(){}

void __ct__CFloorMap(){}

CFloorMap::~CFloorMap() {}

void func_8024BE1C(){}

void func_8024C104(){}

void func_8024C1FC(){}

void func_8024C8F8(){}

void func_8024CB94(){}

void func_8024CE1C(){}


void func_8024CE68(){}

void func_8024D23C(){}

void func_8024D614(){}

void func_8024DA0C(){}

void func_8024DE08(){}

void func_8024E2BC(){}

void func_8024E650(){}

void func_8024E828(){}

void func_8024EA00(){}

void func_8024EC24(){}

void func_8024EE50(){}

void func_8024F1FC(){}

u32 func_8024F538(void* self) {
    u8 val = *(u8*)((u8*)self + 0x41);
    u32 result = __cntlzw(val - 2);
    return result >> 5;
}

u16 func_8024F54C(void* self) { return *(u16*)((u8*)self + 0x5A); }

u8 func_8024F554(void* self) { return static_cast<CFloorMapFull*>(self)->field_58; }

void func_8024F55C(void* self) {
    extern int CSysWin_isActive(void*);
    extern void func_801D216C(void*, int);
    extern void func_8022B8E4(void*);
    extern void func_80138078(unsigned long);
    u8* p = (u8*)self;
    if (p[0x58] && CSysWin_isActive(p + 0xB8)) {
        func_801D216C(p + 0xA0, 0);
        func_8022B8E4(p + 0xB8);
        p[0x58] = 0;
        func_80138078(6);
    }
}

extern "C" char lbl_eu_8050BEA8[];

typedef void* (*VFuncPtr)(void*, const char*, u32);

void func_8024F5C4(void* self, u32 arg2) {
    void* ptr = *(void**)((u8*)self + 0x32D4);
    if (!ptr) return;
    void* obj = *(void**)((u8*)ptr + 0x10);
    VFuncPtr* vt = *(VFuncPtr**)obj;
    void* result = vt[15](obj, (char*)&lbl_eu_8050BEA8 + 0xEE, 1);
    *(u8*)((u8*)result + 0xBB) = (*(u8*)((u8*)result + 0xBB) & 0x7F) | (u8)arg2;
}

unsigned char func_8024F630(void) {
    extern unsigned int lbl_eu_8066479C;
    extern int func_8003B1EC(unsigned int);
    return (unsigned char)func_8003B1EC(lbl_eu_8066479C);
}

void func_8024F658(void* self) {
    extern int CSysWin_getUnk34(void*);
    u8* p = (u8*)self;
    if (p[0x58]) return;
    if (CSysWin_getUnk34(p + 0xB8)) return;
    if (CSysWin_getUnk34(p + 0xF4)) return;
    u8 val = p[0x208];
    u32 result = __cntlzw(val);
    p[0x208] = result >> 5;
}

u8 func_8024F6BC(void* self) {
    CFloorMapFull* full = static_cast<CFloorMapFull*>(self);
    if (full->field_58) return 0;
    return full->field_208;
}

u8 func_8024F6D8(void* self) { return static_cast<CFloorMapFull*>(self)->field_208; }

u32 func_8024F6E0(void* self) {
    s8 idx1 = *(s8*)((u8*)self + 0x206);
    u16 val;
    if (idx1 < 0) {
        val = 0;
    } else {
        s8 idx0 = *(s8*)((u8*)self + 0x205);
        s8 idx2 = *(s8*)((u8*)self + 0x207);
        u32 offset = idx0 * 0x30C + (idx2 + idx1) * 0x18;
        val = *(u16*)((u8*)self + offset + 0x214);
    }
    return val != 0 ? 1 : 0;
}

void func_8024F72C(void* self) {
    extern int CSysWin_getUnk34(void*);
    u8* p = (u8*)self;
    if (CSysWin_getUnk34(p + 0xB8)) return;
    if (CSysWin_getUnk34(p + 0xF4)) return;
    p[0x333C] = (u32)__cntlzw(p[0x333C]) >> 5;
}

u32 func_8024F784(void* self) {
    extern int CSysWin_getUnk34(void*);
    u8* p = (u8*)self;
    if (CSysWin_getUnk34(p + 0xB8)) return 1;
    return CSysWin_getUnk34(p + 0xF4);
}

void func_8024F7CC(){}

void func_8024FB78(){}

void CFloorMap::OnFileEvent() const {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_80250CB4(){}
