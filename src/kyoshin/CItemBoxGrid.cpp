// Auto-scaffolded catalog TU for kyoshin/CItemBoxGrid
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CItemBoxGrid.hpp"
#include <stdio.h>

// Forward declarations for external functions
extern int func_8022D09C(void*);
extern int func_8022DB74(void*);
extern void __dl__FPv(void*);
extern void* func_801394D4(u32);
extern void func_801D4AE0(void*, int, void*);
extern void func_801CFF28(void*);
extern int CSysWin_isActive(void*);
extern int CSysWin_getUnk34(void*);
extern void func_801D216C(void*, int);
extern void func_801D0950(void*);
extern u32 func_80137510(u32, float);
extern void* func_80157C4C(u8, s16);
extern u32 func_801392E4(u32);
extern u32 func_801361E8(u32, const char*, u32);
extern u32 func_80139358(u32);
extern void* CItem_initItemImplInstances(void*);
extern u32 func_801D3320(void*);
extern void func_80158118(void*, u32);
extern u32 func_80208360(void*);
extern void func_80208760(void*, void*);
extern void func_8022E498(void*, void*);
extern void func_8022D0F8(void*, void*, u8);
extern u32 func_801D32DC(void*);
extern u32 getItemBoxState__FP12CItemBoxInfo(void*);
extern u32 func_801EB018(void*);
extern u32 func_8022D08C(void*);
extern u32 CSysWin_isReady(void*);
void* __dt__13CArtsBookItemFv(void* self, int mode);
void* __dt__11CVisionItemFv(void* self, int mode);
void* __dt__10CQuestItemFv(void* self, int mode);
extern u32 func_801EB020(void*);
extern u32 func_80208358(void*);
extern u32 func_8022D094(void*);
extern u32 func_8022DB6C(void*);
extern void advanceItemBoxState__FP12CItemBoxInfo(void*);
extern void func_801EB410(void*, int);
extern void func_801EB0D4(void*);
extern void func_801D0328(void*);
extern void func_801CFFEC(void*);
extern void func_801D05D4(void*, int);
extern void func_80138078__FUl(u32);
extern u32 func_8015780C(void*);
extern u32 func_801D3328(void*);
extern void func_801D3454(void*);
extern void func_801D3620(void*);
extern void func_801D3698(void*);
extern void func_801D3724(void*);
extern void func_801D377C(void*);
extern void func_801D3408(void*);
extern u32 func_801EB028(void*);
extern u32 func_801EB064(void*);
extern u32 func_801EB04C(void*);
extern u32 func_801EB218(void*);
extern u32 func_801EB314(void*);
extern void func_801EB178(void*);
extern void func_8020844C(void*);
extern void func_802083A4(void*);
extern void func_80208838(void*);
extern void func_8022E3A8(void*);
extern void func_8022E490(void*);
extern void func_8022E2F8(void*);
extern void func_8022E3AC(void*);
extern void func_8022E488(void*);
extern void func_8022DD68(void*);
extern void func_8022D0D0(void*);
extern void func_8022B8E4(void*);
extern void func_801D47D4(void*, u32, u32, u32);
extern u32 func_801D421C(void*);
extern void func_801D4C3C(void*, u32);
extern void func_80124270(void*, u32);
extern u32 func_80137E7C(void*, u32);
extern u32 func_80137924(void*, u32);
extern u32 func_8013600C(void*, u32);
extern u32 func_800A32BC(u32);
extern u32 func_8003B1EC(u32);
extern u32 func_8009CF8C(u32);
extern u32 func_80139A18(void*);
extern void func_80136190(void*, u32, u32);
extern void func_801C5158(void*, u32);
extern u32 func_801392B4(void*);
extern u32 func_801393CC(void*);
extern u32 func_8026178C(void*);
extern u32 func_8025FB10(void*);
extern u32 func_802083CC(void*);
extern u32 func_8009EC9C(u32);
extern u32 func_800A082C(void);
extern u32 code80135FDC_getByte_64077(void*);
extern void func_801C4B60(void*, u8, u8, u8, u8);
extern void func_801D1F9C(void*, u32);
extern u32 func_801C62AC(void*, u32);
extern u32 func_801C631C(void*, u32);
extern u16 ArrayGet12(u8);
extern int func_801C6E90(void*);
extern u32 func_801D4260(void*, u8);
extern void func_801D0BD8(void*);
extern void func_801CFD2C(void*);
extern const float lbl_eu_80667F40;
extern const float lbl_eu_80667F48;
extern const float lbl_eu_80667F80;
extern float lbl_eu_80667F78;
extern const float lbl_eu_80667F30;
extern const float lbl_eu_80667F34;
extern const float lbl_eu_80667F38;
extern const float lbl_eu_80667F50;
extern const float lbl_eu_80667F54;
extern const float lbl_eu_80667F58;
extern const float lbl_eu_80667F74;
extern const float lbl_eu_80667F7C;
extern const float lbl_eu_80667F88;
extern const float lbl_eu_80667F8C;
extern const float lbl_eu_80667F90;
extern const float lbl_eu_80667F94;
extern const float lbl_eu_80667F98;

extern u32 lbl_eu_80664098;
extern u32 lbl_eu_806640D8;
extern u32 lbl_eu_806640EC;

extern u32 lbl_eu_806640F8;
extern u32 lbl_eu_80664104;
extern u32 lbl_eu_80664110;
extern u32 lbl_eu_80664488;
extern u32 lbl_eu_80664490;
extern u32 lbl_eu_80664498;
extern u32 lbl_eu_806644A0;
extern u32 lbl_eu_806644A8;
extern u32 lbl_eu_806644B0;
extern u32 lbl_eu_806644B8;
extern u32 lbl_eu_806644C0;
extern u32 lbl_eu_806644C8;
extern u32 lbl_eu_806644D0;
extern u32 lbl_eu_806644D8;
extern u32 lbl_eu_806644E0;
extern u32 lbl_eu_806644E8;
extern u32 lbl_eu_806644F0;
extern u32 lbl_eu_806644F8;
extern u32 lbl_eu_80664500;
extern u32 lbl_eu_80664508;
extern u32 lbl_eu_8066450C;
extern u32 lbl_eu_80664510;

