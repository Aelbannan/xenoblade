// Auto-scaffolded catalog TU for monolib/src/coli/code_804B59C8
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <revolution/os/OSFastCast.h>
#include <revolution/mtx/mtxvec.h>
#include <revolution/mtx/vec.h>
#include <nw4r/math/math_types.h>
#include "monolib/math/CVec3.hpp"

// ---------------------------------------------------------------------------
// Data globals
// ---------------------------------------------------------------------------

// Scene-resource entry count (sbss; shared with monolib/scn/code_804BD8E8).
extern s32 lbl_eu_80665988;

// Collision state manager (lbl_eu_8065F1C8): reference vectors at 0x00/0x0C,
// sampled half-float triple at 0xC8, result at 0xD4, mover hook at 0xE0,
// matrix source pointer at 0xE8, embedded matrix at 0xF0, registration
// filter flags at 0x128, acceptance thresholds at 0x120/0x130.
struct CColiMgr {
    nw4r::math::_VEC3 field_0x00;   // 0x00
    nw4r::math::_VEC3 field_0x0C;   // 0x0C
    u8 field_0x18[0x9C - 0x18];     // 0x18
    void* field_0x9C;               // 0x9C
    void* field_0xA0;               // 0xA0
    u8 field_0xA4[0xC8 - 0xA4];     // 0xA4
    nw4r::math::_VEC3 field_0xC8;   // 0xC8
    f32 field_0xD4;                 // 0xD4
    u8 field_0xD8[0xE0 - 0xD8];     // 0xD8 - pad
    void* field_0xE0;               // 0xE0
    u8 field_0xE4[0xE8 - 0xE4];     // 0xE4
    Mtx* field_0xE8;                // 0xE8 - matrix source pointer
    u32 field_0xEC;                 // 0xEC
    Mtx field_0xF0;                 // 0xF0 - embedded matrix
    f32 field_0x120;                // 0x120
    u8 field_0x124[0x128 - 0x124];  // 0x124
    u32 field_0x128;                // 0x128 - registration filter flags
    u8 field_0x12C[0x130 - 0x12C];  // 0x12C
    u32 field_0x130;                // 0x130
};
extern CColiMgr lbl_eu_8065F1C8;

// Collision registration workspace (lbl_eu_8065D1A0, .bss).  The 0x2000-byte
// mover bit-table (also symbolised as lbl_eu_8065F1A0) starts at +0x2000 and
// the collision manager (lbl_eu_8065F1C8) lives at +0x2028; the registered
// index list (count at +0x215E, entries at +0x2160) is appended by the
// register helpers below.  The bit-table and the list region overlap in
// memory, so the two views are expressed as union members.
struct CColiListMgr {
    u8 field_0x00[0x136];  // 0x00
    u16 count;             // 0x136 - number of registered indices
    u16 list[0x10];        // 0x138
};
struct CColiWork {
    union {
        struct {
            u8 field_0x0000[0x2000];  // 0x0000
            u32 bitTable[0x80];       // 0x2000 (== lbl_eu_8065F1A0)
        } b;
        struct {
            u8 field_0x0000[0x2028];  // 0x0000
            CColiListMgr listMgr;     // 0x2028 (== lbl_eu_8065F1C8)
        } l;
    };
};
extern CColiWork lbl_eu_8065D1A0;

// Base of the collision "seen" bit array (pointer value held in sdata).
extern u32 lbl_eu_80663AC8;

// Acceptance-threshold constant (sdata2).
extern const f32 lbl_eu_8066AED0;

// 0x24-byte scene-resource entry (lbl_eu_8065F428 holds 32 of these).
struct ScnResourceEntry {
    u32 field_0x00;   // 0x00
    u32 field_0x04;   // 0x04
    u32 field_0x08;   // 0x08
    u32 field_0x0C;   // 0x0C
    u32 field_0x10;   // 0x10
    u32 field_0x14;   // 0x14
    void* field_0x18; // 0x18
    u32 field_0x1C;   // 0x1C
    int field_0x20;   // 0x20
};
extern ScnResourceEntry lbl_eu_8065F428[32];

// Object owning a 6-byte half-float triple table at +0x1C.
struct CColiSrc {
    u8 field_0x00[0x1C];  // 0x00
    void* field_0x1C;     // 0x1C
};

