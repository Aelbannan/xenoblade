// Auto-scaffolded catalog TU for monolib/src/effect/code_804CC2B8
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include <math.h>

#include <revolution/gx/GXLight.h>
#include <revolution/MTX.h>
#include "monolib/math/CVec3.hpp"
#include "monolib/math/CCol4.hpp"
#include "monolib/math/Random.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/effect/code_804CC2B8.hpp"

// Float constant 0.0f, shared across this TU
extern f32 lbl_eu_8066B0DC;

// Effect struct - partial layout; only fields accessed by matching functions
// are declared. The struct is large (~0x338 bytes).
struct EffectStruct {
    s16 field_0x00;
    s16 field_0x02;
    s16 field_0x04;
    u16 field_0x06; // flags/bitfield
    u32 field_0x08; // pointer (signalled object)
    u32 field_0x0c;
    u32 field_0x10; // also used as float via type punning
    u32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    u8 pad_0x20[0x328 - 0x20];
    void* field_0x328;
    void* field_0x32c;
    void* field_0x330;
    void* field_0x334;
    ~EffectStruct();
};

EffectStruct::~EffectStruct() {
    if (field_0x32c) {
        func_80495E84(*(void**)((u8*)field_0x08 + 0x10));
        field_0x32c = 0;
    }
    if (field_0x328) {
        if (field_0x328) {
            void** vt = *(void***)((u8*)field_0x328 + 0x184);
            ((void (*)(void*, int))vt[1])(field_0x328, 1);
        }
        field_0x328 = 0;
    }
    if (field_0x330) {
        __dt__804D80F0(field_0x330, 1);
        field_0x330 = 0;
    }
    if (field_0x334) {
        __dl__FPv(field_0x334);
        field_0x334 = 0;
    }
    field_0x00 = -1;
    field_0x02 = -1;
    field_0x04 = -1;
    field_0x06 = 0;
}

// Target 6: object pointed to by a list owner (field_0x14 at 0x14).
struct EffectInfo {
    u8 pad_0x00[0x1c];
    u16 field_0x1c;
};

struct ListOwner {
    u8 pad_0x00[0x14];
    EffectInfo* field_0x14;
};

// Target 6: signed-ordered node view (same address as EffectNode but field_0x06
// read unsigned for the bit tests).
struct Vend {
    s16 field_0x00;
    s16 field_0x02;
    s16 field_0x04;
    u16 field_0x06;
};

// Target 3: position/transform layout.
struct EffPos {
    u8 pad_0x00[0xac];
    Vec field_0xac;          // 0xac,0xb0,0xb4
    Vec field_0xb8;          // 0xb8,0xbc,0xc0
    u8 pad_0xc4[0x130 - 0xc4];
    Mtx field_0x130;
};

// Target 2: singly-linked node and list-control layout.
struct Node2 {
    Node2* field_0x00;       // next
    Node2* field_0x04;
};

struct Node2Control {
    u8 pad_0x00[0x04];
    Node2* field_0x04;
};

// 3D segment-vs-region test used by func_804CDE50 / func_804CDD78.
s32 func_804CDF20(void* self, Vec* a, Vec* b, Vec* c);

extern "C" void func_804CD0A4(EffectStruct* self) DECOMP_DONT_INLINE;

// Larger scene/effect layout used by targets 3, 8, 9, 10. Only fields touched
// by the reconstructed functions are declared.
struct SceneSubObj {
    u32 field_0x00;              // 0x00
    u8 pad_0x04[0x1c - 0x04];
    u16 field_0x1c;              // 0x1c flags
    u8 pad_0x1e[0xe4 - 0x1e];
    u32 field_0xe4;              // 0xe4
    u8 pad_0xe8[0xec - 0xe8];
    u8* field_0xec;              // 0xec
    u8 pad_0xf0[0xf8 - 0xf0];
    u32 field_0xf8;              // 0xf8
    u8 pad_0xfc[0x100 - 0xfc];
    u32 field_0x100;             // 0x100 pointer to a sample/tone table
    u8 pad_0x104[0x108 - 0x104];
    u32 field_0x108;             // 0x108 pointer used by target 6
};

// Master effect/scene object shared across most of this TU. Field offsets are
// recovered from the retail accesses. Many ranges are typed-punned (a matrix
// region is also used as vectors), so several sub-ranges overlap in meaning.
// Region at self+0xdc holding a matrix at +0x18 (0xf4). 0x48 bytes total.
struct SceneSubLater {
    u8 pad_0x00[0x18];
    Mtx field_0x18;
};

struct EffectScene {
    u16 field_0x00;
    u16 field_0x02;
    u16 field_0x04;
    u16 field_0x06;          // flags bitfield
    u32 field_0x08;
    SceneSubObj* field_0x0c; // pointer to a sub-object
    u32 field_0x10;          // also read/written as f32 (union)
    u32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    u8 pad_0x20[0xa0 - 0x20];
    Vec field_0xa0;          // 0xa0,0xa4,0xa8
    Vec field_0xac;          // 0xac,0xb0,0xb4
    Vec field_0xb8;          // 0xb8,0xbc,0xc0
    Vec field_0xc4;          // 0xc4,0xc8,0xcc
    Vec field_0xd0;          // 0xd0,0xd4,0xd8
    SceneSubLater field_0xdc; // 0xdc..0x124 sub-object (matrix at +0x18 = 0xf4)
    Vec field_0x124;         // 0x124,0x128,0x12c
    Mtx field_0x130;         // 0x130..0x160
    Mtx field_0x160;         // 0x160..0x190
    Vec field_0x190;         // 0x190,0x194,0x198
    u32 field_0x19c;         // 0x19c
    u8 pad_0x1a0[0x1cc - 0x1a0];
    u32 field_0x1cc;         // 0x1cc
    u8 pad_0x1d0[0x1d8 - 0x1d0];
    f32 field_0x1d8;         // 0x1d8
    u32 field_0x1dc;         // 0x1dc
    u8 pad_0x1e0[0x21c - 0x1e0];
    Vec field_0x21c;         // 0x21c,0x220,0x224
    u8 pad_0x228[0x234 - 0x228];
    f32 field_0x234;         // 0x234
    u8 pad_0x238[0x258 - 0x238];
    s16 field_0x258;
    s16 field_0x25a;
    u8 pad_0x25c[0x29c - 0x25c];
    u32 field_0x29c;
    u32 field_0x2a0;
    u32 field_0x2a4;
    u8 pad_0x2a8[0x2b0 - 0x2a8];
    f32 field_0x2b0;
    f32 field_0x2b4;
    f32 field_0x2b8;
    f32 field_0x2bc;
    f32 field_0x2c0;
    f32 field_0x2c4;
    f32 field_0x2c8;
    f32 field_0x2cc;
    f32 field_0x2d0;
    f32 field_0x2d4;
    f32 field_0x2d8;
    u8 pad_0x2dc[0x328 - 0x2dc];
    void* field_0x328;
    void* field_0x32c;
    void* field_0x330;
    void* field_0x334;
};