extern const char lbl_eu_80505590[];
extern const char lbl_eu_805055F0[];
extern const char lbl_eu_8050560C[];
extern const char lbl_eu_80505628[];
extern const char lbl_eu_80534740[];
extern const char lbl_eu_80534818[];
extern const char lbl_eu_80573D18[];
extern const char lbl_eu_8050566C[];
extern u32 lbl_eu_80664514;
extern char lbl_eu_806640F4[];
extern u32 func_80157C20(u8);
extern void func_801CE390(void*);
extern float func_801C9F88(void*, void*);
extern u32 func_80137444__FPQ34nw4r3lyt13AnimTransformf(void*, float);
extern void CopyEntry9Bytes(char*, const char*);
extern void __ct__CVisionItem(void*);
extern void __ct__CArtsBookItem(void*);
extern void SetEntry9Bytes(unsigned char*, unsigned short, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);
extern void func_801C562C(void*, char*);

u8 func_801C67F8(CItemBoxGridFull* self);
u8 func_801C6840(CItemBoxGridFull* self);

u8 CItemBoxGrid::GetField61() { return reinterpret_cast<CItemBoxGridFull*>(this)->field_61; }

// Constructor for sub-item grid.
void* __ct__801C5514(void* self) {
    u8* p = (u8*)self;
    u32 i;
    // Initialize entry array
    u8* end = p + 0x2800;
    u8* cur = p;
    while (cur < end) {
        SetEntry9Bytes(cur, (unsigned short)0xFFFF,
                       (unsigned char)0, (unsigned char)0, (unsigned char)0,
                       (unsigned char)0, (unsigned char)0, (unsigned char)0,
                       (unsigned char)0);
        cur += 10;
    }
    *(u16*)(p + 0x2800) = 0;
    p[0x2802] = 0;
    p[0x2803] = 0;
    p[0x2804] = 0;
    *(u32*)(p + 0x2CA8) = (u32)&lbl_eu_80534818;
    *(u16*)(p + 0x34AC) = 0;
    __ct__CVisionItem(p + 0x34B0);
    __ct__CArtsBookItem(p + 0x3CB8);
    lbl_eu_80664514 = (u32)self;
    for (i = 0; i < 0x400; i++) {
        char tmp[27];
        SetEntry9Bytes((unsigned char*)tmp, (unsigned short)0xFFFF,
                       (unsigned char)0, (unsigned char)0, (unsigned char)0,
                       (unsigned char)0, (unsigned char)0, (unsigned char)0,
                       (unsigned char)0);
        // func_801C562C copies from tmp to p+i*10
        func_801C562C(p + i * 10, tmp);
    }
    return self;
}

void SetEntry9Bytes(unsigned char* p, unsigned short a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f, unsigned char g, unsigned char h) {
    unsigned char* buf = (unsigned char*)p;
    *((unsigned short*)(buf + 0)) = a;
    buf[2] = b;
    buf[3] = c;
    buf[4] = d;
    buf[5] = e;
    buf[6] = f;
    buf[7] = g;
    buf[8] = h;
}

// Copy a 9-byte entry from src to dst.
void func_801C562C(void* dst, void* src) {
    u8* d = (u8*)dst;
    u8* s = (u8*)src;
    short v0 = *(short*)(s + 0);
    u8 v1 = s[2];
    u8 v2 = s[3];
    u8 v3 = s[4];
    u8 v4 = s[5];
    u8 v5 = s[6];
    u8 v6 = s[7];
    u8 v7 = s[8];
    *(short*)(d + 0) = v0;
    d[2] = v1;
    d[3] = v2;
    d[4] = v3;
    d[5] = v4;
    d[6] = v5;
    d[7] = v6;
    d[8] = v7;
}

// Destructor for the main grid container.
void* __dt__801C5670(void* self, int mode) {
    u8* p = (u8*)self;
    if (!self) return self;
    lbl_eu_80664514 = 0;
    __dt__13CArtsBookItemFv(p + 0x3cb8, 0xFFFFFFFF);
    __dt__11CVisionItemFv(p + 0x34b0, 0xFFFFFFFF);
    if (mode > 0) __dl__FPv(self);
    return self;
}

void func_801C56D8() { }

// Search for a matching short id in an array, return 1 if found.
int func_801C51BC(void* obj, u32 id) {
    u16 count = *(u16*)((u8*)obj + 0x804);
    u16 i;
    for (i = 0; i < count; i++) {
        if (*(u16*)((u8*)obj + 4 + i * 2) == (u16)id) {
            return 1;
        }
    }
    return 0;
}

// Convert category to timing value - complex float math
// Compute grid row count.
u32 func_801C5E5C(void* self) {
    u8* p = (u8*)self;
    u32 val = func_80157C20(p[0x2802]);
    float f = (float)(s32)val;
    float f50 = lbl_eu_80667F50;
    if (f >= f50) return 10;
    float div = f / lbl_eu_80667F30;
    s32 result = (s32)div;
    float frac = div - (float)result;
    if (lbl_eu_80667F34 != frac) result++;
    return (u8)result;
}

// Increment a sub-index counter; wrap to 0 when reaching the limit.
void func_801C5EF4(CItemBoxGridFull* self) {
    u8 idx = self->field_2804 + 1;
    self->field_2804 = idx;
    if ((s8)idx >= self->field_2803) {
        self->field_2804 = 0;
    }
}

// Decrement a sub-index counter; borrow from the limit field when underflowing.
void func_801C5F20(CItemBoxGridFull* self) {
    u8 idx = self->field_2804 - 1;
    self->field_2804 = idx;
    if ((s8)idx < 0) {
        self->field_2804 = self->field_2803 - 1;
    }
}

// Lookup entry in 10-byte stride table; return stored short or -1.
s16 func_801C5F48(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= self->field_2800) return -1;
    return *(s16*)((u8*)self + offset * 0xa);
}

void func_801C5FC0() { }

// Round a double to nearest integer with .5 tie-breaking biased away from zero.
long func_801C6158(double f) {
    if (f > 0.0) {
        return (long)(f + 0.5);
    } else {
        return (long)(f - 0.5);
    }
}

void func_801C618C() { }

// Lookup entry, check category; return word >> 20 or 0.
u32 func_801C62AC(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= self->field_2800) return 0;
    s16 val = *(s16*)((u8*)self + offset * 0xa);
    void* obj = func_80157C4C(self->field_2802, val);
    if (!obj || !*(u32*)obj) return 0;
    return *(u32*)obj >> 20;
}

