// Translation unit: monolib/src/coli/code_804A6C60
// Collision subsystem - method dispatch table handlers and utilities.
// Each stub below is a catalog placeholder pending decompilation.

#include <harness_catalog.h>
#include <monolib/coli/code_804A6C60.hpp>
#include <monolib/coli/coli_types.hpp>
#include <monolib/util/MemManager.hpp>
#include <nw4r/math.h>
#include <revolution/mtx/mtx.h>
#include <revolution/mtx/mtxvec.h>
#include <revolution/mtx/vec.h>

using ml::coli::CColiObject;
using nw4r::math::VEC3;
using nw4r::math::_VEC3;

// SDA2 zero constant used to fill the axis-helper vectors (retail @sda21 reloc).
extern const float lbl_eu_8066AE24;
// SDA2 threshold/override constants (retail @sda21 relocs): 8066AE80 gates
// the per-axis AABB move in func_804B08A0, 8066AE88 overwrites v.y in
// func_804B24A4 when the flag byte is set. 8066AE40 scales the +0x60
// scalar in func_804A7C64; 8066AE8C/8066AE90 fill the axis block in
// func_804B0B54.
extern const float lbl_eu_8066AE80;
extern const float lbl_eu_8066AE88;
extern const float lbl_eu_8066AE40;
extern const float lbl_eu_8066AE8C;
extern const float lbl_eu_8066AE90;

// Registered-collision globals (sbss 0x65910..0x65938) written by
// func_804A7E18 / func_804A7EC8: the current collision object plus its
// sub-objects. Plain global-scope externs so MWCC emits the exact @sda21
// symbol names (namespace-scope declarations would be mangled).
extern ml::coli::CColiObject* lbl_eu_80665910;
extern ml::coli::CColiObject* lbl_eu_80665914;
extern ml::coli::CColiObject* lbl_eu_80665918;
extern ml::coli::CColiObject* lbl_eu_8066591C;
extern ml::coli::CColiObject* lbl_eu_80665920;
extern ml::coli::CColiObject* lbl_eu_80665928;
extern ml::coli::CColiObject* lbl_eu_8066592C;
extern ml::coli::CColiObject* lbl_eu_80665930;
extern u32 lbl_eu_80665938;

/**
 * Collision object - layout recovered incrementally from retail access
 * patterns in this TU; unknown regions are kept as padding. field_0x00 is
 * the AABB corner pair + axis helpers; it is also read as u32 byte offsets
 * (func_804A7E18) and as a partner-object pointer (func_804ABCA4).
 * 0x04/0x08 are a kind/valid word pair (func_804B0B54, func_804B1BDC);
 * 0x60/0x64 are scalars (func_804A7C64); 0x310/0x314 tail fields.
 */
struct CColiObject {
    union {
        _VEC3 field_0x00[4];   // 0x00..0x2f corner pair + axes
        struct {
            CColiObject* field_0x00_obj; // 0x00 partner-object pointer
            u32 field_0x04;              // 0x04 kind/valid word
            u32 field_0x08;              // 0x08
            union {
                u32 field_0x10;          // 0x10 sub-object byte offset
                f32 field_0x10_f;        // 0x10 radius view (func_804ABCA4)
            };
            u8 _14[4];                   // 0x14
            u32 field_0x18;              // 0x18
            u8 _1c[4];                   // 0x1c
            u32 field_0x20;              // 0x20
            u8 _24[4];                   // 0x24
            u32 field_0x28;              // 0x28
        };
    };
    union {
        _VEC3 field_0x30;     // 0x30..0x3b vector copied raw (lwz/stw)
        struct {
            u32 field_0x30_off;          // 0x30
            u8 _34[4];                   // 0x34
            u32 field_0x38;              // 0x38
        };
    };
    union {
        u8 field_0x3c[8];     // 0x3c..0x43
        struct {
            union {
                f32 field_0x3c_f;      // 0x3c (lfs view)
                u32 field_0x3c_u;      // 0x3c (stw view)
            };
            union {
                u32 field_0x40;        // 0x40 (lwz/stw view)
                u8 field_0x40_b;       // 0x40 (lbz view)
            };
        };
    };
    VEC3 field_0x44;          // 0x44
    VEC3 field_0x50;          // 0x50
    union {
        u8 _5c[0xa8 - 0x5c];  // 0x5c..0xa7
        struct {
            f32 field_0x5c;            // 0x5c radius / scalar
            f32 field_0x60;            // 0x60 (func_804A7C64: sdata2 const * f2)
            f32 field_0x64;            // 0x64 (func_804A7C64: f3)
            u8 _68[0x8c - 0x68];       // 0x68..0x8b
            u32 field_0x8c;            // 0x8c
            u8 _90[0xa8 - 0x90];       // 0x90..0xa7
        };
    };
    u32 field_0xa8;           // 0xa8 - behaviour flags
    u8 _ac[0x210 - 0xac];     // 0xac..0x20f
    u32 field_0x210;          // 0x210
    u8 _214[0x2d4 - 0x214];   // 0x214..0x2d3
    u32 field_0x2d4;          // 0x2d4
    u8 _2d8[0x310 - 0x2d8];   // 0x2d8..0x30f
    f32 field_0x310;          // 0x310
    u32 field_0x314;          // 0x314
};

static inline f32 clamp01(f32 value) {
    if (value < lbl_eu_8066AE44) return lbl_eu_8066AE44;
    if (value > lbl_eu_8066AE3C) return lbl_eu_8066AE3C;
    return value;
}

void func_804A6C60(void){}

void func_804A6D90(void){}

void func_804A6DC0(void){}

void func_804A6E20(void){}

void func_804A70F8(void){}

// Axis frame build (sibling of func_804A7834): the 3x4 matrix diagonal
// comes from src->field_0x00[2] (a direction vector), the zero constant
// fills the rest, and three axis slots get src's corner plus a mixed pair
// of other's axis vectors. FPR allocation follows local declaration order
// (z, y, x, then the sdata2 zero - matching retail's load sequence).
// The fills go through a _VEC3* so MWCC cannot alias-prove the add stores
// (written via self) against them - retail keeps all 12 fill stores and
// schedules the add loads in place after them.
// 12-float view of the field_0x00 matrix block used by func_804A732C's
// axis-slot writes (the retail keeps the three k-fills that are overwritten,
// so the add stores must look non-aliasing to MWCC's dead-store eliminator).
struct CColiM12f804A732C {
    f32 f[12];   // +0x00..+0x2f
};

