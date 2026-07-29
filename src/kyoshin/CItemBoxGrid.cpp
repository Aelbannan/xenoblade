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

void func_801C562C(){}

void __dt__801C5670(){}

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

void func_801C5F48(){}

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

void func_801C62AC(){}

void func_801C631C(){}

void func_801C6388(){}

void func_801C6528(){}

void func_801C65A0(){}

void func_801C6618(){}

void func_801C6690(){}

// Lookup a byte from a 10-byte-entry table indexed by (field_2804 * 0x1e + idx).
// Returns byte at offset 7 within the entry, or 0 if out of bounds.
u8 func_801C6708(CItemBoxGridFull* self, u16 idx) {
    s16 offset = (s8)self->field_2804 * 0x1e + idx;
    if (offset >= 0x400) return 0;
    return ((u8*)self)[offset * 0xa + 7];
}

// Same as func_801C6708 but returns byte at offset 8 within each entry.
u8 func_801C673C(CItemBoxGridFull* self, u16 idx) {
    s16 offset = (s8)self->field_2804 * 0x1e + idx;
    if (offset >= 0x400) return 0;
    return ((u8*)self)[offset * 0xa + 8];
}

void func_801C6770(){}

void func_801C67F8(){}

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

void func_801C68A0(){}

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

void func_801C8994(){}

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

void func_801CB038(){}


void func_801CB0FC(){}

// Get field depending on window state.
u8 func_801CB184(void* self) {
    if (CSysWin_getUnk34((u8*)self + 0x4e8)) return 0;
    if (*(u32*)((u8*)self + 0x58) != 3) return 0;
    return ((u8*)self)[0x542];
}

u8 CItemBoxGrid::GetField549() { return reinterpret_cast<CItemBoxGridFull*>(this)->field_549; }

void func_801CB1E4(){}

void func_801CB28C(){}

void func_801CB38C(){}

void func_801CB480(){}

void CItemBoxGrid::PushToList(unsigned char val) {
    unsigned char count = reinterpret_cast<unsigned char*>(this)[0x6e];
    if (count >= 0xc) {
        return;
    }
    reinterpret_cast<unsigned char*>(this)[0x62 + count] = val;
    reinterpret_cast<unsigned char*>(this)[0x6e] = count + 1;
}

void func_801CB4E4(){}

void func_801CB56C(){}

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
    if (*(u32*)((u8*)self + 0x58) != 3) return 0;
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

// Check sub-obj; set field and show sub-window.
void func_801CE390(void* self) {
    u8* p = (u8*)self;
    if (func_80137510(*(u32*)(p + 0x48), 0.0f)) {
        p[0x61] = 1;
        *(u32*)(p + 0x58) = 0;
        func_801D216C(p + 0x70, 0);
    }
}

void func_801CE3E8(){}

void func_801CE4B4(){}

void func_801CE524(){}

void func_801CE974(){}

// Check if sub-obj is active; set state to 3 and clear flag.
void func_801CE9E8(void* self) {
    u8* p = (u8*)self;
    if (func_8022D09C(p + 0x440)) {
        *(u32*)(p + 0x58) = 3;
        p[0x528] = 0;
    }
}

void func_801CEA30(){}

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

void func_801CEB3C(){}

void func_801CEBF0(){}

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

void func_801D11B8(){}

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
void sinit_801D1E30(){}

extern u8 lbl_eu_805347F8[];