// Retail: lwz r3,0(r3); blr  - returns the node/scene type from field_0x00.
u32 func_804CC800(const SceneSubObj* self) { return self->field_0x00; }

// Sample/tone table node for func_804CD9EC (r8): s16 values, a u8 flag and a f32 scale.
struct ToneNode {
    s16 field_0x00;
    s16 field_0x02;
    s16 field_0x04;
    u8 field_0x06;
    u8 pad_0x07;
    f32 field_0x08;
};
// Material/target flags object for func_804CD9EC (r5): u16 flags + a 3-float scale.
struct MatFlags {
    u16 field_0x00;
    u8 pad_0x02[0x34 - 0x02];
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
};

// Target 5: accumulate a scaled tone/dir vector into field_0xb8.
void func_804CD9EC(EffectScene* self) {
    SceneSubObj* sub = self->field_0x0c;
    ToneNode* t = (ToneNode*)sub->field_0x100;
    if (t) {
        f32 scale = (t->field_0x08 / lbl_eu_8066B100) * self->field_0x18;
        if (scale != 0.0f) {
            Vec v;
            v.x = scale * (f32)t->field_0x00;
            v.y = scale * (f32)t->field_0x02;
            v.z = scale * (f32)t->field_0x04;
            if (t->field_0x06 == 0) {
                f32 s = *(const f32*)&self->field_0x10;
                v.x *= s;
                v.y *= s;
                v.z *= s;
            }
            MatFlags* m = (MatFlags*)self->field_0x08;
            if (m->field_0x00 & 0x10) {
                v.x *= m->field_0x34;
                v.y *= m->field_0x38;
                v.z *= m->field_0x3c;
            }
            self->field_0xb8.x += v.x;
            self->field_0xb8.y += v.y;
            self->field_0xb8.z += v.z;
        }
    }
}

// Target 9: pull a sub-object's aligned direction, scale it and fold into b8.
void func_804CDB2C(EffectScene* self, Vec* p1) {
    SceneSubObj* sub = self->field_0x0c;
    u32 cls = sub->field_0x00;
    if ((u32)(cls - 9) <= 2) return;
    u8* pf = (u8*)sub->field_0xf8;
    if (!pf) return;
    s32 r = pf ? *(u8*)(pf - 0xf) : 0;
    if (self->field_0x18 <= 0.0f) r = 0;
    if (*(const f32*)&self->field_0x10 <= 1.0f) r = 3;
    func_804EE60C(&self->field_0xdc);
    if (r) {
        EffectNode* node = func_804E0114((s16)self->field_0x04);
        if (r & 2) func_804EE658(&self->field_0xdc, node);
        if (r & 1) func_804EE8FC(&self->field_0xdc, node);
    }
    f32 f31 = self->field_0x234 * self->field_0x18;
    if (f31 == 0.0f) return;
    Vec res;
    PSMTXMultVec(self->field_0xdc.field_0x18, &self->field_0x21c, &res);
    bool eq = (res.x == p1->x && res.y == p1->y && res.z == p1->z);
    if (eq) return;
    Vec d;
    d.x = res.x - p1->x;
    d.y = res.y - p1->y;
    d.z = res.z - p1->z;
    f32 d2 = d.x * d.x + d.y * d.y + d.z * d.z;
    if (d2 == 0.0f) {
        d.x = 0.0f;
        d.y = 0.0f;
        d.z = 0.0f;
    } else {
        PSVECNormalize(&d, &d);
    }
    d.x *= f31;
    d.y *= f31;
    d.z *= f31;
    self->field_0xb8.x += d.x;
    self->field_0xb8.y += d.y;
    self->field_0xb8.z += d.z;
}

// Source object for target 10: a Vec at 0, a Vec at 0xc, a Vec at 0x18 and an
// input matrix at 0x24.
struct SrcMtx {
    Vec field_0x00;
    Vec field_0x0c;
    Vec field_0x18;
    Mtx field_0x24;
};

// Target 10: copy + invert a source matrix, transform several source vectors
// and stash them across the scene, then feed the sub-object helper and zero the
// local dir vector.
void func_804CC808(EffectScene* self, const SrcMtx* src) {
    u32 i;
    for (i = 0; i < 12; i++) {
        ((u32*)&self->field_0x130)[i] = ((const u32*)&src->field_0x24)[i];
    }
    Mtx tmp;
    PSMTXInverse(self->field_0x130, tmp);
    Vec v0;
    PSMTXMultVec(tmp, (const Vec*)src, &v0);
    self->field_0xa0 = v0;
    self->field_0xac = v0;
    self->field_0x124 = v0;
    Vec v1;
    PSMTXMultVec(tmp, &src->field_0x18, &v1);
    self->field_0xd0 = v1;
    PSMTXInverse(self->field_0x160, tmp);
    Vec v2;
    PSMTXMultVec(tmp, &src->field_0x0c, &v2);
    self->field_0xc4 = v2;
    SceneSubObj* sub = self->field_0x0c;
    u8* pf = (u8*)sub->field_0xf8;
    u32 b = pf ? *(u8*)(pf - 0x10) : 0;
    u32 c = pf ? (u32)(pf - 8) : 0;
    func_804EE558(&self->field_0xdc, self->field_0x08, b, c, 0);
    self->field_0xb8.x = ml::CVec3::zero.x;
    self->field_0xb8.y = ml::CVec3::zero.y;
    self->field_0xb8.z = ml::CVec3::zero.z;
    self->field_0x06 = (u16)(self->field_0x06 | 0x800);
}

// Forward decls for the target-3 frame pipeline.
void func_804CE418(EffectScene* self, Vec* out);
void func_804CD0CC(EffectScene* self);
void func_804CD9EC(EffectScene* self);
void func_804CDB2C(EffectScene* self, Vec* p1);
void func_804CDD78(EffectScene* self, const Vec* p2, const Vec* p3);
void func_804CDE50(EffectScene* self, const Vec* p2, const Vec* p3);

void func_804CE160(EffectScene* self, const ml::CVec3& a, const ml::CVec3& b);
void func_804CE140(EffectScene* self);
void func_804CE264(EffectScene* self, Vec* p1, Vec* p3);
void func_804CE388(EffectScene* self, Vec* p);
void func_804CE3E8(EffectScene* self);

