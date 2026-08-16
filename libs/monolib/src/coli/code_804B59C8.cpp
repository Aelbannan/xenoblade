// Auto-scaffolded catalog TU for monolib/src/coli/code_804B59C8
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <math.h>
#include <cstring>
#include <revolution/os/OSFastCast.h>
#include <revolution/mtx/mtxvec.h>
#include <revolution/mtx/mtx.h>
#include <revolution/mtx/vec.h>
#include <nw4r/math/math_types.h>
#include <nw4r/math/math_triangular.h>
#include "monolib/math/CVec3.hpp"
#include "monolib/util/MemManager.hpp"

// ---------------------------------------------------------------------------
// Data globals
// ---------------------------------------------------------------------------

// Scene-resource entry count (sbss; shared with monolib/scn/code_804BD8E8).
extern s32 lbl_eu_80665988;

// Scene-resource registration bitmask / counters (sbss).
extern u32 lbl_eu_8066598C;
extern u32 lbl_eu_80665990;

// List of scene-resource category indices (bss, 16 u16 entries).
extern u16 lbl_eu_8065F300[];

// Collision state manager (lbl_eu_8065F1C8): ray offset at 0x00, ray
// direction at 0x0C, reference position at 0x18, AABB at 0x24/0x30, sampled
// normal at 0xC8, ray t at 0xD4, registration thresholds at 0xD8/0xDC,
// mover hook at 0xE0, matrix source pointer at 0xE8, embedded matrix at 0xF0,
// registration filter flags at 0x128, acceptance thresholds at 0x120/0x130.
struct CColiMover;
struct CColiMgr {
    nw4r::math::_VEC3 field_0x00;    // 0x00 - ray offset
    nw4r::math::_VEC3 field_0x0C;    // 0x0C - ray direction
    nw4r::math::_VEC3 field_0x18;    // 0x18 - reference position
    nw4r::math::_VEC3 field_0x24;    // 0x24 - AABB min corner
    nw4r::math::_VEC3 field_0x30;    // 0x30 - AABB max corner
    nw4r::math::_VEC3 field_0x3C[6]; // 0x3C - six OBB plane normals (func_804B8108)
    f32 field_0x84[6];               // 0x84 - six OBB plane offsets
    int field_0x9C;                  // 0x9C - vector count for the array walk
    void* field_0xA0;                // 0xA0
    nw4r::math::_VEC3 field_0xA4[3]; // 0xA4 - per-vertex sampled points
    nw4r::math::_VEC3 field_0xC8;    // 0xC8 - sampled normal
    f32 field_0xD4;                  // 0xD4 - ray t
    f32 field_0xD8;                  // 0xD8 - distance threshold
    f32 field_0xDC;                  // 0xDC - last registered distance
    CColiMover* field_0xE0;          // 0xE0 - mover hook
    u8 field_0xE4[0xE8 - 0xE4];      // 0xE4
    Mtx* field_0xE8;                 // 0xE8 - matrix source pointer
    u32 field_0xEC;                  // 0xEC
    Mtx field_0xF0;                  // 0xF0 - embedded matrix
    f32 field_0x120;                 // 0x120 - acceptance radius (func_804BBFA0)
    f32 field_0x124;                 // 0x124 - squared acceptance radius
    u32 field_0x128;                 // 0x128 - registration filter flags
    u32 field_0x12C;                 // 0x12C - second registration filter flags
    u32 field_0x130;                 // 0x130
    u8 field_0x134;                  // 0x134
    u16 field_0x136;                 // 0x136 - registered-index count (== CColiListMgr.count)
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

// Zeroing-window base for the seen-bit array (sdata).
extern u8* lbl_eu_80663ACC;

// Acceptance-threshold constants (sdata2).
extern const f32 lbl_eu_8066AED0;
extern const f32 lbl_eu_8066AED4;
extern const f32 lbl_eu_8066AEDC;
extern const f32 lbl_eu_8066AED8;  // normal-orientation flip scale
extern const f32 lbl_eu_8066AEE0;  // box raycast plane-distance threshold
// Query-box registration distance threshold (func_804B8C2C).
extern const f32 lbl_eu_8066AEE4;
// GQR5 scale table default distance threshold (func_804B9E14 / func_804B877C).
extern const f32 lbl_eu_8066AEE8;
// FIdx (512-count) to degree conversion factor (func_804BA8B4 heading math).
extern const f32 lbl_eu_8066AEEC;

// Default mover-state configuration table (.data).  The first 9 words feed
// func_804B7804; the block at +0x2C4 holds the per-stage matrix
// configuration used by func_804B9E14 / func_804B877C.
extern u32 lbl_eu_8056F508[0x340 / 4];

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

// Object owning a 6-byte half-float triple table at +0x1C, plus a float
// vertex table at +0x18 and a half-float triple-3 table at +0x20.
struct CColiSrc {
    u8 field_0x00[0x18];                          // 0x00
    u8* field_0x18;                               // 0x18 - float vertex table (12-byte stride)
    void* field_0x1C;                             // 0x1C - half-float triple table
    u8* field_0x20;                               // 0x20 - half-float triple-3 table (18-byte stride)
    u8 field_0x24[0x94 - 0x24];                   // 0x24
    void (CColiSrc::*field_0x94)(const ml::CVec3*, const ml::CVec3*);  // 0x94 - mover callback
    u8 field_0xA0[0xB8 - 0xA0];                   // 0xA0
    bool (CColiSrc::*field_0xB8)(f32);            // 0xB8 - height gate
};

// 12-byte collision entry (indexed from CColiThing::field_0x38).
struct CColiEntry12 {
    u8 field_0x00[0x02];
    u16 field_0x02;   // 0x02 - bit flags (word = >>5, bit = & 0x1F)
    u32 field_0x04;   // 0x04 - element index (func_804BBFA0)
    u32 field_0x08;   // 0x08 - byte offset
};

// Collision registration object: base pointer at 0x14, result pointer at
// 0x30, 12-byte entry table at 0x38.
struct CColiThing {
    u8 field_0x00[0x14];                  // 0x00
    u8* field_0x14;                       // 0x14 - base pointer
    u8 field_0x18[0x30 - 0x18];           // 0x18
    u8* field_0x30;                       // 0x30 - result pointer
    u8 field_0x34[0x38 - 0x34];           // 0x34
    CColiEntry12* field_0x38;             // 0x38 - 12-byte entry table
};

// Collision volume: 3x3 rotation (as a 3x4 matrix) at 0x08, and the AABB
// min corner at 0xC8 / max corner at 0xD4.
struct CColiVolume {
    u8 field_0x00[0x08];                       // 0x00
    Mtx field_0x08;                            // 0x08 - 3x4, only columns 0..2 copied
    f32 field_0x38;                            // 0x38
    u8 field_0x3C[0xC8 - 0x3C];                // 0x3C
    f32 field_0xC8;                            // 0xC8 - AABB min.x
    f32 field_0xCC;                            // 0xCC - AABB min.y
    f32 field_0xD0;                            // 0xD0 - AABB min.z
    f32 field_0xD4;                            // 0xD4 - AABB max.x
    f32 field_0xD8;                            // 0xD8 - AABB max.y
    f32 field_0xDC;                            // 0xDC - AABB max.z
};

// Scene-resource header word: the registration copy stores the raw word
// while the collision math reads the float view (union type punning).
union ScnHeadWord {
    u32 w;
    f32 f;
};

// Mover-state object initialised by func_804B7804: counters at 0x14..0x44,
// allocation at 0x60, 9-word configuration copy at 0x7C..0x9C.
struct CColiInEntry;
struct CColiLink;
struct CColiVolEntry;
struct CColiMoverState {
    u8 field_0x00[0x14];
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1C;
    u32 field_0x20;
    u8 field_0x24[0x28 - 0x24];
    u32 field_0x28;
    u32 field_0x2C;
    u32 field_0x30;
    CColiLink* field_0x34;         // 0x34 - linked object (index at +0x04)
    CColiEntry12* field_0x38;      // 0x38 - 12-byte entry table
    u32 field_0x3C;
    CColiVolEntry* field_0x40;     // 0x40 - 0x80-byte volume entry table
    const CColiInEntry* field_0x44;  // 0x44 - input entry array (0x3C-byte stride)
    u8 field_0x48[0x5C - 0x48];
    u32 field_0x5C;
    u8* field_0x60;
    f32 field_0x64;   // 0x64 - upper Y bound (func_804B7044 gate)
    f32 field_0x68;   // 0x68 - lower Y bound
    u32 field_0x6C;
    u32 field_0x70;
    u8 field_0x74[0x78 - 0x74];
    u32 field_0x78;
    u32 field_0x7C;
    u32 field_0x80;
    u32 field_0x84;
    u32 field_0x88;
    u32 field_0x8C;
    u32 field_0x90;
    u32 field_0x94;
    u32 field_0x98;
    u32 field_0x9C;
    u32 field_0xA0;
    u32 field_0xA4;
    u32 field_0xA8;
    u32 field_0xAC;
    u32 field_0xB0;
    u32 field_0xB4;
    u32 field_0xB8;
    u32 field_0xBC;
    u32 field_0xC0;
};

// 0x3C-byte input record: 12 matrix floats plus an extrusion scalar.
struct CColiInEntry {
    u32 field_0x00;   // 0x00
    f32 m[12];        // 0x04 - 12 matrix floats (3x4)
    f32 field_0x34;   // 0x34
    f32 field_0x38;   // 0x38 - half-extent (radius)
};

// Object linked from CColiMoverState::field_0x34 (element index at +0x04).
struct CColiLink {
    u32 field_0x00;
    u32 field_0x04;   // 0x04 - element index for func_804BBFA0
};

// 0x80-byte volume entry (indexed from CColiMoverState::field_0x40): 3x3
// rotation with 0x10 row stride at +0x04, full 3x4 matrix at +0x34 and an
// extrusion scale at +0x7C.
struct CColiVolEntry {
    u16 field_0x00;        // 0x00 - index into the CColiEntry12 table
    u16 field_0x02;        // 0x02 - flags (bit 0x8000 selects the config table)
    f32 m[3][4];           // 0x04 - 3x3 rotation (row stride 0x10)
    Mtx field_0x34;        // 0x34 - 3x4 matrix
    u8 field_0x64[0x7C - 0x64];
    f32 field_0x7C;        // 0x7C - scale
};

// 48-byte dual-view matrix: float view for math, u64 view for the retail
// 8-byte word-copy schedule.
union MtxView {
    Mtx m;   // f32[3][4]
    u64 w[6];
    u32 u[12];
};

// 0xE0-byte mover-state entry (indexed from CColiMoverState::field_0x60).
struct CColiOutEntry {
    u32 field_0x00;        // 0x00 - flags (0 / 2)
    f32 field_0x04;        // 0x04 - 0.0
    MtxView mtx;           // 0x08 - current matrix
    MtxView inv;           // 0x38 - current inverse
    MtxView savedMtx;      // 0x68 - previous matrix
    MtxView savedInv;      // 0x98 - previous inverse
    f32 field_0xC8;        // 0xC8 - aabb (max + radius)
    f32 field_0xCC;        // 0xCC
    f32 field_0xD0;        // 0xD0
    f32 field_0xD4;        // 0xD4 - aabb (min - radius)
    f32 field_0xD8;        // 0xD8
    f32 field_0xDC;        // 0xDC
};

// Shared mover-state reset helper (defined in the sibling coli unit; C
// linkage so the bl reloc carries the retail name).
extern "C" void func_804BAE10(CColiMoverState* self);

// Query/mover object: scalar offset at +0x00, three u16 vertex indices into
// the object's float vertex table at +0x04, a u16 index into the half-float
// triple-3 table at +0x0A, half-float table index at +0x10.
struct CColiMover {
    f32 field_0x00;             // 0x00 - scalar (offset along ray)
    u16 field_0x04[3];          // 0x04 - vertex indices (float vertex table)
    u16 field_0x0A;             // 0x0A - index into the triple-3 table
    u8 field_0x0C[0x10 - 0x0C]; // 0x0C
    u16 field_0x10;             // 0x10 - index into the half-float triple table
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
    void (CColiRegObj::*field_0xAC)(u16*, u16);  // 0xAC - register callback (func_804B791C dispatch)
    bool (CColiRegObj::*field_0xB8)(f32);              // 0xB8 - height gate
};

// Segment tests shared with the sibling coli unit (code_804BAE10).
struct Vec3 { f32 x; f32 y; f32 z; };
bool func_804BB0C8(const f32* a, const f32* b);

// Collision-scan state object: field_0x74 is a bit index used to size the
// zeroing window below the shared bit-table (func_804BA7BC).
struct CColiScan {
    u8 field_0x00[0x74];  // 0x00
    s32 field_0x74;       // 0x74
};

void func_804BC494(CColiScan* self, const Vec* a, const Vec* b);
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

// Query-box words written by func_804BBFA0 / func_804BC134 (two Vec3s).
extern u32 lbl_eu_8065F3F0[6];

// Collision-register callbacks (retail symbols in this TU).
void func_804B708C(CColiSrc* a, CColiMover* b);
void func_804B71CC(CColiSrc* a, CColiMover* b);

// Sibling-unit helpers (C linkage: the defining TU declares them extern "C", so
// the retail symbol names are unmangled).
extern "C" u32 func_804BC9A0(CColiMoverState* self);
extern "C" void func_804BBFA0(CColiMoverState* self, const Vec* src, u32 idx, f32 radius);
extern "C" void func_804BBFD4(CColiMoverState* self, const Vec* a, const Vec* b, u32 idx);
extern "C" void func_804BC134(CColiMoverState* self, const Vec* src, u32 idx, f32 y);
extern "C" void func_804BC164(CColiMoverState* self, const Vec* a, const Vec* b, u32 idx,
                            f32 s0, f32 s1, f32 s2);

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

// func_804B5CEC - sample the half-float triple for the mover's table index,
// then ask the object's height gate (ptmf +0xB8) whether the Y component is
// acceptable. When it is, ray-cast against the manager's reference vectors:
// t is the distance along the direction vector (0x0C) from the offset plane
// (0x00) to the mover. Register the mover when t lands in [0, 0x8066AED4].
void func_804B5CEC(CColiSrc* a, CColiMover* b) {
    u16 idx = b->field_0x10;
    s16* tri = (s16*)((u8*)a->field_0x1C + idx * 6);
    lbl_eu_8065F1C8.field_0xC8.x = __OSs16tof32(&tri[0]);
    lbl_eu_8065F1C8.field_0xC8.y = __OSs16tof32(&tri[1]);
    lbl_eu_8065F1C8.field_0xC8.z = __OSs16tof32(&tri[2]);
    if ((a->*a->field_0xB8)(lbl_eu_8065F1C8.field_0xC8.y) != 0) return;
    f32 dot2 = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0xC8,
                                   (const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0x0C);
    if (dot2 == lbl_eu_8066AED0) return;
    f32 dot = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0xC8,
                                  (const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0x00);
    f32 t = (b->field_0x00 - dot) / dot2;
    lbl_eu_8065F1C8.field_0xD4 = t;
    if (!(t >= lbl_eu_8066AED0)) return;
    if (!(t <= lbl_eu_8066AED4)) return;
    lbl_eu_8065F1C8.field_0xE0 = b;
    func_804B708C(a, b);
}

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

// func_804B5E68 - sample the mover's half-float normal triple into the
// manager, then clip the mover's three-vertex polygon against the six OBB
// planes (normals at 0x3C, offsets at 0x84).  Each clip pass computes the
// signed distance of every polygon vertex from the plane, returns early when
// the whole polygon is on the far side, and otherwise Sutherland-Hodgman
// clips the polygon: vertices inside are kept, and edges crossing the plane
// contribute an intersection point (vertex + edge * (-dist / (n.edge))).  The
// surviving polygon and the sampled normal go to the mover callback (ptmf
// +0x94) and the scene-resource count is incremented.
void func_804B5E68(CColiSrc* a, CColiMover* b) {
    CColiMgr* mgr = &lbl_eu_8065F1C8;
    u16 idx = b->field_0x10;
    s16* tri = (s16*)((u8*)a->field_0x1C + idx * 6);
    mgr->field_0xC8.x = __OSs16tof32(&tri[0]);
    mgr->field_0xC8.y = __OSs16tof32(&tri[1]);
    mgr->field_0xC8.z = __OSs16tof32(&tri[2]);
    Vec3 poly[8];
    Vec3 out[8];
    f32 dist[8];
    poly[0] = *(Vec3*)(a->field_0x18 + b->field_0x04[0] * 12);
    poly[1] = *(Vec3*)(a->field_0x18 + b->field_0x04[1] * 12);
    poly[2] = *(Vec3*)(a->field_0x18 + b->field_0x04[2] * 12);
    Vec3* cur = poly;
    Vec3* nxt = out;
    const f32 zero = lbl_eu_8066AED0;
    int count = 3;
    for (int i = 0; i < 6; i++) {
        const nw4r::math::_VEC3* n = &mgr->field_0x3C[i];
        int all = 0;
        int below = 0;
        for (int j = 0; j < count; j++) {
            f32 d = cur[j].x * n->x + cur[j].y * n->y + cur[j].z * n->z - mgr->field_0x84[i];
            dist[j] = d;
            all |= 1u << j;
            if (d < zero) below |= 1u << j;
        }
        if (all == below) return;
        if (below != 0) {
            int k = 0;
            for (int j = 0; j < count; j++) {
                int j2 = j + 1;
                if (j2 >= count) j2 = 0;
                if (below & (1u << j)) {
                    if (!(below & (1u << j2))) {
                        f32 denom = (cur[j2].x - cur[j].x) * n->x + (cur[j2].y - cur[j].y) * n->y
                                  + (cur[j2].z - cur[j].z) * n->z;
                        if (denom != zero) {
                            f32 t = -dist[j] / denom;
                            nxt[k].x = cur[j].x + (cur[j2].x - cur[j].x) * t;
                            nxt[k].y = cur[j].y + (cur[j2].y - cur[j].y) * t;
                            nxt[k].z = cur[j].z + (cur[j2].z - cur[j].z) * t;
                            k++;
                        }
                    }
                } else {
                    nxt[k] = cur[j];
                    k++;
                    if (below & (1u << j2)) {
                        f32 denom = (cur[j2].x - cur[j].x) * n->x + (cur[j2].y - cur[j].y) * n->y
                                  + (cur[j2].z - cur[j].z) * n->z;
                        if (denom != zero) {
                            f32 t = -dist[j] / denom;
                            nxt[k].x = cur[j].x + (cur[j2].x - cur[j].x) * t;
                            nxt[k].y = cur[j].y + (cur[j2].y - cur[j].y) * t;
                            nxt[k].z = cur[j].z + (cur[j2].z - cur[j].z) * t;
                            k++;
                        }
                    }
                }
            }
            count = k;
            Vec3* swp = cur;
            cur = nxt;
            nxt = swp;
        }
    }
    mgr->field_0x9C = count;
    (a->*a->field_0x94)((const ml::CVec3*)cur, (const ml::CVec3*)&mgr->field_0xC8);
    lbl_eu_80665988++;
}

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

// func_804B6728 - register a scene-resource hit by distance: when the
// squared distance from head to the manager's reference position is within
// the threshold (0xD8) and, when an entry already exists, only if the new
// distance is smaller than the last registered one, overwrite slot 0 with
// the head/info blocks and the manager's mover hook + counters.
void func_804B6728(u32 a, const ScnHeadWord* head, const u32* info) {
    Vec3 d;
    d.x = lbl_eu_8065F1C8.field_0x18.x - head[0].f;
    d.y = lbl_eu_8065F1C8.field_0x18.y - head[1].f;
    d.z = lbl_eu_8065F1C8.field_0x18.z - head[2].f;
    f32 dist2 = d.x * d.x + d.y * d.y + d.z * d.z;
    if (lbl_eu_8065F1C8.field_0xD8 >= dist2) {
        if (lbl_eu_80665988 != 0) {
            if (lbl_eu_8065F1C8.field_0xDC > dist2) {
                ScnResourceEntry* e = &lbl_eu_8065F428[0];
                e->field_0x00 = head[0].w;
                e->field_0x04 = head[1].w;
                e->field_0x08 = head[2].w;
                e->field_0x0C = info[0];
                e->field_0x10 = info[1];
                e->field_0x14 = info[2];
                e->field_0x18 = lbl_eu_8065F1C8.field_0xE0;
                e->field_0x1C = lbl_eu_8065F1C8.field_0xEC;
                e->field_0x20 = lbl_eu_8065F1C8.field_0x130;
                lbl_eu_8065F1C8.field_0xDC = dist2;
            }
        } else {
            ScnResourceEntry* e = &lbl_eu_8065F428[0];
            e->field_0x00 = head[0].w;
            e->field_0x04 = head[1].w;
            e->field_0x08 = head[2].w;
            e->field_0x0C = info[0];
            e->field_0x10 = info[1];
            e->field_0x14 = info[2];
            e->field_0x18 = lbl_eu_8065F1C8.field_0xE0;
            e->field_0x1C = lbl_eu_8065F1C8.field_0xEC;
            e->field_0x20 = lbl_eu_8065F1C8.field_0x130;
            lbl_eu_8065F1C8.field_0xDC = dist2;
        }
        lbl_eu_80665988 = 1;
    }
}

void func_804B684C(u32 a, const ScnHeadWord* head, const u32* info) {
    if (lbl_eu_80665988 != 0) {
        f32 dy = __fabs(lbl_eu_8065F1C8.field_0x18.y - head[1].f);
        if (lbl_eu_8065F1C8.field_0xDC > dy) {
            lbl_eu_8065F428[0].field_0x00 = head[0].w;
            lbl_eu_8065F428[0].field_0x04 = head[1].w;
            lbl_eu_8065F428[0].field_0x08 = head[2].w;
            lbl_eu_8065F428[0].field_0x0C = info[0];
            lbl_eu_8065F428[0].field_0x10 = info[1];
            lbl_eu_8065F428[0].field_0x14 = info[2];
            lbl_eu_8065F428[0].field_0x18 = lbl_eu_8065F1C8.field_0xE0;
            lbl_eu_8065F428[0].field_0x1C = lbl_eu_8065F1C8.field_0xEC;
            lbl_eu_8065F428[0].field_0x20 = lbl_eu_8065F1C8.field_0x130;
            lbl_eu_8065F1C8.field_0xDC = dy;
        }
    } else {
        lbl_eu_8065F428[0].field_0x00 = head[0].w;
        lbl_eu_8065F428[0].field_0x04 = head[1].w;
        lbl_eu_8065F428[0].field_0x08 = head[2].w;
        lbl_eu_8065F428[0].field_0x0C = info[0];
        lbl_eu_8065F428[0].field_0x10 = info[1];
        lbl_eu_8065F428[0].field_0x14 = info[2];
        lbl_eu_8065F428[0].field_0x18 = lbl_eu_8065F1C8.field_0xE0;
        lbl_eu_8065F428[0].field_0x1C = lbl_eu_8065F1C8.field_0xEC;
        lbl_eu_8065F428[0].field_0x20 = lbl_eu_8065F1C8.field_0x130;
        lbl_eu_8065F1C8.field_0xDC = __fabs(lbl_eu_8065F1C8.field_0x18.y - head[1].f);
    }
    lbl_eu_80665988 = 1;
}

// Invoke the manager's callback slot with the stored context pointer.
void func_804B6954(void* a, void* b, void* c) {
    void (*fn)(void*, void*, void*) = (void (*)(void*, void*, void*))lbl_eu_8065F1C8.field_0xA0;
    void* p = (void*)lbl_eu_8065F1C8.field_0x9C;
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

// func_804B6A3C - build the ray-triangle surface: out1 is the anchor
// transformed by the matrix source (0xE8); p0/p1 are the anchor plus the
// manager's per-vertex sampled points (0xA4), transformed, and used as the
// triangle edges.  The cross-product normal is oriented toward the query
// point transformed by the embedded matrix (0xF0), normalised (falling back
// to ml::CVec3::zero), and both vectors go to the mover callback (ptmf
// +0xA0).
void func_804B6A3C(CColiRegObj* self, const Vec* in1, const Vec* in2) {
    ml::CVec3 out1;
    ml::CVec3 out2;
    ml::CVec3 p0;
    ml::CVec3 p1;
    PSMTXMultVec(*lbl_eu_8065F1C8.field_0xE8, in1, out1);
    nw4r::math::VEC3Add((nw4r::math::VEC3*)&p0, (const nw4r::math::VEC3*)in1,
                        (const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0xA4[0]);
    nw4r::math::VEC3Add((nw4r::math::VEC3*)&p1, (const nw4r::math::VEC3*)in1,
                        (const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0xA4[1]);
    PSMTXMultVec(*lbl_eu_8065F1C8.field_0xE8, p0, p0);
    PSMTXMultVec(*lbl_eu_8065F1C8.field_0xE8, p1, p1);
    PSMTXMultVec(lbl_eu_8065F1C8.field_0xF0, in2, out2);
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&p0, (const nw4r::math::VEC3*)&p0,
                        (const nw4r::math::VEC3*)&out1);
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&p1, (const nw4r::math::VEC3*)&p1,
                        (const nw4r::math::VEC3*)&out1);
    PSVECCrossProduct(p0, p1, p0);
    f32 dot = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&p0,
                                  (const nw4r::math::VEC3*)&out2);
    if (dot < lbl_eu_8066AED0) {
        out2 = p0 * lbl_eu_8066AED8;
    } else {
        out2 = p0;
    }
    f32 len2 = out2.y * out2.y;
    len2 = len2 + out2.x * out2.x;
    len2 = len2 + out2.z * out2.z;
    if (lbl_eu_8066AED0 == len2) {
        out2 = ml::CVec3::zero;
    } else {
        PSVECNormalize(out2, out2);
    }
    (self->*self->field_0xA0)(&out1, &out2);
}

// func_804B6C48 - normalise `in` against the manager's embedded matrix
// (falling back to ml::CVec3::zero for a null vector), then ask the object's
// height gate (ptmf +0xB8) whether the Y component is acceptable. When it is,
// transform `a` by the matrix source and hand both vectors to the mover
// callback (ptmf +0xA0).
void func_804B6C48(CColiRegObj* self, const Vec* a, const Vec* in) {
    ml::CVec3 out;
    PSMTXMultVec(lbl_eu_8065F1C8.field_0xF0, in, out);
    f32 len2 = out.y * out.y;
    len2 = len2 + out.x * out.x;
    len2 = len2 + out.z * out.z;
    f32 zero = lbl_eu_8066AED0;
    if (zero == len2) {
        out = ml::CVec3::zero;
    } else {
        PSVECNormalize(out, out);
    }
    if ((self->*self->field_0xB8)(out.y) == 0) {
        ml::CVec3 transformed;
        PSMTXMultVec(*lbl_eu_8065F1C8.field_0xE8, a, transformed);
        (self->*self->field_0xA0)(&transformed, &out);
    }
}

// func_804B6D30 - like func_804B6A3C, but the oriented normal is first
// passed to the object's height gate (ptmf +0xB8); the mover callback
// (ptmf +0xA0) only fires when the gate accepts out2.y.
void func_804B6D30(CColiRegObj* self, const Vec* in1, const Vec* in2) {
    ml::CVec3 out1;
    ml::CVec3 out2;
    ml::CVec3 p0;
    ml::CVec3 p1;
    PSMTXMultVec(*lbl_eu_8065F1C8.field_0xE8, in1, out1);
    nw4r::math::VEC3Add((nw4r::math::VEC3*)&p0, (const nw4r::math::VEC3*)in1,
                        (const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0xA4[0]);
    nw4r::math::VEC3Add((nw4r::math::VEC3*)&p1, (const nw4r::math::VEC3*)in1,
                        (const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0xA4[1]);
    PSMTXMultVec(*lbl_eu_8065F1C8.field_0xE8, p0, p0);
    PSMTXMultVec(*lbl_eu_8065F1C8.field_0xE8, p1, p1);
    PSMTXMultVec(lbl_eu_8065F1C8.field_0xF0, in2, out2);
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&p0, (const nw4r::math::VEC3*)&p0,
                        (const nw4r::math::VEC3*)&out1);
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&p1, (const nw4r::math::VEC3*)&p1,
                        (const nw4r::math::VEC3*)&out1);
    PSVECCrossProduct(p0, p1, p0);
    f32 dot = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&p0,
                                  (const nw4r::math::VEC3*)&out2);
    if (dot < lbl_eu_8066AED0) {
        out2 = p0 * lbl_eu_8066AED8;
    } else {
        out2 = p0;
    }
    f32 len2 = out2.y * out2.y;
    len2 = len2 + out2.x * out2.x;
    len2 = len2 + out2.z * out2.z;
    if (lbl_eu_8066AED0 == len2) {
        out2 = ml::CVec3::zero;
    } else {
        PSVECNormalize(out2, out2);
    }
    if ((self->*self->field_0xB8)(out2.y) == 0) {
        (self->*self->field_0xA0)(&out1, &out2);
    }
}

