// Auto-scaffolded catalog TU for kyoshin/CItemBoxGrid
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CItemBoxGrid.hpp"

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
extern void func_80137510(u32, float);
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
extern void func_801D05D4(void*, int);
extern void func_801CFF28(void*);
extern void func_80138078__FUl(u32);
u8 func_801C67F8(CItemBoxGridFull* self);
u8 func_801C6840(CItemBoxGridFull* self);

u8 CItemBoxGrid::GetField61() { return reinterpret_cast<CItemBoxGridFull*>(this)->field_61; }

void __ct__801C5514(){}

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
    extern u32 lbl_eu_80664514;
    lbl_eu_80664514 = 0;
    __dt__13CArtsBookItemFv(p + 0x3cb8, 0xFFFFFFFF);
    __dt__11CVisionItemFv(p + 0x34b0, 0xFFFFFFFF);
    if (mode > 0) __dl__FPv(self);
    return self;
}

void func_801C56D8(){}

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
void func_801C5E5C(){}

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

void func_801C5FC0(){}

// Round a double to nearest integer with .5 tie-breaking biased away from zero.
long func_801C6158(double f) {
    if (f > 0.0) {
        return (long)(f + 0.5);
    } else {
        return (long)(f - 0.5);
    }
}

void func_801C618C(){}

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

void func_801C6388(){}

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

void func_801C6938(){}

void func_801C6A44(){}

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

void func_801C6EC0(){}

void func_801C7730(){}

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

void func_801C7958(){}

void func_801C7C7C(){}

void func_801C7EF0(){}

void func_801C81D0(){}

void func_801C82D0(){}

void func_801C83E4(){}

void func_801C8534(){}

void func_801C8634(){}

void func_801C87CC(){}

void func_801C88B0(){}

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

void func_801C8ACC(){}

void func_801C8C58(){}

void func_801C8DE4(){}

void func_801C8F04(){}

void func_801C9040(){}

void func_801C9158(){}

void func_801C9270(){}

void func_801C9390(){}

void func_801C94E0(){}

void func_801C9630(){}

void func_801C9780(){}

void func_801C98D0(){}

void func_801C9A3C(){}

void func_801C9B8C(){}

void func_801C9CCC(){}

void func_801C9E1C(){}

void func_801C9F88(){}

void func_801CA070(){}

void func_801CA110(){}

void __ct__CItemBoxGrid(){}

void __dt__12CItemBoxGridFv(){}

void func_801CAA6C(){}

void func_801CABC8(){}

void func_801CAD8C(){}

void func_801CAE9C(){}

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

void func_801CB28C(){}

void func_801CB38C(){}

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

void func_801CB5F0(){}

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

void func_801CBA04(){}

void func_801CBDE8(){}

void func_801CC0EC(){}

void func_801CC3F4(){}

void func_801CC4E8(){}

void func_801CC5DC(){}

void func_801CC7B0(){}

void func_801CCAF0(){}

// Store value and call helpers.
void func_801CDB94(void* self, u32 val) {
    *(u16*)((u8*)self + 0x52e) = (u16)val;
    void* res = func_801394D4(val & 0xFFFF);
    func_801D4AE0((u8*)self + 0x1d8, 0, res);
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

void func_801CDC40(){}

void func_801CDEE8(){}

void func_801CDFB4(){}

void func_801CE108(){}

void func_801CE1A0(){}

float lbl_eu_80667F78;

namespace nw4r { namespace lyt { class AnimTransform; } }
void func_80137444(nw4r::lyt::AnimTransform*, float);

void CheckState4_Animate(char* self) {
    if (*(unsigned char*)((char*)self + 0x527) != 4) {
        return;
    }
    func_80137444(*(nw4r::lyt::AnimTransform**)((char*)self + 0x50), lbl_eu_80667F78);
}

void func_801CE2F8(){}

void func_801CE390(){}

void func_801CE3E8(){}

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

void func_801CE524(){}

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

void func_801CECD0(){}

void func_801CF240(){}

void func_801CF71C(){}

void func_801CF900(){}

void func_801CFA58(){}

void func_801CFCBC(){}

void func_801CFD2C(){}

void func_801CFF28(){}

void func_801CFFEC(){}

void func_801D0328(){}

void func_801D05D4(){}

void func_801D0950(){}

void func_801D0BD8(){}

void func_801D0E88(){}

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

void func_801D1220(){}

void func_801D12D4(){}

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

void func_801C4BB4(){}
// Add id to list if not already present and capacity check passes.
void func_801C5158(void* self, u32 id) {
    if (func_801C51BC(self, id)) return;
    if ((func_801392E4(id) & 0xFFFF) == 12) return;
    u16 count = *(u16*)((u8*)self + 0x804);
    *(u16*)((u8*)self + 4 + count * 2) = (u16)id;
    *(u16*)((u8*)self + 0x804) = count + 1;
}
void func_801C5254(){}
void func_801C53D8(){}
void OnFileEvent__12CItemBoxGridFP10CEventFile(){}

void sinit_801D1E30(){}

extern u8 lbl_eu_805347F8[];