// Target 3: process a frame of the firework/placer scene if the 0x10 flag is set.
void func_804CCF84(EffectScene* self) {
    if (!(self->field_0x06 & 0x10)) return;
    u32 type = func_804CC800(self->field_0x0c);
    Vec stackA;
    Vec stackB;
    func_8004B0B0(&stackA);
    func_8004B0B0(&stackB);
    self->field_0x06 = (u16)(self->field_0x06 & (u16)~0x40);
    func_804CE418(self, &stackB);
    func_804CD0CC(self);
    func_804CD9EC(self);
    func_804CDB2C(self, &stackB);
    func_804CE418(self, &stackA);
    if (lbl_eu_8066B0D8 > *(const f32*)&self->field_0x10) {
        func_804CDD78(self, &stackB, &stackA);
        func_804CDE50(self, &stackB, &stackA);
    }
    if (!(self->field_0x06 & 0x2)) {
        func_804CE140(self);
    }
    func_804CE160(self, *(const ml::CVec3*)&stackB, *(const ml::CVec3*)&stackA);
    func_804CE264(self, &stackA, &self->field_0x190);
    if (type == 0xd) func_804CE388(self, &stackA);
    if (type == 0x8) func_804CE3E8(self);
}

void func_804CD0CC(EffectScene* self){}

void func_804CDE50(EffectScene* self, const Vec* p2, const Vec* p3) {
    SceneSubObj* sub = self->field_0x0c;
    u32 cls = sub->field_0x00;
    if ((u32)(cls - 9) <= 2) return;
    u8* fo = (u8*)sub->field_0xf8;
    u8 b = fo ? *(u8*)(fo - 0xd) : 0;
    if (!b) return;
    Vec out;
    PSMTXMultVec(self->field_0xdc.field_0x18, &self->field_0x21c, &out);
    Vec res;
    res.x = out.x;
    res.y = out.y;
    res.z = out.z;
    if (!func_804CDF20((void*)self, (Vec*)p2, (Vec*)p3, &res)) return;
    f32 z = lbl_eu_8066B0DC;
    u32 fl = self->field_0x06 & (u32)~0x8000;
    self->field_0x06 = (u16)fl;
    self->field_0x1c = z;
}

s32 func_804CDF20(void* self, Vec* a, Vec* b, Vec* c) {
    if (a->x == b->x && a->y == b->y && a->z == b->z) {
        f32 dx = a->x - c->x;
        f32 dy = a->y - c->y;
        f32 dz = a->z - c->z;
        return (dx * dx + dy * dy + dz * dz <= lbl_eu_8066B104);
    }
    f32 adx = __fabs(b->x - a->x);
    f32 ady = __fabs(b->y - a->y);
    f32 adz = __fabs(b->z - a->z);
    if (ady < adx) {
        if (adz < adx) {
            // X dominant
            if (a->x <= c->x) {
                if (c->x <= b->x) return 1;
            }
            if (b->x <= c->x) {
                if (c->x <= a->x) return 1;
            }
            return 0;
        } else {
            // Z dominant
            if (a->z <= c->z) {
                if (c->z <= b->z) return 1;
            }
            if (b->z <= c->z) {
                if (c->z <= a->z) return 1;
            }
            return 0;
        }
    } else {
        if (adz < ady) {
            // Y dominant
            if (a->y <= c->y) {
                if (c->y <= b->y) return 1;
            }
            if (b->y <= c->y) {
                if (c->y <= a->y) return 1;
            }
            return 0;
        } else {
            // Z dominant
            if (a->z <= c->z) {
                if (c->z <= b->z) return 1;
            }
            if (b->z <= c->z) {
                if (c->z <= a->z) return 1;
            }
            return 0;
        }
    }
}

void func_804CDD78(EffectScene* self, const Vec* p2, const Vec* p3) {
    if (self->field_0x06 & 0x0100) return;
    Vec out;
    PSMTXMultVec(self->field_0x160, &self->field_0xc4, &out);
    Vec res;
    res.x = out.x;
    res.y = out.y;
    res.z = out.z;
    if (!func_804CDF20((void*)self, (Vec*)p2, (Vec*)p3, &res)) return;
    self->field_0x06 = (u16)(self->field_0x06 | 0x300);
    u32* sub = (u32*)self->field_0x0c;
    u8* fo = (u8*)(sub[0x3b]);
    if (!fo) return;
    if (!*(s8*)(fo - 0xa)) return;
    f32 z = lbl_eu_8066B0DC;
    u32 fl = self->field_0x06 & (u32)~0x8000;
    self->field_0x06 = (u16)fl;
    self->field_0x1c = z;
}

// Forward decls used by func_804CE264 and func_804CCF84.
u32 func_804CE378(const SceneSubObj* self);
u32 func_804CE380(const SceneSubObj* self);
void func_804CE4C0(EffectScene* self, Vec* a, u32 type, Vec* p3);
void func_804CE9A4(EffectScene* self, u32* a, u32* b, u32 type, Vec* p1,
                   Vec* spA, Vec* spB);
void func_804CF700(EffectScene* self, u32* a, Vec* p1);
void func_804CFBC8(EffectScene* self, u32 b, u32* a, Vec* spB, u32* c, bool flag);
void func_804CE79C(EffectScene* self, Vec* out, u32 type);

// TK func_804CE264: builds two stack vectors, feeds child position/type helpers,
// sets a flag bit from field_0x1d8, then posts updates.
void func_804CE264(EffectScene* self, Vec* p1, Vec* p3) {
    u32 type = func_804CE378(self->field_0x0c);
    Vec stackA;
    Vec stackB;
    func_8006BEC0(&stackA);
    func_8004B0B0(&stackB);
    func_804CE4C0(self, &stackA, type, p3);
    func_804CE79C(self, &stackB, type);
    func_804CF700(self, &self->field_0x1cc, p1);
    self->field_0x06 =
        (u16)((self->field_0x06 & (u16)~0x20) |
              ((self->field_0x1d8 != 0.0f) ? (u16)0x20 : 0));
    func_804CE9A4(self, &self->field_0x19c, &self->field_0x1dc, type, p1,
                  &stackA, &stackB);
    if (self->field_0x32c) {
        func_804CFBC8(self, (u32)self->field_0x32c, &self->field_0x19c,
                      &stackB, &self->field_0x1cc,
                      func_804CE380((const SceneSubObj*)self->field_0x08) == 0);
    }
}