// func_804B6F58 - normalise `in` against the manager's embedded matrix,
// transform every vector of the `vecs` array by the matrix source in place,
// then hand the array and the normalised vector to the mover callback
// (ptmf +0xA0).
void func_804B6F58(CColiRegObj* self, ml::CVec3* vecs, const Vec* in) {
    ml::CVec3 out;
    PSMTXMultVec(lbl_eu_8065F1C8.field_0xF0, in, out);
    f32 len2 = out.y * out.y;
    len2 = len2 + out.x * out.x;
    len2 = len2 + out.z * out.z;
    f32 zero = lbl_eu_8066AED0;
    if (zero == len2) {
        out = ml::CVec3::zero;
    } else {
        PSVECNormalize(out, out);
    }
    for (int i = 0; i < lbl_eu_8065F1C8.field_0x9C; i++) {
        PSMTXMultVec(*lbl_eu_8065F1C8.field_0xE8, (const Vec*)&vecs[i], (Vec*)&vecs[i]);
    }
    (self->*self->field_0xA0)(vecs, &out);
}

extern "C" u32 func_804B7044(u8* self, float f) {
    u32 r = 0;
    if (f <= *(float*)((u8*)self + 0x64) && f >= *(float*)((u8*)self + 0x68)) r = 1;
    return r;
}