// Query/mover object: position at +0x00, half-float table index at +0x10.
struct CColiMover {
    f32 field_0x00;             // 0x00
    u8 field_0x04[0x10 - 0x04]; // 0x04
    u16 field_0x10;             // 0x10
};

// 0x80-byte collision entry (indexed from CColiObj::field_0x40): the bit
// flags at +0x02 address a word/bit pair in the registration bit-table, and
// the two float triples at +0x64/+0x70 are the segment tested by the
// func_804BB0C8 family in the sibling unit.
struct CColiEntry {
    u8 field_0x00[0x02];
    u16 field_0x02;     // 0x02 - bit flags (word = >>5 & 0x7F, bit = & 0x1F)
    u8 field_0x04[0x60];
    f32 field_0x64[3];  // 0x64
    f32 field_0x70[3];  // 0x70
    u32 field_0x7C;     // 0x7C - pad to 0x80
};

// Collision object owning the 0x80-byte entry table at +0x40.
struct CColiObj {
    u8 field_0x00[0x40];
    CColiEntry* field_0x40;  // 0x40
};

// Registration query: guard flag at +0x00, entry-table index at +0x02.
struct CColiQuery {
    u16 field_0x00;  // 0x00
    u16 field_0x02;  // 0x02
};

// 0x14-byte registration entry with a u16 category/type id at +0x12.
struct CColiEntry2 {
    u8 field_0x00[0x12];
    u16 field_0x12;  // 0x12
};

// Collision registration object: category table +0x28, entry table +0x30,
// and two callback member-function pointers at +0x7C / +0xA0 invoked by the
// register helpers below.  The virtual destructor is only declared (never
// defined or called in this TU) to make the member-function-pointer type
// 12 bytes wide; nothing is emitted for it.
struct CColiRegObj {
    u8 field_0x00[0x28];                               // 0x00
    u32* field_0x28;                                   // 0x28
    u8 field_0x2C[0x30 - 0x2C];                        // 0x2C - pad
    CColiEntry2* field_0x30;                           // 0x30
    u8 field_0x34[0x7C - 0x34];                        // 0x34
    void (CColiRegObj::*field_0x7C)();                 // 0x7C
    u8 field_0x88[0xA0 - 0x88];                        // 0x88 - pad
    void (CColiRegObj::*field_0xA0)(const ml::CVec3*, const ml::CVec3*);  // 0xA0
};

// Segment tests shared with the sibling coli unit (code_804BAE10).
struct Vec3 { f32 x; f32 y; f32 z; };
bool func_804BB0C8(const f32* a, const f32* b);
bool func_804BB1A0(const Vec3* a, const Vec3* b);
bool func_804BB228(const Vec3* a, const Vec3* b);

// Movement-tracked mover: previous position +0x0C, saved copy +0x38,
// current position +0x50, distance-squared threshold +0x5C, flag +0x60.
struct CTrackedMover {
    u8 field_0x00[0x0C];         // 0x00
    f32 field_0x0C;              // 0x0C
    f32 field_0x10;              // 0x10
    f32 field_0x14;              // 0x14
    f32 field_0x18;              // 0x18
    f32 field_0x1C;              // 0x1C
    f32 field_0x20;              // 0x20
    u8 field_0x24[0x38 - 0x24];  // 0x24
    f32 field_0x38;              // 0x38
    f32 field_0x3C;              // 0x3C
    f32 field_0x40;              // 0x40
    f32 field_0x44;              // 0x44
    f32 field_0x48;              // 0x48
    f32 field_0x4C;              // 0x4C
    f32 field_0x50;              // 0x50
    f32 field_0x54;              // 0x54
    f32 field_0x58;              // 0x58
    f32 field_0x5C;              // 0x5C
    u8 field_0x60;               // 0x60
};

// Collision-register callbacks (retail symbols in this TU).
void func_804B708C(CColiSrc* a, CColiMover* b);
void func_804B71CC(CColiSrc* a, CColiMover* b);