// Axis frame build (sibling of func_804A7834): the 3x4 matrix diagonal
// comes from src->field_0x00[2] (a direction vector), the zero constant
// fills the rest, and three axis slots get src's corner plus a mixed pair
// of other's axis vectors. The four corner rows are written as whole VEC3
// copies so MWCC keeps all twelve stores (retail overwrites three with the
// axis slots instead of dropping the earlier k fills).
void func_804A732C(CColiObject* self, CColiObject* src, CColiObject* other) {
    f32 z = src->field_0x00[2].z;
    f32 y = src->field_0x00[2].y;
    f32 x = src->field_0x00[2].x;
    f32 k = lbl_eu_8066AE24;
    self->field_0x00[0].x = x;
    self->field_0x00[0].y = k;
    self->field_0x00[0].z = k;
    self->field_0x00[1].x = k;
    self->field_0x00[1].y = k;
    self->field_0x00[1].z = y;
    self->field_0x00[2].x = k;
    self->field_0x00[2].y = k;
    self->field_0x00[2].z = k;
    self->field_0x00[3].x = k;
    self->field_0x00[3].y = z;
    self->field_0x00[3].z = k;
    self->field_0x00[1].x = src->field_0x00[0].x + other->field_0x00[2].x;
    self->field_0x00[2].y = src->field_0x00[0].y + other->field_0x00[3].x;
    self->field_0x00[3].z = src->field_0x00[0].z + other->field_0x00[3].y;
}

void func_804A73A0(void){}

void func_804A763C(void){}

void func_804A7834(CColiObject* self, const CColiObject* other) {
    // Diagonal 3x4 scale-matrix fill: other->field_0x00[2] (a direction
    // vector) placed on the main diagonal, zero constant elsewhere.
    // FPR allocation follows local declaration order: k->f0, x->f1, y->f2, z->f3.
    // MWCC hoists the sdata2 const load to slot 0 (retail loads z, y, x first);
    // the register map and input-load order match retail exactly.
    f32 k = lbl_eu_8066AE24;
    f32 x = other->field_0x00[2].x;
    f32 y = other->field_0x00[2].y;
    f32 z = other->field_0x00[2].z;
    self->field_0x00[0].x = x;
    self->field_0x00[0].y = k;
    self->field_0x00[0].z = k;
    self->field_0x00[1].x = k;
    self->field_0x00[1].y = k;
    self->field_0x00[1].z = y;
    self->field_0x00[2].x = k;
    self->field_0x00[2].y = k;
    self->field_0x00[2].z = k;
    self->field_0x00[3].x = k;
    self->field_0x00[3].y = z;
    self->field_0x00[3].z = k;
}

void func_804A79B4(){}

void func_804A7ACC(){}

// Embedded CColiProc base sub-object at +0x04; only its constructor runs
// from this TU (retail symbol __ct__CColiProc, defined in the sibling
// CColiProc unit).
struct CColiProcLocal {
    u32 field_0x0;   // +0x0
    u32 field_0x4;   // +0x4
    u32 field_0x8;   // +0x8
    u32 field_0xc;   // +0xc
};
extern "C" void __ct__CColiProc(CColiProcLocal* self);

// Local proc seeded by func_804B25A4 and classified by func_804B2CBC (both
// defined in the sibling CColiProc TU; same-TU stubs are declared below).
extern "C" void func_804B25A4(CColiProcLocal* self, CColiObject* target,
                              u32 a, u32 b);
extern "C" int func_804B2CBC(CColiProcLocal* proc, CColiObject* obj);
extern "C" void func_804A7D1C(CColiObject* self);
extern "C" int func_804AD410(CColiObject* self, const f32* a, const VEC3* b,
                             const Mtx m);
extern "C" void func_804B0EA0(CColiObject* self);

// u32 view of a CColiObject used by the func_804A7878 constructor: the
// sub-spec words are copied raw (lwz/stw) into the +0x44 point and the
// +0x40/+0x8c/+0x314 fields are seeded as integers.
struct CColiCtor804A7878 {
    u8 _00[0x04];
    u32 field_0x04;            // +0x04 embedded CColiProc base
    u8 _08[0x3c - 0x08];       // +0x08 .. +0x3b
    CColiObject* field_0x3c;   // +0x3c partner / self slot
    u32 field_0x40;            // +0x40 behaviour flags
    u32 field_0x44;            // +0x44 point (raw u32 copy)
    u32 field_0x48;            // +0x48
    u32 field_0x4c;            // +0x4c
    u8 _50[0x5c - 0x50];       // +0x50 .. +0x5b
    f32 field_0x5c;            // +0x5c scalar (func_804A790C radius)
    f32 field_0x60;            // +0x60 (func_804A7C64: sdata2 const * f2)
    f32 field_0x64;            // +0x64 (func_804A7C64: f3)
    u8 _68[0x8c - 0x68];       // +0x68 .. +0x8b
    u32 field_0x8c;            // +0x8c
    u8 _90[0x210 - 0x90];      // +0x90 .. +0x20f
    u32 field_0x210;           // +0x210
    u8 _214[0x2d4 - 0x214];    // +0x214 .. +0x2d3
    u32 field_0x2d4;           // +0x2d4
    u8 _2d8[0x314 - 0x2d8];    // +0x2d8 .. +0x313
    u32 field_0x314;           // +0x314
};

// Partner-sample helper (sibling of func_804A7878 / func_804A7BDC): run the
// embedded CColiProc base ctor at +0x04, copy the 3-word segment point into
// the +0x44 slot, seed the +0x5c scalar and clear the state fields. The
// +0x3c partner slot points at the caller-supplied index or at the embedded
// proc itself. auto_inline off: retail calls this via `bl`.
#pragma push
#pragma auto_inline off
CColiObject* func_804A790C(CColiObject* out, const VEC3* in, f32 d, u32 idx) {
    CColiCtor804A7878* s = (CColiCtor804A7878*)out;
    __ct__CColiProc((CColiProcLocal*)&s->field_0x04);
    const u32* src = (const u32*)in;
    s->field_0x44 = src[0];
    s->field_0x48 = src[1];
    s->field_0x4c = src[2];
    s->field_0x5c = d;
    s->field_0x40 = 0;
    s->field_0x8c = 0;
    s->field_0x210 = 0;
    s->field_0x2d4 = 0;
    if (idx != 0) {
        s->field_0x3c = (CColiObject*)idx;
    } else {
        s->field_0x3c = (CColiObject*)&s->field_0x04;
    }
    s->field_0x314 = 0;
    return out;
}
#pragma pop

// 3-word sub-object spec copied raw into the +0x44 point.
struct CColiSubSpec804A7878 {
    u32 field_0x00;   // +0x00
    u32 field_0x04;   // +0x04
    u32 field_0x08;   // +0x08
};