extern "C" u32 func_804B7074(u8* self, float f) { return f <= *(float*)((u8*)self + 0x64); }

// func_804B708C - resolve the raycast hit point (offset + dir * t) and test
// it against the mover's three vertices: store the mover's half-float vertex
// triple into the manager's point slot and bail out as soon as any vertex
// is within the acceptance radius; otherwise hand the hit point and sampled
// normal to the object's mover callback (ptmf +0x94).
void func_804B708C(CColiSrc* self, CColiMover* b) {
    Vec3 tmp;
    tmp.z = lbl_eu_8065F1C8.field_0x00.z + lbl_eu_8065F1C8.field_0x0C.z * lbl_eu_8065F1C8.field_0xD4;
    tmp.x = lbl_eu_8065F1C8.field_0x00.x + lbl_eu_8065F1C8.field_0x0C.x * lbl_eu_8065F1C8.field_0xD4;
    tmp.y = lbl_eu_8065F1C8.field_0x00.y + lbl_eu_8065F1C8.field_0x0C.y * lbl_eu_8065F1C8.field_0xD4;
    const f32 threshold = lbl_eu_8066AEDC;
    u8* triBase = self->field_0x20 + lbl_eu_8065F1C8.field_0xE0->field_0x0A * 18;
    for (int i = 0; i < 3; i++) {
        Vec3* e = (Vec3*)(self->field_0x18 + lbl_eu_8065F1C8.field_0xE0->field_0x04[i] * 12);
        Vec3 d;
        d.x = tmp.x - e->x;
        d.y = tmp.y - e->y;
        d.z = tmp.z - e->z;
        s16* tri = (s16*)(triBase + i * 6);
        lbl_eu_8065F1C8.field_0xA4[i].x = __OSs16tof32(&tri[0]);
        lbl_eu_8065F1C8.field_0xA4[i].y = __OSs16tof32(&tri[1]);
        lbl_eu_8065F1C8.field_0xA4[i].z = __OSs16tof32(&tri[2]);
        f32 dist2 = d.x * lbl_eu_8065F1C8.field_0xA4[i].x
                  + d.y * lbl_eu_8065F1C8.field_0xA4[i].y
                  + d.z * lbl_eu_8065F1C8.field_0xA4[i].z;
        if (dist2 <= threshold) return;
    }
    (self->*self->field_0x94)((const ml::CVec3*)&tmp, (const ml::CVec3*)&lbl_eu_8065F1C8.field_0xC8);
}

// func_804B71CC - resolve the hit point (offset - normal * t) and test it
// against the mover's three vertices: the vertex sample triples are stored
// into the manager's point slot and the loop breaks as soon as any vertex is
// within the acceptance radius.  When a vertex is too close, the closest
// point on the polygon edge (j, j+1) is found by projection and, when the
// offset is within the squared acceptance radius, the scene-resource bit is
// set and the mover callback (ptmf +0x94) is invoked with the closest point
// and the sampled normal.  When all three vertices pass, the callback gets
// the raw hit point.
void func_804B71CC(CColiSrc* a, CColiMover* b) {
    CColiMgr* mgr = &lbl_eu_8065F1C8;
    Vec3 tmp;
    tmp.z = mgr->field_0x00.z - mgr->field_0xC8.z * mgr->field_0xD4;
    tmp.x = mgr->field_0x00.x - mgr->field_0xC8.x * mgr->field_0xD4;
    tmp.y = mgr->field_0x00.y - mgr->field_0xC8.y * mgr->field_0xD4;
    CColiMover* m = mgr->field_0xE0;
    Vec3 d[3];
    Vec3* v[4];
    v[0] = (Vec3*)(a->field_0x18 + m->field_0x04[0] * 12);
    d[0].x = tmp.x - v[0]->x;
    d[0].y = tmp.y - v[0]->y;
    d[0].z = tmp.z - v[0]->z;
    v[1] = (Vec3*)(a->field_0x18 + m->field_0x04[1] * 12);
    d[1].x = tmp.x - v[1]->x;
    d[1].y = tmp.y - v[1]->y;
    d[1].z = tmp.z - v[1]->z;
    v[2] = (Vec3*)(a->field_0x18 + m->field_0x04[2] * 12);
    d[2].x = tmp.x - v[2]->x;
    d[2].y = tmp.y - v[2]->y;
    d[2].z = tmp.z - v[2]->z;
    int i;
    for (i = 0; i < 3; i++) {
        s16* tri = (s16*)(a->field_0x20 + m->field_0x0A * 18 + i * 6);
        mgr->field_0xA4[i].x = __OSs16tof32(&tri[0]);
        mgr->field_0xA4[i].y = __OSs16tof32(&tri[1]);
        mgr->field_0xA4[i].z = __OSs16tof32(&tri[2]);
        f32 dot = d[i].x * mgr->field_0xA4[i].x + d[i].y * mgr->field_0xA4[i].y
                + d[i].z * mgr->field_0xA4[i].z;
        if (dot <= lbl_eu_8066AEE0) break;
    }
    if (i == 3) {
        (a->*a->field_0x94)((const ml::CVec3*)&tmp, (const ml::CVec3*)&mgr->field_0xC8);
        return;
    }
    v[3] = v[0];
    for (int j = 0; j < 3; j++) {
        Vec3 edge;
        edge.x = v[j + 1]->x - v[j]->x;
        edge.y = v[j + 1]->y - v[j]->y;
        edge.z = v[j + 1]->z - v[j]->z;
        f32 denom = edge.x * edge.x + edge.y * edge.y + edge.z * edge.z;
        f32 t = (edge.x * d[j].x + edge.y * d[j].y + edge.z * d[j].z) / denom;
        if (t < lbl_eu_8066AED0) t = lbl_eu_8066AED0;
        if (t > lbl_eu_8066AED4) t = lbl_eu_8066AED4;
        tmp.x = v[j]->x + edge.x * t;
        tmp.y = v[j]->y + edge.y * t;
        tmp.z = v[j]->z + edge.z * t;
        f32 dx = mgr->field_0x00.x - tmp.x;
        f32 dy = mgr->field_0x00.y - tmp.y;
        f32 dz = mgr->field_0x00.z - tmp.z;
        if (dx * dx + dy * dy + dz * dz < mgr->field_0x124) {
            lbl_eu_8066598C |= 1u << lbl_eu_80665988;
            (a->*a->field_0x94)((const ml::CVec3*)&tmp, (const ml::CVec3*)&mgr->field_0xC8);
            return;
        }
    }
}

// Patch a set of relative offsets (embedded sub-object pointers) into
// absolute addresses on the target (+0x50..+0x5C and +0x74..+0x7C).
extern "C" void func_804BC9DC(void*, u8*, u8*);
extern "C" void func_804B74F0(void* self, u8* base) {
    u32* src = (u32*)base;
    u32* dst = (u32*)((u8*)self + 0x50);
    dst[0] = (u32)base + src[0];
    dst[1] = (u32)base + src[1];
    dst[2] = (u32)base + src[2];
    dst[3] = (u32)base + src[3];
    *(u32*)((u8*)self + 0x74) = src[4];
    func_804BC9DC(self, base + src[5], base + src[6]);
}

// func_804B7540 - (re)allocate the mover-state entry table: free the old
// allocation, allocate count * 0xE0 bytes, then fill each entry from the
// 0x3C-byte input records (matrix copy, inverse, previous-matrix copies,
// AABB = translation +/- half-extent).
void func_804B7540(CColiMoverState* self, const CColiInEntry* input, int count) {
    self->field_0x44 = input;
    self->field_0x78 = count;
    if (count == 0) return;
    if (self->field_0x60 != 0) {
        mtl::MemManager::deallocate(self->field_0x60);
        self->field_0x60 = 0;
    }
    self->field_0x60 =
        (u8*)mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM1(), count * 0xE0, 0x20);
    for (int i = 0; i < count; i++) {
        const CColiInEntry* in = &input[i];
        f32 v = in->m[0];
        ((CColiOutEntry*)(self->field_0x60 + i * 0xE0))->mtx.m[0][0] = v;
        v = in->m[1];
        ((CColiOutEntry*)(self->field_0x60 + i * 0xE0))->mtx.m[0][1] = v;
        v = in->m[2];
        ((CColiOutEntry*)(self->field_0x60 + i * 0xE0))->mtx.m[0][2] = v;
        v = in->m[3];
        ((CColiOutEntry*)(self->field_0x60 + i * 0xE0))->mtx.m[0][3] = v;
        v = in->m[4];
        ((CColiOutEntry*)(self->field_0x60 + i * 0xE0))->mtx.m[1][0] = v;
        v = in->m[5];
        ((CColiOutEntry*)(self->field_0x60 + i * 0xE0))->mtx.m[1][1] = v;
        v = in->m[6];
        ((CColiOutEntry*)(self->field_0x60 + i * 0xE0))->mtx.m[1][2] = v;
        v = in->m[7];
        ((CColiOutEntry*)(self->field_0x60 + i * 0xE0))->mtx.m[1][3] = v;
        v = in->m[8];
        ((CColiOutEntry*)(self->field_0x60 + i * 0xE0))->mtx.m[2][0] = v;
        v = in->m[9];
        ((CColiOutEntry*)(self->field_0x60 + i * 0xE0))->mtx.m[2][1] = v;
        v = in->m[10];
        ((CColiOutEntry*)(self->field_0x60 + i * 0xE0))->mtx.m[2][2] = v;
        v = in->m[11];
        ((CColiOutEntry*)(self->field_0x60 + i * 0xE0))->mtx.m[2][3] = v;
        CColiOutEntry* out = (CColiOutEntry*)(self->field_0x60 + i * 0xE0);
        PSMTXInverse(out->mtx.m, out->inv.m);
        out = (CColiOutEntry*)(self->field_0x60 + i * 0xE0);
        out->savedMtx.w[0] = out->mtx.w[0];
        out->savedMtx.w[1] = out->mtx.w[1];
        out->savedMtx.w[2] = out->mtx.w[2];
        out->savedMtx.w[3] = out->mtx.w[3];
        out->savedMtx.w[4] = out->mtx.w[4];
        out->savedMtx.w[5] = out->mtx.w[5];
        out->savedInv.w[0] = out->inv.w[0];
        out->savedInv.w[1] = out->inv.w[1];
        out->savedInv.w[2] = out->inv.w[2];
        out->savedInv.w[3] = out->inv.w[3];
        out->savedInv.w[4] = out->inv.w[4];
        out->savedInv.w[5] = out->inv.w[5];
        out->field_0x04 = lbl_eu_8066AED0;
        out->field_0xC8 = in->m[3] + in->field_0x38;
        out->field_0xCC = in->m[7] + in->field_0x38;
        out->field_0xD0 = in->m[11] + in->field_0x38;
        out->field_0xD4 = in->m[3] - in->field_0x38;
        out->field_0xD8 = in->m[7] - in->field_0x38;
        out->field_0xDC = in->m[11] - in->field_0x38;
        out->field_0x00 = 0;
    }
}

// func_804B7804 - initialise the mover-state object: zero the counters, copy
// the 9-word default configuration table, reset the shared manager
// (identity matrix, active flag, seen-bit window) and free the previous
// allocation.
void func_804B7804(CColiMoverState* self) {
    const u32* s0 = &lbl_eu_8056F508[0];
    const u32* s1 = &lbl_eu_8056F508[3];
    const u32* s2 = &lbl_eu_8056F508[6];
    self->field_0x14 = 0;
    self->field_0x18 = 0;
    self->field_0x1C = 0;
    self->field_0x20 = 0;
    self->field_0x28 = 0;
    self->field_0x2C = 0;
    self->field_0x30 = 0;
    self->field_0x38 = 0;
    self->field_0x3C = 0;
    self->field_0x40 = 0;
    self->field_0x44 = 0;
    self->field_0x6C = 0;
    self->field_0x70 = 0;
    self->field_0x78 = 0;
    self->field_0x7C = s0[0];
    self->field_0x80 = s0[1];
    self->field_0x84 = s0[2];
    self->field_0x88 = s1[0];
    self->field_0x8C = s1[1];
    self->field_0x90 = s1[2];
    self->field_0x94 = s2[0];
    self->field_0x98 = s2[1];
    self->field_0x9C = s2[2];
    self->field_0x5C = 0;
    func_804BAE10(self);
    PSMTXIdentity(lbl_eu_8065F1C8.field_0xF0);
    lbl_eu_8065F1C8.field_0x134 = 1;
    memset(lbl_eu_8065F1A0, 0, 0x28);
    if (self->field_0x60 != 0) {
        mtl::MemManager::deallocate(self->field_0x60);
        self->field_0x60 = 0;
    }
}

// func_804B791C: forward the entry pointer + value to the +0xAC register
// callback member pointer (MWCC lowers the call to `addi r12,self,0xAC; bl
// __ptmf_scall` with the args passed through untouched).
void func_804B791C(CColiRegObj* self, u16* entry, u16 value) {
    (self->*self->field_0xAC)(entry, value);
}

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

// func_804B7A00 - like func_804B7944, but the entry's category value must
// also pass the manager's second acceptance flag (0x12C) before the register
// callback fires.
void func_804B7A00(CColiRegObj* self, u16* list, int count) {
    for (int i = 0; i < count; i++, list++) {
        u16 idx = *list;
        u32 wordIdx = idx >> 5;
        u32 bitIdx = idx & 0x1F;
        u32 w = ((u32*)lbl_eu_80663AC8)[wordIdx];
        u32 mask = 1u << bitIdx;
        if (w & mask) continue;
        ((u32*)lbl_eu_80663AC8)[wordIdx] = w | mask;
        u32 v = self->field_0x28[(self->field_0x30 + *list)->field_0x12];
        if (lbl_eu_8065F1C8.field_0x128 & v) continue;
        if ((lbl_eu_8065F1C8.field_0x12C & v) == 0) continue;
        (self->*self->field_0x7C)();
    }
}

void func_804B7AC8(void) {}

void func_804B7ACC(){}

// 16-byte raycast entry indexed from CColiObj2::field_0x5C.
struct CColiEntry16 {
    f32 field_0x00;  // 0x00 - plane offset
    u16 field_0x04;  // 0x04
    u16 field_0x06;  // 0x06 - second segment vertex index (field_0x50)
    u16 field_0x08;  // 0x08
    u16 field_0x0A;  // 0x0A - third segment vertex index (field_0x50)
    u16 field_0x0C;  // 0x0C - matrix record index (field_0x58)
    u16 field_0x0E;  // 0x0E - ray origin vertex index (field_0x54)
};

// 48-byte matrix record: four direction vectors (0x30-byte stride table).
struct CColiMtx4 {
    Vec3 v[4];
};

// Raycast-test object: two vertex tables, a 0x30-stride matrix table and
// the 16-byte entry table.
struct CColiObj2 {
    u8 field_0x00[0x50];           // 0x00
    Vec3* field_0x50;              // 0x50 - segment vertex table (12-byte stride)
    Vec3* field_0x54;              // 0x54 - ray vertex table (12-byte stride)
    CColiMtx4* field_0x58;         // 0x58 - matrix table (0x30-byte stride)
    CColiEntry16* field_0x5C;      // 0x5C - entry table (16-byte stride)
};