// func_804B59C8 - track mover movement. When the squared distance between
// the current position (0x50) and the previous position (0x0C) is at least
// the stored threshold (0x5C), refresh the threshold and save the previous
// position block (0x0C..0x20) to 0x38..0x4C. First move always registers.
void func_804B59C8(CTrackedMover* self) {
    nw4r::math::VEC3 delta;
    nw4r::math::VEC3Sub(&delta, (const nw4r::math::VEC3*)&self->field_0x50,
                        (const nw4r::math::VEC3*)&self->field_0x0C);
    f32 dist2 = nw4r::math::VEC3LenSq(&delta);
    if (self->field_0x60 == 0) {
        self->field_0x60 = 1;
    } else if (dist2 >= self->field_0x5C) {
        return;
    }
    self->field_0x5C = dist2;
    self->field_0x38 = self->field_0x0C;
    self->field_0x3C = self->field_0x10;
    self->field_0x40 = self->field_0x14;
    self->field_0x44 = self->field_0x18;
    self->field_0x48 = self->field_0x1C;
    self->field_0x4C = self->field_0x20;
}

u32 func_804B5A68(void) {
    extern u32 lbl_eu_80663AC8;
    return lbl_eu_80663AC8;
}

extern char lbl_eu_8065F1A0[];
extern "C" void* func_804B5A70(void){ return (void*)&lbl_eu_8065F1A0; }

void __dt__804B5A7C(){}

void __dt__804B5AD0(){}

void func_804B5B34(void) {}

void func_804B5B38(){}

void func_804B5BB0(){}

// func_804B5C58 - sample the half-float triple for the mover's table index,
// then ray-cast against the manager's reference vectors: t is the distance
// along the direction vector (0x0C) from the offset plane (0x00) to the
// mover. Register the mover when the hit is in front.
void func_804B5C58(CColiSrc* a, CColiMover* b) {
    u16 idx = b->field_0x10;
    s16* tri = (s16*)((u8*)a->field_0x1C + idx * 6);
    lbl_eu_8065F1C8.field_0xC8.x = __OSs16tof32(&tri[0]);
    lbl_eu_8065F1C8.field_0xC8.y = __OSs16tof32(&tri[1]);
    lbl_eu_8065F1C8.field_0xC8.z = __OSs16tof32(&tri[2]);
    f32 dot2 = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0xC8,
                                   (const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0x0C);
    if (dot2 == lbl_eu_8066AED0) return;
    f32 dot = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0xC8,
                                  (const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0x00);
    f32 t = (b->field_0x00 - dot) / dot2;
    lbl_eu_8065F1C8.field_0xD4 = t;
    if (t <= lbl_eu_8066AED0) return;
    lbl_eu_8065F1C8.field_0xE0 = b;
    func_804B708C(a, b);
}

void func_804B5CEC(){}

// func_804B5DE8 - sample the half-float triple for the mover's table index,
// then check the offset of the mover from the plane defined by the sampled
// normal (0xC8) and the offset vector (0x00). Register the mover when the
// offset lands in the acceptance window [const, 0x120).
void func_804B5DE8(CColiSrc* a, CColiMover* b) {
    u16 idx = b->field_0x10;
    s16* tri = (s16*)((u8*)a->field_0x1C + idx * 6);
    lbl_eu_8065F1C8.field_0xC8.x = __OSs16tof32(&tri[0]);
    lbl_eu_8065F1C8.field_0xC8.y = __OSs16tof32(&tri[1]);
    lbl_eu_8065F1C8.field_0xC8.z = __OSs16tof32(&tri[2]);
    f32 dot = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0xC8,
                                  (const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0x00);
    f32 diff = dot - b->field_0x00;
    lbl_eu_8065F1C8.field_0xD4 = diff;
    if (lbl_eu_8066AED0 > diff) return;
    if (diff >= lbl_eu_8065F1C8.field_0x120) return;
    lbl_eu_8065F1C8.field_0xE0 = b;
    func_804B71CC(a, b);
}

void func_804B5E68(){}

void func_804B61F8(void) {}

void func_804B61FC(){}

void func_804B62B0(){}

void func_804B6364(){}