// Target 2: identity-fill then per-type axis/rotation setup from the sub-object.
void func_804CE4C0(EffectScene* self, Vec* a, u32 type, Vec* p3) {
    Mtx& out = *reinterpret_cast<Mtx*>(a);
    ml::CMat34 m = ml::CMat34::identity;
    out[0][0] = m.m[0][0]; out[0][1] = m.m[0][1]; out[0][2] = m.m[0][2]; out[0][3] = m.m[0][3];
    out[1][0] = m.m[1][0]; out[1][1] = m.m[1][1]; out[1][2] = m.m[1][2]; out[1][3] = m.m[1][3];
    out[2][0] = m.m[2][0]; out[2][1] = m.m[2][1]; out[2][2] = m.m[2][2]; out[2][3] = m.m[2][3];
    switch (type) {
    case 0:
        if (self->field_0x2d8 != 0.0f) func_804DD4F8(&out);
        break;
    case 1:
        func_804DB980(&out, (const Vec*)&self->field_0x2d0,
                      *(s32*)((u8*)self->field_0x0c + 0x48));
        break;
    case 2:
    case 3:
    case 4: {
        Vec v;
        v.x = self->field_0x2d0;
        v.y = lbl_eu_8066B110 + self->field_0x2d4;
        v.z = self->field_0x2d8;
        func_804DB980(&out, &v, *(s32*)((u8*)self->field_0x0c + 0x48));
        Vec an;
        an.x = -atan2(p3->y, sqrt(p3->x * p3->x + p3->z * p3->z));
        an.y = atan2(p3->x, p3->z);
        an.z = 0.0f;
        Mtx m2;
        func_804DCA88(&m2, &an);
        Mtx conc;
        PSMTXConcat(m2, out, conc);
        out[0][0] = conc[0][0]; out[0][1] = conc[0][1]; out[0][2] = conc[0][2]; out[0][3] = conc[0][3];
        out[1][0] = conc[1][0]; out[1][1] = conc[1][1]; out[1][2] = conc[1][2]; out[1][3] = conc[1][3];
        out[2][0] = conc[2][0]; out[2][1] = conc[2][1]; out[2][2] = conc[2][2]; out[2][3] = conc[2][3];
        break;
    }
    }
}
void func_804CE9A4(EffectScene* self, u32* a, u32* b, u32 type, Vec* p1,
                   Vec* spA, Vec* spB){}
void func_804CF700(EffectScene* self, u32* a, Vec* p1){}
void func_804CFBC8(EffectScene* self, u32 b, u32* a, Vec* spB, u32* c,
                   bool flag){}

u32 func_804CE378(const SceneSubObj* self) { return *(u32*)((u8*)self + 0x4); }

u32 func_804CE380(const SceneSubObj* self) { return *(u32*)((u8*)self + 0x5c); }

void func_804CE388(EffectScene* self, Vec* p){}

void func_804CE160(EffectScene* self, const ml::CVec3& a, const ml::CVec3& b) {
    ml::CVec3 delta = a - b;
    ml::CVec3 out = delta;
    // If every component already lies inside [c1, c2], the direction is safe and
    // needs no re-normalization.
    f32 c1 = lbl_eu_8066B108;
    f32 c2 = lbl_eu_8066B10C;
    if (out.x < c1 || c2 < out.x || out.y < c1 || c2 < out.y || out.z < c1 || c2 < out.z) {
        if (out.x * out.x + out.y * out.y + out.z * out.z == 0.0f) {
            out = ml::CVec3::zero;
        } else {
            PSVECNormalize((const Vec*)&out, (Vec*)&out);
        }
    }
    self->field_0x190.x = out.x;
    self->field_0x190.y = out.y;
    self->field_0x190.z = out.z;
}

void func_804CE418(EffectScene* self, Vec* out) {
    Vec src = self->field_0xac;
    out->x = src.x;
    out->y = src.y;
    out->z = src.z;
    Vec res;
    PSMTXMultVec(self->field_0x130, out, &res);
    out->x = res.x + self->field_0xb8.x;
    out->y = res.y + self->field_0xb8.y;
    out->z = res.z + self->field_0xb8.z;
}

// Target 7: compute the inherited/dir vector `out` for a node from self position
// and scale by per-sub-node u16 factors based on its type.
void func_804CE79C(EffectScene* self, Vec* out, s32 type) {
    *(u32*)&out->x = self->field_0x29c;
    *(u32*)&out->y = self->field_0x2a0;
    *(u32*)&out->z = self->field_0x2a4;
    SceneSubObj* sub = self->field_0x0c;
    u32 t = sub->field_0x00;
    if ((t >= 9 && t <= 11) || t == 0) {
        u16 fl = *(u16*)&sub->field_0x1c;
        if (!(fl & 0x20) && (fl & 0x10)) {
            if (self->field_0x258 >= 0) {
                U16Pair* p = func_804DF2A8(*(const void**)((u8*)self->field_0x08 + 0x0c));
                f32 sx = (f32)p->a;
                f32 sy = (f32)p->b;
                out->x *= sx;
                out->y *= sy;
            } else if (self->field_0x25a >= 0) {
                U16Pair* p = func_804DF2A8(*(const void**)((u8*)self->field_0x08 + 0x0c));
                f32 sx = (f32)p->a;
                f32 sy = (f32)p->b;
                out->x *= sx;
                out->y *= sy;
            }
            out->z = 0.0f;
        }
    } else if (t == 8) {
        out->x = 1.0f;
        out->y = 1.0f;
        out->z = 1.0f;
    }
    if ((u32)(type - 3) <= 2 || (u32)(type - 9) <= 1) {
        f32 fy = out->y;
        f32 fz = out->z;
        out->z = fy;
        out->y = fz;
    }
    MatFlags* mat = (MatFlags*)self->field_0x08;
    if (mat->field_0x00 & 0x10) {
        u32* m = (u32*)sub->field_0xe4;
        u8 b = m ? *(u8*)((u8*)m - 0xe) : 0;
        if (b != 0xb) {
            out->x *= mat->field_0x34;
            out->y *= mat->field_0x38;
            out->z *= mat->field_0x3c;
        }
    }
}

void func_804CE9A4(){}

void func_804CF700(){}

void func_804CFBC8(){}

// Target 6: us-804d4308 - reset two placer color globals, set channel 0 material
// color from a shared GXColor constant, then reset two more globals to -1.
void func_804D0194() {
    GXColor color;
    *(u32*)&color = lbl_eu_8066B124;
    lbl_eu_806659A8 = -1;
    lbl_eu_806659AC = -1;
    reinterpret_cast<void (*)(GXChannelID, const GXColor*)>(GXSetChanMatColor)(
        GX_COLOR0A0, &color);
    lbl_eu_806659B0 = -1;
    lbl_eu_806659B4 = -1;
}

void func_804D01E0(){}

void func_804D0AB4(){}