// func_804B7B6C - raycast against a box: reserve the entry's seen-bit,
// intersect the ray (manager offset 0x00 / direction 0x0C) with the entry's
// plane and vertex, and accept when the hit point passes the four
// plane-distance tests of the two segments (t in [0, 0x8066AED4]).
bool func_804B7B6C(CColiObj2* self, const u16* list, int count) {
    for (int i = 0; i < count; i++) {
        u16 idx = list[i];
        u32* bits = (u32*)lbl_eu_80663ACC;
        u32 wordIdx = idx >> 5;
        u32 bitIdx = idx & 0x1F;
        u32 w = bits[wordIdx];
        u32 mask = 1u << bitIdx;
        if (w & mask) continue;
        bits[wordIdx] = w | mask;
        CColiEntry16* entry = &self->field_0x5C[idx];
        Vec3* vert = &self->field_0x54[entry->field_0x0E];
        f32 dot2 = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)vert,
                                       (const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0x0C);
        if (dot2 == lbl_eu_8066AED0) continue;
        f32 t = (entry->field_0x00
                 - nw4r::math::VEC3Dot((const nw4r::math::VEC3*)vert,
                                       (const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0x00))
                / dot2;
        if (!(t >= lbl_eu_8066AED0)) continue;
        if (!(t <= lbl_eu_8066AED4)) continue;
        ml::CVec3 hit;
        hit.y = lbl_eu_8065F1C8.field_0x00.y + lbl_eu_8065F1C8.field_0x0C.y * t;
        hit.x = lbl_eu_8065F1C8.field_0x00.x + lbl_eu_8065F1C8.field_0x0C.x * t;
        hit.z = lbl_eu_8065F1C8.field_0x00.z + lbl_eu_8065F1C8.field_0x0C.z * t;
        Vec3* v2 = &self->field_0x50[entry->field_0x06];
        ml::CVec3 seg;
        nw4r::math::VEC3Sub((nw4r::math::VEC3*)&seg, (const nw4r::math::VEC3*)&hit,
                            (const nw4r::math::VEC3*)v2);
        CColiMtx4* m2 = &self->field_0x58[entry->field_0x0C];
        if (nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&seg,
                                (const nw4r::math::VEC3*)&m2->v[0]) <= lbl_eu_8066AEE0)
            continue;
        if (nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&seg,
                                (const nw4r::math::VEC3*)&m2->v[1]) <= lbl_eu_8066AEE0)
            continue;
        Vec3* v3 = &self->field_0x50[entry->field_0x0A];
        nw4r::math::VEC3Sub((nw4r::math::VEC3*)&seg, (const nw4r::math::VEC3*)&hit,
                            (const nw4r::math::VEC3*)v3);
        if (nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&seg,
                                (const nw4r::math::VEC3*)&m2->v[2]) <= lbl_eu_8066AEE0)
            continue;
        if (nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&seg,
                                (const nw4r::math::VEC3*)&m2->v[3]) <= lbl_eu_8066AEE0)
            continue;
        return true;
    }
    return false;
}

extern char lbl_eu_8065F1A0[];
void func_804B7D9C(int, int bit_index) {
    if (bit_index > 0x140) bit_index = 0x140;
    int word_idx = bit_index >> 5;
    int bit_off = bit_index & 0x1F;
    int mask = 1 << bit_off;
    int* bits = (int*)&lbl_eu_8065F1A0;
    bits[word_idx] &= ~mask;
}

void func_804B7DD4(int, int bit_index) {
    if (bit_index > 0x140) bit_index = 0x140;
    int word_idx = bit_index >> 5;
    int bit_off = bit_index & 0x1F;
    int mask = 1 << bit_off;
    int* bits = (int*)&lbl_eu_8065F1A0;
    bits[word_idx] |= mask;
}

// func_804B7E0C - update one mover-state entry: save the current matrix and
// inverse into the previous slots, install the new 48-byte matrix block,
// recompute the inverse, then expand the AABB to cover the movement between
// the previous and new translations plus/minus the entry half-extent.
void func_804B7E0C(CColiMoverState* self, int idx, const u32* data) {
    if (self->field_0x78 == 0) return;
    CColiOutEntry* out = (CColiOutEntry*)(self->field_0x60 + idx * 0xE0);
    const CColiInEntry* in = &self->field_0x44[idx];
    out->field_0x00 |= 2;
    {
        u32 lo, hi;
        lo = out->mtx.u[1];
        hi = out->mtx.u[0];
        out->savedMtx.u[0] = hi;
        out->savedMtx.u[1] = lo;
        lo = out->mtx.u[3];
        hi = out->mtx.u[2];
        out->savedMtx.u[2] = hi;
        out->savedMtx.u[3] = lo;
        lo = out->mtx.u[5];
        hi = out->mtx.u[4];
        out->savedMtx.u[4] = hi;
        out->savedMtx.u[5] = lo;
        lo = out->mtx.u[7];
        hi = out->mtx.u[6];
        out->savedMtx.u[6] = hi;
        out->savedMtx.u[7] = lo;
        lo = out->mtx.u[9];
        hi = out->mtx.u[8];
        out->savedMtx.u[8] = hi;
        out->savedMtx.u[9] = lo;
        lo = out->mtx.u[11];
        hi = out->mtx.u[10];
        out->savedMtx.u[10] = hi;
        out->savedMtx.u[11] = lo;
        lo = out->inv.u[1];
        hi = out->inv.u[0];
        out->savedInv.u[0] = hi;
        out->savedInv.u[1] = lo;
        lo = out->inv.u[3];
        hi = out->inv.u[2];
        out->savedInv.u[2] = hi;
        out->savedInv.u[3] = lo;
        lo = out->inv.u[5];
        hi = out->inv.u[4];
        out->savedInv.u[4] = hi;
        out->savedInv.u[5] = lo;
        lo = out->inv.u[7];
        hi = out->inv.u[6];
        out->savedInv.u[6] = hi;
        out->savedInv.u[7] = lo;
        lo = out->inv.u[9];
        hi = out->inv.u[8];
        out->savedInv.u[8] = hi;
        out->savedInv.u[9] = lo;
        lo = out->inv.u[11];
        hi = out->inv.u[10];
        out->savedInv.u[10] = hi;
        out->savedInv.u[11] = lo;
    }
    out->mtx.u[0] = data[0];
    out->mtx.u[1] = data[1];
    out->mtx.u[2] = data[2];
    out->mtx.u[3] = data[3];
    out->mtx.u[4] = data[4];
    out->mtx.u[5] = data[5];
    out->mtx.u[6] = data[6];
    out->mtx.u[7] = data[7];
    out->mtx.u[8] = data[8];
    out->mtx.u[9] = data[9];
    out->mtx.u[10] = data[10];
    out->mtx.u[11] = data[11];
    PSMTXInverse(out->mtx.m, out->inv.m);
    if (out->savedMtx.m[0][3] > out->mtx.m[0][3]) {
        out->field_0xC8 = out->savedMtx.m[0][3] + in->field_0x38;
        out->field_0xD4 = out->mtx.m[0][3] - in->field_0x38;
    } else {
        out->field_0xC8 = out->mtx.m[0][3] + in->field_0x38;
        out->field_0xD4 = out->savedMtx.m[0][3] - in->field_0x38;
    }
    if (out->savedMtx.m[1][3] > out->mtx.m[1][3]) {
        out->field_0xCC = out->savedMtx.m[1][3] + in->field_0x38;
        out->field_0xD8 = out->mtx.m[1][3] - in->field_0x38;
    } else {
        out->field_0xCC = out->mtx.m[1][3] + in->field_0x38;
        out->field_0xD8 = out->savedMtx.m[1][3] - in->field_0x38;
    }
    if (out->savedMtx.m[2][3] > out->mtx.m[2][3]) {
        out->field_0xD0 = out->savedMtx.m[2][3] + in->field_0x38;
        out->field_0xDC = out->mtx.m[2][3] - in->field_0x38;
    } else {
        out->field_0xD0 = out->mtx.m[2][3] + in->field_0x38;
        out->field_0xDC = out->savedMtx.m[2][3] - in->field_0x38;
    }
}

// func_804B8078: when the mover state is active, align the entry's flag word
// to 4 and store the pooled float constant at +4.
void func_804B8078(CColiMoverState* self, int idx) {
    if (self->field_0x78 == 0) {
        return;
    }
    CColiOutEntry* e = &reinterpret_cast<CColiOutEntry*>(self->field_0x60)[idx];
    // Retail emits the wrap-mask rlwinm r0,r0,0,31,29 (0xFFFFFFFC); MWCC's
    // u32 mask lowers to the normal MB=0,ME=29 encoding, so the approved
    // DECOMP_PPC_RLWINM intrinsic (PLAN.md 17.6) selects the retail opcode.
    e->field_0x00 = DECOMP_PPC_RLWINM(e->field_0x00, 0, 31, 29);
    e->field_0x04 = lbl_eu_8066AED0;
}

// func_804B80A4: when the mover state is active (field_0x78), return a
// pointer to the mover-state entry's matrix at +8 (0xE0 stride); else null.
void* func_804B80A4(CColiMoverState* self, int idx) {
    if (self->field_0x78 != 0) {
        return &reinterpret_cast<CColiOutEntry*>(self->field_0x60)[idx].mtx;
    }
    return 0;
}

// Scan the input entry array (+0x44, 0x3C stride) for the entry whose high
// halfword matches id, returning its index (retail counted loop with bdnz;
// -O4,p strength-reduces the count to addic./bne). Signed count keeps the
// loop pre-check on the gate-1 cmpi so the extra cmpli is CSE'd away.
#pragma optimize_for_size on
int func_804B80CC(CColiMoverState* self, u16 id) {
    int count = (int)self->field_0x78;
    if (count != 0) {
        const CColiInEntry* e = self->field_0x44;
        for (int i = 0; i < count; i++, e++) {
            if ((int)((u16*)&e->field_0x00)[1] == (int)id) return i;
        }
    }
    return 0;
}
#pragma optimize_for_size off

// func_804B8108 - build the six OBB plane definitions from a rotation
// matrix and the manager's reference point: the three unit axes are
// transformed by the matrix (normal transform), normalised (falling back to
// ml::CVec3::zero for null axes), then stored as the plane normals at
// 0x3C/0x48/0x54/0x60/0x6C/0x78 (+/- each axis, flipped by 0x8066AED8) with
// the plane offsets at 0x84..0x98 computed as (center +/- axis*scale).axis.
void func_804B8108(CColiMoverState* unused, const Vec* center, const Mtx mtx, f32 scaleV2, f32 scaleV1, f32 scaleV0) {
    CColiMgr* mgr = &lbl_eu_8065F1C8;
    const f32 zero = lbl_eu_8066AED0;
    const f32 flip = lbl_eu_8066AED8;
    Vec3 v0;
    Vec3 v1;
    Vec3 v2;
    v2.x = zero;
    v2.y = flip;
    v2.z = zero;
    v1.x = flip;
    v1.y = zero;
    v1.z = zero;
    v0.x = zero;
    v0.y = zero;
    v0.z = flip;
    nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&v0, (const nw4r::math::MTX34*)mtx,
                                    (const nw4r::math::VEC3*)&v0);
    nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&v1, (const nw4r::math::MTX34*)mtx,
                                    (const nw4r::math::VEC3*)&v1);
    nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&v2, (const nw4r::math::MTX34*)mtx,
                                    (const nw4r::math::VEC3*)&v2);
    if (zero == (v0.x * v0.x + v0.y * v0.y + v0.z * v0.z)) {
        v0 = *(const Vec3*)&ml::CVec3::zero;
    } else {
        PSVECNormalize((const Vec*)&v0, (Vec*)&v0);
    }
    if (zero == (v1.x * v1.x + v1.y * v1.y + v1.z * v1.z)) {
        v1 = *(const Vec3*)&ml::CVec3::zero;
    } else {
        PSVECNormalize((const Vec*)&v1, (Vec*)&v1);
    }
    if (zero == (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z)) {
        v2 = *(const Vec3*)&ml::CVec3::zero;
    } else {
        PSVECNormalize((const Vec*)&v2, (Vec*)&v2);
    }
    mgr->field_0x3C[0] = *(const nw4r::math::_VEC3*)&v1;
    mgr->field_0x3C[1].x = v1.x * flip;
    mgr->field_0x3C[1].y = v1.y * flip;
    mgr->field_0x3C[1].z = v1.z * flip;
    mgr->field_0x3C[2] = *(const nw4r::math::_VEC3*)&v2;
    mgr->field_0x3C[3].x = v2.x * flip;
    mgr->field_0x3C[3].y = v2.y * flip;
    mgr->field_0x3C[3].z = v2.z * flip;
    mgr->field_0x3C[4] = *(const nw4r::math::_VEC3*)&v0;
    mgr->field_0x3C[5].x = v0.x * flip;
    mgr->field_0x3C[5].y = v0.y * flip;
    mgr->field_0x3C[5].z = v0.z * flip;
    mgr->field_0x84[0] = (center->x + v1.x * flip * scaleV1) * mgr->field_0x3C[0].x
                        + (center->y + v1.y * flip * scaleV1) * mgr->field_0x3C[0].y
                        + (center->z + v1.z * flip * scaleV1) * mgr->field_0x3C[0].z;
    mgr->field_0x84[1] = (center->x + v1.x * scaleV1) * mgr->field_0x3C[1].x
                        + (center->y + v1.y * scaleV1) * mgr->field_0x3C[1].y
                        + (center->z + v1.z * scaleV1) * mgr->field_0x3C[1].z;
    mgr->field_0x84[2] = (center->x + v2.x * flip * scaleV2) * mgr->field_0x3C[2].x
                        + (center->y + v2.y * flip * scaleV2) * mgr->field_0x3C[2].y
                        + (center->z + v2.z * flip * scaleV2) * mgr->field_0x3C[2].z;
    mgr->field_0x84[3] = (center->x + v2.x * scaleV2) * mgr->field_0x3C[3].x
                        + (center->y + v2.y * scaleV2) * mgr->field_0x3C[3].y
                        + (center->z + v2.z * scaleV2) * mgr->field_0x3C[3].z;
    mgr->field_0x84[4] = (center->x + v0.x * scaleV0) * mgr->field_0x3C[4].x
                        + (center->y + v0.y * scaleV0) * mgr->field_0x3C[4].y
                        + (center->z + v0.z * scaleV0) * mgr->field_0x3C[4].z;
    mgr->field_0x84[5] = (center->x + v0.x * flip * scaleV0) * mgr->field_0x3C[5].x
                        + (center->y + v0.y * flip * scaleV0) * mgr->field_0x3C[5].y
                        + (center->z + v0.z * flip * scaleV0) * mgr->field_0x3C[5].z;
}