// Lookup entry, check category; return obj ptr or 0.
void* func_801C631C(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= self->field_2800) return 0;
    s16 val = *(s16*)((u8*)self + offset * 0xa);
    void* obj = func_80157C4C(self->field_2802, val);
    if (!obj || !*(u32*)obj) return 0;
    return obj;
}

// Check item entry validity based on category.
s32 func_801C6388(CItemBoxGridFull* self, u16 idx) {
    u8* p = (u8*)self;
    s8 base = (s8)p[0x2804];
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= *(u16*)(p + 0x2800)) return 0;
    s16 val = *(s16*)(p + offset * 0xa);
    void* obj = func_80157C4C(p[0x2802], val);
    if (!obj || !*(u32*)obj) return 0;
    u8 cat = p[0x2802];
    if ((u32)(cat - 4) <= 4) {
        // category 4-8
        void* inst = CItem_initItemImplInstances(obj);
        void** vtbl = *(void***)inst;
        u8 result = (u8)((u32(*)(void*, void*))vtbl[12])(inst, obj);
        if (!result) {
            u8 v2 = ((u8*)p + offset * 0xa)[2];
            return (s8)((v2 - 1) | 1);
        }
        u32 j;
        for (j = 0; j < result; j++) {
            void* inst2 = CItem_initItemImplInstances(obj);
            void** vtbl2 = *(void***)inst2;
            s16 r = (s16)((s32(*)(void*, void*, u32))vtbl2[16])(inst2, obj, j);
            if (r == -1) {
                void* inst3 = CItem_initItemImplInstances(obj);
                void** vtbl3 = *(void***)inst3;
                void* res3 = ((void*(*)(void*, void*, u32))vtbl3[11])(inst3, obj, j);
                if (res3 && (*(u16*)((u8*)res3 + 4) & 1)) return -3;
            }
        }
        return -2;
    } else if ((u32)(cat - 10) <= 3) {
        // category 10-13
        u8 v6 = ((u8*)obj)[6];
        return (s8)v6;
    } else if (cat == 2) {
        // same as cat 4-8
        void* inst = CItem_initItemImplInstances(obj);
        void** vtbl = *(void***)inst;
        u8 result = (u8)((u32(*)(void*, void*))vtbl[12])(inst, obj);
        if (!result) {
            u8 v2 = ((u8*)p + offset * 0xa)[2];
            return (s8)((v2 - 1) | 1);
        }
        u32 j;
        for (j = 0; j < result; j++) {
            void* inst2 = CItem_initItemImplInstances(obj);
            void** vtbl2 = *(void***)inst2;
            s16 r = (s16)((s32(*)(void*, void*, u32))vtbl2[16])(inst2, obj, j);
            if (r == -1) {
                void* inst3 = CItem_initItemImplInstances(obj);
                void** vtbl3 = *(void***)inst3;
                void* res3 = ((void*(*)(void*, void*, u32))vtbl3[11])(inst3, obj, j);
                if (res3 && (*(u16*)((u8*)res3 + 4) & 1)) return -3;
            }
        }
        return -2;
    } else if (cat == 3) {
        u8 v2 = ((u8*)p + offset * 0xa)[2];
        return (s8)((v2 - 1) | 1);
    } else if (cat == 9) {
        return 0;
    }
    return 0;
}

// Lookup entry, check category; return byte at offset 2 or 0.
u8 func_801C6528(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= self->field_2800) return 0;
    void* entry = (u8*)self + offset * 0xa;
    s16 val = *(s16*)entry;
    void* obj = func_80157C4C(self->field_2802, val);
    if (!obj || !*(u32*)obj) return 0;
    return ((u8*)entry)[2];
}

// Lookup entry, check category; return byte at offset 4 or 0.
u8 func_801C65A0(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= self->field_2800) return 0;
    void* entry = (u8*)self + offset * 0xa;
    s16 val = *(s16*)entry;
    void* obj = func_80157C4C(self->field_2802, val);
    if (!obj || !*(u32*)obj) return 0;
    return ((u8*)entry)[4];
}

// Lookup entry, check category; return byte at offset 5 or 0.
u8 func_801C6618(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= self->field_2800) return 0;
    void* entry = (u8*)self + offset * 0xa;
    s16 val = *(s16*)entry;
    void* obj = func_80157C4C(self->field_2802, val);
    if (!obj || !*(u32*)obj) return 0;
    return ((u8*)entry)[5];
}

// Lookup entry, check category; return byte at offset 3 or 0.
u8 func_801C6690(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= self->field_2800) return 0;
    void* entry = (u8*)self + offset * 0xa;
    s16 val = *(s16*)entry;
    void* obj = func_80157C4C(self->field_2802, val);
    if (!obj || !*(u32*)obj) return 0;
    return ((u8*)entry)[3];
}

// Lookup a byte from a 10-byte-entry table indexed by (field_2804 * 0x1e + idx).
// Returns byte at offset 7 within the entry, or 0 if out of bounds.
u8 func_801C6708(CItemBoxGridFull* self, u16 idx) {
    s32 tmp = (s8)self->field_2804 * 0x1e + idx;
    u32 offset = (u32)(u16)tmp;
    if (offset >= 0x400) return 0;
    return ((u8*)self)[offset * 0xa + 7];
}

u8 func_801C673C(CItemBoxGridFull* self, u16 idx) {
    s32 tmp = (s8)self->field_2804 * 0x1e + idx;
    u32 offset = (u32)(u16)tmp;
    if (offset >= 0x400) return 0;
    return ((u8*)self)[offset * 0xa + 8];
}


// Toggle an entry's flag based on category cap.
void func_801C6770(CItemBoxGridFull* self, u16 idx) {
    u16 offset = (u16)((s8)self->field_2804 * 0x1e + idx);
    if (offset >= 0x400) return;
    u8* entry = (u8*)self + offset * 0xa;
    if (entry[8] == 0) {
        u8 cap = func_801C6840(self);
        if (func_801C67F8(self) >= cap) return;
    }
    entry[8] = entry[8] ? 0 : 1;
}

// Count entries with non-zero byte at offset 8 in a 10-byte stride array.
u8 func_801C67F8(CItemBoxGridFull* self) {
    u16 count = self->field_2800;
    u16 i;
    u16 result = 0;
    for (i = 0; i < count; i++) {
        u8* entry = (u8*)self + i * 0xa;
        if (entry[8] != 0) {
            result++;
        }
    }
    return (u8)result;
}