// Init-region view used by func_804CC3A4 (matrices + color at 0x19c..0x20c).
struct EffInitView {
    ml::CVec3 field_0x190;
    Mtx field_0x19c;
    ml::CCol4 field_0x1cc;
    Mtx field_0x1dc;
};
// Source node: +0/+0x4 u32s, +0x8 s16, +0xa u8 flag selector.
struct EffSrc3 {
    u32 field_0x00;
    u32 field_0x04;
    s16 field_0x08;
    u8 field_0x0a;
};

// Target 3: initialise an effect scene from a source node. Packs a flag nibble
// into field_0x06, zeroes the body, seeds matrices/color, then applies a random
// scale (clamped to 1.0f) to field_0x14 and clears a flag bit if negative.
void func_804CC3A4(EffectScene* out, const EffSrc3* node) {
    out->field_0x04 = (u16)node->field_0x08;
    out->field_0x06 = 0xC000u;
    u32 r = (u32)ml::math::mtRand();
    out->field_0x06 |= (u16)((r >> 17) & 1) << 24;  // bit packed (masked by u16 store, kept for shape)
    out->field_0x08 = node->field_0x00;
    out->field_0x0c = (SceneSubObj*)node->field_0x04;
    out->field_0x10 = lbl_eu_8066B0D8;
    *(f32*)&out->field_0x14 = lbl_eu_8066B0D8;
    out->field_0x18 = lbl_eu_8066B0DC;
    out->field_0x1c = lbl_eu_8066B0DC;
    memset((u8*)out + 0x20, 0, 0x80);
    EffInitView& iv = *reinterpret_cast<EffInitView*>((u8*)out + 0x190);
    iv.field_0x190 = ml::CVec3(lbl_eu_8066B0D8, lbl_eu_8066B0D8, lbl_eu_8066B0DC);
    ml::CMat34 ik;
    ik = ml::CMat34::identity;
    iv.field_0x19c[0][0] = ik.m[0][0]; iv.field_0x19c[0][1] = ik.m[0][1]; iv.field_0x19c[0][2] = ik.m[0][2]; iv.field_0x19c[0][3] = ik.m[0][3];
    iv.field_0x19c[1][0] = ik.m[1][0]; iv.field_0x19c[1][1] = ik.m[1][1]; iv.field_0x19c[1][2] = ik.m[1][2]; iv.field_0x19c[1][3] = ik.m[1][3];
    iv.field_0x19c[2][0] = ik.m[2][0]; iv.field_0x19c[2][1] = ik.m[2][1]; iv.field_0x19c[2][2] = ik.m[2][2]; iv.field_0x19c[2][3] = ik.m[2][3];
    iv.field_0x1cc = ml::CCol4::white;
    iv.field_0x1dc[0][0] = ik.m[0][0]; iv.field_0x1dc[0][1] = ik.m[0][1]; iv.field_0x1dc[0][2] = ik.m[0][2]; iv.field_0x1dc[0][3] = ik.m[0][3];
    iv.field_0x1dc[1][0] = ik.m[1][0]; iv.field_0x1dc[1][1] = ik.m[1][1]; iv.field_0x1dc[1][2] = ik.m[1][2]; iv.field_0x1dc[1][3] = ik.m[1][3];
    iv.field_0x1dc[2][0] = ik.m[2][0]; iv.field_0x1dc[2][1] = ik.m[2][1]; iv.field_0x1dc[2][2] = ik.m[2][2]; iv.field_0x1dc[2][3] = ik.m[2][3];
    ml::math::mtRand();  // re-seed consumed value; see brief tail
    f32 src = *(f32*)((u8*)(*(void**)((u8*)out->field_0x0c + 0xf4)) + 0x8);
    f32 v = (ml::math::mtRand() / lbl_eu_8066B0E0 - lbl_eu_8066B0E4) * src + lbl_eu_8066B0D8;
    if (v <= lbl_eu_8066B0DC) v = lbl_eu_8066B0D8;
    *(f32*)&out->field_0x14 = v;
}

void func_804D189C(){}

void func_804D20EC(){}

void func_804D2690(){}

void func_804D2B60(){}

void func_804D3098(){}

void func_804D361C(){}

// u16 -> float through the shared 0x4330000000000000 magic constant.
inline f32 u16ToF_b0e8(u16 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[0] = 0x43300000u;
    c.w[1] = v;
    return (f32)(c.d - lbl_eu_8066B0E8);
}
s32 func_804D3B14(EffectScene* self, ml::CVec3* vec, ml::CVec3* vec2) {
    if (lbl_eu_8066B0DC <= vec->z) return 0;
    Mtx m;
    func_80496120(*(void**)((u8*)self->field_0x08 + 0x10), m, -1);
    f32 x = vec->x, y = vec->y, z = vec->z;
    f32 sc = lbl_eu_8066B0D8 / (-z);
    f32 oz = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3];
    vec->x = (m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3]) * sc;
    vec->y = (m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3]) * sc;
    vec->z = oz;
    vec->x = (lbl_eu_8066B0E4 * vec->x + lbl_eu_8066B0E4) * u16ToF_b0e8(CDeviceVI::getRenderModeObj()->fbWidth);
    vec->z = 0.0f;
    vec->y = (lbl_eu_8066B0E4 * vec->y + lbl_eu_8066B0E4) * u16ToF_b0e8(CDeviceVI::getRenderModeObj()->efbHeight);
    if (vec2) {
        vec2->x = (m[1][0] * vec2->x + m[1][1] * vec2->y + m[1][2] * vec2->z + m[1][3]) * sc;
        vec2->y = (m[0][0] * vec2->x + m[0][1] * vec2->y + m[0][2] * vec2->z + m[0][3]) * sc;
        vec2->z = m[2][0] * vec2->x + m[2][1] * vec2->y + m[2][2] * vec2->z + m[2][3];
        vec2->x = vec2->x * u16ToF_b0e8(CDeviceVI::getRenderModeObj()->fbWidth);
        vec2->z = 0.0f;
        vec2->y = vec2->y * u16ToF_b0e8(CDeviceVI::getRenderModeObj()->efbHeight);
    }
    return 1;
}