// CColiObject constructor: run the embedded CColiProc base ctor at +0x04,
// copy the 3-word sub-spec into the +0x44 point, seed the +0x40 behaviour
// flags (= 0xf) and clear +0x8c/+0x314; the +0x3c partner slot points at
// the caller-supplied parent object or at the embedded proc itself.
CColiObject* func_804A7878(CColiObject* self, const CColiSubSpec804A7878* spec,
                           CColiObject* parent) {
    CColiCtor804A7878* s = (CColiCtor804A7878*)self;
    __ct__CColiProc((CColiProcLocal*)&s->field_0x04);
    s->field_0x44 = spec->field_0x00;
    s->field_0x48 = spec->field_0x04;
    s->field_0x4c = spec->field_0x08;
    s->field_0x40 = 0xf;
    s->field_0x8c = 0;
    if (parent != 0) {
        s->field_0x3c = parent;
    } else {
        s->field_0x3c = (CColiObject*)&s->field_0x04;
    }
    s->field_0x314 = 0;
    return self;
}

// CColiObject initialiser: run the embedded CColiProc base ctor at +0x04,
// copy the 3-word vector into the +0x44 point, then seed the +0x5c scalar,
// the +0x40 sub-object offset (= 0x14) and the cleared state fields.
void func_804A7BDC(CColiObject* self, const VEC3* v, f32 f) {
    __ct__CColiProc((CColiProcLocal*)&self->field_0x10);
    self->field_0x44 = *v;
    self->field_0x5c = f;
    self->field_0x40 = 0x14;
    self->field_0x8c = 0;
    self->field_0x210 = 0;
    self->field_0x2d4 = 0;
    self->field_0x3c_u = 0;
    self->field_0x314 = 0;
}

// Constructor sibling of func_804A790C / func_804A7878: run the embedded
// CColiProc base ctor at +0x04, copy the 3-word sub-spec into the +0x44
// point, then seed the +0x5c/+0x60/+0x64 scalars (0x60 = sdata2 const *
// f2) and the +0x40 sub-object offset (= 0x19). Returns self.
CColiObject* func_804A7C64(CColiObject* self, const CColiSubSpec804A7878* spec,
                           f32 f1, f32 f2, f32 f3) {
    CColiCtor804A7878* s = (CColiCtor804A7878*)self;
    __ct__CColiProc((CColiProcLocal*)&s->field_0x04);
    s->field_0x44 = spec->field_0x00;
    s->field_0x48 = spec->field_0x04;
    s->field_0x4c = spec->field_0x08;
    s->field_0x5c = f1;
    s->field_0x60 = lbl_eu_8066AE40 * f2;
    s->field_0x64 = f3;
    s->field_0x40 = 0x19;
    s->field_0x8c = 0;
    return self;
}

// Local-object constructor (retail body still to be decompiled); called via
// `bl` from func_804B1BDC.
// auto_inline off: retail calls this via `bl`.
#pragma push
#pragma auto_inline off
extern "C" void func_804A7D1C(CColiObject* self) {}
#pragma pop

// Register the collision object and its sub-objects with the global
// collision state (sbss 0x65910..0x65930). The first parameter is unused
// (retail keeps it in r3 and the object in r4). Each field at 0x10..0x40
// holds a byte offset to a sub-object relative to self; the chain is stored
// as absolute pointers, and the object itself is always registered last.
// The seven pointers are computed into locals first so MWCC hoists all the
// loads/adds ahead of the stores (retail's schedule).
void func_804A7E18(u32 unused, CColiObject* self) {
    if (self->field_0x10 != 0) {
        CColiObject* p0 = (CColiObject*)((u8*)self + self->field_0x10);
        CColiObject* p1 = (CColiObject*)((u8*)self + self->field_0x38);
        CColiObject* p2 = (CColiObject*)((u8*)self + self->field_0x28);
        CColiObject* p3 = (CColiObject*)((u8*)self + self->field_0x20);
        CColiObject* p4 = (CColiObject*)((u8*)self + self->field_0x18);
        CColiObject* p5 = (CColiObject*)((u8*)self + self->field_0x30_off);
        CColiObject* p6 = (CColiObject*)((u8*)self + self->field_0x40);
        lbl_eu_80665910 = p0;
        lbl_eu_80665914 = p1;
        lbl_eu_80665920 = p2;
        lbl_eu_80665918 = p3;
        lbl_eu_8066591C = p4;
        lbl_eu_80665928 = p5;
        lbl_eu_8066592C = p6;
    }
    lbl_eu_80665930 = self;
}

void func_804A7E7C(){}

u32 func_804A7EC8(u32 unused, u32 val) { lbl_eu_80665938 = val; return unused; }

// Object handed to the collision dispatch handlers: func_804A7F0C stores the
// caller parameter into field_0x00 and dispatches through the ptmf table at
// lbl_eu_8056F1D8 selected by (field_0x40 + lbl_eu_80665938).
struct CColiDispatchObj {
    u32 field_0x00;        // +0x00
    u8 _04[0x40 - 0x04];   // +0x04 .. +0x40
    u32 field_0x40;        // +0x40 dispatch index
};

// Member-function-pointer dispatch table (12-byte ptmf stride, 0x1A8 bytes).
// Each entry is invoked with the dispatch object as `this` and no arguments;
// MWCC lowers the member-fn-pointer call to __ptmf_scall.
typedef void (CColiDispatchObj::*CColiDispatchFn)();
extern CColiDispatchFn lbl_eu_8056F1D8[];

void func_804A7ED0(CColiObject* self, const VEC3* v) {
    // Add v to both embedded vectors; the nw4r VEC3Add helper reproduces
    // the retail paired-single (psq_l/ps_add/psq_st) sequence.
    VEC3Add(&self->field_0x44, &self->field_0x44, v);
    VEC3Add(&self->field_0x50, &self->field_0x50, v);
}

void func_804A7F0C(CColiDispatchObj* self, u32 val) {
    self->field_0x00 = val;
    u32 idx = self->field_0x40 + lbl_eu_80665938;
    (self->*lbl_eu_8056F1D8[idx])();
}

void func_804A7F50(){}

// No-op handler. Used as a default stub in collision dispatch tables for
// method slots that require no action (e.g. optional callbacks).
void func_804A822C() {}

void func_804A8230(){}

void func_804A8850(){}

// Partner transform view used by func_804A8BE0 / func_804ABBF4: the +0x34
// matrix maps self's point into the local frame, +0x64/+0x68/+0x6c are the
// half-extents, +0x04 the partner point.
struct CColiXfrm804ABBF4 {
    u8 _00[0x04];        // +0x00
    VEC3 field_0x04;     // +0x04..+0x0f partner point (func_804A8BE0 arg)
    u8 _10[0x34 - 0x10]; // +0x10..+0x33
    Mtx field_0x34;      // +0x34..+0x63 transform
    f32 field_0x64;      // +0x64 half-extent x
    f32 field_0x68;      // +0x68 half-extent y
    f32 field_0x6c;      // +0x6c half-extent z
};