// Return a duration/stride value based on the category byte at offset 0x2802.
u8 func_801C6840(CItemBoxGridFull* self) {
    u8 cat = self->field_2802;
    u32 d = cat - 4;
    if (d <= 4) goto ret30;
    if (cat == 2) goto ret30;
    if (cat == 0xb) {
        return 0x3c;
    }
    return 0;
ret30:
    return 0x1e;
}

int LookupIndexedByte(char* obj) {
    char off = *(signed char*)((char*)obj + 0x2804);
    if (off >= 0x400) return 0;
    return *(unsigned char*)((char*)obj + off + 0x28a5);
}

// Iterate entries and init item instances.
void func_801C68A0(CItemBoxGridFull* self) {
    u32 i;
    for (i = 0; i < self->field_2800; i++) {
        u8* entry = (u8*)self + i * 10;
        if (entry[8] != 0) continue;
        s16 val = *(s16*)entry;
        void* obj = func_80157C4C(self->field_2802, val);
        if (!obj || !*(u32*)obj) continue;
        void* inst = CItem_initItemImplInstances(obj);
        void** vtbl = *(void***)inst;
        ((void(*)(void*, void*))vtbl[4])(inst, obj);
    }
}

u32 func_801C6938(void* self, u32 idx) { return 0; }

void func_801C6A44() { }

// Check if an object has type 9 (extracted from vtable bits) and subtype 2.
int func_801C6E90(void* obj) {
    u32 w = *(u32*)obj;
    u32 type = (w >> 12) & 0xF;
    int result = 0;
    if (type == 9) {
        u8 sub = *(u8*)((u8*)obj + 7) & 3;
        if (sub == 2) {
            result = 1;
        }
    }
    return result;
}

// Check entry state.
u32 func_801C6EC0(CItemBoxGridFull* self, u16 idx) {
    u8* p = (u8*)self;
    s8 base = (s8)p[0x2804];
    u16 offset = (u16)(base * 0x1e + idx);
    u16 count = *(u16*)(p + 0x2800);
    if (offset >= count) return 0;
    void* obj = func_80157C4C(p[0x2802], *(s16*)(p + offset * 10));
    if (!obj) return 0;
    return func_8015780C(obj);
}