// Target 6: build an 18-float matrix from a count of scale bytes and two ints.
// (byte scale A after the B0E4 constant; B scales the computed direction terms)
void func_804D3DB0(EffectScene* self, f32* out, s32 i5, s32 i6, s32 i7) {
    SceneSubObj* sub = self->field_0x0c;
    u8* p = (u8*)sub->field_0x108;
    f32 A = 1.0f;
    f32 B = 1.0f;
    if (p) {
        if (i7 == 0) {
            A = (f32)(p ? *(p - 4) : 1);
            B = (f32)(p ? *(p - 3) : 1);
        } else {
            A = (f32)(p ? *(p - 2) : 1);
            B = (f32)(p ? *(p - 1) : 1);
        }
    }
    f32 inv = 1.0f / (f32)i6;
    f32 e5 = (f32)i5;
    f32 K = lbl_eu_8066B0E4;
    out[0] = 0.0f;
    out[1] = B * e5 * inv;
    out[2] = 0.0f;
    out[3] = K * A;
    out[4] = B;
    out[5] = 0.0f;
    out[6] = A;
    out[7] = B;
    out[8] = 0.0f;
    out[9] = 0.0f;
    out[10] = B * (1.0f + inv);
    out[11] = 0.0f;
    out[12] = K * A;
    out[13] = B * (1.0f + inv);
    out[14] = 0.0f;
    out[15] = A;
    out[16] = B * (1.0f + inv);
    out[17] = 0.0f;
}

// Target 5 initializer layout: 0x18 is written as a raw int here, though other
// functions read 0x18 as a float (type-punned offset).
struct EffectInit {
    s16 field_0x00;
    s16 field_0x02;
    s16 field_0x04;
    s16 field_0x06;
    s16 field_0x08;
    s16 field_0x0a;
    s16 field_0x0c;
    u8 pad_0x0e[0x10 - 0x0e];
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
};

// Target 5: us-804d80d4
void func_804D3F60(EffectInit* self) {
    self->field_0x06 = -1;
    self->field_0x08 = -1;
    self->field_0x0a = 0;
    self->field_0x00 = 0;
    self->field_0x02 = 0;
    self->field_0x04 = 0;
    self->field_0x0c = 0;
    self->field_0x10 = 0;
    self->field_0x14 = 0;
    self->field_0x18 = 0;
}

// Target 10: us-804d8108 - walk the node chain by index, releasing each node,
// then clear the head list.
void func_804D3F94(EffectNode* self) {
    s32 idx = self->field_0x06;
    while (idx >= 0) {
        s32 old = idx;
        EffectNode* node = func_804E0114(idx);
        idx = node->field_0x02;
        func_804E0098((s16)old);
    }
    self->field_0x06 = -1;
    self->field_0x08 = -1;
    self->field_0x0a = 0;
    self->field_0x0c = 0;
    self->field_0x18 = 0;
}

// Node type used by func_804CCA64 (per-node update state; float sub-fields at
// 0x10..0x1c), s16 flags/links at 0x00..0x0e, pointers at 0x08/0x0c.
struct EffectNode7 {
    u8 pad_0x00[0x06];
    u16 field_0x06;
    u32 field_0x08;
    u32 field_0x0c;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
};

s32 func_804D41F8(ListOwner* self, const Vend* target);
void func_804D5E90(EffectNode* self);

// Target 7: per-node per-frame update. Only the head (visible frame-advance) is
// reconstructed; the sprite/sub-effect emission body is elided from the brief.
void func_804CCA64(void* nodeptr, f32 f) {
    EffectNode7* node = (EffectNode7*)nodeptr;
    node->field_0x18 = f * node->field_0x14;
}

// Target 4: walk the node chain updating each; advance via the step.
void func_804D401C(EffectNode* self, f32 f) {
    if (self->field_0x0a <= 0) return;
    EffectNode* node;
    if (self->field_0x06 >= 0) {
        node = func_804E0114(self->field_0x06);
        self->field_0x00 = -1;
        self->field_0x02 = self->field_0x06;
        self->field_0x04 = node->field_0x02;
        self->field_0x0c = 0;
    } else {
        node = 0;
    }
    f32 lim = lbl_eu_8066B140;
    while (node) {
        func_804CCA64(node, f);
        if (f > lim) {
            if (!(*(u16*)&node->field_0x06 & 1)) {
                if (func_804D41F8((ListOwner*)self, (Vend*)node)) {
                    func_804D5E90(self);
                }
            }
        }
        if (self->field_0x04 >= 0) {
            self->field_0x00 = self->field_0x02;
            self->field_0x02 = self->field_0x04;
            node = func_804E0114(self->field_0x04);
            self->field_0x04 = node->field_0x02;
            self->field_0x0c = (s16)(self->field_0x0c + 1);
        } else {
            node = 0;
        }
    }
}

void func_804D4144(EffectNode* self) {
    if (self->field_0x0a <= 0) return;
    EffectNode* node;
    if (self->field_0x06 >= 0) {
        node = func_804E0114(self->field_0x06);
        self->field_0x00 = -1;
        self->field_0x02 = self->field_0x06;
        self->field_0x04 = node->field_0x02;
        self->field_0x0c = 0;
    } else {
        node = 0;
    }
    while (node) {
        func_804CD0A4((EffectStruct*)(void*)node);
        if (self->field_0x04 >= 0) {
            self->field_0x00 = self->field_0x02;
            self->field_0x02 = self->field_0x04;
            node = func_804E0114(self->field_0x04);
            self->field_0x04 = node->field_0x02;
            self->field_0x0c += 1;
        } else {
            node = 0;
        }
    }
}

// Target 6: scan node slots for one matching the pattern in r28; return 0 if a
// candidate lacks the "linked" bit, else 1. Flag map: 0x2000 gate, 0x8000 slot
// busy, 0x0800 already-linked.
s32 func_804D41F8(ListOwner* self, const Vend* target) {
    if (!((self->field_0x14->field_0x1c >> 13) & 1)) return 1;
    s32 result = 1;
    s16 i = 0;
    while ((s32)(s16)i < (s32)func_804E0104()) {
        Vend* node = (Vend*)func_804E0114(i);
        if (node) {
            u16 nf = node->field_0x06;
            if ((nf >> 15) & 1) {
                if (node->field_0x04 == target->field_0x00) {
                    if ((nf >> 11) & 1) {
                        node->field_0x04 = -1;
                    } else {
                        result = 0;
                    }
                }
            }
        }
        i++;
    }
    return result;
}

void func_804D42B8(){}

// Render-object layout shared by the GX polygon setup functions (targets 2/3).
struct ColorSrc {
    u8 pad_0x00[0xac];
    f32 field_0xac;
    f32 field_0xb0;
    f32 field_0xb4;
    f32 field_0xb8;
};
struct RenderMat {
    u8 pad_0x00[0x1c];
    u16 field_0x1c;             // flags bitfield
    u8 pad_0x1e[0x114 - 0x1e];
    void* field_0x114;          // material data block
};
struct ColorSrcHolder {
    u8 pad_0x00[0x5c];
    ColorSrc* field_0x5c;       // ambient color source
};
struct MtxHolder {
    u8 pad_0x00[0x10];
    ColorSrcHolder* field_0x10; // matrix source + color holder
    u8 pad_0x14[0x5c - 0x14];
    s32 field_0x5c;             // tev swap index
};
struct RenderObj {
    u8 pad_0x00[0x10];
    MtxHolder* field_0x10;
    RenderMat* field_0x14;
    s32 field_0x18;
};