// Transform the (radius, 0, 0) normal of self (+0x5c) into the partner's
// frame (+0x34) and ask func_804AD410 to classify the point against the
// partner's half-extents (+0x64/+0x04/+0x34). On a miss, re-run the same
// check with self's +0x48 scalar temporarily swapped to +0x310 when flag
// bit 4 is set; the +0x48 scalar is restored before returning.
int func_804A8BE0(CColiObject* self) {
    CColiXfrm804ABBF4* xf = (CColiXfrm804ABBF4*)self->field_0x00_obj;
    VEC3 v;
    v.x = self->field_0x5c;
    v.y = lbl_eu_8066AE44;
    v.z = lbl_eu_8066AE44;
    nw4r::math::VEC3TransformNormal(
        &v, (const nw4r::math::MTX34*)xf->field_0x34, &v);
    PSVECMag((const Vec*)&v);
    int result = func_804AD410(self, &xf->field_0x64, &xf->field_0x04,
                               xf->field_0x34);
    if (result == 0) {
        if (self->field_0x8c & 0x8) {
            f32 save = self->field_0x44.y;
            self->field_0x44.y = self->field_0x310;
            PSVECMag((const Vec*)&v);
            result = func_804AD410(self, &xf->field_0x64, &xf->field_0x04,
                                   xf->field_0x34);
            self->field_0x44.y = save;
        } else {
            result = 0;
        }
    }
    return result;
}

void func_804A8CB0(){}

void func_804A98C4(){}

void func_804AA4F4(){}

void func_804AA504(){}

void func_804AA870(){}

void func_804AAA98(){}

void func_804AAD90(){}

void func_804AB524(){}

void func_804AB538(){}

void func_804ABA08(){}

// Partner cylinder geometry reached through CColiObject::field_0x00: the
// centre point at +0x04, the radius at +0x10 and the y-extent bounds at
// +0x14 (upper) / +0x18 (lower). func_804AC4F4 uses the +0x14/+0x18 values
// as y-offsets added to the centre instead.
struct CColiCylinder804ABA68 {
    u8 _00[0x04];
    VEC3 field_0x04;   // +0x04 centre
    f32 field_0x10;    // +0x10 radius
    f32 field_0x14;    // +0x14 y upper bound / offset
    f32 field_0x18;    // +0x18 y lower bound / offset
    u8 _1c[0x28 - 0x1c];   // +0x1c .. +0x27
    f32 field_0x28;    // +0x28 scalar (func_804ABDD4 radius term)
};

// Cylinder-overlap test: true when the horizontal squared distance from
// self's point (+0x44) to the partner's centre (+0x04) is within the
// partner's squared radius (+0x10) and the vertical offset lies between
// the partner's y bounds (+0x14 upper, +0x18 lower). VEC3Sub lowers to
// the retail paired-single (psq_l/ps_sub/psq_st) sequence.
bool func_804ABA68(CColiObject* self) {
    CColiCylinder804ABA68* other = (CColiCylinder804ABA68*)self->field_0x00_obj;
    VEC3 diff;
    VEC3Sub(&diff, &self->field_0x44, &other->field_0x04);
    if (diff.y <= other->field_0x14 && diff.y >= other->field_0x18) {
        // Squared horizontal distance vs squared radius; the dist
        // expression is evaluated first so the r^2 fmuls lands between the
        // dx^2 and the fmadds (retail's schedule).
        f32 dist = diff.x * diff.x + diff.z * diff.z;
        if (other->field_0x10 * other->field_0x10 >= dist) {
            return true;
        }
    }
    return false;
}
void func_804ABAF0(){}

// Partner transform AABB test: the partner's +0x34 matrix maps self's point
// (+0x44) into the local frame, then each axis of the result is checked
// against the partner's half-extents at +0x64/+0x68/+0x6c (|axis| <= extent).
bool func_804ABBF4(CColiObject* self) {
    CColiXfrm804ABBF4* obj = (CColiXfrm804ABBF4*)self->field_0x00_obj;
    VEC3 out;
    PSMTXMultVec(obj->field_0x34, self->field_0x44, out);
    obj = (CColiXfrm804ABBF4*)self->field_0x00_obj;
    if (obj->field_0x64 < out.x || -obj->field_0x64 > out.x) return false;
    if (obj->field_0x68 < out.y || -obj->field_0x68 > out.y) return false;
    if (obj->field_0x6c < out.z || -obj->field_0x6c > out.z) return false;
    return true;
}

// Partner collision record reached through CColiObject::field_0x00: the
// point at +0x04 and the radius at +0x10.
struct CColiSphere804ABCA4 {
    u8 _00[0x04];
    VEC3 field_0x04;   // +0x04 centre
    f32 field_0x10;    // +0x10 radius
};

// Sphere-overlap test: true when the squared distance between self's point
// (+0x44) and the partner's point (+0x04) is within the squared sum of the
// two radii (+0x10, +0x5c). VEC3Sub/VEC3LenSq lower to the retail
// paired-single sequence (psq_l/ps_sub/psq_st, ps_mul/ps_madd/ps_sum0).
bool func_804ABCA4(CColiObject* self) {
    CColiObject* other = self->field_0x00_obj;
    VEC3 diff;
    VEC3Sub(&diff, &self->field_0x44, (VEC3*)((u8*)other + 4));
    f32 r = other->field_0x10_f + self->field_0x5c;
    return r * r >= VEC3LenSq(&diff);
}

// Cylinder-capped sphere test: clamp self's point (+0x44) y-coordinate into
// the partner's [centre.y + lower, centre.y + upper] band, then test the
// squared distance against the squared sum of the partner radius (+0x10) and
// self's scalar (+0x5c). Mirrors func_804ABCA4 with the clamped point;
// VEC3Sub/VEC3LenSq lower to the retail paired-single sequence.
bool func_804ABD0C(CColiObject* self) {
    CColiCylinder804ABA68* obj = (CColiCylinder804ABA68*)self->field_0x00_obj;
    VEC3 v;
    VEC3 diff;
    v.x = obj->field_0x04.x;
    v.y = obj->field_0x04.y;
    v.z = obj->field_0x04.z;
    if (v.y + obj->field_0x14 <= self->field_0x44.y) {
        v.y = v.y + obj->field_0x14;
    } else {
        f32 lo = v.y + obj->field_0x18;
        if (lo >= self->field_0x44.y) {
            v.y = lo;
        } else {
            v.y = self->field_0x44.y;
        }
    }
    f32 r = ((CColiCylinder804ABA68*)self->field_0x00_obj)->field_0x10 +
            self->field_0x5c;
    VEC3Sub(&diff, &self->field_0x44, &v);
    return r * r >= VEC3LenSq(&diff);
}