// func_804B85C8 - register a collision volume: reject when its bit is
// already set in the seen table or when its AABB overlaps the manager's
// AABB; otherwise invalidate the seen-bit window, copy the volume's 3x3
// rotation into the manager's embedded matrix and update the hook pointers.
u32 func_804B85C8(CColiThing* self, CColiVolume* vol, CColiQuery* query, u32 arg6) {
    u16 flags = query->field_0x02;
    u32* bits = (u32*)&lbl_eu_8065F1A0;
    if (bits[flags >> 5] & (1u << (flags & 0x1F))) return 0;
    if (vol->field_0xD4 > lbl_eu_8065F1C8.field_0x24.x) return 0;
    if (vol->field_0xC8 < lbl_eu_8065F1C8.field_0x30.x) return 0;
    if (vol->field_0xDC > lbl_eu_8065F1C8.field_0x24.z) return 0;
    if (vol->field_0xD0 < lbl_eu_8065F1C8.field_0x30.z) return 0;
    if (vol->field_0xD8 > lbl_eu_8065F1C8.field_0x24.y) return 0;
    if (vol->field_0xCC < lbl_eu_8065F1C8.field_0x30.y) return 0;
    CColiEntry12* entry = &self->field_0x38[query->field_0x00];
    DCZeroRange((u8*)lbl_eu_80663AC8, ((entry->field_0x02 >> 5) + 1) * 4);
    lbl_eu_8065F1C8.field_0xF0[0][0] = vol->field_0x08[0][0];
    lbl_eu_8065F1C8.field_0xF0[0][1] = vol->field_0x08[0][1];
    lbl_eu_8065F1C8.field_0xF0[0][2] = vol->field_0x08[0][2];
    lbl_eu_8065F1C8.field_0xF0[1][0] = vol->field_0x08[1][0];
    lbl_eu_8065F1C8.field_0xF0[1][1] = vol->field_0x08[1][1];
    lbl_eu_8065F1C8.field_0xF0[1][2] = vol->field_0x08[1][2];
    lbl_eu_8065F1C8.field_0xF0[2][0] = vol->field_0x08[2][0];
    lbl_eu_8065F1C8.field_0xF0[2][1] = vol->field_0x08[2][1];
    lbl_eu_8065F1C8.field_0xF0[2][2] = vol->field_0x08[2][2];
    self->field_0x30 = self->field_0x14 + entry->field_0x08;
    lbl_eu_8065F1C8.field_0xE8 = &vol->field_0x08;
    lbl_eu_8065F1C8.field_0xEC = (u32)&vol->field_0x38;
    lbl_eu_8065F1C8.field_0x130 = arg6 + 1;
    return 1;
}

// func_804B877C - like func_804B9E14 but the initial query either uses the
// linked object's own matrix (when manager field_0x134 is set) or swaps in
// the default config table's matrix for the query and restores it after, and
// the per-stage matrix configuration is read from the low block of the
// default table.  Same GQR5 fast-cast wall as func_804B9E14.
void func_804B877C(CColiMoverState* state, const Vec* offset, u32 flags, int mode, f32 scale) {
    CColiMgr* mgr = &lbl_eu_8065F1C8;
    mgr->field_0x00 = *(const nw4r::math::_VEC3*)offset;
    mgr->field_0x18 = *(const nw4r::math::_VEC3*)offset;
    mgr->field_0x120 = scale;
    mgr->field_0x124 = scale * scale;
    state->field_0x7C = lbl_eu_8056F508[0x24 / 4];
    state->field_0x80 = lbl_eu_8056F508[0x28 / 4];
    state->field_0x84 = lbl_eu_8056F508[0x2C / 4];
    state->field_0x94 = lbl_eu_8056F508[0x30 / 4];
    state->field_0x98 = lbl_eu_8056F508[0x34 / 4];
    state->field_0x9C = lbl_eu_8056F508[0x38 / 4];
    state->field_0x88 = lbl_eu_8056F508[0x3C / 4];
    state->field_0x8C = lbl_eu_8056F508[0x40 / 4];
    state->field_0x90 = lbl_eu_8056F508[0x44 / 4];
    mgr->field_0x128 = flags;
    mgr->field_0x12C = mode;
    if (mode != 0) {
        state->field_0xAC = lbl_eu_8056F508[0x48 / 4];
        state->field_0xB0 = lbl_eu_8056F508[0x4C / 4];
        state->field_0xB4 = lbl_eu_8056F508[0x50 / 4];
    } else {
        state->field_0xAC = lbl_eu_8056F508[0x54 / 4];
        state->field_0xB0 = lbl_eu_8056F508[0x58 / 4];
        state->field_0xB4 = lbl_eu_8056F508[0x5C / 4];
    }
    DCZeroRange((u8*)lbl_eu_80663AC8,
                (((s32)(state->field_0x6C + state->field_0x70) >> 5) + 1) * 4);
    lbl_eu_80665988 = 0;
    lbl_eu_8066598C = 0;
    lbl_eu_80665990 = 0;
    mgr->field_0x136 = 0;
    mgr->field_0xEC = 0;
    mgr->field_0x130 = 0;
    if (func_804BC9A0(state) != 0) {
        if (mgr->field_0x134 == 0) {
            u32 ac = state->field_0xAC;
            u32 b0 = state->field_0xB0;
            u32 b4 = state->field_0xB4;
            state->field_0xAC = lbl_eu_8056F508[0x60 / 4];
            state->field_0xB0 = lbl_eu_8056F508[0x64 / 4];
            state->field_0xB4 = lbl_eu_8056F508[0x68 / 4];
            func_804BBFA0(state, offset, state->field_0x34->field_0x04, scale);
            state->field_0xAC = ac;
            state->field_0xB0 = b0;
            state->field_0xB4 = b4;
        } else {
            func_804BBFA0(state, offset, state->field_0x34->field_0x04, scale);
        }
        if (mgr->field_0x136 != 0 || state->field_0x78 != 0) {
            state->field_0xA0 = lbl_eu_8056F508[0x6C / 4];
            state->field_0xA4 = lbl_eu_8056F508[0x70 / 4];
            state->field_0xA8 = lbl_eu_8056F508[0x74 / 4];
            for (int i = 0; i < mgr->field_0x136; i++) {
                CColiVolEntry* entry = &state->field_0x40[lbl_eu_8065F300[i]];
                if ((entry->field_0x02 & 0x8000) != 0) {
                    state->field_0x94 = lbl_eu_8056F508[0x78 / 4];
                    state->field_0x98 = lbl_eu_8056F508[0x7C / 4];
                    state->field_0x9C = lbl_eu_8056F508[0x80 / 4];
                } else {
                    state->field_0x94 = lbl_eu_8056F508[0x84 / 4];
                    state->field_0x98 = lbl_eu_8056F508[0x88 / 4];
                    state->field_0x9C = lbl_eu_8056F508[0x8C / 4];
                }
                mgr->field_0x120 = scale * entry->field_0x7C;
                mgr->field_0x124 = mgr->field_0x120 * mgr->field_0x120;
                PSMTXMultVec(entry->field_0x34, offset, (Vec*)&mgr->field_0x00);
                CColiEntry12* e12 = &state->field_0x38[entry->field_0x00];
                DCZeroRange((u8*)lbl_eu_80663AC8, ((e12->field_0x02 >> 5) + 1) * 4);
                mgr->field_0xF0[0][0] = entry->m[0][0];
                mgr->field_0xF0[0][1] = entry->m[0][1];
                mgr->field_0xF0[0][2] = entry->m[0][2];
                mgr->field_0xF0[1][0] = entry->m[1][0];
                mgr->field_0xF0[1][1] = entry->m[1][1];
                mgr->field_0xF0[1][2] = entry->m[1][2];
                mgr->field_0xF0[2][0] = entry->m[2][0];
                mgr->field_0xF0[2][1] = entry->m[2][1];
                mgr->field_0xF0[2][2] = entry->m[2][2];
                state->field_0x30 = state->field_0x14 + e12->field_0x08;
                mgr->field_0xE8 = (Mtx*)&entry->m[0][0];
                mgr->field_0xEC = (u32)&entry->field_0x34;
                func_804BBFA0(state, offset, e12->field_0x04, mgr->field_0x120);
            }
            if (state->field_0x78 != 0) {
                state->field_0x94 = lbl_eu_8056F508[0x90 / 4];
                state->field_0x98 = lbl_eu_8056F508[0x94 / 4];
                state->field_0x9C = lbl_eu_8056F508[0x98 / 4];
                mgr->field_0x24.x = offset->x + scale;
                mgr->field_0x24.y = offset->y + scale;
                mgr->field_0x24.z = offset->z + scale;
                mgr->field_0x30.x = offset->x - scale;
                mgr->field_0x30.y = offset->y - scale;
                mgr->field_0x30.z = offset->z - scale;
                for (int i = 0; i < state->field_0x78; i++) {
                    CColiOutEntry* out = (CColiOutEntry*)(state->field_0x60 + i * 0xE0);
                    const CColiInEntry* in = &state->field_0x44[i];
                    if (func_804B85C8((CColiThing*)state, (CColiVolume*)out, (CColiQuery*)in, i) != 0) {
                        mgr->field_0x120 = scale * in->field_0x34;
                        mgr->field_0x124 = mgr->field_0x120 * mgr->field_0x120;
                        PSMTXMultVec(out->inv.m, offset, (Vec*)&mgr->field_0x00);
                        CColiEntry12* e12 = &state->field_0x38[in->field_0x00];
                        func_804BBFA0(state, offset, e12->field_0x04, mgr->field_0x120);
                    }
                }
            }
            state->field_0x30 = state->field_0x2C;
        }
    }
}

// func_804B8C2C - register a segment (a -> b): store `a` into the manager's
// reference position, the direction `b - a` into the reference direction and
// the distance threshold, install the per-stage matrix config, then walk the
// scene-resource volumes and the mover-state entries, casting each volume's
// matrix along the segment.  The AABB stored at the end is the per-component
// min/max of the two endpoints.  Void.
void func_804B8C2C(CColiMoverState* self, const Vec* a, const Vec* b, u32 flags128,
                   u32 flags12C, u32 mode) {
    CColiMgr* mgr = &lbl_eu_8065F1C8;
    const u32* tbl = lbl_eu_8056F508;
    {
        u32 ax = *(const u32*)&a->x;
        u32 ay = *(const u32*)&a->y;
        u32 az = *(const u32*)&a->z;
        *(u32*)&mgr->field_0x00.x = ax;
        *(u32*)&mgr->field_0x00.y = ay;
        *(u32*)&mgr->field_0x00.z = az;
        nw4r::math::VEC3 diff;
        nw4r::math::VEC3Sub(&diff, (const nw4r::math::VEC3*)b, (const nw4r::math::VEC3*)a);
        Vec3 d2;
        d2.x = diff.x;
        d2.y = diff.y;
        d2.z = diff.z;
        *(u32*)&mgr->field_0x0C.x = *(u32*)&d2.x;
        *(u32*)&mgr->field_0x0C.y = *(u32*)&d2.y;
        *(u32*)&mgr->field_0x0C.z = *(u32*)&d2.z;
        mgr->field_0xD8 = lbl_eu_8066AEE4;
        *(u32*)&mgr->field_0x18.x = ax;
        *(u32*)&mgr->field_0x18.y = ay;
        *(u32*)&mgr->field_0x18.z = az;
    }
    {
        const u32* g0 = &tbl[0x9C / 4];
        self->field_0x7C = g0[0];
        self->field_0x80 = g0[1];
        self->field_0x84 = g0[2];
        const u32* g1 = &tbl[0xA8 / 4];
        self->field_0x88 = g1[0];
        self->field_0x8C = g1[1];
        self->field_0x90 = g1[2];
    }
    if (mode != 0) {
        const u32* cfg = &tbl[0xB4 / 4];
        self->field_0x94 = cfg[0];
        self->field_0x98 = cfg[1];
        self->field_0x9C = cfg[2];
    } else {
        const u32* cfg = &tbl[0xC0 / 4];
        self->field_0x94 = cfg[0];
        self->field_0x98 = cfg[1];
        self->field_0x9C = cfg[2];
    }
    mgr->field_0x128 = flags128;
    mgr->field_0x12C = flags12C;
    if (flags12C != 0) {
        const u32* cfg = &tbl[0xCC / 4];
        self->field_0xAC = cfg[0];
        self->field_0xB0 = cfg[1];
        self->field_0xB4 = cfg[2];
    } else {
        const u32* cfg = &tbl[0xD8 / 4];
        self->field_0xAC = cfg[0];
        self->field_0xB0 = cfg[1];
        self->field_0xB4 = cfg[2];
    }
    DCZeroRange((u8*)lbl_eu_80663AC8,
                (((s32)(self->field_0x6C + self->field_0x70) >> 5) + 1) * 4);
    lbl_eu_80665988 = 0;
    lbl_eu_8066598C = 0;
    lbl_eu_80665990 = 0;
    mgr->field_0x136 = 0;
    mgr->field_0xEC = 0;
    mgr->field_0x130 = 0;
    if (func_804BC9A0(self) == 0) return;
    func_804BBFD4(self, a, b, self->field_0x34->field_0x04);
    if (mgr->field_0x136 == 0 && self->field_0x78 == 0) return;
    if (mode != 0) {
        const u32* cfg = &tbl[0xE4 / 4];
        self->field_0xA0 = cfg[0];
        self->field_0xA4 = cfg[1];
        self->field_0xA8 = cfg[2];
    } else {
        const u32* cfg = &tbl[0xF0 / 4];
        self->field_0xA0 = cfg[0];
        self->field_0xA4 = cfg[1];
        self->field_0xA8 = cfg[2];
    }
    int resCount = mgr->field_0x136;
    if (resCount != 0) {
        for (int i = 0; i < resCount; i++) {
            CColiVolEntry* vol = &self->field_0x40[lbl_eu_8065F300[i]];
            if (vol->field_0x02 & 0x8000) {
                const u32* cfg = &tbl[0xFC / 4];
                self->field_0x94 = cfg[0];
                self->field_0x98 = cfg[1];
                self->field_0x9C = cfg[2];
            } else {
                const u32* cfg = &tbl[0x108 / 4];
                self->field_0x94 = cfg[0];
                self->field_0x98 = cfg[1];
                self->field_0x9C = cfg[2];
            }
            Vec3 v3;
            PSMTXMultVec(vol->field_0x34, a, (Vec*)&mgr->field_0x00);
            PSMTXMultVec(vol->field_0x34, b, (Vec*)&v3);
            {
                nw4r::math::VEC3 diff;
                nw4r::math::VEC3Sub(&diff, (const nw4r::math::VEC3*)&v3,
                                    (const nw4r::math::VEC3*)&mgr->field_0x00);
                Vec3 d2;
                d2.x = diff.x;
                d2.y = diff.y;
                d2.z = diff.z;
                *(u32*)&mgr->field_0x0C.x = *(u32*)&d2.x;
                *(u32*)&mgr->field_0x0C.y = *(u32*)&d2.y;
                *(u32*)&mgr->field_0x0C.z = *(u32*)&d2.z;
            }
            CColiEntry12* e12 = &self->field_0x38[vol->field_0x00];
            DCZeroRange((u8*)lbl_eu_80663AC8, ((e12->field_0x02 >> 5) + 1) * 4);
            mgr->field_0xF0[0][0] = vol->m[0][0];
            mgr->field_0xF0[0][1] = vol->m[0][1];
            mgr->field_0xF0[0][2] = vol->m[0][2];
            mgr->field_0xF0[1][0] = vol->m[1][0];
            mgr->field_0xF0[1][1] = vol->m[1][1];
            mgr->field_0xF0[1][2] = vol->m[1][2];
            mgr->field_0xF0[2][0] = vol->m[2][0];
            mgr->field_0xF0[2][1] = vol->m[2][1];
            mgr->field_0xF0[2][2] = vol->m[2][2];
            self->field_0x30 = self->field_0x14 + e12->field_0x08;
            mgr->field_0xE8 = (Mtx*)&vol->m[0][0];
            mgr->field_0xEC = (u32)&vol->field_0x34;
            func_804BBFD4(self, (const Vec*)&mgr->field_0x00, (const Vec*)&v3,
                          e12->field_0x04);
        }
    }
    if (self->field_0x78 != 0) {
        const u32* cfg = &tbl[0x114 / 4];
        self->field_0x94 = cfg[0];
        self->field_0x98 = cfg[1];
        self->field_0x9C = cfg[2];
        if (a->x > b->x) {
            mgr->field_0x24.x = a->x;
            mgr->field_0x30.x = b->x;
        } else {
            mgr->field_0x24.x = b->x;
            mgr->field_0x30.x = a->x;
        }
        if (a->y > b->y) {
            mgr->field_0x24.y = a->y;
            mgr->field_0x30.y = b->y;
        } else {
            mgr->field_0x24.y = b->y;
            mgr->field_0x30.y = a->y;
        }
        if (a->z > b->z) {
            mgr->field_0x24.z = a->z;
            mgr->field_0x30.z = b->z;
        } else {
            mgr->field_0x24.z = b->z;
            mgr->field_0x30.z = a->z;
        }
        CColiOutEntry* out = (CColiOutEntry*)self->field_0x60;
        const CColiInEntry* in = self->field_0x44;
        int i = 0;
        while (i < (s32)self->field_0x78) {
            if (func_804B85C8((CColiThing*)self, (CColiVolume*)out, (CColiQuery*)in, i) != 0) {
                Vec3 v4;
                PSMTXMultVec(out->savedMtx.m, a, (Vec*)&mgr->field_0x00);
                PSMTXMultVec(out->savedMtx.m, b, (Vec*)&v4);
                {
                    nw4r::math::VEC3 diff;
                    nw4r::math::VEC3Sub(&diff, (const nw4r::math::VEC3*)&v4,
                                        (const nw4r::math::VEC3*)&mgr->field_0x00);
                    Vec3 d2;
                    d2.x = diff.x;
                    d2.y = diff.y;
                    d2.z = diff.z;
                    *(u32*)&mgr->field_0x0C.x = *(u32*)&d2.x;
                    *(u32*)&mgr->field_0x0C.y = *(u32*)&d2.y;
                    *(u32*)&mgr->field_0x0C.z = *(u32*)&d2.z;
                }
                func_804BBFD4(self, (const Vec*)&mgr->field_0x00, (const Vec*)&v4,
                              self->field_0x38[((CColiQuery*)in)->field_0x00].field_0x04);
            }
            i++;
            out++;
            in++;
        }
    }
    self->field_0x30 = self->field_0x2C;
}