// Target 2 (us-804d98d8): push GX state for a polygon/material pass.
// Sets vertex descs + channel control + ambient color + tex coord gen + blend
// and TEV swap tables selected by the active sub-mode, then runs the shared
// texture/finalize helpers.
void func_804D5764(RenderObj* self) {
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    void* q = self->field_0x14->field_0x114;
    u8 b = q ? *(u8*)((u8*)q - 0x1c) : 0;
    if (b == 0) {
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    } else {
        GXSetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        ColorSrc* cs = self->field_0x10->field_0x10->field_0x5c;
        f32 s[4] = { cs->field_0xac, cs->field_0xb0, cs->field_0xb4, cs->field_0xb8 };
        f32 f7 = q ? *(f32*)((u8*)q - 4) : lbl_eu_8066B144;
        f32 c[4];
        c[0] = (s[0] * f7) * lbl_eu_8066B148;
        c[1] = (s[1] * f7) * lbl_eu_8066B148;
        c[2] = (s[2] * f7) * lbl_eu_8066B148;
        c[3] = s[3];
        for (int i = 0; i < 4; i++) {
            if (c[i] > lbl_eu_8066B14C) c[i] = lbl_eu_8066B14C;
            else if (c[i] < lbl_eu_8066B140) c[i] = lbl_eu_8066B140;
        }
        GXColor col;
        col.r = (u8)(lbl_eu_8066B150 * c[0]);
        col.g = (u8)(lbl_eu_8066B150 * c[1]);
        col.b = (u8)(lbl_eu_8066B150 * c[2]);
        col.a = (u8)(lbl_eu_8066B150 * c[3]);
        GXSetChanAmbColor(GX_COLOR0A0, col);
        GXSetChanAmbColor(GX_COLOR1A1, col);
    }
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e, GX_FALSE, 0x7d);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x21, GX_FALSE, 0x7d);
    void* m = func_80496264((void*)self->field_0x10->field_0x10, -1);
    GXLoadPosMtxImm((const float(*)[4])((u8*)m + 0xcc), 0);
    q = self->field_0x14->field_0x114;
    u16 b2 = q ? *(u16*)((u8*)q - 0x1a) : 0;
    switch (b2) {
    case 0: GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR); break;
    case 1: GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR); break;
    case 2: GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ZERO, GX_BL_ZERO, GX_LO_CLEAR); break;
    case 3: GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_SRCCLR, GX_LO_CLEAR); break;
    }
    s32 r30 = self->field_0x10->field_0x5c;
    if (self->field_0x18 != r30) {
        switch (r30) {
        case 0:
            GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
            break;
        case 1:
            GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
            break;
        case 2:
            GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
            break;
        default:
            GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
            break;
        }
        self->field_0x18 = r30;
    }
    q = self->field_0x14->field_0x114;
    u16 dd0 = q ? *(u16*)((u8*)q - 0x1a) : 0;
    s32 bfl = 0;
    if (dd0 == 0 && !((self->field_0x14->field_0x1c >> 20) & 1)) bfl = 1;
    s32 tp = 0;
    if (bfl && !((self->field_0x14->field_0x1c >> 25) & 1)) tp = 1;
    func_804D8AA4(tp, bfl);
    func_804D0194();
}

// Target 2 (us-804d92b0): near-twin of func_804D5764. Same GX pass but uses
// a palette array for POS vertex data, no position matrix load, and table
// based blends.
void func_804D513C(RenderObj* self) {
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_CLR0, GX_NONE);
    RenderMat* mat = self->field_0x14;
    u8* arr = (mat->field_0x1c >> 20) & 1 ? lbl_eu_80524520 : lbl_eu_80524500;
    GXSetArray(GX_VA_POS, arr, 3);
    void* q = mat->field_0x114;
    u8 b = q ? *(u8*)((u8*)q - 0x1c) : 0;
    if (b == 0) {
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    } else {
        GXSetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        ColorSrc* cs = self->field_0x10->field_0x10->field_0x5c;
        f32 s[4] = { cs->field_0xac, cs->field_0xb0, cs->field_0xb4, cs->field_0xb8 };
        f32 f7 = q ? *(f32*)((u8*)q - 4) : lbl_eu_8066B144;
        f32 c[4];
        c[0] = (s[0] * f7) * lbl_eu_8066B148;
        c[1] = (s[1] * f7) * lbl_eu_8066B148;
        c[2] = (s[2] * f7) * lbl_eu_8066B148;
        c[3] = s[3];
        for (int i = 0; i < 4; i++) {
            if (c[i] > lbl_eu_8066B14C) c[i] = lbl_eu_8066B14C;
            else if (c[i] < lbl_eu_8066B140) c[i] = lbl_eu_8066B140;
        }
        GXColor col;
        col.r = (u8)(lbl_eu_8066B150 * c[0]);
        col.g = (u8)(lbl_eu_8066B150 * c[1]);
        col.b = (u8)(lbl_eu_8066B150 * c[2]);
        col.a = (u8)(lbl_eu_8066B150 * c[3]);
        GXSetChanAmbColor(GX_COLOR0A0, col);
        GXSetChanAmbColor(GX_COLOR1A1, col);
    }
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e, GX_FALSE, 0x7d);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x21, GX_FALSE, 0x7d);
    q = mat->field_0x114;
    u16 b2 = q ? *(u16*)((u8*)q - 0x1a) : 0;
    switch (b2) {
    case 0: GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR); break;
    case 1: GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR); break;
    case 2: GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ZERO, GX_BL_ZERO, GX_LO_CLEAR); break;
    case 3: GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_SRCCLR, GX_LO_CLEAR); break;
    }
    s32 r30 = self->field_0x10->field_0x5c;
    if (self->field_0x18 != r30) {
        switch (r30) {
        case 0:
            GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
            break;
        case 1:
            GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
            break;
        case 2:
            GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
            break;
        default:
            GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
            GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
            break;
        }
        self->field_0x18 = r30;
    }
    q = mat->field_0x114;
    u16 dd0 = q ? *(u16*)((u8*)q - 0x1a) : 0;
    s32 bfl = 0;
    if (dd0 == 0 && !((mat->field_0x1c >> 20) & 1)) bfl = 1;
    s32 tp = 0;
    if (bfl && !((mat->field_0x1c >> 25) & 1)) tp = 1;
    func_804D8AA4(tp, bfl);
    func_804D0194();
}