// Clear inactive entries and sort remaining.
void func_801C7730(CItemBoxGridFull* self) {
    u8* p = (u8*)self;
    u16 count = self->field_2800;
    u16 i;
    for (i = 0; i < count; i++) {
        u8* e = p + i * 10;
        if (e[8]) {
            SetEntry9Bytes((unsigned char*)e, (unsigned short)0xFFFF, (unsigned char)0, (unsigned char)0, (unsigned char)0, (unsigned char)0, (unsigned char)0, (unsigned char)0, (unsigned char)0);
            self->field_2800--;
        }
    }
    // Bubble sort remaining
    u16 pass;
    for (pass = 0; pass < 0x3FF; pass++) {
        u16 stay = 0x3FF - pass;
        u16 j;
        u32 swapped = 0;
        for (j = 0; j < stay; j++) {
            u8* e1 = p + j * 10;
            u8* e2 = p + (j + 1) * 10;
            s16 v1 = *(s16*)e1;
            s16 v2 = *(s16*)e2;
            u32 a = v1 + 1;
            u32 b = v2 + 1;
            if (a <= b) continue;
            char tmp[27];
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

void CopyEntry9Bytes(char* dst, const char* src) {
    *(short*)((char*)dst + 0) = *(short*)((char*)src + 0);
    *(char*)((char*)dst + 2) = *(char*)((char*)src + 2);
    *(char*)((char*)dst + 3) = *(char*)((char*)src + 3);
    *(char*)((char*)dst + 4) = *(char*)((char*)src + 4);
    *(char*)((char*)dst + 5) = *(char*)((char*)src + 5);
    *(char*)((char*)dst + 6) = *(char*)((char*)src + 6);
    *(char*)((char*)dst + 7) = *(char*)((char*)src + 7);
    *(char*)((char*)dst + 8) = *(char*)((char*)src + 8);
}

void func_801C7958() { }

void func_801C7C7C() { }

// Sort entries with item instance comparison.
void func_801C7EF0(CItemBoxGridFull* self, u32 mode) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            s16 v1 = *(s16*)e1;
            s16 v2 = *(s16*)e2;
            void* obj1 = func_80157C4C(self->field_2802, v1);
            void* obj2 = func_80157C4C(self->field_2802, v2);
            if (!obj1 || !obj2) continue;
            // Compare based on mode
            u32 cmp = 0;
            if (mode == 3) {
                cmp = CItem_initItemImplInstances(obj2) > CItem_initItemImplInstances(obj1);
            } else {
                u16 w1 = *(u16*)((u8*)obj1 + 4);
                u16 w2 = *(u16*)((u8*)obj2 + 4);
                cmp = w1 > w2;
            }
            if (!cmp) continue;
            // Swap using CopyEntry9Bytes / func_801C562C
            char tmp[9];
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes((char*)e1, (const char*)e2);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte comparison.
void func_801C81D0(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            if (e1[6] <= e2[6]) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte comparison.
void func_801C82D0(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            if (e1[6] <= e2[6]) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Sort entries by kind.
void func_801C83E4(CItemBoxGridFull* self) {
    u8* p = (u8*)self;
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = p + j * 10;
            u8* e2 = p + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            u32 w1 = *(u32*)obj1;
            u32 k1 = func_80139358(w1 >> 20);
            u32 w2 = *(u32*)obj2;
            u32 k2 = func_80139358(w2 >> 20);
            u32 c1 = func_801361E8(lbl_eu_80664104, &lbl_eu_8050566C[0x212], k1 & 0xFFFF);
            u32 c2 = func_801361E8(lbl_eu_80664104, &lbl_eu_8050566C[0x212], k2 & 0xFFFF);
            if ((c1 & 0xFF) <= (c2 & 0xFF)) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte comparison.
void func_801C8534(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            if (e1[6] <= e2[6]) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte comparison.
void func_801C8634(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            if (e1[6] <= e2[6]) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte at offset 6.
void func_801C87CC(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        u8 tmp[16];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            u8 v1 = e1[6];
            u8 v2 = e2[6];
            if (v1 <= v2) continue;
            CopyEntry9Bytes((char*)tmp, (const char*)e1);
            CopyEntry9Bytes((char*)tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes((char*)tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte at offset 6.
void func_801C88B0(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            u8 v1 = e1[6];
            u8 v2 = e2[6];
            if (v1 <= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort entries using item comparison.
void func_801C8994(CItemBoxGridFull* self) {
    u32 i;
    for (i = 0; i < self->field_2800 - 1; i++) {
        u32 j;
        int swapped = 0;
        u32 limit = self->field_2800 - 1 - i;
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            s16 val1 = *(s16*)e1;
            s16 val2 = *(s16*)e2;
            void* obj1 = func_80157C4C(self->field_2802, val1);
            void* obj2 = func_80157C4C(self->field_2802, val2);
            u16 w1 = *(u16*)((u8*)obj1 + 4);
            u16 w2 = *(u16*)((u8*)obj2 + 4);
            if (w1 <= w2) continue;
            // Swap entries
            u8 tmp[9];
            // CopyEntry9Bytes
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C8ACC(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C8C58(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C8DE4(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            char tmp[27];
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte comparison.
void func_801C8F04(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            if (e1[6] <= e2[6]) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte comparison.
void func_801C9040(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            if (e1[6] <= e2[6]) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte comparison.
void func_801C9158(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            if (e1[6] <= e2[6]) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte comparison.
void func_801C9270(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            if (e1[6] <= e2[6]) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C9390(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C94E0(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C9630(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C9780(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C98D0(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C9A3C(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte comparison.
void func_801C9B8C(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            if (e1[6] <= e2[6]) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C9CCC(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C9E1C(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

float func_801C9F88(void* self, void* entry) { return 0.0f; }

// Check item slots for first valid entry.
u32 func_801CA070(void* self, void* item) {
    void* inst = CItem_initItemImplInstances(item);
    void** vtbl = *(void***)inst;
    u16 count = (u16)((u32(*)(void*, void*))vtbl[12])(inst, item);
    u32 i;
    for (i = 0; i < count; i++) {
        void* inst2 = CItem_initItemImplInstances(item);
        void** vtbl2 = *(void***)inst2;
        void* obj = ((void*(*)(void*, void*, u32))vtbl2[11])(inst2, item, i);
        if (!obj) continue;
        if (*(u16*)((u8*)obj + 4) & 1) return 1;
    }
    return 0;
}

// Check if entry kind exists in pool.
u32 func_801CA110(void* self, void* entry) {
    u32 val = *(u32*)entry;
    u32 obj = *(u32*)(lbl_eu_806640F4 + val);
    u32 kind = func_80139358(val >> 20);
    u32 i;
    for (i = 1; i <= 10; i++) {
        char buf[64];
        sprintf(buf, &lbl_eu_8050566C[0x132], (u8)i);
        if (func_801361E8(obj, buf, kind & 0xFFFF)) return i;
    }
    return 0;
}

void __ct__CItemBoxGrid() { }

void __dt__12CItemBoxGridFv() { }

void func_801CAA6C() { }

void func_801CABC8() { }

void func_801CAD8C() { }

void func_801CAE9C() { }

// Check if item grid is fully ready.
u32 func_801CB038(void* self) {
    u8* p = (u8*)self;
    if (!func_801D32DC(p + 0xe8)) return 0;
    if (!getItemBoxState__FP12CItemBoxInfo(p + 0x1d8)) return 0;
    if (!func_801EB018(p + 0x3e4)) return 0;
    if (!func_8022D08C(p + 0x440)) return 0;
    if (!CSysWin_isReady(p + 0x4ac)) return 0;
    if (CSysWin_isReady(p + 0x4e8)) return p[0x60];
    return 0;
}


// Check if item grid is active.
u32 func_801CB0FC(void* self) {
    u8* p = (u8*)self;
    if (p[0x528]) return 1;
    if (CSysWin_getUnk34(p + 0x4ac)) return 1;
    if (CSysWin_getUnk34(p + 0x4e8)) return 1;
    if (p[0x544]) return 1;
    return func_801D3320(p + 0xe8);
}

// Get field depending on window state.
u8 func_801CB184(void* self) {
    if (CSysWin_getUnk34((u8*)self + 0x4e8)) return 0;
    s32 state = *(s32*)((u8*)self + 0x58);
    if (state != 3) return 0;
    return ((u8*)self)[0x542];
}

u8 CItemBoxGrid::GetField549() { return reinterpret_cast<CItemBoxGridFull*>(this)->field_549; }

// Check if any sub-system is active.
u32 func_801CB1E4(void* self) {
    u8* p = (u8*)self;
    if (func_801EB020(p + 0x3e4)) return 1;
    if (func_80208358(p + 0x418)) return 1;
    if (func_8022D094(p + 0x440)) return 1;
    if (func_8022DB6C(p + 0x468)) return 1;
    if (CSysWin_getUnk34(p + 0x4ac)) return 1;
    return CSysWin_getUnk34(p + 0x4e8);
}

// Initialize item display state.
void func_801CB28C(void* self) {
    u8* p = (u8*)self;
    u32 state = *(u32*)(p + 0x58);
    if (state) return;
    *(u32*)(p + 0x58) = 1;
    p[0x61] = 0;
    func_801CFD2C(self);
    func_801D0BD8(self);
    func_801D421C(p + 0x1D8);
    s8 off = (s8)p[0x6F];
    u8 val = p[off + 0x62];
    func_801D4260(p + 0x1D8, val);
    u8 idx0 = p[0x525];
    u8 idx1 = p[0x524];
    u32 entry_idx = (idx1 + idx0 * 10) & 0xFF;
    u8* sub = p + 0x54C;
    u32 r1 = func_801C631C(sub, entry_idx);
    u32 r2 = func_801C62AC(sub, entry_idx);
    func_801D47D4(p + 0x1D8, r2 & 0xFFFF, r1, 1);
    u32 v = func_801C6938(sub, entry_idx);
    func_801D4AE0((void*)(p + 0x1D8), 1, (void*)v);
    func_801D216C(p + 0xB8, 0);
    p[0x549] = 0;
    if (p[0x527] == 4) func_801D4C3C(p + 0x1D8, 1);
}

// Advance item box state.
void func_801CB38C(void* self) {
    u8* p = (u8*)self;
    if (*(u32*)(p + 0x58) != 3) return;
    if (func_801D3320(p + 0xe8)) return;
    *(u32*)(p + 0x58) = 4;
    void* obj = (void*)*(u32*)(p + 0x44);
    void** vtbl = *(void***)obj;
    ((void(*)(void*, void*, int))vtbl[11])(obj, (void*)*(u32*)(p + 0x50), 0);
    ((void(*)(void*, void*, int))vtbl[11])(obj, (void*)*(u32*)(p + 0x48), 0);
    ((void(*)(void*, void*, int))vtbl[11])(obj, (void*)*(u32*)(p + 0x4c), 1);
    p[0x61] = 0;
    func_801D216C(p + 0x70, 0);
    func_801D216C(p + 0x88, 0);
    func_801D216C(p + 0xd0, 0);
    func_801D216C(p + 0xb8, 0);
    advanceItemBoxState__FP12CItemBoxInfo(p + 0x1d8);
    if (!p[0x52c]) func_80138078__FUl(6);
}

// Clear a 14-byte region (list/array init).
void func_801CB480(void* self) {
    int i;
    u8* p = (u8*)self;
    for (i = 0; i < 12; i++) p[0x62 + i] = 0;
    p[0x6e] = 0;
    p[0x6f] = 0;
}

void CItemBoxGrid::PushToList(unsigned char val) {
    unsigned char count = reinterpret_cast<unsigned char*>(this)[0x6e];
    if (count >= 0xc) {
        return;
    }
    reinterpret_cast<unsigned char*>(this)[0x62 + count] = val;
    reinterpret_cast<unsigned char*>(this)[0x6e] = count + 1;
}

// Increment list counter with wrap.
void func_801CB4E4(void* self) {
    u8* p = (u8*)self;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x528]) return;
    u8 idx = p[0x6f] + 1;
    p[0x6f] = idx;
    if ((s8)idx >= (s8)p[0x6e]) {
        p[0x6f] = 0;
    }
    func_801D05D4(self, 0);
    func_801CFF28(self);
    func_80138078__FUl(0x70);
}

// Decrement a list counter with wrap.
void func_801CB56C(void* self) {
    u8* p = (u8*)self;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x528]) return;
    u8 idx = p[0x6f] - 1;
    p[0x6f] = idx;
    if ((s8)idx < 0) {
        p[0x6f] = p[0x6e] - 1;
    }
    func_801D05D4(self, 1);
    func_801CFF28(self);
    func_80138078__FUl(0x70);
}

void func_801CB5F0() { }

unsigned short ArrayGet12(const unsigned short* p, unsigned char i) {
    if (i < 12) {
        return p[i];
    }
    return 0;
}

// Copy 3 words (12 bytes) from a 12-byte-entry array at index idx.
void func_801CB9D8(u32* dst, u32* src, int idx) {
    if (idx >= 12) return;
    u32* entry = src + idx * 3; // each entry is 12 bytes = 3 words
    dst[0] = entry[6];          // offset 0x18
    dst[1] = entry[7];          // offset 0x1C
    dst[2] = entry[8];          // offset 0x20
}

void func_801CBA04() { }

void func_801CBDE8() { }

void func_801CC0EC() { }

// Grid state update with conditions.
// Grid state update.
void func_801CC3F4(void* self) {
    u8* p = (u8*)self;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x528]) return;
    if (CSysWin_getUnk34(p + 0x4ac)) return;
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) return;
    if (func_8022DB6C(p + 0x468)) return;
    // Inline func_801C5EF4 for sub-struct at offset 0x54c
    u8* sub = p + 0x54c;
    u8 idx = sub[0x2804] + 1;
    sub[0x2804] = idx;
    if ((s8)idx >= (s8)sub[0x2803]) sub[0x2804] = 0;
    // LookupIndexedByte for sub
    u8 val;
    s8 off = (s8)sub[0x2804];
    if (off < 0x400) val = sub[off + 0x28a5]; else val = 0;
    if ((s8)p[0x525] < (s8)val) p[0x525] = val - 1;
    func_801CFFEC(self);
    func_801D0328(self);
    u8 f = sub[0x2803] ? sub[0x2803] : 1;
    if (f != 1) func_80138078__FUl(0xa);
}

// Grid state update (decrement variant).
void func_801CC4E8(void* self) {
    u8* p = (u8*)self;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x528]) return;
    if (CSysWin_getUnk34(p + 0x4ac)) return;
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) return;
    if (func_8022DB6C(p + 0x468)) return;
    u8* sub = p + 0x54c;
    u8 idx = sub[0x2804] - 1;
    sub[0x2804] = idx;
    if ((s8)idx < 0) sub[0x2804] = sub[0x2803] - 1;
    u8 val;
    s8 off = (s8)sub[0x2804];
    if (off < 0x400) val = sub[off + 0x28a5]; else val = 0;
    if ((s8)p[0x525] < (s8)val) p[0x525] = val - 1;
    func_801CFFEC(self);
    func_801D0328(self);
    u8 f = sub[0x2803] ? sub[0x2803] : 1;
    if (f != 1) func_80138078__FUl(0xa);
}

void func_801CC5DC() { }

void func_801CC7B0() { }

void func_801CCAF0() { }

// Store value and call helpers.
void func_801CDB94(void* self, u32 val) {
    *(u16*)((u8*)self + 0x52e) = (u16)val;
    void* res = func_801394D4(val & 0xFFFF);
    func_801D4AE0((void*)((u8*)self + 0x1d8), 0, res);
    func_801CFF28(self);
}

// Get field depending on obj state.
u8 func_801CDBE0(void* self) {
    if (!func_8022D09C((u8*)self + 0x440)) return 0;
    s32 state = *(s32*)((u8*)self + 0x58);
    if (state != 3) return 0;
    return ((u8*)self)[0x52c];
}

u8 CItemBoxGrid::GetField52D() { return reinterpret_cast<CItemBoxGridFull*>(this)->field_52D; }

void func_801CDC40() { }

// Check conditions and update state.
void func_801CDEE8(void* self) {
    u8* p = (u8*)self;
    if (*(u32*)(p + 0x58) != 3) return;
    if (p[0x528]) return;
    if (p[0x525] == 0xFF) return;
    if (CSysWin_getUnk34(p + 0x4ac)) return;
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) return;
    if (func_8022DB6C(p + 0x468)) return;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x544]) return;
    p[0x525] = 0xFF;
    func_801D0950(self);
    func_801D0328(self);
    func_80138078__FUl(2);
}

// Check prerequisites and return result.
u32 func_801CDFB4(void* self) {
    u8* p = (u8*)self;
    if (*(u32*)(p + 0x58) != 3) return 0;
    if (func_801EB020(p + 0x3e4)) return 0;
    if (func_80208358(p + 0x418)) return 0;
    if (func_8022D094(p + 0x440)) return 0;
    if (func_8022DB6C(p + 0x468)) return 0;
    if (CSysWin_getUnk34(p + 0x4ac)) return 0;
    if (CSysWin_getUnk34(p + 0x4e8)) return 0;
    return 1;
}

// Animate grid panels (alt variant).
void func_801CE108(void* self) {
    u8* p = (u8*)self;
    u32 obj = *(u32*)(p + 0x48);
    float f = lbl_eu_80667F78;
    if (!func_80137444__FPQ34nw4r3lyt13AnimTransformf((void*)obj, f)) return;
    void** vtbl;
    u32 ptr44 = *(u32*)(p + 0x44);
    u32 ptr50 = *(u32*)(p + 0x50);
    vtbl = *(void***)ptr44;
    ((void(*)(void*, u32, int))vtbl[0x2C / 4])((void*)ptr44, ptr50, 0);
    u32 ptr48 = *(u32*)(p + 0x48);
    ((void(*)(void*, u32, int))vtbl[0x2C / 4])((void*)ptr44, ptr48, 0);
    u32 ptr4C = *(u32*)(p + 0x4C);
    ((void(*)(void*, u32, int))vtbl[0x2C / 4])((void*)ptr44, ptr4C, 1);
    *(u32*)(p + 0x58) = 2;
}

void func_801CE1A0() { }


namespace nw4r { namespace lyt { class AnimTransform; } }
void func_80137444(nw4r::lyt::AnimTransform*, float);

void CheckState4_Animate(char* self) {
    if (*(unsigned char*)((char*)self + 0x527) != 4) {
        return;
    }
    func_80137444(*(nw4r::lyt::AnimTransform**)((char*)self + 0x50), lbl_eu_80667F78);
}


// Animate grid panels.
void func_801CE2F8(void* self) {
    u8* p = (u8*)self;
    u32 obj = *(u32*)(p + 0x4C);
    float f = lbl_eu_80667F78;
    if (!func_80137510(obj, f)) return;
    void** vtbl;
    u32 ptr44 = *(u32*)(p + 0x44);
    u32 ptr4C = *(u32*)(p + 0x4C);
    vtbl = *(void***)ptr44;
    ((void(*)(void*, u32, int))vtbl[0x2C / 4])((void*)ptr44, ptr4C, 0);
    u32 ptr50 = *(u32*)(p + 0x50);
    ((void(*)(void*, u32, int))vtbl[0x2C / 4])((void*)ptr44, ptr50, 0);
    u32 ptr48 = *(u32*)(p + 0x48);
    ((void(*)(void*, u32, int))vtbl[0x2C / 4])((void*)ptr44, ptr48, 1);
    *(u32*)(p + 0x58) = 5;
}

void func_801CE390() { }

void func_801CE3E8() { }

// Handle sub-object activation.
void func_801CE4B4(void* self) {
    u8* p = (u8*)self;
    if (!func_80208360(p + 0x418)) return;
    *(u32*)(p + 0x58) = 7;
    u8 temp[16];
    func_80208760(temp, p + 0x418);
    void** vtbl = *(void***)(p + 0xa0);
    ((void(*)(void*, void*))vtbl[4])(p + 0xa0, temp);
    func_801D216C(p + 0xa0, 1);
}

void func_801CE524() { }

// Handle sub-object activation with copy.
void func_801CE974(void* self) {
    u8* p = (u8*)self;
    if (!func_8022D09C(p + 0x440)) return;
    *(u32*)(p + 0x58) = 0xd;
    func_801D216C(p + 0xa0, 1);
    u8 temp[16];
    func_8022D0F8(temp, p + 0x440, p[0x529]);
    void** vtbl = *(void***)(p + 0xa0);
    ((void(*)(void*, void*))vtbl[4])(p + 0xa0, temp);
}

// Check if sub-obj is active; set state to 3 and clear flag.
void func_801CE9E8(void* self) {
    u8* p = (u8*)self;
    if (func_8022D09C(p + 0x440)) {
        *(u32*)(p + 0x58) = 3;
        p[0x528] = 0;
    }
}

// Handle sub-object activation (variant).
void func_801CEA30(void* self) {
    u8* p = (u8*)self;
    if (!func_8022DB74(p + 0x468)) return;
    *(u32*)(p + 0x58) = 0x10;
    u8 temp[16];
    func_8022E498(temp, p + 0x468);
    void** vtbl = *(void***)(p + 0xd0);
    ((void(*)(void*, void*))vtbl[4])(p + 0xd0, temp);
    func_801D216C(p + 0xd0, 1);
}

// Check if sub-obj is active; set state to 3 and clear flag.
void func_801CEAA0(void* self) {
    u8* p = (u8*)self;
    if (func_8022DB74(p + 0x468)) {
        *(u32*)(p + 0x58) = 3;
        p[0x528] = 0;
    }
}

// Check system window; if active, set state and call helpers.
void func_801CEAE8(void* self) {
    u8* p = (u8*)self;
    if (CSysWin_isActive(p + 0x4ac)) {
        *(u32*)(p + 0x58) = 0x16;
        func_801D216C(p + 0xa0, 1);
        func_801D0950(self);
    }
}

// Handle system window state change.
void func_801CEB3C(void* self) {
    u8* p = (u8*)self;
    if (!CSysWin_isActive(p + 0x4ac)) return;
    if ((s8)p[0x540]) {
        *(u32*)(p + 0x58) = 3;
        func_801D216C(p + 0x70, 1);
        func_801D0950(self);
    } else {
        u8 idx = p[0x6f];
        u8 cat = *(u8*)((u8*)self + idx + 0x62);
        if (cat > 9) {
            func_801EB410(p + 0x3e4, 1);
        } else {
            func_801EB410(p + 0x3e4, 0);
        }
        func_801EB0D4(p + 0x3e4);
        *(u32*)(p + 0x58) = 0x18;
        p[0x528] = 1;
    }
}

// Handle system window activation.
void func_801CEBF0(void* self) {
    u8* p = (u8*)self;
    if (!CSysWin_isActive(p + 0x4e8)) return;
    *(u32*)(p + 0x58) = 3;
    if (!p[0x542] && !p[0x544]) {
        func_801D216C(p + 0x70, 1);
    }
    if (!p[0x544]) {
        func_801D0950(self);
    }
    if (p[0x544]) {
        func_801D216C(p + 0xa0, 1);
    }
}

// Check conditions; set flags.
void func_801CEC80(void* self) {
    if (!*(u32*)((u8*)self + 0x44)) return;
    if (!*(u32*)((u8*)self + 0x40)) return;
    ((u8*)self)[0x60] = 1;
    ((u8*)self)[0x54] = 1;
}

void func_801CECD0() { }

void func_801CF240() { }

void func_801CF71C() { }

void func_801CF900() { }

void func_801CFA58() { }

// Format text and set on layout pane.
void func_801CFCBC(void* self, u32 val) {
    u8* p = (u8*)self;
    char buf[64];
    sprintf(buf, &lbl_eu_8050566C[0x513], val + 1);
    u32 obj = *(u32*)(p + 0x44);
    u32 sub = *(u32*)(obj + 0x10);
    void** vtbl = *(void***)sub;
    void* ret = ((void*(*)(void*, char*, int))vtbl[0x3C / 4])((void*)sub, buf, 1);
    func_80124270(ret, val);
}

void func_801CFD2C() { }

void func_801CFF28() { }

void func_801CFFEC(void* self){}

void func_801D0328(void* self){}

void func_801D05D4() { }

void func_801D0950() { }

void func_801D0BD8() { }

void func_801D0E88() { }

// Handle item event dispatch.
void func_801D11B8(void* self, void* item, int eventType) {
    if (!item) return;
    if (eventType >= 1) {
        u32 w = *(u32*)item;
        func_80158118(item, w >> 20);
    } else {
        void* inst = CItem_initItemImplInstances(item);
        void** vtbl = *(void***)inst;
        ((void(*)(void*, void*))vtbl[4])(inst, item);
    }
}

// Dispatch based on entry category.
u32 func_801D1220(void* self) {
    u8* p = (u8*)self;
    s8 idx = (s8)p[0x6f];
    u8 cat = p[idx + 0x62];
    u32 result;
    switch (cat) {
        case 0:  result = 0x33; break;
        case 1:  result = 0x3C; break;
        case 2:  result = 0x34; break;
        case 3:  result = 0x35; break;
        case 4:  result = 0x36; break;
        case 5:  result = 0x37; break;
        case 6:  result = 0x38; break;
        case 7:  result = 0x3D; break;
        case 8:  result = 0x3E; break;
        case 9:  result = 0x39; break;
        case 10: result = 0x3B; break;
        case 11: result = 0x3A; break;
        default: result = 0; break;
    }
    if (result) {
        // Would call func_80136190 with string constants
        // Skipped due to string constant inaccessibility
    }
    return result;
}

// Check item availability in item storage.
u32 func_801D12D4(void* self, void* entry) {
    return 1;
}

void CItemBoxGrid::OnFileEvent() {}

void CopyVec4s(short* dst, const short* src) { dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2]; dst[3] = src[3]; }

// Split a u32 into its four bytes, stored as shorts (big-endian order).
void func_801D1F9C(short* dst, unsigned long val) {
    dst[0] = (val >> 24) & 0xFF;
    dst[1] = (val >> 16) & 0xFF;
    dst[2] = (val >> 8) & 0xFF;
    dst[3] = val & 0xFF;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// CArtsBookItem constructor
void __ct__CArtsBookItem(CArtsBookItem* self) {
    u8* vtable;
    // vtable setup
    *(u16*)((u8*)self + 0x804) = 0;
}
// Standard MWCC virtual destructor
void* __dt__10CQuestItemFv(void* self, int mode) {
    if (self && mode > 0) __dl__FPv(self);
    return self;
}
void* __dt__11CVisionItemFv(void* self, int mode) {
    if (self && mode > 0) __dl__FPv(self);
    return self;
}
void* __dt__13CArtsBookItemFv(void* self, int mode) {
    if (self && mode > 0) __dl__FPv(self);
    return self;
}

void func_801C4BB4() { }
// Add id to list if not already present and capacity check passes.
void func_801C5158(void* self, u32 id) {
    if (func_801C51BC(self, id)) return;
    if ((func_801392E4(id) & 0xFFFF) == 12) return;
    u16 count = *(u16*)((u8*)self + 0x804);
    *(u16*)((u8*)self + 4 + count * 2) = (u16)id;
    *(u16*)((u8*)self + 0x804) = count + 1;
}
void func_801C5254() { }
void func_801C53D8() { }
void OnFileEvent__12CItemBoxGridFP10CEventFile() { }

// Static initialization of color table entries.
void sinit_801D1E30(void) {
    func_801D1F9C((void*)&lbl_eu_80664488, 0);
    func_801D1F9C((void*)&lbl_eu_80664490, 0);
    func_801C4B60((void*)&lbl_eu_80664498, 0x79, 0x49, 0x07, 0x00);
    func_801C4B60((void*)&lbl_eu_806644A0, 0xED, 0xCD, 0x83, 0x00);
    func_801C4B60((void*)&lbl_eu_806644A8, 0x1A, 0x43, 0x53, 0x00);
    func_801C4B60((void*)&lbl_eu_806644B0, 0xC4, 0xE8, 0xEB, 0x00);
    func_801C4B60((void*)&lbl_eu_806644B8, 0x74, 0x54, 0x1D, 0x00);
    func_801C4B60((void*)&lbl_eu_806644C0, 0xD5, 0xB9, 0x78, 0x00);
    func_801C4B60((void*)&lbl_eu_806644C8, 0x3D, 0x68, 0x78, 0x00);
    func_801C4B60((void*)&lbl_eu_806644D0, 0xC4, 0xE8, 0xEB, 0x00);
    func_801D1F9C((void*)&lbl_eu_806644D8, 0);
    func_801D1F9C((void*)&lbl_eu_806644E0, 0);
    func_801C4B60((void*)&lbl_eu_806644E8, 0x12, 0xA3, 0xE7, 0x00);
    func_801C4B60((void*)&lbl_eu_806644F0, 0xFF, 0xFF, 0xFF, 0x00);
    func_801C4B60((void*)&lbl_eu_806644F8, 0xB3, 0x09, 0xC0, 0x00);
    func_801C4B60((void*)&lbl_eu_80664500, 0xFF, 0xFF, 0xFF, 0x00);
}

extern u8 lbl_eu_805347F8[];