// func_804B91E0 - begin a registration pass from a vertical query: store the
// source point and Y extent into the manager, install the per-stage matrix
// config, then walk the scene-resource volumes (0x80-byte stride) and the
// mover-state entries, casting each volume's matrix along the manager's
// current reference.  The resource loop is gated on the registered-index
// count; the mover loop on the entry count.  Void.
void func_804B91E0(CColiMoverState* self, const Vec* src, u32 flags128, u32 flags12C,
                   u32 mode, f32 yOff, f32 yRange) {
    CColiMgr* mgr = &lbl_eu_8065F1C8;
    const u32* tbl = lbl_eu_8056F508;
    const u32* g0 = &tbl[0x1A4 / 4];
    const u32* g1 = &tbl[0x1B0 / 4];
    const u32* g2 = &tbl[0x1BC / 4];
    {
        u32 sx = *(const u32*)&src->x;
        u32 sy = *(const u32*)&src->y;
        u32 sz = *(const u32*)&src->z;
        *(u32*)&mgr->field_0x00.x = sx;
        *(u32*)&mgr->field_0x00.y = sy;
        *(u32*)&mgr->field_0x00.z = sz;
        mgr->field_0x0C.x = lbl_eu_8066AED0;
        mgr->field_0x0C.y = yOff;
        mgr->field_0x0C.z = lbl_eu_8066AED0;
        *(u32*)&mgr->field_0x18.x = sx;
        *(u32*)&mgr->field_0x18.y = sy;
        *(u32*)&mgr->field_0x18.z = sz;
    }
    self->field_0x64 = yRange;
    self->field_0x68 = -yRange;
    {
        self->field_0x7C = g0[0];
        self->field_0x80 = g0[1];
        self->field_0x84 = g0[2];
        self->field_0x88 = g1[0];
        self->field_0x8C = g1[1];
        self->field_0x90 = g1[2];
        self->field_0xB8 = g2[0];
        self->field_0xBC = g2[1];
        self->field_0xC0 = g2[2];
    }
    if (mode != 0) {
        const u32* cfg = &tbl[0x1C8 / 4];
        self->field_0x94 = cfg[0];
        self->field_0x98 = cfg[1];
        self->field_0x9C = cfg[2];
    } else {
        const u32* cfg = &tbl[0x1D4 / 4];
        self->field_0x94 = cfg[0];
        self->field_0x98 = cfg[1];
        self->field_0x9C = cfg[2];
    }
    mgr->field_0x128 = flags128;
    mgr->field_0x12C = flags12C;
    if (flags12C != 0) {
        const u32* cfg = &tbl[0x1E0 / 4];
        self->field_0xAC = cfg[0];
        self->field_0xB0 = cfg[1];
        self->field_0xB4 = cfg[2];
    } else {
        const u32* cfg = &tbl[0x1EC / 4];
        self->field_0xAC = cfg[0];
        self->field_0xB0 = cfg[1];
        self->field_0xB4 = cfg[2];
    }
    DCZeroRange((u8*)lbl_eu_80663AC8,
                (((s32)(self->field_0x6C + self->field_0x70) >> 5) + 1) * 4);
    lbl_eu_80665988 = 0;
    lbl_eu_8066598C = 0;
    lbl_eu_80665990 = 0;
    mgr->field_0x136 = 0;
    mgr->field_0xEC = 0;
    mgr->field_0x130 = 0;
    if (func_804BC9A0(self) == 0) return;
    if (yOff < lbl_eu_8066AED0) {
        Vec3 v;
        v.x = src->x;
        v.y = src->y + yOff;
        v.z = src->z;
        func_804BC134(self, (const Vec*)&v, self->field_0x34->field_0x04, src->y);
    } else {
        func_804BC134(self, src, self->field_0x34->field_0x04, yOff + src->y);
    }
    if (mgr->field_0x136 == 0 && self->field_0x78 == 0) return;
    {
        const u32* g0 = &tbl[0x1F8 / 4];
        self->field_0x7C = g0[0];
        self->field_0x80 = g0[1];
        self->field_0x84 = g0[2];
    }
    if (mode != 0) {
        const u32* cfg = &tbl[0x204 / 4];
        self->field_0xA0 = cfg[0];
        self->field_0xA4 = cfg[1];
        self->field_0xA8 = cfg[2];
    } else {
        const u32* cfg = &tbl[0x210 / 4];
        self->field_0xA0 = cfg[0];
        self->field_0xA4 = cfg[1];
        self->field_0xA8 = cfg[2];
    }
    Vec3 vA;
    Vec3 vB;
    vA.x = mgr->field_0x00.x;
    vA.y = mgr->field_0x00.y;
    vA.z = mgr->field_0x00.z;
    vB.x = vA.x;
    vB.y = vA.y + yOff;
    vB.z = vA.z;
    int resCount = mgr->field_0x136;
    if (resCount != 0) {
        for (int i = 0; i < resCount; i++) {
            CColiVolEntry* vol = &self->field_0x40[lbl_eu_8065F300[i]];
            if (vol->field_0x02 & 0x8000) {
                const u32* cfg = &tbl[0x21C / 4];
                self->field_0x94 = cfg[0];
                self->field_0x98 = cfg[1];
                self->field_0x9C = cfg[2];
            } else {
                const u32* cfg = &tbl[0x228 / 4];
                self->field_0x94 = cfg[0];
                self->field_0x98 = cfg[1];
                self->field_0x9C = cfg[2];
            }
            Vec3 v3;
            PSMTXMultVec(vol->field_0x34, (const Vec*)&vA, (Vec*)&mgr->field_0x00);
            PSMTXMultVec(vol->field_0x34, (const Vec*)&vB, (Vec*)&v3);
            {
                nw4r::math::VEC3 diff;
                nw4r::math::VEC3Sub(&diff, (const nw4r::math::VEC3*)&v3,
                                    (const nw4r::math::VEC3*)&mgr->field_0x00);
                Vec3 d2;
                d2.x = diff.x;
                d2.y = diff.y;
                d2.z = diff.z;
                *(u32*)&mgr->field_0x0C.x = *(u32*)&d2.x;
                *(u32*)&mgr->field_0x0C.y = *(u32*)&d2.y;
                *(u32*)&mgr->field_0x0C.z = *(u32*)&d2.z;
            }
            CColiEntry12* e12 = &self->field_0x38[vol->field_0x00];
            DCZeroRange((u8*)lbl_eu_80663AC8, ((e12->field_0x02 >> 5) + 1) * 4);
            mgr->field_0xF0[0][0] = vol->m[0][0];
            mgr->field_0xF0[0][1] = vol->m[0][1];
            mgr->field_0xF0[0][2] = vol->m[0][2];
            mgr->field_0xF0[1][0] = vol->m[1][0];
            mgr->field_0xF0[1][1] = vol->m[1][1];
            mgr->field_0xF0[1][2] = vol->m[1][2];
            mgr->field_0xF0[2][0] = vol->m[2][0];
            mgr->field_0xF0[2][1] = vol->m[2][1];
            mgr->field_0xF0[2][2] = vol->m[2][2];
            self->field_0x30 = self->field_0x14 + e12->field_0x08;
            mgr->field_0xE8 = (Mtx*)&vol->m[0][0];
            mgr->field_0xEC = (u32)&vol->field_0x34;
            func_804BBFD4(self, (const Vec*)&mgr->field_0x00, (const Vec*)&v3, e12->field_0x04);
        }
    }
    if (self->field_0x78 != 0) {
        const u32* cfg = &tbl[0x234 / 4];
        self->field_0x94 = cfg[0];
        self->field_0x98 = cfg[1];
        self->field_0x9C = cfg[2];
        if (yOff < lbl_eu_8066AED0) {
            mgr->field_0x24.x = src->x;
            mgr->field_0x24.y = src->y;
            mgr->field_0x24.z = src->z;
            mgr->field_0x30.x = src->x;
            mgr->field_0x30.y = src->y + yOff;
            mgr->field_0x30.z = src->z;
        } else {
            mgr->field_0x24.x = src->x;
            mgr->field_0x24.y = src->y + yOff;
            mgr->field_0x24.z = src->z;
            mgr->field_0x30.x = src->x;
            mgr->field_0x30.y = src->y;
            mgr->field_0x30.z = src->z;
        }
        CColiOutEntry* out = (CColiOutEntry*)self->field_0x60;
        const CColiInEntry* in = self->field_0x44;
        int i = 0;
        while (i < (s32)self->field_0x78) {
            if (func_804B85C8((CColiThing*)self, (CColiVolume*)out, (CColiQuery*)in, i) != 0) {
                Vec3 v4;
                PSMTXMultVec(out->savedMtx.m, (const Vec*)&vA, (Vec*)&mgr->field_0x00);
                PSMTXMultVec(out->savedMtx.m, (const Vec*)&vB, (Vec*)&v4);
                {
                    nw4r::math::VEC3 diff;
                    nw4r::math::VEC3Sub(&diff, (const nw4r::math::VEC3*)&v4,
                                        (const nw4r::math::VEC3*)&mgr->field_0x00);
                    Vec3 d2;
                    d2.x = diff.x;
                    d2.y = diff.y;
                    d2.z = diff.z;
                    *(u32*)&mgr->field_0x0C.x = *(u32*)&d2.x;
                    *(u32*)&mgr->field_0x0C.y = *(u32*)&d2.y;
                    *(u32*)&mgr->field_0x0C.z = *(u32*)&d2.z;
                }
                func_804BBFD4(self, (const Vec*)&mgr->field_0x00, (const Vec*)&v4,
                              self->field_0x38[((CColiQuery*)in)->field_0x00].field_0x04);
            }
            i++;
            out++;
            in++;
        }
    }
    self->field_0x30 = self->field_0x2C;
}