// Target 8: us-804d9ebc - attach node list entry by index.
EffectNode* func_804D5D48(EffectNode* self) {
    s32 idx = self->field_0x06;
    EffectNode* node = 0;
    if (idx >= 0) {
        node = func_804E0114(idx);
        self->field_0x00 = -1;
        self->field_0x02 = self->field_0x06;
        self->field_0x04 = node->field_0x02;
        self->field_0x0c = 0;
    }
    return node;
}

// Target 9: us-804d9f20 - step to next node slot.
EffectNode* func_804D5DAC(EffectNode* self) {
    s32 cur = self->field_0x04;
    EffectNode* node = 0;
    if (cur >= 0) {
        self->field_0x00 = self->field_0x02;
        self->field_0x02 = cur;
        node = func_804E0114(cur);
        self->field_0x04 = node->field_0x02;
        self->field_0x0c = self->field_0x0c + 1;
    }
    return node;
}

void func_804D5E10(EffectNode* self, s32 index) {
    s32 v = func_804DFFA8(index);
    if ((s16)v < 0) return;
    func_804E0114((s16)v);
    if (self->field_0x06 >= 0) {
        EffectNode* last = func_804E0114(self->field_0x08);
        last->field_0x02 = (s16)v;
        self->field_0x08 = (s16)v;
    } else {
        self->field_0x06 = (s16)v;
        self->field_0x08 = (s16)v;
    }
    self->field_0x0a += 1;
}

void func_804D5E90(EffectNode* self) {
    if (self->field_0x02 == self->field_0x08) {
        self->field_0x08 = self->field_0x00;
    }
    if (self->field_0x00 >= 0) {
        EffectNode* nodeA = func_804E0114(self->field_0x00);
        EffectNode* nodeB = func_804E0114(self->field_0x02);
        nodeA->field_0x02 = nodeB->field_0x02;
        self->field_0x04 = nodeB->field_0x02;
        func_804E0098(self->field_0x02);
        self->field_0x02 = self->field_0x00;
    } else {
        EffectNode* node = func_804E0114(self->field_0x02);
        s16 next = node->field_0x02;
        self->field_0x04 = next;
        self->field_0x06 = next;
        func_804E0098(self->field_0x02);
        self->field_0x02 = -1;
    }
    if (self->field_0x0a > 0) self->field_0x0a -= 1;
}

// Target 2: allocate a (count*4 mock) float table and fill it with a 4-term
// cubic basis across t in [-?] stepping by f8/count.
f32* func_804D5F54(void* this_, s32 count) {
    const void* mem = lbl_eu_8065FC18.field_0x04;
    u32 req = (u32)((u32)count << 4);
    if (req > getMaxAllocSize__Q23mtl10MemManagerFUl((u32)mem)) return 0;
    f32* buf = (f32*)allocate_array__Q23mtl10MemManagerFUlUl(req, (u32)mem);
    if (!buf) return 0;
    f32 f8 = lbl_eu_8066B158;
    f32 f5 = lbl_eu_8066B160;
    f32 f7 = lbl_eu_8066B164;
    f32 f1 = lbl_eu_8066B168;
    f32 t = lbl_eu_8066B15C;
    f32 dt = f8 / (f32)count;
    f32* p = buf;
    while (t < f8) {
        f32 a2 = t * t;
        f32 a3 = a2 * t;
        f32 g = f7 * a2;
        p[0] = f8 + f5 * a3 - g;
        p[1] = t + (a3 - f5 * a2);
        p[2] = a3 - a2;
        p[3] = f1 * a3 + g;
        t += dt;
        p += 4;
    }
    return buf;
}

void func_804D6070(void) {}

void func_804D6074(){}

s32 func_804D6BC0(void* unused, Node2Control* c, Node2** p5, Node2** p6,
               Node2** p7, Node2** p8, Node2** p9) {
    *p6 = *p5;
    *p6 = (*p5)->field_0x04;
    *p7 = *p5;
    *p8 = *p5;
    *p8 = (*p5)->field_0x00;
    *p9 = *p5;
    *p9 = (*p5)->field_0x00;
    *p9 = (*p5)->field_0x00->field_0x00;
    Node2* e = c->field_0x04;
    if (*p7 == e->field_0x00) *p6 = *p7;
    if (*p8 == e) {
        *p8 = *p7;
        *p9 = *p7;
        return 1;
    }
    if (*p9 == e) *p9 = *p8;
    return 0;
}

// Target 1: us-804d8184 - simple setter: stores two params at offsets 0x10/0x14
void func_804D4010(EffectStruct* self, u32 a, u32 b) {
    self->field_0x10 = a;
    self->field_0x14 = b;
}

// Target 2: us-804d22b4 - if field_0x1c <= 0.0f, clear bit 15 (0x8000) of field_0x06
// SOFT-CAP: MWCC optimizes wrapping mask 0xFFFF7FFF (rlwinm 17,15) to
// non-wrapping 0x00007FFF (rlwinm 17,31) because upper bits are zero after lhz.
// Semantically equivalent - same result for all inputs.
void func_804CE140(EffectScene* self) {
    if (lbl_eu_8066B0DC < self->field_0x1c) return;
    u32 tmp = self->field_0x06;
    tmp &= ~0x8000;
    self->field_0x06 = (u16)tmp;
}

// Target 3: us-804d1218 - if bits 0x0800 and 0x8000 of field_0x06 are set, add field_0x18 to field_0x10
extern "C" void func_804CD0A4(EffectStruct* self) {
    u32 flags = self->field_0x06;
    if (!((flags >> 11) & 1)) return;
    if (!((flags >> 15) & 1)) return;
    // field_0x10 is stored as u32 but used as float here
    *(f32*)&self->field_0x10 += self->field_0x18;
}

// Target 4: us-804d0414 - init: set first three fields to -1, rest to 0
void func_804CC2B8(EffectStruct* self) {
    self->field_0x00 = -1;
    self->field_0x02 = -1;
    self->field_0x04 = -1;
    self->field_0x06 = 0;
    self->field_0x328 = 0;
    self->field_0x32c = 0;
    self->field_0x330 = 0;
    self->field_0x334 = 0;
}

// Target 5: us-804d255c - if field_0x328 != 0 and field_0x18 > 0.0f, set bit 6 (0x40) of field_0x06
void func_804CE3E8(EffectScene* self) {
    if (self->field_0x328 == 0) return;
    if (self->field_0x18 <= lbl_eu_8066B0DC) return;
    self->field_0x06 |= 0x40;
}