// Sphere test against the partner's point pair: squared distance from
// self's point (+0x44) to the partner's +0x04 point, then to its +0x10
// point, each within the squared sum of the partner +0x28 scalar and
// self's +0x5c radius. Mirrors func_804ABCA4 with the second point.
bool func_804ABDD4(CColiObject* self) {
    CColiCylinder804ABA68* obj = (CColiCylinder804ABA68*)self->field_0x00_obj;
    f32 r5c = self->field_0x5c;
    VEC3 diff1;
    VEC3Sub(&diff1, &self->field_0x44, &obj->field_0x04);
    f32 r = obj->field_0x28 + r5c;
    f32 r2 = r * r;
    if (r2 >= VEC3LenSq(&diff1)) {
        return true;
    }
    VEC3 diff2;
    VEC3Sub(&diff2, &self->field_0x44, (VEC3*)&obj->field_0x10);
    return r2 >= VEC3LenSq(&diff2);
}

void func_804ABF08(){}

void func_804AC020(){}

void func_804AC198(){}

void func_804AC3B0(){}

void func_804AC4E4(){}

void func_804AC57C(){}

// Contact object sampled by func_804AC5D8 / func_804AF808: field_0x00 links
// the paired collision object; the VEC3s at +0x10/+0x20/+0x30 and the scalar
// at +0x70 describe the segment passed to the clip helper.
struct CColiContactObj {
    CColiContactObj* field_0x00; // +0x00 linked partner
    u8 _04[0x10 - 0x04];         // +0x04 .. +0x10
    f32 field_0x10;              // +0x10 (point rows are 0x10-stride)
    u8 _14[0x20 - 0x14];         // +0x14 .. +0x20
    f32 field_0x20;              // +0x20
    u8 _24[0x30 - 0x24];         // +0x24 .. +0x30
    f32 field_0x30;              // +0x30
    u8 _34[0x70 - 0x34];         // +0x34 .. +0x70
    f32 field_0x70;              // +0x70
};

extern "C" void func_804AF808(CColiContactObj* self, const VEC3* v, f32 f);

// Sample the partner's point (+0x04) into v, clamp v.y so the caller's
// +0x48 scalar lies inside [v.y + field_0x18, v.y + field_0x14], then hand
// the clamped segment to the clip helper along with the partner radius
// (+0x10). The VEC3 fields are written component-wise so MWCC keeps v.y
// live in a register for the clamping adds (retail's f2).
void func_804AC4F4(CColiObject* self) {
    CColiCylinder804ABA68* obj = (CColiCylinder804ABA68*)self->field_0x00_obj;
    VEC3 v;
    v.x = obj->field_0x04.x;
    v.y = obj->field_0x04.y;
    v.z = obj->field_0x04.z;
    if (v.y + obj->field_0x14 <= self->field_0x44.y) {
        v.y = v.y + obj->field_0x14;
    } else {
        f32 hi = v.y + obj->field_0x18;
        if (hi >= self->field_0x44.y) {
            v.y = hi;
        } else {
            v.y = self->field_0x44.y;
        }
    }
    func_804AF808((CColiContactObj*)self, &v,
                  ((CColiCylinder804ABA68*)self->field_0x00_obj)->field_0x10);
}

// Sphere-overlap test against the contact partner: squared distance from
// self's point (+0x44) to the partner's point rows (+0x10/+0x20/+0x30) is
// within the squared sum of the partner scalar (+0x70) and self's radius
// (+0x5c). Mirrors func_804ABCA4 with the v built from the partner rows.
bool func_804ABE84(CColiObject* self) {
    CColiContactObj* obj = (CColiContactObj*)self->field_0x00_obj;
    VEC3 v(obj->field_0x10, obj->field_0x20, obj->field_0x30);
    VEC3 diff;
    VEC3Sub(&diff, &self->field_0x44, &v);
    f32 r = obj->field_0x70 + self->field_0x5c;
    return r * r >= VEC3LenSq(&diff);
}

extern "C" void func_804AF808(CColiContactObj* self, const VEC3* v, f32 f);

extern "C" void func_804AF808(CColiContactObj* self, const VEC3* v, f32 f);

void func_804AC5D8(CColiContactObj* self) {
    // Sample the partner's point rows (+0x10/+0x20/+0x30) plus the scalar at
    // +0x70, then ask the clip helper to classify the segment. The VEC3
    // 3-float ctor inlines to loads z,y,x into f2,f1,f0 then ascending stores.
    CColiContactObj* obj = self->field_0x00;
    VEC3 v(obj->field_0x10, obj->field_0x20, obj->field_0x30);
    func_804AF808(self, &v, obj->field_0x70);
}

// Default false-return handler. Used as a placeholder in collision method
// dispatch tables for boolean-returning virtual method slots that are
// not overridden (always returns 0 / false). The self pointer is unused.
int func_804AC61C(CColiObject* /*self*/) { return 0; }

void func_804AC624(){}

void func_804AC9F4(){}

void func_804ACD9C(){}

void func_804AD1E0(){}

#pragma push
#pragma auto_inline off
// Classify the transformed point against the partner's half-extent block
// (+0x64), point (+0x04) and matrix (+0x34). Stub body (not yet decompiled);
// auto_inline off keeps callers emitting a real `bl`.
extern "C" int func_804AD410(CColiObject* self, const f32* a, const VEC3* b,
                             const Mtx m) {
    return 0;
}
#pragma pop

void func_804AD8FC(){}

void func_804ADD3C(){}

void func_804AE0D0(){}

void func_804AE11C(){}

void func_804AE388(){}

void func_804AE9A4(){}

void func_804AEC8C(){}

void func_804AF07C(){}

void func_804AF09C(){}

void func_804AF2F0(){}

void func_804AF310(){}

void func_804AF32C(){}

// Stub body (not yet decompiled): auto_inline off keeps callers emitting a
// real `bl` instead of inlining the empty body.
#pragma push
#pragma auto_inline off
extern "C" void func_804AF808(CColiContactObj* self, const VEC3* v, f32 f) {}
#pragma pop

// Cylinder test: the offset of self's point (+0x44) from other is within a
// vertical cylinder of radius a and height b around other's point (y in
// [0, b], horizontal squared distance <= a^2). The VEC3Sub + component
// reads reproduce the retail paired-single sequence.
bool func_804AF98C(CColiObject* self, const VEC3* other, f32 a, f32 b) {
    VEC3 diff;
    VEC3Sub(&diff, &self->field_0x44, other);
    if (diff.y <= b && diff.y >= lbl_eu_8066AE44 &&
        diff.x * diff.x + diff.z * diff.z <= a * a) {
        return true;
    }
    return false;
}