// func_804B9818 - like func_804B91E0 but without the mode parameter: the
// scene-resource loop also checks the 12-byte entry's flag bit 0, and the
// resource loop runs when either the registered-index count or the entry
// count is non-zero.  Void.
void func_804B9818(CColiMoverState* self, const Vec* src, u32 flags128, u32 flags12C,
                   f32 yOff, f32 yRange) {
    CColiMgr* mgr = &lbl_eu_8065F1C8;
    const u32* tbl = lbl_eu_8056F508;
    {
        u32 sx = *(const u32*)&src->x;
        u32 sy = *(const u32*)&src->y;
        u32 sz = *(const u32*)&src->z;
        *(u32*)&mgr->field_0x00.x = sx;
        *(u32*)&mgr->field_0x00.y = sy;
        *(u32*)&mgr->field_0x00.z = sz;
        mgr->field_0x0C.x = lbl_eu_8066AED0;
        mgr->field_0x0C.y = yOff;
        mgr->field_0x0C.z = lbl_eu_8066AED0;
        *(u32*)&mgr->field_0x18.x = sx;
        *(u32*)&mgr->field_0x18.y = sy;
        *(u32*)&mgr->field_0x18.z = sz;
    }
    self->field_0x64 = yRange;
    self->field_0x68 = -yRange;
    {
        const u32* g0 = &tbl[0x240 / 4];
        self->field_0x7C = g0[0];
        self->field_0x80 = g0[1];
        self->field_0x84 = g0[2];
        const u32* g1 = &tbl[0x24C / 4];
        self->field_0x88 = g1[0];
        self->field_0x8C = g1[1];
        self->field_0x90 = g1[2];
        const u32* g2 = &tbl[0x258 / 4];
        self->field_0xB8 = g2[0];
        self->field_0xBC = g2[1];
        self->field_0xC0 = g2[2];
        const u32* g3 = &tbl[0x264 / 4];
        self->field_0x94 = g3[0];
        self->field_0x98 = g3[1];
        self->field_0x9C = g3[2];
    }
    mgr->field_0x128 = flags128;
    mgr->field_0x12C = flags12C;
    if (flags12C != 0) {
        const u32* cfg = &tbl[0x270 / 4];
        self->field_0xAC = cfg[0];
        self->field_0xB0 = cfg[1];
        self->field_0xB4 = cfg[2];
    } else {
        const u32* cfg = &tbl[0x27C / 4];
        self->field_0xAC = cfg[0];
        self->field_0xB0 = cfg[1];
        self->field_0xB4 = cfg[2];
    }
    DCZeroRange((u8*)lbl_eu_80663AC8,
                (((s32)(self->field_0x6C + self->field_0x70) >> 5) + 1) * 4);
    lbl_eu_80665988 = 0;
    lbl_eu_8066598C = 0;
    lbl_eu_80665990 = 0;
    mgr->field_0x136 = 0;
    mgr->field_0xEC = 0;
    mgr->field_0x130 = 0;
    if (func_804BC9A0(self) == 0) return;
    if (yOff < lbl_eu_8066AED0) {
        Vec3 v;
        v.x = src->x;
        v.y = src->y + yOff;
        v.z = src->z;
        func_804BC134(self, (const Vec*)&v, self->field_0x34->field_0x04, src->y);
    } else {
        func_804BC134(self, src, self->field_0x34->field_0x04, yOff + src->y);
    }
    if (mgr->field_0x136 == 0 && self->field_0x78 == 0) return;
    {
        const u32* g0 = &tbl[0x288 / 4];
        self->field_0x7C = g0[0];
        self->field_0x80 = g0[1];
        self->field_0x84 = g0[2];
        const u32* g1 = &tbl[0x294 / 4];
        self->field_0xA0 = g1[0];
        self->field_0xA4 = g1[1];
        self->field_0xA8 = g1[2];
    }
    Vec3 vA;
    Vec3 vB;
    vA.x = mgr->field_0x00.x;
    vA.y = mgr->field_0x00.y;
    vA.z = mgr->field_0x00.z;
    vB.x = vA.x;
    vB.y = vA.y + yOff;
    vB.z = vA.z;
    int resCount = mgr->field_0x136;
    if (resCount != 0 || self->field_0x78 != 0) {
        for (int i = 0; i < resCount; i++) {
            CColiVolEntry* vol = &self->field_0x40[lbl_eu_8065F300[i]];
            CColiEntry12* e12 = &self->field_0x38[vol->field_0x00];
            if ((e12->field_0x00[1] & 1) != 0) {
                if (vol->field_0x02 & 0x8000) {
                    const u32* cfg = &tbl[0x2A0 / 4];
                    self->field_0x94 = cfg[0];
                    self->field_0x98 = cfg[1];
                    self->field_0x9C = cfg[2];
                } else {
                    const u32* cfg = &tbl[0x2AC / 4];
                    self->field_0x94 = cfg[0];
                    self->field_0x98 = cfg[1];
                    self->field_0x9C = cfg[2];
                }
                Vec3 v3;
                PSMTXMultVec(vol->field_0x34, (const Vec*)&vA, (Vec*)&mgr->field_0x00);
                PSMTXMultVec(vol->field_0x34, (const Vec*)&vB, (Vec*)&v3);
                {
                    nw4r::math::VEC3 diff;
                    nw4r::math::VEC3Sub(&diff, (const nw4r::math::VEC3*)&v3,
                                        (const nw4r::math::VEC3*)&mgr->field_0x00);
                    Vec3 d2;
                    d2.x = diff.x;
                    d2.y = diff.y;
                    d2.z = diff.z;
                    *(u32*)&mgr->field_0x0C.x = *(u32*)&d2.x;
                    *(u32*)&mgr->field_0x0C.y = *(u32*)&d2.y;
                    *(u32*)&mgr->field_0x0C.z = *(u32*)&d2.z;
                }
                DCZeroRange((u8*)lbl_eu_80663AC8, ((e12->field_0x02 >> 5) + 1) * 4);
                mgr->field_0xF0[0][0] = vol->m[0][0];
                mgr->field_0xF0[0][1] = vol->m[0][1];
                mgr->field_0xF0[0][2] = vol->m[0][2];
                mgr->field_0xF0[1][0] = vol->m[1][0];
                mgr->field_0xF0[1][1] = vol->m[1][1];
                mgr->field_0xF0[1][2] = vol->m[1][2];
                mgr->field_0xF0[2][0] = vol->m[2][0];
                mgr->field_0xF0[2][1] = vol->m[2][1];
                mgr->field_0xF0[2][2] = vol->m[2][2];
                self->field_0x30 = self->field_0x14 + e12->field_0x08;
                mgr->field_0xE8 = (Mtx*)&vol->m[0][0];
                mgr->field_0xEC = (u32)&vol->field_0x34;
                func_804BBFD4(self, (const Vec*)&mgr->field_0x00, (const Vec*)&v3,
                              e12->field_0x04);
            }
        }
    }
    if (self->field_0x78 != 0) {
        const u32* cfg = &tbl[0x2B8 / 4];
        self->field_0x94 = cfg[0];
        self->field_0x98 = cfg[1];
        self->field_0x9C = cfg[2];
        if (yOff < lbl_eu_8066AED0) {
            mgr->field_0x24.x = src->x;
            mgr->field_0x24.y = src->y;
            mgr->field_0x24.z = src->z;
            mgr->field_0x30.x = src->x;
            mgr->field_0x30.y = src->y + yOff;
            mgr->field_0x30.z = src->z;
        } else {
            mgr->field_0x24.x = src->x;
            mgr->field_0x24.y = src->y + yOff;
            mgr->field_0x24.z = src->z;
            mgr->field_0x30.x = src->x;
            mgr->field_0x30.y = src->y;
            mgr->field_0x30.z = src->z;
        }
        CColiOutEntry* out = (CColiOutEntry*)self->field_0x60;
        const CColiInEntry* in = self->field_0x44;
        int i = 0;
        while (i < (s32)self->field_0x78) {
            if (func_804B85C8((CColiThing*)self, (CColiVolume*)out, (CColiQuery*)in, i) != 0) {
                Vec3 v4;
                PSMTXMultVec(out->savedMtx.m, (const Vec*)&vA, (Vec*)&mgr->field_0x00);
                PSMTXMultVec(out->savedMtx.m, (const Vec*)&vB, (Vec*)&v4);
                {
                    nw4r::math::VEC3 diff;
                    nw4r::math::VEC3Sub(&diff, (const nw4r::math::VEC3*)&v4,
                                        (const nw4r::math::VEC3*)&mgr->field_0x00);
                    Vec3 d2;
                    d2.x = diff.x;
                    d2.y = diff.y;
                    d2.z = diff.z;
                    *(u32*)&mgr->field_0x0C.x = *(u32*)&d2.x;
                    *(u32*)&mgr->field_0x0C.y = *(u32*)&d2.y;
                    *(u32*)&mgr->field_0x0C.z = *(u32*)&d2.z;
                }
                func_804BBFD4(self, (const Vec*)&mgr->field_0x00, (const Vec*)&v4,
                              self->field_0x38[((CColiQuery*)in)->field_0x00].field_0x04);
            }
            i++;
            out++;
            in++;
        }
    }
    self->field_0x30 = self->field_0x2C;
}

// func_804B9E14 - begin a registration pass: record the offset/scale into
// the manager, install the per-stage matrix configuration from the default
// table, zero the scene-resource state and then process every registered
// volume entry (0x80-byte stride) plus every mover-state entry, handing each
// accepted volume to the query helper.  The GQR5 fast-cast setup (scale 14)
// around the pass is a known wall: no high-level C emits the orphaned
// mtspr writes, so the prologue/epilogue cannot be byte-matched.
void func_804B9E14(CColiMoverState* state, const Vec* offset, u32 flags, int mode, f32 scale) {
    CColiMgr* mgr = &lbl_eu_8065F1C8;
    mgr->field_0x00 = *(const nw4r::math::_VEC3*)offset;
    mgr->field_0x18 = *(const nw4r::math::_VEC3*)offset;
    mgr->field_0x120 = scale;
    mgr->field_0x124 = scale * scale;
    mgr->field_0xD8 = lbl_eu_8066AEE8;
    state->field_0x7C = lbl_eu_8056F508[0x2C4 / 4];
    state->field_0x80 = lbl_eu_8056F508[0x2C8 / 4];
    state->field_0x84 = lbl_eu_8056F508[0x2CC / 4];
    state->field_0x94 = lbl_eu_8056F508[0x2D0 / 4];
    state->field_0x98 = lbl_eu_8056F508[0x2D4 / 4];
    state->field_0x9C = lbl_eu_8056F508[0x2D8 / 4];
    state->field_0x88 = lbl_eu_8056F508[0x2DC / 4];
    state->field_0x8C = lbl_eu_8056F508[0x2E0 / 4];
    state->field_0x90 = lbl_eu_8056F508[0x2E4 / 4];
    mgr->field_0x128 = flags;
    mgr->field_0x12C = mode;
    if (mode != 0) {
        state->field_0xAC = lbl_eu_8056F508[0x2E8 / 4];
        state->field_0xB0 = lbl_eu_8056F508[0x2EC / 4];
        state->field_0xB4 = lbl_eu_8056F508[0x2F0 / 4];
    } else {
        state->field_0xAC = lbl_eu_8056F508[0x2F4 / 4];
        state->field_0xB0 = lbl_eu_8056F508[0x2F8 / 4];
        state->field_0xB4 = lbl_eu_8056F508[0x2FC / 4];
    }
    DCZeroRange((u8*)lbl_eu_80663AC8,
                (((s32)(state->field_0x6C + state->field_0x70) >> 5) + 1) * 4);
    lbl_eu_80665988 = 0;
    lbl_eu_8066598C = 0;
    lbl_eu_80665990 = 0;
    mgr->field_0x136 = 0;
    mgr->field_0xEC = 0;
    mgr->field_0x130 = 0;
    if (func_804BC9A0(state) != 0) {
        func_804BBFA0(state, offset, state->field_0x34->field_0x04, scale);
        if (mgr->field_0x136 != 0 || state->field_0x78 != 0) {
            state->field_0x7C = lbl_eu_8056F508[0x300 / 4];
            state->field_0x80 = lbl_eu_8056F508[0x304 / 4];
            state->field_0x84 = lbl_eu_8056F508[0x308 / 4];
            state->field_0xA0 = lbl_eu_8056F508[0x30C / 4];
            state->field_0xA4 = lbl_eu_8056F508[0x310 / 4];
            state->field_0xA8 = lbl_eu_8056F508[0x314 / 4];
            for (int i = 0; i < mgr->field_0x136; i++) {
                CColiVolEntry* entry = &state->field_0x40[lbl_eu_8065F300[i]];
                if ((entry->field_0x02 & 0x8000) != 0) {
                    state->field_0x94 = lbl_eu_8056F508[0x318 / 4];
                    state->field_0x98 = lbl_eu_8056F508[0x31C / 4];
                    state->field_0x9C = lbl_eu_8056F508[0x320 / 4];
                } else {
                    state->field_0x94 = lbl_eu_8056F508[0x324 / 4];
                    state->field_0x98 = lbl_eu_8056F508[0x328 / 4];
                    state->field_0x9C = lbl_eu_8056F508[0x32C / 4];
                }
                mgr->field_0x120 = scale * entry->field_0x7C;
                mgr->field_0x124 = mgr->field_0x120 * mgr->field_0x120;
                PSMTXMultVec(entry->field_0x34, offset, (Vec*)&mgr->field_0x00);
                CColiEntry12* e12 = &state->field_0x38[entry->field_0x00];
                DCZeroRange((u8*)lbl_eu_80663AC8, ((e12->field_0x02 >> 5) + 1) * 4);
                mgr->field_0xF0[0][0] = entry->m[0][0];
                mgr->field_0xF0[0][1] = entry->m[0][1];
                mgr->field_0xF0[0][2] = entry->m[0][2];
                mgr->field_0xF0[1][0] = entry->m[1][0];
                mgr->field_0xF0[1][1] = entry->m[1][1];
                mgr->field_0xF0[1][2] = entry->m[1][2];
                mgr->field_0xF0[2][0] = entry->m[2][0];
                mgr->field_0xF0[2][1] = entry->m[2][1];
                mgr->field_0xF0[2][2] = entry->m[2][2];
                state->field_0x30 = state->field_0x14 + e12->field_0x08;
                mgr->field_0xE8 = (Mtx*)&entry->m[0][0];
                mgr->field_0xEC = (u32)&entry->field_0x34;
                func_804BBFA0(state, offset, e12->field_0x04, mgr->field_0x120);
            }
            if (state->field_0x78 != 0) {
                state->field_0x94 = lbl_eu_8056F508[0x330 / 4];
                state->field_0x98 = lbl_eu_8056F508[0x334 / 4];
                state->field_0x9C = lbl_eu_8056F508[0x338 / 4];
                mgr->field_0x24.x = offset->x + scale;
                mgr->field_0x24.y = offset->y + scale;
                mgr->field_0x24.z = offset->z + scale;
                mgr->field_0x30.x = offset->x - scale;
                mgr->field_0x30.y = offset->y - scale;
                mgr->field_0x30.z = offset->z - scale;
                for (int i = 0; i < state->field_0x78; i++) {
                    CColiOutEntry* out = (CColiOutEntry*)(state->field_0x60 + i * 0xE0);
                    const CColiInEntry* in = &state->field_0x44[i];
                    if (func_804B85C8((CColiThing*)state, (CColiVolume*)out, (CColiQuery*)in, i) != 0) {
                        mgr->field_0x120 = scale * in->field_0x34;
                        mgr->field_0x124 = mgr->field_0x120 * mgr->field_0x120;
                        PSMTXMultVec(out->inv.m, offset, (Vec*)&mgr->field_0x00);
                        CColiEntry12* e12 = &state->field_0x38[in->field_0x00];
                        func_804BBFA0(state, offset, e12->field_0x04, mgr->field_0x120);
                    }
                }
            }
            state->field_0x30 = state->field_0x2C;
        }
    }
}