// func_804B6418 - register the query's collision index when its entry's
// segment test passes and the index is not already registered: the entry's
// flag bits address the shared bit-table (lbl_eu_8065D1A0+0x2000) and the
// accepted indices are appended to the 16-slot list at +0x2160.
void func_804B6418(CColiObj* self, CColiQuery* query) {
    CColiWork* w = &lbl_eu_8065D1A0;
    if (query->field_0x00 != 0) return;
    u16 idx = query->field_0x02;
    CColiEntry* entry = &self->field_0x40[idx];
    u16 flags = entry->field_0x02;
    if (w->b.bitTable[(flags >> 5) & 0x7F] & (1u << (flags & 0x1F))) return;
    if (!func_804BB0C8((const f32*)&entry->field_0x64, (const f32*)&entry->field_0x70)) return;
    CColiListMgr* lm = &w->l.listMgr;
    u16 v = query->field_0x02;
    if (lm->count < 0x10) {
        int cnt = lm->count;
        lm->list[cnt] = v;
        lm->count = cnt + 1;
    }
}

void func_804B64CC(CColiObj* self, CColiQuery* query) {
    CColiWork* w = &lbl_eu_8065D1A0;
    if (query->field_0x00 != 0) return;
    u16 idx = query->field_0x02;
    CColiEntry* entry = &self->field_0x40[idx];
    u16 flags = entry->field_0x02;
    if (w->b.bitTable[(flags >> 5) & 0x7F] & (1u << (flags & 0x1F))) return;
    if (!func_804BB1A0((const Vec3*)&entry->field_0x64, (const Vec3*)&entry->field_0x70)) return;
    CColiListMgr* lm = &w->l.listMgr;
    u16 v = query->field_0x02;
    if (lm->count < 0x10) {
        int cnt = lm->count;
        lm->list[cnt] = v;
        lm->count = cnt + 1;
    }
}

void func_804B6580(CColiObj* self, CColiQuery* query) {
    CColiWork* w = &lbl_eu_8065D1A0;
    if (query->field_0x00 != 0) return;
    u16 idx = query->field_0x02;
    CColiEntry* entry = &self->field_0x40[idx];
    u16 flags = entry->field_0x02;
    if (w->b.bitTable[(flags >> 5) & 0x7F] & (1u << (flags & 0x1F))) return;
    if (!func_804BB228((const Vec3*)&entry->field_0x64, (const Vec3*)&entry->field_0x70)) return;
    CColiListMgr* lm = &w->l.listMgr;
    u16 v = query->field_0x02;
    if (lm->count < 0x10) {
        int cnt = lm->count;
        lm->list[cnt] = v;
        lm->count = cnt + 1;
    }
}

void func_804B6634(void) {}

// func_804B6638 / func_804B66B0 - append a new scene-resource entry to the
// 32-slot table (lbl_eu_8065F428): copy the 12-byte header and info block
// from the arguments and fill the manager-owned fields from lbl_eu_8065F1C8.
void func_804B6638(const u32* head, const u32* info) {
    s32 count = lbl_eu_80665988;
    if (count >= 0x20) return;
    lbl_eu_8065F428[count].field_0x00 = head[0];
    lbl_eu_8065F428[count].field_0x04 = head[1];
    lbl_eu_8065F428[count].field_0x08 = head[2];
    lbl_eu_8065F428[count].field_0x0C = info[0];
    lbl_eu_8065F428[count].field_0x10 = info[1];
    lbl_eu_8065F428[count].field_0x14 = info[2];
    lbl_eu_8065F428[count].field_0x18 = lbl_eu_8065F1C8.field_0xE0;
    lbl_eu_8065F428[count].field_0x1C = lbl_eu_8065F1C8.field_0xEC;
    lbl_eu_8065F428[count].field_0x20 = lbl_eu_8065F1C8.field_0x130;
    lbl_eu_80665988 = count + 1;
}

void func_804B66B0(const u32* head, const u32* info) {
    s32 count = lbl_eu_80665988;
    if (count >= 0x20) return;
    lbl_eu_8065F428[count].field_0x00 = head[0];
    lbl_eu_8065F428[count].field_0x04 = head[1];
    lbl_eu_8065F428[count].field_0x08 = head[2];
    lbl_eu_8065F428[count].field_0x0C = info[0];
    lbl_eu_8065F428[count].field_0x10 = info[1];
    lbl_eu_8065F428[count].field_0x14 = info[2];
    lbl_eu_8065F428[count].field_0x18 = lbl_eu_8065F1C8.field_0xE0;
    lbl_eu_8065F428[count].field_0x1C = lbl_eu_8065F1C8.field_0xEC;
    lbl_eu_8065F428[count].field_0x20 = lbl_eu_8065F1C8.field_0x130;
    lbl_eu_80665988 = count + 1;
}