void func_804AFA08(){}

// Clip the segment (pA, pB) against the frame spanned by (pC, pD): writes the
// entry/exit points of the portion of the segment that lies within the strip
// between the two parallel lines through pC and pD into pOutA/pOutB.
extern "C" void func_804AFB28(VEC3* pOutA, VEC3* pOutB, const VEC3* pA,
                              const VEC3* pB, const VEC3* pC, const VEC3* pD) {
    VEC3 v3, v1, v4;
    VEC3 scratch;
    VEC3 v2;
    VEC3Sub(&v1, pD, pC);
    VEC3Scale(&v2, &v1, lbl_eu_8066AE50);
    VEC3Sub(&v3, pB, pA);
    VEC3Sub(&v4, pC, pA);

    f32 a = VEC3Dot(&v1, &v1);
    f32 b = VEC3Dot(&v3, &v3);
    f32 c = VEC3Dot(&v3, &v2);
    f32 d = VEC3Dot(&v3, &v4);
    f32 e = VEC3Dot(&v2, &v4);

    f32 t = (b * e - c * d) / (a * b - c * c);
    f32 u = (d - c * t) / b;

    if ((u < lbl_eu_8066AE44 || u > lbl_eu_8066AE3C) &&
        (t < lbl_eu_8066AE44 || t > lbl_eu_8066AE3C)) {
        // both out of range
        f32 uc = clamp01(u);
        VEC3Scale(&scratch, &v3, uc);
        VEC3Add(pOutA, pA, &scratch);
        VEC3Sub(&v4, pOutA, pC);
        f32 s = VEC3Dot(&v1, &v4) / a;
        if (s < lbl_eu_8066AE44 || s > lbl_eu_8066AE3C) {
            f32 sc = clamp01(s);
            VEC3Scale(&scratch, &v1, sc);
            VEC3Add(pOutB, pC, &scratch);
            VEC3Sub(&v4, pOutB, pA);
            f32 s2 = VEC3Dot(&v3, &v4) / b;
            f32 sc2 = clamp01(s2);
            VEC3Scale(&scratch, &v3, sc2);
            VEC3Add(pOutA, pA, &scratch);
        } else {
            VEC3Scale(&scratch, &v1, s);
            VEC3Add(pOutB, pC, &scratch);
        }
    } else {
        if (u < lbl_eu_8066AE44 || u > lbl_eu_8066AE3C) {
            // u out, t in
            f32 uc = clamp01(u);
            VEC3Scale(&scratch, &v3, uc);
            VEC3Add(pOutA, pA, &scratch);
            VEC3Sub(&v4, pOutA, pC);
            f32 s = VEC3Dot(&v1, &v4) / a;
            f32 sc = clamp01(s);
            VEC3Scale(&scratch, &v1, sc);
            VEC3Add(pOutB, pC, &scratch);
        } else {
            if (t < lbl_eu_8066AE44 || t > lbl_eu_8066AE3C) {
                // t out, u in
                f32 tc = clamp01(t);
                VEC3Scale(&scratch, &v1, tc);
                VEC3Add(pOutB, pC, &scratch);
                VEC3Sub(&v4, pOutB, pA);
                f32 s2 = VEC3Dot(&v3, &v4) / b;
                f32 sc2 = clamp01(s2);
                VEC3Scale(&scratch, &v3, sc2);
                VEC3Add(pOutA, pA, &scratch);
            } else {
                // both in range
                VEC3Scale(&scratch, &v3, u);
                VEC3Add(pOutA, pA, &scratch);
                VEC3Scale(&scratch, &v1, t);
                VEC3Add(pOutB, pC, &scratch);
            }
        }
    }
}

void func_804B028C(){}

// Write the segment (in - d, in + d) as the embedded AABB corner pair.
// auto_inline off: retail calls this via `bl` (separate TU in the original
// source), so callers must not inline the body.
#pragma push
#pragma auto_inline off
extern "C" void func_804B06FC(CColiObject* self, const VEC3* in, f32 d) {
    self->field_0x00[0].x = in->x + d;
    self->field_0x00[0].y = in->y + d;
    self->field_0x00[0].z = in->z + d;
    self->field_0x00[1].x = in->x - d;
    self->field_0x00[1].y = in->y - d;
    self->field_0x00[1].z = in->z - d;
}
#pragma pop

void func_804B073C(CColiObject* self, const VEC3* in, f32 a, f32 b, f32 c) {
    // Asymmetric AABB corner pair around in (half-extents a/b/c mixed axes).
    self->field_0x00[0].x = in->x + a;
    self->field_0x00[0].y = in->y + b;
    self->field_0x00[0].z = in->z + a;
    self->field_0x00[1].x = in->x - a;
    self->field_0x00[1].y = in->y + c;
    self->field_0x00[1].z = in->z - a;
}

// AABB corner pair from two objects: field_0x00[0] gets the component-wise
// maximum of the two points, field_0x00[1] the minimum.
void func_804B077C(CColiObject* out, const CColiObject* a, const CColiObject* b) {
    if (a->field_0x00[0].x > b->field_0x00[0].x) {
        out->field_0x00[0].x = a->field_0x00[0].x;
        out->field_0x00[1].x = b->field_0x00[0].x;
    } else {
        out->field_0x00[0].x = b->field_0x00[0].x;
        out->field_0x00[1].x = a->field_0x00[0].x;
    }
    if (a->field_0x00[0].y > b->field_0x00[0].y) {
        out->field_0x00[0].y = a->field_0x00[0].y;
        out->field_0x00[1].y = b->field_0x00[0].y;
    } else {
        out->field_0x00[0].y = b->field_0x00[0].y;
        out->field_0x00[1].y = a->field_0x00[0].y;
    }
    // z block: walk a f32* with *q++ so MWCC keeps a base register across
    // the branch (retail materialises addi r4, r3, 8 for the z pair).
    f32* q = &out->field_0x00[0].z;
    if (a->field_0x00[0].z > b->field_0x00[0].z) {
        *q++ = a->field_0x00[0].z;
        q++;
        q++;
        *q = b->field_0x00[0].z;
    } else {
        *q++ = b->field_0x00[0].z;
        q++;
        q++;
        *q = a->field_0x00[0].z;
    }
}

void func_804B07F0(){}