// func_804BA26C - register a box query: record the matrix translation and
// per-stage matrix config, then walk the mover-state entries and the
// scene-resource volumes, casting each volume's matrix and concat with the
// input matrix.  The query box written by func_804BC134 is copied into the
// manager's AABB before the mover walk.  Void.
void func_804BA26C(CColiMoverState* self, u32 flag, const Mtx mtx, u32 flags128,
                   u32 flags12C, f32 sA, f32 sB, f32 sC) {
    CColiMgr* mgr = &lbl_eu_8065F1C8;
    const u32* tbl = lbl_eu_8056F508;
    DCZeroRange((u8*)lbl_eu_80663AC8,
                (((s32)(self->field_0x6C + self->field_0x70) >> 5) + 1) * 4);
    lbl_eu_80665988 = 0;
    lbl_eu_8066598C = 0;
    lbl_eu_80665990 = 0;
    mgr->field_0x136 = 0;
    mgr->field_0xEC = 0;
    mgr->field_0x130 = 0;
    Vec3 trans;
    trans.x = mtx[0][3];
    trans.y = mtx[1][3];
    trans.z = mtx[2][3];
    func_804B8108(self, (const Vec*)&trans, mtx, sA, sB, sC);
    {
        const u32* g0 = &tbl[0x33C / 4];
        self->field_0x7C = g0[0];
        self->field_0x80 = g0[1];
        self->field_0x84 = g0[2];
        const u32* g1 = &tbl[0x348 / 4];
        self->field_0x88 = g1[0];
        self->field_0x8C = g1[1];
        self->field_0x90 = g1[2];
    }
    mgr->field_0x128 = flags128;
    mgr->field_0x12C = flags12C;
    if (flags12C != 0) {
        const u32* cfg = &tbl[0x354 / 4];
        self->field_0xAC = cfg[0];
        self->field_0xB0 = cfg[1];
        self->field_0xB4 = cfg[2];
    } else {
        const u32* cfg = &tbl[0x360 / 4];
        self->field_0xAC = cfg[0];
        self->field_0xB0 = cfg[1];
        self->field_0xB4 = cfg[2];
    }
    mgr->field_0xE8 = 0;
    mgr->field_0xA0 = (void*)(uintptr_t)flag;
    if (flag != 0) {
        const u32* cfg = &tbl[0x36C / 4];
        self->field_0x94 = cfg[0];
        self->field_0x98 = cfg[1];
        self->field_0x9C = cfg[2];
    } else {
        const u32* cfg = &tbl[0x378 / 4];
        self->field_0x94 = cfg[0];
        self->field_0x98 = cfg[1];
        self->field_0x9C = cfg[2];
    }
    if (func_804BC9A0(self) == 0) return;
    func_804BC164(self, (const Vec*)&mgr->field_0x3C, (const Vec*)&trans,
                  self->field_0x34->field_0x04, sA, sB, sC);
    if (flag == 0 && lbl_eu_80665988 != 0) return;
    if (mgr->field_0x136 == 0 && self->field_0x78 == 0) return;
    if (flag != 0) {
        const u32* c0 = &tbl[0x384 / 4];
        self->field_0x94 = c0[0];
        self->field_0x98 = c0[1];
        self->field_0x9C = c0[2];
        const u32* c1 = &tbl[0x390 / 4];
        self->field_0xA0 = c1[0];
        self->field_0xA4 = c1[1];
        self->field_0xA8 = c1[2];
    }
    if (self->field_0x78 != 0) {
        const u32* q = lbl_eu_8065F3F0;
        *(u32*)&mgr->field_0x24.x = q[0];
        *(u32*)&mgr->field_0x24.y = q[1];
        *(u32*)&mgr->field_0x24.z = q[2];
        *(u32*)&mgr->field_0x30.x = q[3];
        *(u32*)&mgr->field_0x30.y = q[4];
        *(u32*)&mgr->field_0x30.z = q[5];
        CColiOutEntry* out = (CColiOutEntry*)self->field_0x60;
        const CColiInEntry* in = self->field_0x44;
        int i = 0;
        while (i < (s32)self->field_0x78) {
            if (func_804B85C8((CColiThing*)self, (CColiVolume*)out, (CColiQuery*)in, i) != 0) {
                Vec3 v4;
                PSMTXMultVec(out->savedMtx.m, (const Vec*)&trans, (Vec*)&v4);
                Mtx m4;
                PSMTXConcat(*(const Mtx*)(uintptr_t)mgr->field_0xEC, mtx, m4);
                Vec3 v5;
                v5.x = v4.x;
                v5.y = v4.y;
                v5.z = v4.z;
                f32 sc = in->field_0x34;
                func_804B8108(self, (const Vec*)&v5, m4, sA * sc, sB * sc, sC * sc);
                func_804BC164(self, (const Vec*)&mgr->field_0x3C, (const Vec*)&v4,
                              self->field_0x38[((CColiQuery*)in)->field_0x00].field_0x04,
                              sA * sc, sB * sc, sC * sc);
                if (flag == 0 && lbl_eu_80665988 != 0) {
                    self->field_0x30 = self->field_0x2C;
                    return;
                }
            }
            i++;
            out++;
            in++;
        }
    }
    int resCount = mgr->field_0x136;
    if (resCount != 0) {
        for (int i = 0; i < resCount; i++) {
            CColiVolEntry* vol = &self->field_0x40[lbl_eu_8065F300[i]];
            CColiEntry12* e12 = &self->field_0x38[vol->field_0x00];
            DCZeroRange((u8*)lbl_eu_80663AC8, ((e12->field_0x02 >> 5) + 1) * 4);
            mgr->field_0xF0[0][0] = vol->m[0][0];
            mgr->field_0xF0[0][1] = vol->m[0][1];
            mgr->field_0xF0[0][2] = vol->m[0][2];
            mgr->field_0xF0[1][0] = vol->m[1][0];
            mgr->field_0xF0[1][1] = vol->m[1][1];
            mgr->field_0xF0[1][2] = vol->m[1][2];
            mgr->field_0xF0[2][0] = vol->m[2][0];
            mgr->field_0xF0[2][1] = vol->m[2][1];
            mgr->field_0xF0[2][2] = vol->m[2][2];
            self->field_0x30 = self->field_0x14 + e12->field_0x08;
            mgr->field_0xE8 = (Mtx*)&vol->m[0][0];
            mgr->field_0xEC = (u32)&vol->field_0x34;
            Vec3 v6;
            PSMTXMultVec(vol->field_0x34, (const Vec*)&trans, (Vec*)&v6);
            Mtx m5;
            PSMTXConcat(*(const Mtx*)(uintptr_t)mgr->field_0xEC, mtx, m5);
            Vec3 v7;
            v7.x = v6.x;
            v7.y = v6.y;
            v7.z = v6.z;
            f32 sc = vol->field_0x7C;
            func_804B8108(self, (const Vec*)&v7, m5, sA * sc, sB * sc, sC * sc);
            func_804BC164(self, (const Vec*)&mgr->field_0x3C, (const Vec*)&v6,
                          e12->field_0x04, sA * sc, sB * sc, sC * sc);
            if (flag == 0 && lbl_eu_80665988 != 0) {
                self->field_0x30 = self->field_0x2C;
                return;
            }
        }
    }
    self->field_0x30 = self->field_0x2C;
}

// func_804BA7BC - refresh the manager's reference vector (0x00) from `a` and
// direction vector (0x0C) from the delta `b - a`, zero the word window just
// below the shared bit-table (its base is stored in lbl_eu_80663ACC), then
// delegate to func_804BC494.
void func_804BA7BC(CColiScan* self, const Vec* a, const Vec* b) {
    lbl_eu_8065F1C8.field_0x00 = *(const nw4r::math::_VEC3*)a;
    nw4r::math::VEC3 tmp;
    nw4r::math::VEC3Sub(&tmp, (const nw4r::math::VEC3*)b, (const nw4r::math::VEC3*)a);
    nw4r::math::VEC3 out(tmp);
    lbl_eu_8065F1C8.field_0x0C = out;
    s32 v = self->field_0x74;
    u8* ptr = (u8*)&lbl_eu_8065D1A0 + (0x2000 - ((v >> 5) + 1) * 4);
    lbl_eu_80663ACC = ptr;
    DCZeroRange(ptr, ((v >> 5) + 1) * 4);
    func_804BC494(self, a, b);
}

// func_804BA8B4 - move a collision query along a vertical ray: copy `src` into
// the manager's reference position (raw u32 words), record the Y bounds and
// registration filters, install the per-stage matrix config from the default
// table, then walk every mover-state entry whose flag bit 1 is set: register
// the volume and cast the saved inverse matrix along the ray.  When the pass
// registered anything, the last entry's displacement (src - scene-resource
// header, in its own matrix space) is written to `dst` and, when `angle` is
// non-null, the heading computed from the saved/current matrix Z columns.
u32 func_804BA8B4(CColiMoverState* self, Vec* src, Vec* dst, f32* angle,
                  u32 flagsA, u32 flagsB, f32 yOff, f32 yRange) {
    const u32* tbl = lbl_eu_8056F508;
    if (func_804BC9A0(self) == 0) return 0;
    if (self->field_0x78 == 0) return 0;
    {
        *(u32*)&lbl_eu_8065F1C8.field_0x18.x = *(const u32*)&src->x;
        *(u32*)&lbl_eu_8065F1C8.field_0x18.y = *(const u32*)&src->y;
        *(u32*)&lbl_eu_8065F1C8.field_0x18.z = *(const u32*)&src->z;
    }
    self->field_0x64 = yRange;
    self->field_0x68 = -yRange;
    lbl_eu_8065F1C8.field_0x128 = flagsA;
    lbl_eu_8065F1C8.field_0x12C = flagsB;
    if (flagsB != 0) {
        const u32* cfg = &tbl[0x39C / 4];
        self->field_0xAC = cfg[0];
        self->field_0xB0 = cfg[1];
        self->field_0xB4 = cfg[2];
    } else {
        const u32* cfg = &tbl[0x3A8 / 4];
        self->field_0xAC = cfg[0];
        self->field_0xB0 = cfg[1];
        self->field_0xB4 = cfg[2];
    }
    lbl_eu_80665988 = 0;
    lbl_eu_8066598C = 0;
    lbl_eu_80665990 = 0;
    {
        const u32* g0 = &tbl[0x3B4 / 4];
        self->field_0x7C = g0[0];
        self->field_0x80 = g0[1];
        self->field_0x84 = g0[2];
        const u32* g1 = &tbl[0x3C0 / 4];
        self->field_0x94 = g1[0];
        self->field_0x98 = g1[1];
        self->field_0x9C = g1[2];
        const u32* g2 = &tbl[0x3CC / 4];
        self->field_0xA0 = g2[0];
        self->field_0xA4 = g2[1];
        self->field_0xA8 = g2[2];
        const u32* g3 = &tbl[0x3D8 / 4];
        self->field_0xB8 = g3[0];
        self->field_0xBC = g3[1];
        self->field_0xC0 = g3[2];
    }
    Vec3 lo;
    Vec3 hi;
    lo.x = src->x;
    lo.y = src->y;
    lo.z = src->z;
    hi.x = src->x;
    hi.y = src->y + yOff;
    hi.z = src->z;
    if (yOff < lbl_eu_8066AED0) {
        lbl_eu_8065F1C8.field_0x24.x = lo.x;
        lbl_eu_8065F1C8.field_0x24.y = lo.y;
        lbl_eu_8065F1C8.field_0x24.z = lo.z;
        lbl_eu_8065F1C8.field_0x30.x = hi.x;
        lbl_eu_8065F1C8.field_0x30.y = hi.y;
        lbl_eu_8065F1C8.field_0x30.z = hi.z;
    } else {
        lbl_eu_8065F1C8.field_0x24.x = hi.x;
        lbl_eu_8065F1C8.field_0x24.y = hi.y;
        lbl_eu_8065F1C8.field_0x24.z = hi.z;
        lbl_eu_8065F1C8.field_0x30.x = lo.x;
        lbl_eu_8065F1C8.field_0x30.y = lo.y;
        lbl_eu_8065F1C8.field_0x30.z = lo.z;
    }
    CColiOutEntry* out = (CColiOutEntry*)self->field_0x60;
    const CColiInEntry* in = self->field_0x44;
    int i = 0;
    while (i < (s32)self->field_0x78) {
        if ((out->field_0x00 & 2) != 0) {
            if (func_804B85C8((CColiThing*)self, (CColiVolume*)out, (CColiQuery*)in, i) != 0) {
                Vec3 tmp;
                PSMTXMultVec(out->savedInv.m, (const Vec*)&lo, (Vec*)&lbl_eu_8065F1C8.field_0x00);
                PSMTXMultVec(out->savedInv.m, (const Vec*)&hi, (Vec*)&tmp);
                lbl_eu_8065F1C8.field_0xE8 = (Mtx*)&out->savedMtx;
                nw4r::math::VEC3 diff;
                nw4r::math::VEC3Sub(&diff, (const nw4r::math::VEC3*)&tmp,
                                    (const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0x00);
                Vec3 d2;
                d2.x = diff.x;
                d2.y = diff.y;
                d2.z = diff.z;
                *(u32*)&lbl_eu_8065F1C8.field_0x0C.x = *(u32*)&d2.x;
                *(u32*)&lbl_eu_8065F1C8.field_0x0C.y = *(u32*)&d2.y;
                *(u32*)&lbl_eu_8065F1C8.field_0x0C.z = *(u32*)&d2.z;
                lbl_eu_8065F1C8.field_0xEC = (u32)&out->savedInv;
                func_804BBFD4(self, (const Vec*)&lbl_eu_8065F1C8.field_0x00, (const Vec*)&tmp,
                              self->field_0x38[((CColiQuery*)in)->field_0x00].field_0x04);
            }
        }
        i++;
        out++;
        in++;
    }
    self->field_0x30 = self->field_0x2C;
    if (lbl_eu_80665988 == 0) return 0;
    CColiOutEntry* last =
        (CColiOutEntry*)(self->field_0x60 + (lbl_eu_8065F428[0].field_0x20 - 1) * 0xE0);
    Vec3 t1;
    PSMTXMultVec(last->savedInv.m, (const Vec*)&lbl_eu_8065F428[0], (Vec*)&t1);
    PSMTXMultVec(last->mtx.m, (const Vec*)&t1, src);
    {
        nw4r::math::VEC3 d3;
        nw4r::math::VEC3Sub(&d3, (const nw4r::math::VEC3*)src,
                            (const nw4r::math::VEC3*)&lbl_eu_8065F428[0]);
        Vec3 d4;
        d4.x = d3.x;
        d4.y = d3.y;
        d4.z = d3.z;
        *(u32*)&dst->x = *(u32*)&d4.x;
        *(u32*)&dst->y = *(u32*)&d4.y;
        *(u32*)&dst->z = *(u32*)&d4.z;
    }
    if (angle != 0) {
        Vec3 up1;
        Vec3 up2;
        up1.x = lbl_eu_8066AED0;
        up1.y = lbl_eu_8066AED0;
        up1.z = lbl_eu_8066AED8;
        up2.x = lbl_eu_8066AED0;
        up2.y = lbl_eu_8066AED0;
        up2.z = lbl_eu_8066AED8;
        PSMTXMultVec(last->savedMtx.m, (const Vec*)&up1, (Vec*)&up1);
        PSMTXMultVec(last->mtx.m, (const Vec*)&up2, (Vec*)&up2);
        up1.x -= last->savedMtx.m[0][3];
        up1.y -= last->savedMtx.m[1][3];
        up1.z -= last->savedMtx.m[2][3];
        up2.x -= last->mtx.m[0][3];
        up2.y -= last->mtx.m[1][3];
        up2.z -= last->mtx.m[2][3];
        if (up1.x == lbl_eu_8066AED0 && up1.z == lbl_eu_8066AED0) {
            *angle = lbl_eu_8066AED0;
        } else if (up2.x == lbl_eu_8066AED0 && up2.z == lbl_eu_8066AED0) {
            *angle = lbl_eu_8066AED0;
        } else {
            f32 a1 = nw4r::math::Atan2FIdx(up1.x, up1.z);
            f32 a2 = nw4r::math::Atan2FIdx(up2.x, up2.z);
            *angle = lbl_eu_8066AEEC * a2 - lbl_eu_8066AEEC * a1;
        }
    }
    return 1;
}

extern "C" u32 func_804BADA0(u8* self) { return *(u32*)((u8*)self + 0x5C) != 0; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_804BADB4(){}