void func_804B6728(){}

void func_804B684C(){}

// Invoke the manager's callback slot with the stored context pointer.
void func_804B6954(void* a, void* b, void* c) {
    void (*fn)(void*, void*, void*) = (void (*)(void*, void*, void*))lbl_eu_8065F1C8.field_0xA0;
    void* p = lbl_eu_8065F1C8.field_0x9C;
    fn(b, c, p);
}

// func_804B6974 - transform the two input vectors by the manager's matrix
// source (0xE8) and embedded matrix (0xF0), then normalise the second result
// (falling back to ml::CVec3::zero for a null vector) and hand both to the
// mover callback (member-function pointer at +0xA0).
void func_804B6974(CColiRegObj* self, const Vec* in1, const Vec* in2) {
    ml::CVec3 out1;
    ml::CVec3 out2;
    PSMTXMultVec(*lbl_eu_8065F1C8.field_0xE8, in1, out1);
    PSMTXMultVec(lbl_eu_8065F1C8.field_0xF0, in2, out2);
    f32 len2 = out2.y * out2.y + out2.x * out2.x + out2.z * out2.z;
    if (lbl_eu_8066AED0 == len2) {
        out2 = ml::CVec3::zero;
    } else {
        PSVECNormalize(out2, out2);
    }
    (self->*self->field_0xA0)(&out1, &out2);
}

void func_804B6A3C(){}

void func_804B6C48(){}

void func_804B6D30(){}

void func_804B6F58(){}

extern "C" u32 func_804B7044(u8* self, float f) {
    u32 r = 0;
    if (f <= *(float*)((u8*)self + 0x64) && f >= *(float*)((u8*)self + 0x68)) r = 1;
    return r;
}

extern "C" u32 func_804B7074(u8* self, float f) { return f <= *(float*)((u8*)self + 0x64); }

void func_804B708C(CColiSrc* a, CColiMover* b){}

void func_804B71CC(CColiSrc* a, CColiMover* b){}

void func_804B74F0(){}

void func_804B7540(){}

void func_804B7804(){}

void func_804B791C(){}

// func_804B7944 - register every index of the u16 list: set the index's bit
// in the shared bit-table (base held in lbl_eu_80663AC8) and, when the
// entry's category passes the manager's filter flags, invoke the register
// callback (member-function pointer at +0x7C).
void func_804B7944(CColiRegObj* self, const u16* list, int count) {
    for (int i = 0; i < count; i++) {
        u16 idx = list[i];
        u32 base = lbl_eu_80663AC8;
        u32 wordIdx = idx >> 5;
        u32 bitIdx = idx & 0x1F;
        u32 w = ((u32*)base)[wordIdx];
        u32 mask = 1u << bitIdx;
        if (w & mask) continue;
        ((u32*)base)[wordIdx] = w | mask;
        CColiEntry2* e = &self->field_0x30[idx];
        u32 v = self->field_0x28[e->field_0x12];
        if (lbl_eu_8065F1C8.field_0x128 & v) continue;
        (self->*self->field_0x7C)();
    }
}

void func_804B7A00(){}

void func_804B7AC8(void) {}

void func_804B7ACC(){}

void func_804B7B6C(){}

extern char lbl_eu_8065F1A0[];
void func_804B7D9C(int, int bit_index) {
    if (bit_index > 0x140) bit_index = 0x140;
    int word_idx = bit_index >> 5;
    int bit_off = bit_index & 0x1F;
    int mask = 1 << bit_off;
    int* bits = (int*)&lbl_eu_8065F1A0;
    bits[word_idx] &= ~mask;
}

void func_804B7DD4(){}

void func_804B7E0C(){}

void func_804B8078(){}

void func_804B80A4(){}

void func_804B80CC(){}

void func_804B8108(){}

void func_804B85C8(){}

void func_804B877C(){}

void func_804B8C2C(){}

void func_804B91E0(){}

void func_804B9818(){}

void func_804B9E14(){}

void func_804BA26C(){}

void func_804BA7BC(){}

void func_804BA8B4(){}

extern "C" u32 func_804BADA0(u8* self) { return *(u32*)((u8*)self + 0x5C) != 0; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_804BADB4(){}