// AABB-containment test: true when b's box (min corner at field_0x00[1],
// max corner at field_0x00[2]) lies inside a's box (min at field_0x00[1],
// max at field_0x00[0]). Component pairs are compared in x, z, y order to
// mirror the retail branch sequence. auto_inline off: retail calls this via
// `bl` (separate TU in the original source).
#pragma push
#pragma auto_inline off
extern "C" bool func_804B0818(const CColiObject* a, const CColiObject* b) {
    if (b->field_0x00[1].x >= a->field_0x00[1].x &&
        b->field_0x00[2].x <= a->field_0x00[0].x &&
        b->field_0x00[1].z >= a->field_0x00[1].z &&
        b->field_0x00[2].z <= a->field_0x00[0].z &&
        b->field_0x00[1].y >= a->field_0x00[1].y &&
        b->field_0x00[2].y <= a->field_0x00[0].y) {
        return true;
    }
    return false;
}
#pragma pop

// Move the embedded AABB corner pair (two 3-float rows) along v: components
// above the sdata2 threshold extend the max corner (row 0), the rest drag the
// min corner (row 1). The z block rebases the base pointer (self += 2 floats)
// so the two stores use 0/0xc displacements (retail's addi r3, r3, 8 shape).
void func_804B08A0(f32* self, const f32* v) {
    f32 threshold = lbl_eu_8066AE80;
    if (v[0] > threshold) {
        self[0] += v[0];
    } else {
        self[3] += v[0];
    }
    if (v[1] > threshold) {
        self[1] += v[1];
    } else {
        self[4] += v[1];
    }
    self += 2;
    if (v[2] > threshold) {
        self[0] += v[2];
    } else {
        self[3] += v[2];
    }
}

void func_804B0924(){}

// Object whose destructor releases an owned buffer (retail __dt__804B095C).
// Only +0x04 (the MemManager-owned allocation) is touched by the teardown;
// the class identity is unknown (placeholder address name).
struct CColi804B095C {
    u8 _00[0x04];
    u8* field_0x4;   // +0x04 owned buffer (mtl::MemManager::deallocate)
};

void* __dt__804B095C(CColi804B095C* self, int deleting) {
    if (self != 0) {
        // Nested null-check on the buffer: MWCC CSEs the load/cmpwi but
        // emits both beqs (retail has the duplicated branch).
        if (self->field_0x4 != 0) {
            if (self->field_0x4 != 0) {
                mtl::MemManager::deallocate(self->field_0x4);
                self->field_0x4 = 0;
            }
        }
        if (deleting > 0) {
            delete self;
        }
    }
    return self;
}

// Collision list node built by the manager: two 3x4 transforms at
// +0x3c/+0x6c, a keyed doubly-linked list slot at +0x9c..+0xa8 and a
// 12-float axis/AABB block filled with the sdata2 constant. The constant
// is re-read after the identity calls because bl clobbers FPRs.
struct CColiNode804B09C8 {
    u32 field_0x00;                  // +0x00
    u8 _04[4];                       // +0x04
    u32 field_0x08;                  // +0x08
    f32 field_0x0c[6];               // +0x0c..+0x23
    f32 field_0x24[6];               // +0x24..+0x3b
    Mtx field_0x3c;                  // +0x3c..+0x6b
    Mtx field_0x6c;                  // +0x6c..+0x9b
    CColiNode804B09C8* field_0x9c;   // +0x9c previous link
    CColiNode804B09C8* field_0xa0;   // +0xa0 next link
    f32 field_0xa4;                  // +0xa4
    u32 field_0xa8;                  // +0xa8 behaviour flags
    u32 field_0xac;                  // +0xac
    u16 field_0xb0;                  // +0xb0
    u16 field_0xb2;                  // +0xb2
};

void func_804B09C8(CColiNode804B09C8* self) {
    f32 k = lbl_eu_8066AE88;
    self->field_0x00 = 0;
    self->field_0x08 = 0;
    self->field_0xa0 = 0;
    self->field_0x9c = 0;
    self->field_0xa4 = k;
    self->field_0xb2 = 1000;
    self->field_0xa8 = 0;
    self->field_0xb0 = 0;
    self->field_0x24[0] = k;
    self->field_0x24[1] = k;
    self->field_0x24[2] = k;
    self->field_0x24[3] = k;
    self->field_0x24[4] = k;
    self->field_0x24[5] = k;
    self->field_0x0c[0] = k;
    self->field_0x0c[1] = k;
    self->field_0x0c[2] = k;
    self->field_0x0c[3] = k;
    self->field_0x0c[4] = k;
    self->field_0x0c[5] = k;
    PSMTXIdentity(self->field_0x3c);
    PSMTXIdentity(self->field_0x6c);
    self->field_0xa4 = lbl_eu_8066AE88;
    self->field_0xac = 1;
}

extern "C" void func_804B102C(void* self);
extern "C" void func_804B0A6C(void* self, u32 val) { *(u32*)((u8*)self + 0) = val; func_804B102C(self); }

extern "C" void func_804B0A74(void* self, u32 val) { *(u32*)((u8*)self + 8) = val; func_804B102C(self); }

void func_804B0A7C(){}

void func_804B0AD4(CColiObject* self, int arg, f32 x, f32 y) {
    u32 flags = self->field_0xa8;
    self->field_0x30.x = x;
    flags = (flags & ~0x20) | 0x10;
    self->field_0x30.y = y;
    self->field_0xa8 = flags;
    if (arg != 0) {
        self->field_0xa8 |= 0x400;
    } else {
        self->field_0xa8 &= ~0x400;
    }
}

void func_804B0B0C(CColiObject* self, const _VEC3* v, int flag) {
    // Raw-copy the 3-word vector (VEC3 copy lowers to lwz/stw), then update
    // the behaviour flags: clear bit 4, set bit 5, and set/clear bit 10
    // according to the caller-supplied flag.
    u32 flags = self->field_0xa8;
    self->field_0x30 = *v;
    flags = (flags & ~0x10) | 0x20;
    self->field_0xa8 = flags;
    if (flag != 0) {
        self->field_0xa8 |= 0x400;
    } else {
        self->field_0xa8 &= ~0x400;
    }
}

// Position/state update: raw-copy the 3-word vector into the +0x24 slot and
// set flag bit 2. When bit 5 is set, copy the vector into the +0x3c matrix's
// translation column and invert it into +0x6c; when the object is a fresh
// one (kind 0) with bits 4..7 set, fill the +0x0c..+0x20 axis block with the
// two sdata2 constants. Hand the object to func_804B0EA0 last. The vector is
// copied as words (lwz/stw) so the +0x24 slot keeps the raw bit pattern.
void func_804B0B54(CColiObject* self, const _VEC3* v) {
    const u32* src = (const u32*)v;
    u32* dst = (u32*)&self->field_0x00[3];
    u32 flags = self->field_0xa8;
    dst[0] = src[0];
    u32 nf = flags | 0x4;
    self->field_0xa8 = nf;
    dst[1] = src[1];
    dst[2] = src[2];
    if (nf & 0x20) {
        CColiNode804B09C8* node = (CColiNode804B09C8*)self;
        node->field_0x3c[0][3] = self->field_0x00[3].x;
        node->field_0x3c[1][3] = self->field_0x00[3].y;
        node->field_0x3c[2][3] = self->field_0x00[3].z;
        PSMTXInverse(node->field_0x3c, node->field_0x6c);
    }
    if (self->field_0x04 == 0 && (self->field_0xa8 & 0xf0) != 0) {
        // Declared c2 first (c2->f0) but assigned c1 first (its load is
        // emitted first), matching retail's lfs f1,const1; lfs f0,const2.
        f32 c2;
        f32 c1;
        c1 = lbl_eu_8066AE8C;
        c2 = lbl_eu_8066AE90;
        self->field_0x00[1].x = c1;
        self->field_0x00[1].y = c1;
        self->field_0x00[1].z = c1;
        self->field_0x00[2].x = c2;
        self->field_0x00[2].y = c2;
        self->field_0x00[2].z = c2;
    }
    func_804B0EA0(self);
}

void func_804B0C0C(){}

void func_804B0CE8(){}

// Re-sort self into the keyed list after its +0x18 key changed: walk from
// the head and relink self after the last node whose key is still greater.
// The tail updates the +0xa4 scalar from +0x0c, clamped up to the next
// node's value. Skipped entirely when the +0xa8 bit-1 flag is set.
void func_804B0DF4(CColiNode804B09C8* self) {
    if (!(self->field_0xa8 & 0x2)) return;
    CColiNode804B09C8* ins = 0;
    CColiNode804B09C8* oldNext = self->field_0xa0;
    CColiNode804B09C8* next = oldNext;
    if (oldNext != 0) {
        while (next != 0) {
            if (self->field_0x0c[3] >= next->field_0x0c[3]) break;
            ins = next;
            next = next->field_0xa0;
        }
    }
    if (ins != 0) {
        CColiNode804B09C8* prev = self->field_0x9c;
        if (prev != 0) {
            prev->field_0xa0 = oldNext;
        }
        self->field_0xa0->field_0x9c = prev;
        CColiNode804B09C8* n6 = ins->field_0xa0;
        if (n6 != 0) {
            n6->field_0x9c = self;
        }
        self->field_0xa0 = n6;
        self->field_0x9c = ins;
        ins->field_0xa0 = self;
    }
    CColiNode804B09C8* n = self->field_0xa0;
    self->field_0xa4 = self->field_0x0c[0];
    if (n != 0 && self->field_0x0c[0] < n->field_0xa4) {
        self->field_0xa4 = n->field_0xa4;
    }
}

#pragma push
#pragma auto_inline off
// Behaviour-flag refresh helper (retail body still to be decompiled); called
// via `bl` from func_804B0B54.
extern "C" void func_804B0EA0(CColiObject* self) {}
#pragma pop

#pragma push
#pragma auto_inline off
extern "C" void func_804B102C(void* self) {}
#pragma pop

void func_804B1130(){}

void func_804B1164(){}

// Segment AABB handed to func_804B06FC / func_804B0818: two corner VEC3s.
struct CColiSeg804B192C {
    VEC3 field_0x00[2];   // +0x00..+0x17 max/min corner pair
};

extern "C" int func_804B204C(CColiObject* self, const CColiObject* v, u32 flag);

// Move test: build the segment AABB around (in, d) at +0x8 and, when it is
// contained in self's box, sample the partner segment at +0x20 and process
// it, forwarding the helper's status. The seg local is 0x18 bytes so the
// second local lands at sp+0x20 exactly like the retail frame.
int func_804B192C(CColiObject* self, const VEC3* in, f32 d, u32 a4, u32 a5) {
    CColiSeg804B192C seg;
    func_804B06FC((CColiObject*)&seg, in, d);
    if (func_804B0818((CColiObject*)&seg, self)) {
        CColiObject local;
        func_804A790C(&local, in, d, a5);
        return func_804B204C(self, &local, a4);
    }
    return 0;
}

void func_804B19CC(){}

void func_804B1AD8(){}

// State-machine check before segment processing: the object must have a
// non-zero kind, and the +0xa8 flag combination must be (bit 8 set) or
// (bit 1 clear) or (bit 2 clear), with bit 4 set. On success, construct a
// local object via func_804A7D1C, link it to self, seed a local proc and
// forward both to func_804B2CBC, returning its status.
bool func_804B1BDC(CColiObject* self) {
    CColiProcLocal proc;   // +0x8
    CColiObject local;     // +0x18 (0x318 bytes)
    if (self->field_0x04 == 0) goto fail;
    int v = 1;
    CColiObject* tgt;
    u32 a8;
    int w = 1;
    u32 flags = self->field_0xa8;
    u32 a4;
    if (!(flags & 0x100)) {
        if (flags & 0x2) {
            w = 0;
        }
    }
    if (w == 0) {
        if (flags & 0x4) {
            v = 0;
        }
    }
    if (v != 0) return false;
    if (!(flags & 0x8)) goto fail;

    func_804A7D1C(&local);
    local.field_0x314 = (u32)self;
    tgt = self->field_0x00_obj;
    a8 = self->field_0x08;
    a4 = self->field_0x04;
    func_804B25A4(&proc, tgt, a8, a4);
    if (func_804B2CBC(&proc, &local) == 0) goto fail;
    return true;
fail:
    return false;
}

void func_804B1C9C(){}

void func_804B1DC0(u8* self, int arg) {
    int* flags = (int*)((char*)self + 0xa8);
    if (arg != 0) {
        *flags &= ~0x100;
    } else {
        *flags = (*flags | 0x100) & ~0x0E;
    }
}

void func_804B1DEC(){}

// Segment-processing helper (retail body still to be decompiled); returns
// a status code that func_804B192C forwards to its caller.
// auto_inline off: retail calls this via `bl`.
#pragma push
#pragma auto_inline off
extern "C" int func_804B204C(CColiObject* self, const CColiObject* v, u32 flag) { return 0; }
#pragma pop

void func_804B21A8(){}

void func_804B236C(){}

// Scale the +0x24 vector by the +0x3c scalar, optionally overwrite v.y with
// the sdata2 constant when the +0x40 flag byte is set, then add the result
// into the VEC3 pointed at by +0x38.
void func_804B24A4(CColiObject* self) {
    VEC3 v(self->field_0x00[3].x, self->field_0x00[3].y, self->field_0x00[3].z);
    VEC3Scale(&v, &v, self->field_0x3c_f);
    if (self->field_0x40_b != 0) {
        v.y = lbl_eu_8066AE88;
    }
    VEC3Add((VEC3*)self->field_0x38, (VEC3*)self->field_0x38, &v);
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_804B2524(){}
