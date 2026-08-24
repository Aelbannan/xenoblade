// Auto-scaffolded catalog TU for monolib/src/effect/code_804CC2B8
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include <math.h>

#include <revolution/GX.h>
#include <revolution/MTX.h>
#include "monolib/math/CVec3.hpp"
#include "monolib/math/CCol4.hpp"
#include "monolib/math/CMat44.hpp"
#include "monolib/math/Random.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/effect/CERand.hpp"
#include <new>
#include "monolib/effect/CETrail.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/effect/code_804CC2B8.hpp"

// Float constant 0.0f and the 0x4330000000000000 double magic, shared in this TU
extern f32 lbl_eu_8066B0DC;
extern f64 lbl_eu_8066B170;
// Quad-render scale/divisor constants and cross-TU helpers used by the
// func_804D01E0 / func_804D0AB4 render pair (retail C-linkage names).
extern f32 lbl_eu_8066B11C;
extern f32 lbl_eu_8066B128;
extern f32 lbl_eu_8066B130;
extern f32 lbl_eu_8066B134;
extern "C" u32 func_804EEACC(void* link);
extern "C" void* func_804E0168(s32 arg);
extern "C" void func_804DF164(void* res, s32 index, s32 cacheIndex, s32 wrap);
extern "C" void func_804BE4E0(void* dst, int index);
extern "C" void func_804D920C(ml::CMat34* out);
extern "C" void func_804D9274(Mtx mtx);
extern "C" void func_804D927C(Mtx mtx, const void* obj);
extern "C" void func_804D928C(Mtx mtx, const u8* obj);
extern f32 lbl_eu_8066A208;
extern f32 lbl_eu_8066B12C;
extern f32 lbl_eu_8066B138;
extern f32 lbl_eu_8066B120;
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

// Node view used by the quad-render functions (func_804D01E0 / func_804D0AB4):
// a position matrix at 0x00 and an RGBA color word at 0x30.
struct RenderNode {
    Mtx mtx;   // 0x00..0x2f
    u8 b0;     // 0x30
    u8 b1;
    u8 b2;
    u8 b3;
};

// Draw-payload structs shared (by layout) with code_804F0258.cpp; the draw TU
// owns the canonical definitions, these mirrors keep pointer compatibility.
struct DrawQuad {
    f32 x;         // 0x00
    f32 y;         // 0x04
    f32 z;         // 0x08
    f32 field_0xc; // 0x0c size-like
};
struct TexDrawSize {
    u8 pad_0x00[0x0c];
    f32 field_0x0c;
};
// Texture-matrix source payload built on the stack by the node-walk renderers
// (TexMtxSrc prefix plus trailing per-quad scale/color floats).
struct TexMtxSrc {
    void* mTex;     // 0x00 res pointer
    s8 mIndex;      // 0x04 texgen index (field_0x25a)
    u8 pad_0x05[3];
    u32 field_0x08; // 0x08
    f32 field_0x0c;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
};

// Draw entry points (defined in code_804F0258.cpp).
void func_804F0258(void* desktop, DrawQuad* quad, ml::CVec3* dir, TexDrawSize* size,
                   void* material, TexMtxSrc* mtxSrc);
void func_804F2E44(void* desktop, DrawQuad* quad, s32 mode, void* material,
                   TexMtxSrc* mtxSrc);
void func_804F1F18(void* desktop, ml::CVec3* pos, TexDrawSize* size, f32 alpha,
                   f32 alphaMax, void* material, TexMtxSrc* mtxSrc);
void func_804F0F2C(void* desktop, f32 alpha, const ml::CVec3* pos, const TexDrawSize* size,
                   const f32* color, void* material, TexMtxSrc* mtxSrc, const void* clamp);

// Light/material processor blocks driven by func_804CCA64 (one call per block).
extern "C" void func_804E1044(void* blk, void* src, void* mat, f32 v);
extern "C" void func_804E18CC(void* blk, void* src, void* mat, f32 v);
extern "C" void func_804E1A44(void* blk, void* src, void* mat, f32 v);
extern "C" void func_804E1C1C(void* blk, void* src, void* mat, f32 v);
extern "C" void func_804E2EAC(void* blk, void* src, void* mat, f32 v);
extern "C" void func_804E2088(void* blk, void* src, void* mat, f32 v);
extern "C" void func_804E2A5C(void* blk, void* src, void* mat, f32 v);
extern f32 lbl_eu_8066B0F8;

struct D3B14Cam;
s32 func_804D3B14(D3B14Cam* cam, Vec* outA, Vec* outB);

// Object behind EffectScene::field_0x32c whose slot-5 virtual applies the
// accumulated direction weight (called at the end of func_804CCA64).
struct CCA64VirtObj {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void apply(f32); // vtable +0x14
};

// Integer-word views of a Vec; retail copies vectors with lwz/stw pairs.
struct IVec3 {
    u32 x, y, z;
};
// Split word/double-word view: member-wise copy emits the pipelined
// load-x, load-y/z, store order seen in func_804CE418.
struct VecWords {
    u32 x;
    u64 yz;
};

// func_804CF700 helper imports (retail C-linkage names; no shared header
// declares them). Color scale + falloff constants and light-source helpers.
extern f32 lbl_eu_8066B114;
extern f32 lbl_eu_8066B118;
extern f32 lbl_eu_8066B11C;
extern "C" u32* func_804E2B54(void* a, void* b);
extern "C" void* func_804BE538(void* p);
extern "C" s32 func_804BE398(void* p, u32 a, u32 b, u32 c, f32 f1, f32 f2);

// Bitfield views of the u16 flag word at offset 0x06. MWCC allocates bitfields
// MSB-first, so the first declared member is bit 15 (0x8000). Reads emit the
// rotate-form rlwinm (extrwi) MWCC uses for bitfield access; clearing one emits
// the wrap mask.
struct EffectSceneFlags {
    u16 b15 : 1;   // 0x8000
    u16 : 15;
};
struct SceneFlagBits {
    u16 b15 : 1;   // bit 15
    u16 b14 : 1;   // 0x4000
    u16 : 2;       // bits 13, 12
    u16 b11 : 1;   // 0x0800
    u16 b10 : 1;   // 0x0400
    u16 b9 : 1;    // 0x0200
    u16 b8 : 1;    // 0x0100
    u16 b7 : 1;    // 0x0080
    u16 : 5;       // bits 6..2
    u16 b1 : 1;    // 0x0002
    u16 : 1;       // bit 0
};

// u32 view of the flag word pair at 0x04/0x06; the 0x06 halfword's bits are
// bits 16..31 of this container.
struct SceneFlags32 {
    u32 pad_04 : 16;
    u32 b16 : 1;
    u32 b17 : 1;
    u32 : 14;
};

// s32 -> float through the shared 0x4330000000000000 magic constant.
inline f32 s32ToF_b170(s32 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[1] = (u32)v ^ 0x80000000u;
    c.w[0] = 0x43300000u;
    return (f32)(c.d - lbl_eu_8066B170);
}

// s16 -> float through the magic constant 0x4330000080000000 (2^52 + 2^31,
// the signed-conversion pool entry at lbl_eu_8066B0F0). Retail func_804CD9EC
// references that named pool entry for its s16 tone-node conversions.
inline f32 s16ToF_b0f0(s16 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[1] = (u32)(s32)v ^ 0x80000000u;
    c.w[0] = 0x43300000u;
    return (f32)(c.d - lbl_eu_8066B0F0);
}

// u8/u32 -> float through the 2^52 magic constant (lbl_eu_8066B0E8).
// Used by func_804D3DB0's byte-scale fallback conversions.
inline f32 u8ToF_b0e8(u32 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[1] = v;
    c.w[0] = 0x43300000u;
    return (f32)(c.d - lbl_eu_8066B0E8);
}

// s32 -> float through the 2^52 + 2^31 magic constant at lbl_eu_8066B0F0.
// (func_804D3DB0 converts its i5/i6 params with this named entry.)
inline f32 s32ToF_b0f0(s32 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[1] = (u32)v ^ 0x80000000u;
    c.w[0] = 0x43300000u;
    return (f32)(c.d - lbl_eu_8066B0F0);
}

// u16 -> float through the shared 0x4330000000000000 magic constant.
inline f32 u16ToF_b0e8(u16 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[1] = v;
    c.w[0] = 0x43300000u;
    return (f32)(c.d - lbl_eu_8066B0E8);
}

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
};

// Sub-object referenced by field_0x328: its dispatch pointer sits at
// +0x184 (base padding followed by the derived object's own vptr).
struct EffectSubBase {
    u8 pad_0x00[0x184];
};

struct EffectVirtObj : EffectSubBase {
    virtual void release(int);
};

// Retail linker name __dt__804CC2E4 (unmangled C symbol); defined as a free
// function so the object emits the exact retail name.
extern "C" void __attribute__((never_inline)) __dt__804CC2E4(EffectStruct* self) {
    void* saved = self->field_0x32c;
    if (saved != 0) {
        // Two-arg call form keeps the checked pointer live in r4 across the
        // compare (retail passes it through as a spare argument).
        ((void (*)(void*, void*))func_80495E84)(
            *(void**)((u8*)self->field_0x08 + 0x10), saved);
        self->field_0x32c = 0;
    }
    if (self->field_0x328) {
        if (self->field_0x328) {
            ((EffectVirtObj*)self->field_0x328)->release(1);
        }
        self->field_0x328 = 0;
    }
    if (self->field_0x330) {
        __dt__804D80F0(self->field_0x330, 1);
        self->field_0x330 = 0;
    }
    if (self->field_0x334) {
        __dl__FPv(self->field_0x334);
        self->field_0x334 = 0;
    }
    self->field_0x00 = -1;
    self->field_0x02 = -1;
    self->field_0x04 = -1;
    self->field_0x06 = 0;
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

// C-ABI light driver defined in CETrail.cpp (its light/param types are defined
// there). Local forward declarations kept here because this TU cannot include
// the CETrail header from its current writable scope; move to CETrail.hpp
// (or a shared header) when scope allows.
struct CETrailLight;
struct CETrailLightParam;
extern "C" void func_804D8160(CETrailLight* self, void* arg, s32 mode,
                              const CETrailLightParam* p, f32 f1, f32 f2);

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
extern "C" s32 func_804CDF20(void* self, Vec* a, Vec* b, Vec* c);

// Per-node quad emit used by the type-0xc walker (matrix/color payload draw).
extern "C" void func_804EEB40(void* matId, Vec* a, void* color, Vec* b, s32 flagA,
                               s32 flagB, TexMtxSrc* src, f32 f2, f32 f3);

extern "C" void func_804CD0A4(EffectStruct* self) DECOMP_DONT_INLINE;

// Larger scene/effect layout used by targets 3, 8, 9, 10. Only fields touched
// by the reconstructed functions are declared.
// Object behind EffectScene::field_0x08 for func_804CF700: a u16 flag word,
// a pointer at 0x10 and a base RGBA color at 0x40.
struct CF700Obj {
    u16 field_0x00;             // flags (0x4000 gate bit 14)
    u8 pad_0x02[0x10 - 0x02];
    void* field_0x10;
    u8 pad_0x14[0x40 - 0x14];
    f32 field_0x40;             // base color R
    f32 field_0x44;             // G
    f32 field_0x48;             // B
    f32 field_0x4c;             // A
};

// Tone-range table referenced by SceneSubObj::field_0xdc (u16 weights).
struct ToneRange {
    u16 field_0x00;
    u16 field_0x02;
    u16 field_0x04;
    u16 field_0x06;
};

struct SceneSubObj {
    u32 field_0x00;              // 0x00
    u32 field_0x04;              // 0x04
    u8 pad_0x08[0x1c - 0x08];
    u16 field_0x1c;              // 0x1c flags
    u8 pad_0x1e[0x2e - 0x1e];
    s16 field_0x2e;              // 0x2e node count (func_804D0AB4 loop bound)
    u8 pad_0x30[0x40 - 0x30];
    u32 field_0x40;              // 0x40
    u32 field_0x44;              // 0x44
    u8 pad_0x48[0xdc - 0x48];
    ToneRange* field_0xdc;       // 0xdc tone-range table
    u8 pad_0xe0[0xe4 - 0xe0];
    u32 field_0xe4;              // 0xe4
    u32 field_0xe8;              // 0xe8
    u8 pad_0xec[0xec - 0xec];
    u8* field_0xec;              // 0xec
    u8 pad_0xf0[0xf4 - 0xf0];
    u32 field_0xf4;              // 0xf4 pointer (func_804CCA64 gate table)
    u32 field_0xf8;              // 0xf8
    u32 field_0xfc;              // 0xfc
    u32 field_0x100;             // 0x100 pointer to a sample/tone table
    u32 field_0x104;             // 0x104
    u32 field_0x108;             // 0x108 pointer used by target 6
    u32 field_0x10c;             // 0x10c light-check object
    u32 field_0x110;             // 0x110 tone-table pointer checked by target 2
    void* field_0x114;           // 0x114 material-data pointer (func_804CF700)
    u32 field_0x118;             // 0x118 orientation source
    u8 pad_0x11c[0x194 - 0x11c];
    f32 field_0x194;             // tone copy block source (func_804CCA64)
    u8 field_0x198;
    u8 field_0x199;
    u8 field_0x19a;
    u8 field_0x19b;
    u8 pad_0x19c[0x1a0 - 0x19c];
    u32 field_0x1a0;
    u32 field_0x1a4;
    u32 field_0x1a8;
    u32 field_0x1ac;
    u32 field_0x1b0;
    u32 field_0x1b4;
    u32 field_0x1b8;
    u32 field_0x1bc;
    u32 field_0x1c0;
    u8 pad_0x1c4[0x204 - 0x1c4];
    f32 field_0x204;             // second copy block source float
    u8 field_0x208;
    u8 field_0x209;
    u8 field_0x20a;
    u8 field_0x20b;
    u32 field_0x210;
    u32 field_0x214;
    u32 field_0x218;
    u32 field_0x21c;
    u32 field_0x220;
    f32 field_0x224;
    f32 field_0x228;
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
    union {
        struct {
            u16 field_0x04;
            u16 field_0x06;      // flags bitfield
        };
        struct {
            u32 : 16;
            u32 : 1;
            u32 b17 : 1;         // 0x06 bit 1, in a 0x04-based u32 container
            u32 : 14;
        };
    };
    u32 field_0x08;
    SceneSubObj* field_0x0c; // pointer to a sub-object
    u32 field_0x10;          // also read/written as f32 (union)
    u32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    u8 pad_0x20[0xa0 - 0x20];
    Vec field_0xa0;          // 0xa0,0xa4,0xa8
    Vec field_0xac;          // 0xac,0xb0,0xb4
    ml::CVec3 field_0xb8;    // 0xb8,0xbc,0xc0 (CVec3: PS-vector accumulate target)
    Vec field_0xc4;          // 0xc4,0xc8,0xcc
    Vec field_0xd0;          // 0xd0,0xd4,0xd8
    SceneSubLater field_0xdc; // 0xdc..0x124 sub-object (matrix at +0x18 = 0xf4)
    Vec field_0x124;         // 0x124,0x128,0x12c
    Mtx field_0x130;         // 0x130..0x160
    Mtx field_0x160;         // 0x160..0x190
    Vec field_0x190;         // 0x190,0x194,0x198
    Mtx field_0x19c;         // 0x19c..0x1cc
    f32 field_0x1cc;         // 0x1cc color R
    f32 field_0x1d0;         // 0x1d0
    f32 field_0x1d4;         // 0x1d4
    f32 field_0x1d8;         // 0x1d8
    Mtx field_0x1dc;         // 0x1dc..0x20c
    void* field_0x20c;       // light-processor block A
    u8 field_0x210;
    u8 pad_0x211[0x21c - 0x211];
    Vec field_0x21c;         // 0x21c,0x220,0x224
    u8 pad_0x228[0x234 - 0x228];
    f32 field_0x234;         // 0x234
    void* field_0x238;       // light-processor block B
    u8 field_0x23c;
    u8 pad_0x23d[0x24c - 0x23d];
    void* field_0x24c;       // light-processor block C
    u8 field_0x250;
    u8 pad_0x251[0x258 - 0x251];
    s16 field_0x258;
    s16 field_0x25a;
    void* field_0x25c;       // light-processor block D
    u8 field_0x260;          // 0x260 texgen flags
    u8 pad_0x261[0x268 - 0x261];
    f32 field_0x268;         // 0x268
    f32 field_0x26c;
    f32 field_0x270;
    f32 field_0x274;
    void* field_0x278;           // 0x278 tone-copy dest / processor block
    u8 field_0x27c;
    u8 field_0x27d;
    u8 field_0x27e;
    u8 field_0x27f;
    f32 field_0x280;
    u32 field_0x284;
    u32 field_0x288;
    u32 field_0x28c;
    u32 field_0x290;
    u32 field_0x294;
    u32 field_0x298;
    u32 field_0x29c;
    u32 field_0x2a0;
    u32 field_0x2a4;
    void* field_0x2a8;           // 0x2a8 anim block (func_804E214C target)
    u8 pad_0x2ac[0x2b0 - 0x2ac];
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
    u8 pad_0x2dc[0x2e8 - 0x2dc];
    u32 field_0x2e8;             // 0x2e8 tone-copy dest 2 / processor input
    u8 field_0x2ec;
    u8 pad_0x2ed[0x2f4 - 0x2ed];
    u32 field_0x2f4;
    u32 field_0x2f8;
    u32 field_0x2fc;
    u32 field_0x300;
    u32 field_0x304;
    f32 field_0x308;
    f32 field_0x30c;
    void* field_0x310;           // processor block E
    u8 field_0x314;              // 0x314 scale-mode flags
    u8 pad_0x315[0x31c - 0x315];
    f32 field_0x31c;             // 0x31c
    f32 field_0x320;
    f32 field_0x324;
    void* field_0x328;
    void* field_0x32c;
    void* field_0x330;
    void* field_0x334;
};

// Retail: lwz r3,0(r3); blr  - returns the node/scene type from field_0x00.
extern "C" s32 __attribute__((never_inline)) func_804CC800(const SceneSubObj* self) { return self->field_0x00; }

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
    u8 pad_0x02[0x0c - 0x02];
    void* field_0x0c;
    void* field_0x10;
    u8 pad_0x14[0x34 - 0x14];
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
};
// MSB-first bitfield view of MatFlags.field_0x00. b11 = 0x0800 is the
// "apply material scale" gate read by func_804CD9EC / func_804CE79C.
struct MatFlagBits {
    u16 b15 : 1;
    u16 b14 : 1;
    u16 b13 : 1;
    u16 b12 : 1;
    u16 b11 : 1;    // bit 11 (0x0800)
    u16 : 11;
};

// Target 5: accumulate a scaled tone/dir vector into field_0xb8.
extern "C" void __attribute__((never_inline)) func_804CD9EC(EffectScene* self) {
    SceneSubObj* sub = self->field_0x0c;
    ToneNode* t = (ToneNode*)sub->field_0x100;
    if (t) {
        f32 scale = (t->field_0x08 / lbl_eu_8066B100) * self->field_0x18;
        if (scale != lbl_eu_8066B0DC) {
            ml::CVec3 v;
            union { double d; u32 w[2]; } c1, c2, c3;
            c1.w[1] = (u32)(s32)t->field_0x00 ^ 0x80000000u;
            c1.w[0] = 0x43300000u;
            v.x = scale * (f32)(c1.d - lbl_eu_8066B0F0);
            c2.w[1] = (u32)(s32)t->field_0x02 ^ 0x80000000u;
            c2.w[0] = 0x43300000u;
            v.y = scale * (f32)(c2.d - lbl_eu_8066B0F0);
            c3.w[1] = (u32)(s32)t->field_0x04 ^ 0x80000000u;
            c3.w[0] = 0x43300000u;
            v.z = scale * (f32)(c3.d - lbl_eu_8066B0F0);
            if (t->field_0x06 == 0) {
                nw4r::math::VEC3Scale((nw4r::math::VEC3*)&v, (nw4r::math::VEC3*)&v,
                                      *(const f32*)&self->field_0x10);
            }
            // retail materializes (bit != 0) as -bit|bit >> 31 (neg/or/srwi)
            MatFlags* m = (MatFlags*)self->field_0x08;
            u16 fl = m->field_0x00;
            u32 bit = (fl >> 11) & 1;
            u32 cond = (u32)(-bit | bit) >> 31;
            if (cond) {
                v.x *= m->field_0x34;
                v.y *= m->field_0x38;
                v.z *= m->field_0x3c;
            }
            nw4r::math::VEC3Add((nw4r::math::VEC3*)&self->field_0xb8,
                                (nw4r::math::VEC3*)&self->field_0xb8,
                                (nw4r::math::VEC3*)&v);
        }
    }
}

// Target 9: pull a sub-object's aligned direction, scale it and fold into b8.
extern "C" void __attribute__((never_inline)) func_804CDB2C(EffectScene* self, Vec* p1) {
    EffectNode* node;
    Vec diff;
    ml::CVec3 staged;
    ml::CVec3 nrm;
    Vec res;
    ml::CVec3 dir;
    ml::CVec3 scaled;
    SceneSubObj* sub = self->field_0x0c;
    u32 cls = sub->field_0x00;
    if ((u32)(cls - 9) <= 2) return;
    u8* pf = (u8*)sub->field_0xf8;
    if (!pf) return;
    s32 r = pf ? *(volatile u8*)(pf - 0xf) : 0;
    if (self->field_0x18 <= lbl_eu_8066B0DC) r = 0;
    if (*(const f32*)&self->field_0x10 <= lbl_eu_8066B0D8) r = 3;
    func_804EE60C(&self->field_0xdc);
    if ((r & 0xFF) != 0) {
        node = func_804E0114((s16)self->field_0x04);
        if (r & 2) func_804EE658(&self->field_0xdc, node);
        if (r & 1) func_804EE8FC(&self->field_0xdc, node);
    }
    f32 f31 = self->field_0x234 * self->field_0x18;
    if (f31 == lbl_eu_8066B0DC) return;
    PSMTXMultVec(self->field_0xdc.field_0x18, &self->field_0x21c, &res);
    diff.x = res.x;
    diff.y = res.y;
    diff.z = res.z;
    s32 same = 0;
    if (res.x == p1->x) {
        if (res.y == p1->y) {
            if (res.z == p1->z) {
                same = 1;
            }
        }
    }
    if (same) return;
    // Direct PS subtract (retail has no operator- temp here).
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&dir,
                        (const nw4r::math::VEC3*)&diff,
                        (const nw4r::math::VEC3*)p1);
    nrm = dir;
    f32 d2 = dir.y * dir.y + dir.x * dir.x + dir.z * dir.z;
    if (d2 == lbl_eu_8066B0DC) {
        nrm = ml::CVec3::zero;
    } else {
        PSVECNormalize((Vec*)&nrm, (Vec*)&nrm);
    }
    scaled.x = nrm.x * f31;
    scaled.y = nrm.y * f31;
    scaled.z = nrm.z * f31;
    // Retail stages the scaled vector through a scratch row before the
    // paired-single accumulate.
    staged.x = scaled.x;
    staged.y = scaled.y;
    staged.z = scaled.z;
    nw4r::math::VEC3Add((nw4r::math::VEC3*)&self->field_0xb8,
                        (const nw4r::math::VEC3*)&self->field_0xb8,
                        (const nw4r::math::VEC3*)&staged);
}

// Source object for target 10: a Vec at 0, a Vec at 0xc, a Vec at 0x18 and two
// input matrices at 0x24 and 0x54 (retail copies both 48-byte blocks out).
struct SrcMtx {
    Vec field_0x00;
    Vec field_0x0c;
    Vec field_0x18;
    Mtx field_0x24;
    Mtx field_0x54;
};

// 48-byte copyable wrapper over a matrix as six named u64 members; MWCC
// emits per-pair lwz/stw chunks (hi-before-lo) for member-wise 64-bit copies.
typedef union MtxCopy {
    Mtx m;
    u64 w0, w1, w2, w3, w4, w5;
} MtxCopy;
// Target 10: copy + invert a source matrix, transform several source vectors
// and stash them across the scene, then feed the sub-object helper and zero the
// local dir vector.
void func_804CC808(EffectScene* self, const SrcMtx* src) {
    // Two unrolled 48-byte matrix copies (retail emits 6 u64 lwz/stw chunks
    // each; whole-struct copies make MWCC materialize in registers instead).
    *(MtxCopy*)&self->field_0x130 = *(const MtxCopy*)&src->field_0x24;
    *(MtxCopy*)&self->field_0x160 = *(const MtxCopy*)&src->field_0x54;
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
    u32 c = pf ? (u32)(pf - 8) : 0;
    u32 b = pf ? *(u8*)(pf - 0x10) : 0;
    func_804EE558(&self->field_0xdc, self->field_0x08, b, c, 0);
    self->field_0xb8 = ml::CVec3::zero;
    self->field_0x06 = (u16)(self->field_0x06 | 0x800);
}

// Forward decls for the target-3 frame pipeline. The retail functions are
// unmangled linker names, so they need C linkage (the map names them func_*).
extern "C" {
void __attribute__((never_inline)) func_804CE418(EffectScene* self, Vec* out);
void __attribute__((never_inline)) func_804CD0CC(EffectScene* self);
void __attribute__((never_inline)) func_804CD9EC(EffectScene* self);
void __attribute__((never_inline)) func_804CDB2C(EffectScene* self, Vec* p1);
void __attribute__((never_inline)) func_804CDD78(EffectScene* self, const Vec* p2, const Vec* p3);
void __attribute__((never_inline)) func_804CDE50(EffectScene* self, const Vec* p2, const Vec* p3);

void __attribute__((never_inline)) func_804CE160(EffectScene* self, const ml::CVec3* a, const ml::CVec3* b);
void __attribute__((never_inline)) func_804CE140(EffectScene* self);
void __attribute__((never_inline)) func_804CE264(EffectScene* self, Vec* p1, Vec* p3);
void __attribute__((never_inline)) func_804CE388(EffectScene* self, Vec* p);
void __attribute__((never_inline)) func_804CE3E8(EffectScene* self);
void __attribute__((never_inline)) func_804CFBC8(EffectScene* self, class CFBC8Target* obj,
                                                 const ml::CMat34* a, Vec* d, const f32* c,
                                                 bool flag);
}

// Target 3: process a frame of the firework/placer scene if the b11 flag is set.
void func_804CCF84(EffectScene* self) {
    if (!((SceneFlagBits*)&self->field_0x06)->b11) return;
    s32 type = (s32)func_804CC800(self->field_0x0c);
    Vec stackA;
    Vec stackB;
    func_8004B0B0(&stackA);
    func_8004B0B0(&stackB);
    ((SceneFlagBits*)&self->field_0x06)->b9 = 0;
    func_804CE418(self, &stackB);
    func_804CD0CC(self);
    func_804CD9EC(self);
    func_804CDB2C(self, &stackB);
    func_804CE418(self, &stackA);
    if (lbl_eu_8066B0D8 < *(const f32*)&self->field_0x10) {
        func_804CDD78(self, &stackB, &stackA);
        func_804CDE50(self, &stackB, &stackA);
    }
    if (!((SceneFlagBits*)&self->field_0x06)->b14) {
        func_804CE140(self);
    }
    func_804CE160(self, (const ml::CVec3*)&stackB, (const ml::CVec3*)&stackA);
    func_804CE264(self, &stackA, &self->field_0x190);
    if (type == 0xd) func_804CE388(self, &stackA);
    if (type == 0x8) func_804CE3E8(self);
}

extern "C" void __attribute__((never_inline)) func_804CD0CC(EffectScene* self){}

extern "C" void __attribute__((never_inline)) func_804CDE50(EffectScene* self, const Vec* p2, const Vec* p3) {
    SceneSubObj* sub = self->field_0x0c;
    u32 cls = sub->field_0x00;
    if ((u32)(cls - 9) <= 2) return;
    u8* fo = (u8*)sub->field_0xf8;
    if (fo == 0) return;
    u8 b = fo ? *(u8*)(fo - 0xd) : 0;
    if (!b) return;
    Vec res;
    Vec out;
    PSMTXMultVec(self->field_0xdc.field_0x18, &self->field_0x21c, &out);
    res.x = out.x;
    res.y = out.y;
    res.z = out.z;
    if (!func_804CDF20((void*)self, (Vec*)p2, (Vec*)p3, &res)) return;
    // Volatile load/store pair pins the flag update between the constant
    // load and the final float store (retail order: lhz, lfs, rlwinm, sth,
    // stfs).
    u16 flags = *(volatile u16*)&self->field_0x06;
    f32 z = lbl_eu_8066B0DC;
    *(volatile u16*)&self->field_0x06 = (u16)(flags & ~0x4000);
    *(volatile f32*)&self->field_0x1c = z;
}

// Segment-vs-point containment: when a == b the segment degenerates to the
// point a and the test is a radius check (squared distance vs threshold).
// Otherwise pick the dominant axis of |b - a| and range-test c along it.
extern "C" s32 __attribute__((never_inline)) func_804CDF20(void* self, Vec* a, Vec* b, Vec* c) {
    s32 same = 0;
    if (a->x == b->x && a->y == b->y && a->z == b->z) {
        same = 1;
    }
    if (same) {
        // PS-vector subtract plus an element-wise copy keeps retail's paired
        // single diff block and the member copy before the squared-distance sum.
        ml::CVec3 diff = *(const ml::CVec3*)a - *(const ml::CVec3*)c;
        ml::CVec3 t;
        t.x = diff.x;
        t.y = diff.y;
        t.z = diff.z;
        return t.x * t.x + t.y * t.y + t.z * t.z <= lbl_eu_8066B104;
    }
    // fsubs/fabs/frsp shape comes from the double-width builtin here.
    f32 adx = (f32)__fabs(b->x - a->x);
    f32 ady = (f32)__fabs(b->y - a->y);
    f32 adz = (f32)__fabs(b->z - a->z);
    if (ady < adx) {
        if (adz < adx) {
            // X dominant
            if (a->x <= c->x && c->x <= b->x) return 1;
            if (b->x <= c->x && c->x <= a->x) return 1;
            return 0;
        }
    } else if (adz < ady) {
        // Y dominant
        if (a->y <= c->y && c->y <= b->y) return 1;
        if (b->y <= c->y && c->y <= a->y) return 1;
        return 0;
    }
    // Z dominant
    if (a->z <= c->z && c->z <= b->z) return 1;
    if (b->z <= c->z && c->z <= a->z) return 1;
    return 0;
}

extern "C" void __attribute__((never_inline)) func_804CDD78(EffectScene* self, const Vec* p2, const Vec* p3) {
    if (((SceneFlagBits*)&self->field_0x06)->b8) return;
    Vec res;
    Vec out;
    PSMTXMultVec(self->field_0x160, &self->field_0xc4, &out);
    res = out;
    if (!func_804CDF20((void*)self, (Vec*)p2, (Vec*)p3, &res)) return;
    self->field_0x06 = (u16)(self->field_0x06 | 0x300);
    u32* sub = (u32*)self->field_0x0c;
    u8* fo = (u8*)(sub[0x3b]);
    if (fo == 0) return;
    s32 v = (fo != 0) ? *(u8*)(fo - 0xa) : 0;
    if (v == 0) return;
    // Volatile load/store pair pins the constant float load ahead of the flag
    // update (retail order: lhz, lfs, rlwinm, sth, stfs); clears 0x4000.
    u16 flags = *(volatile u16*)&self->field_0x06;
    f32 z = lbl_eu_8066B0DC;
    *(volatile u16*)&self->field_0x06 = (u16)(flags & ~0x4000);
    *(volatile f32*)&self->field_0x1c = z;
}

// Forward decls used by func_804CE264 and func_804CCF84.
extern "C" {
u32 __attribute__((never_inline)) func_804CE378(const SceneSubObj* self);
u32 __attribute__((never_inline)) func_804CE380(const SceneSubObj* self);
void __attribute__((never_inline)) func_804CE4C0(EffectScene* self, Mtx* a, u32 type, Vec* p3);
void __attribute__((never_inline)) func_804CE9A4(EffectScene* self, Mtx* a, Mtx* b, u32 type, Vec* p1,
                   Mtx* spA, Vec* spB);
void __attribute__((never_inline)) func_804CF700(EffectScene* self, f32* a, Vec* p1);
void __attribute__((never_inline)) func_804CFBC8(EffectScene* self, class CFBC8Target* obj,
                                                 const ml::CMat34* a, Vec* spB, const f32* c,
                                                 bool flag);
void __attribute__((never_inline)) func_804CE79C(EffectScene* self, Vec* out, u32 type);
}

// TK func_804CE264: builds a stack matrix + vector, feeds child position/type
// helpers, sets a flag bit from field_0x1d8, then posts updates.
extern "C" void __attribute__((never_inline)) func_804CE264(EffectScene* self, Vec* p1, Vec* p3) {
    u32 type = func_804CE378(self->field_0x0c);
    Mtx stackA;
    Vec stackB;
    func_8006BEC0((Vec*)&stackA);
    func_8004B0B0(&stackB);
    func_804CE4C0(self, &stackA, type, p3);
    func_804CE79C(self, &stackB, type);
    func_804CF700(self, &self->field_0x1cc, p1);
    if (self->field_0x1d8 != lbl_eu_8066B0DC)
        self->field_0x06 |= 0x400;
    else
        self->field_0x06 &= (u16)~0x400;
    func_804CE9A4(self, &self->field_0x19c, &self->field_0x1dc, type, p1,
                  &stackA, &stackB);
    if (self->field_0x32c) {
        func_804CFBC8(self, (CFBC8Target*)self->field_0x32c,
                      (const ml::CMat34*)&self->field_0x19c, &stackB,
                      (const f32*)&self->field_0x1cc,
                      func_804CE380((const SceneSubObj*)self->field_0x08) != 0);
    }
}

// Target 2: identity-fill then per-type axis/rotation setup from the sub-object.
extern "C" void __attribute__((never_inline)) func_804CE4C0(EffectScene* self, Mtx* a, u32 type, Vec* p3) {
    Mtx& out = *a;
    // Direct global→out copy; retail emits the chunked lwz/stw pair pattern.
    *(ml::CMat34*)&out = ml::CMat34::identity;
    switch (type) {
    case 0:
        if (self->field_0x2d8 != lbl_eu_8066B0DC) func_804DD4F8(&out);
        break;
    case 1:
        func_804DB980(&out, (const Vec*)&self->field_0x2d0,
                      *(s32*)((u8*)self->field_0x0c + 0x48));
        break;
    case 2: {
        Vec v;
        v.x = self->field_0x2d0;
        v.y = lbl_eu_8066B110 + self->field_0x2d4;
        v.z = self->field_0x2d8;
        func_804DB980(&out, &v, *(s32*)((u8*)self->field_0x0c + 0x48));
        // Retail computes the Y angle first, then X from sqrt (source order).
        v.y = atan2(p3->x, p3->z);
        v.z = lbl_eu_8066B0DC;
        v.x = -atan2(p3->y, sqrt(p3->x * p3->x + p3->z * p3->z));
        Mtx m2;
        func_804DCA88(&m2, &v);
        Mtx conc;
        PSMTXConcat(m2, out, conc);
        *(MtxCopy*)&out = *(MtxCopy*)&conc;
        break;
    }
    case 3: {
        Vec v;
        // Retail copies the 0x2d0 block as raw words here (not floats).
        *(u32*)&v.x = *(u32*)&self->field_0x2d0;
        *(u32*)&v.y = *(u32*)&self->field_0x2d4;
        *(u32*)&v.z = *(u32*)&self->field_0x2d8;
        func_804DB980(&out, &v, *(s32*)((u8*)self->field_0x0c + 0x48));
        // Retail computes the Y angle first, then X from sqrt (source order).
        v.y = atan2(p3->x, p3->z);
        v.z = lbl_eu_8066B0DC;
        v.x = -atan2(p3->y, sqrt(p3->x * p3->x + p3->z * p3->z));
        Mtx m2;
        func_804DCA88(&m2, &v);
        Mtx conc;
        PSMTXConcat(m2, out, conc);
        *(MtxCopy*)&out = *(MtxCopy*)&conc;
        break;
    }
    default:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        break;
    }
}
extern "C" void __attribute__((never_inline)) func_804CE9A4(EffectScene* self, Mtx* a, Mtx* b, u32 type, Vec* p1,
                   Mtx* spA, Vec* spB) {
    // Orientation/position update for a node: builds the angle matrix from the
    // light-check vector, per-type basis matrix, concatenates the parent
    // matrix (skipped when it is still the identity), and finally produces the
    // two output matrices a (local) and b (parent-combined).
    SceneSubObj* sub = self->field_0x0c;
    u32 tp = type;
    u16 sflags = sub->field_0x1c;
    if (sflags & 0x0800) {
        tp = (sflags & 0x0400) ? 8 : 2;
    }
    s32 ok = 0;
    Mtx mtx1d0;
    if (sub->field_0x10c != 0 && sub->field_0x110 != 0) {
        u8 b = ((u8*)sub->field_0x10c) ? *(u8*)((u8*)sub->field_0x10c - 0x1a) : 0;
        if (b != 0) {
            f32 f0 = sub->field_0x110 ? *(f32*)((u8*)sub->field_0x110 - 0x18) : lbl_eu_8066B0D8;
            if (func_804BE398(p1, 0x4801, 0, 0, -f0, lbl_eu_8066B0DC) != 0) {
                Vec v8;
                func_804BE4E0(&v8, 0);
                Vec ang;
                ang.y = atan2(v8.x, v8.z);
                f32 r = sqrt(v8.x * v8.x + v8.z * v8.z);
                ang.x = lbl_eu_8066B110 - atan2(v8.y, r);
                ang.z = lbl_eu_8066B0DC;
                func_804DCA88(&mtx1d0, &ang);
                ok = 1;
            }
        }
    }
    Mtx mtx1a0;
    s32 mtxFlag = 0;
    if (tp == 0 || tp == 6) {
        void* obj = func_80496264(*(void**)((u8*)self->field_0x08 + 0x10), -1);
        func_804D927C(mtx1a0, obj);
        mtxFlag = 1;
    } else if (tp == 1 || tp == 7) {
        void* obj = func_80496264(*(void**)((u8*)self->field_0x08 + 0x10), -1);
        func_804D928C(mtx1a0, (const u8*)obj);
        mtxFlag = 1;
    }
    s32 keep = 1;
    if (ok == 0 && mtxFlag == 0) {
        u32 t4 = tp - 4;
        if (t4 > 6 || ((1u << t4) & 0x43) == 0) {
            keep = 0;
        }
    }
    s32 r29 = 1;
    if (keep == 0 && sub->field_0x00 != 1) r29 = 0;
    switch (tp) {
    case 0:
    case 1:
    case 2:
    case 6:
    case 7:
    case 8:
        func_804D920C((ml::CMat34*)a);
        break;
    case 3:
    case 4:
    case 5:
    case 9:
    case 10:
        func_804D9274(*a);
        break;
    default:
        break;
    }
    PSMTXScaleApply(*a, *a, spB->x, spB->y, spB->z);
    if (self->field_0x314 & 0x60) {
        Vec scale;
        scale.x = self->field_0x31c;
        scale.y = self->field_0x320;
        scale.z = self->field_0x324;
        u8* p118 = (u8*)sub->field_0x118;
        u8 b = p118 ? *(p118 - 4) : 0;
        if (b != 0) {
            scale.x *= spB->x;
            scale.y *= spB->y;
            scale.z *= spB->z;
        } else {
            MatFlags* mat = (MatFlags*)self->field_0x08;
            if (mat->field_0x00 & 0x0800) {
                u8* pe4 = (u8*)sub->field_0xe4;
                u8 b2 = pe4 ? *(pe4 - 0xe) : 0;
                if (b2 != 0xb) {
                    scale.x *= mat->field_0x34;
                    scale.y *= mat->field_0x38;
                    scale.z *= mat->field_0x3c;
                }
            }
        }
        (*a)[0][3] += scale.x;
        (*a)[1][3] += scale.y;
        (*a)[2][3] += scale.z;
    }
    const f32 eps = lbl_eu_8066A208;
    bool spAisId = __fabs((*spA)[0][0] - ml::CMat34::identity.m[0][0]) <= eps &&
                   __fabs((*spA)[0][1] - ml::CMat34::identity.m[0][1]) <= eps &&
                   __fabs((*spA)[0][2] - ml::CMat34::identity.m[0][2]) <= eps &&
                   __fabs((*spA)[0][3] - ml::CMat34::identity.m[0][3]) <= eps &&
                   __fabs((*spA)[1][0] - ml::CMat34::identity.m[1][0]) <= eps &&
                   __fabs((*spA)[1][1] - ml::CMat34::identity.m[1][1]) <= eps &&
                   __fabs((*spA)[1][2] - ml::CMat34::identity.m[1][2]) <= eps &&
                   __fabs((*spA)[1][3] - ml::CMat34::identity.m[1][3]) <= eps &&
                   __fabs((*spA)[2][0] - ml::CMat34::identity.m[2][0]) <= eps &&
                   __fabs((*spA)[2][1] - ml::CMat34::identity.m[2][1]) <= eps &&
                   __fabs((*spA)[2][2] - ml::CMat34::identity.m[2][2]) <= eps &&
                   __fabs((*spA)[2][3] - ml::CMat34::identity.m[2][3]) <= eps;
    if (!spAisId) {
        Mtx m170;
        PSMTXConcat(*spA, *a, m170);
        *(MtxCopy*)a = *(MtxCopy*)&m170;
    }
    if (ok) {
        bool mId = __fabs(mtx1d0[0][0] - ml::CMat34::identity.m[0][0]) <= eps &&
                   __fabs(mtx1d0[0][1] - ml::CMat34::identity.m[0][1]) <= eps &&
                   __fabs(mtx1d0[0][2] - ml::CMat34::identity.m[0][2]) <= eps &&
                   __fabs(mtx1d0[0][3] - ml::CMat34::identity.m[0][3]) <= eps &&
                   __fabs(mtx1d0[1][0] - ml::CMat34::identity.m[1][0]) <= eps &&
                   __fabs(mtx1d0[1][1] - ml::CMat34::identity.m[1][1]) <= eps &&
                   __fabs(mtx1d0[1][2] - ml::CMat34::identity.m[1][2]) <= eps &&
                   __fabs(mtx1d0[1][3] - ml::CMat34::identity.m[1][3]) <= eps &&
                   __fabs(mtx1d0[2][0] - ml::CMat34::identity.m[2][0]) <= eps &&
                   __fabs(mtx1d0[2][1] - ml::CMat34::identity.m[2][1]) <= eps &&
                   __fabs(mtx1d0[2][2] - ml::CMat34::identity.m[2][2]) <= eps &&
                   __fabs(mtx1d0[2][3] - ml::CMat34::identity.m[2][3]) <= eps;
        if (!mId) {
            Mtx m140;
            PSMTXConcat(mtx1d0, *a, m140);
            *(MtxCopy*)a = *(MtxCopy*)&m140;
        }
    }
    if (mtxFlag) {
        Mtx m110;
        PSMTXConcat(mtx1a0, *a, m110);
        *(MtxCopy*)a = *(MtxCopy*)&m110;
    }
    if (r29) {
        Mtx mB0;
        *(MtxCopy*)&mB0 = *(MtxCopy*)&self->field_0x130;
        mB0[0][3] = lbl_eu_8066B0DC;
        mB0[1][3] = lbl_eu_8066B0DC;
        mB0[2][3] = lbl_eu_8066B0DC;
        PSMTXInverse(mB0, mB0);
        Mtx mE0;
        PSMTXConcat(mB0, *a, mE0);
        *(MtxCopy*)a = *(MtxCopy*)&mE0;
    }
    (*a)[0][3] += self->field_0xac.x;
    (*a)[1][3] += self->field_0xac.y;
    (*a)[2][3] += self->field_0xac.z;
    Mtx m80;
    PSMTXConcat(self->field_0x130, *a, m80);
    *(MtxCopy*)a = *(MtxCopy*)&m80;
    if (r29) {
        Mtx m50;
        PSMTXConcat(self->field_0x130, *spA, m50);
        *(MtxCopy*)b = *(MtxCopy*)&m50;
    } else {
        *(MtxCopy*)b = *(MtxCopy*)spA;
    }
    (*b)[0][3] = (*a)[0][3];
    (*b)[1][3] = (*a)[1][3];
    (*b)[2][3] = (*a)[2][3];
}
extern "C" void __attribute__((never_inline)) func_804CF700(EffectScene* self, f32* out, Vec* p1) {
    // Light-color update: gate on the object's 0x4000 flag, otherwise zero the
    // 4-float output. Base color comes from obj+0x40; an optional per-frame
    // factor (func_804BE398/538 chain) scales it, func_804E2B54 blends the
    // final color, and a distance falloff weights the alpha channel.
    CF700Obj* obj = (CF700Obj*)self->field_0x08;
    // Retail materializes the gate bit as -bit|bit >> 31 before branching.
    u32 gbit = (obj->field_0x00 >> 13) & 1;
    u32 gate = (u32)(-(s32)gbit | gbit) >> 31;
    if (!gate) {
        out[0] = lbl_eu_8066B0DC;
        out[1] = lbl_eu_8066B0DC;
        out[2] = lbl_eu_8066B0DC;
        out[3] = lbl_eu_8066B0DC;
        return;
    }
    // Retail copies the base color as raw words (two 8-byte chunks), so keep
    // a punned u64 view alongside the float channels.
    union {
        f32 c[4];
        u64 w[2];
    } base;
    base.w[0] = *(const u64*)&obj->field_0x40;
    base.w[1] = *(const u64*)&obj->field_0x48;
    SceneSubObj* sub = self->field_0x0c;
    u8* p114 = (u8*)sub->field_0x114;
    if (p114 != 0) {
        u8 en = p114 ? *(u8*)(p114 - 0x1b) : 0;
        if (en != 0) {
            u8* p110 = (u8*)sub->field_0x110;
            if (p110 != 0) {
                f32 f0 = p110 ? *(f32*)(p110 - 0x18) : lbl_eu_8066B0D8;
                if (func_804BE398(p1, 0x4801, 0, 0, -f0, lbl_eu_8066B0DC) != 0) {
                    ml::CVec3 v24;
                    if (func_804BE538(&v24) != 0) {
                        // Optional material tint re-read from field_0x114.
                        u8* pm = (u8*)((EffectScene*)self)->field_0x0c->field_0x114;
                        if (pm != 0) {
                            f32 f3 = pm ? *(f32*)(pm - 4) : lbl_eu_8066B114;
                            v24.x *= f3;
                            v24.y *= f3;
                            v24.z *= f3;
                        }
                        v24.x = v24.x * lbl_eu_8066B118;
                        v24.y = v24.y * lbl_eu_8066B118;
                        v24.z = v24.z * lbl_eu_8066B118;
                        if (v24.x > lbl_eu_8066B0D8) v24.x = lbl_eu_8066B0D8;
                        if (v24.y > lbl_eu_8066B0D8) v24.y = lbl_eu_8066B0D8;
                        if (v24.z > lbl_eu_8066B0D8) v24.z = lbl_eu_8066B0D8;
                        if (v24.x < lbl_eu_8066B0DC) v24.x = lbl_eu_8066B0DC;
                        if (v24.y < lbl_eu_8066B0DC) v24.y = lbl_eu_8066B0DC;
                        if (v24.z < lbl_eu_8066B0DC) v24.z = lbl_eu_8066B0DC;
                        base.c[0] *= v24.x;
                        base.c[1] *= v24.y;
                        base.c[2] *= v24.z;
                    }
                }
            }
        }
    }
    u32 col = *(u32*)func_804E2B54(&self->field_0x2e8, base.c);
    // Two hoisted 0x4330 magic slots mirror retail's prologue stores at sp+0x40
    // and sp+0x48; every int/byte -> float conversion below reuses one of the
    // two slots (alternating), which is what pins the named B0E8/B0F0 doubles.
    union {
        f64 d;
        u32 w[2];
    } c1, c2;
    c1.w[0] = 0x43300000u;
    c2.w[0] = 0x43300000u;
    const u8* cb = (const u8*)&col;
    c1.w[1] = cb[3];
    base.c[3] = (f32)(c1.d - lbl_eu_8066B0E8);
    c2.w[1] = cb[2];
    base.c[2] = (f32)(c2.d - lbl_eu_8066B0E8);
    c1.w[1] = cb[1];
    base.c[1] = (f32)(c1.d - lbl_eu_8066B0E8);
    c2.w[1] = cb[0];
    base.c[0] = (f32)(c2.d - lbl_eu_8066B0E8);
    s32 alpha = (s32)base.c[3];
    // Distance-based alpha falloff: two segments (rising then falling)
    // defined by the tone-range u16s; skipped when sub bit 0x0800 is set,
    // the table is missing or the class is 4.
    if (!((sub->field_0x1c & 0x0800)) && sub->field_0xdc != 0 && sub->field_0x00 != 4) {
        void* rsrc = func_80496264(obj->field_0x10, -1);
        ml::CVec3 diff = *(const ml::CVec3*)p1 - *(const ml::CVec3*)((u8*)rsrc + 0x10c);
        f32 mag = PSVECMag(diff);
        ToneRange* tr = sub->field_0xdc;
        // Slot usage per conversion mirrors retail's alternating scratch words.
        c1.w[1] = (u32)((s32)tr->field_0x00 - (s32)tr->field_0x02) ^ 0x80000000u;
        f32 range1 = (f32)(c1.d - lbl_eu_8066B0F0);
        c2.w[1] = tr->field_0x00;
        f32 lim0 = (f32)(c2.d - lbl_eu_8066B0E8);
        if (range1 > lbl_eu_8066B0DC && mag <= lim0) {
            c1.w[1] = tr->field_0x02;
            f32 t = (mag - (f32)(c1.d - lbl_eu_8066B0E8)) / range1;
            if (t <= lbl_eu_8066B0DC) t = lbl_eu_8066B0DC;
            c2.w[1] = (u32)(alpha & 0xff);
            alpha = (s32)((f32)(c2.d - lbl_eu_8066B0E8) * t);
        } else {
            c1.w[1] = (u32)((s32)tr->field_0x04 - (s32)tr->field_0x06) ^ 0x80000000u;
            f32 range2 = (f32)(c1.d - lbl_eu_8066B0F0);
            c2.w[1] = tr->field_0x04;
            f32 lim2 = (f32)(c2.d - lbl_eu_8066B0E8);
            if (range2 > lbl_eu_8066B0DC && lim2 <= mag) {
                f32 t = (mag - lim2) / range2;
                f32 s = lbl_eu_8066B0D8 - t;
                if (s <= lbl_eu_8066B0DC) s = lbl_eu_8066B0DC;
                c2.w[1] = (u32)(alpha & 0xff);
                alpha = (s32)((f32)(c2.d - lbl_eu_8066B0E8) * s);
            }
        }
    }
    c1.w[1] = (u32)(alpha & 0xff);
    base.c[3] = (f32)(c1.d - lbl_eu_8066B0E8);
    out[0] = base.c[0] / lbl_eu_8066B11C;
    out[1] = base.c[1] / lbl_eu_8066B11C;
    out[2] = base.c[2] / lbl_eu_8066B11C;
    out[3] = base.c[3] / lbl_eu_8066B11C;
}
// Polymorphic render target driven by func_804CFBC8. Vtable slots recovered
// from the retail indirect calls (offsets 0x18/0x1c/0x20/0x24/0x28/0x5c).
class CFBC8Target {
public:
    virtual void unk00();
    virtual void unk04();
    virtual void unk08();
    virtual void unk0C();
    virtual void unk10();
    virtual void unk14();
    virtual void setFlag(bool flag);      // +0x18
    virtual void unk1C(u32 arg);          // +0x1c
    virtual void setAlpha(f32 alpha);     // +0x20
    virtual Vec* getPosSlot();            // +0x24 returns the vec slot to fill
    virtual void applyDir(Vec* dir);      // +0x28
    virtual void unk2C();
    virtual void unk30();
    virtual void unk34();
    virtual void unk38();
    virtual void unk3C();
    virtual void unk40();
    virtual void unk44();
    virtual void unk48();
    virtual void unk4C();
    virtual void unk50();
    virtual void unk54();
    virtual void unk58();
    virtual void finish();                // +0x5c
};

// Applies the scene matrix `a` to the target: pushes color/alpha/flag state,
// decomposes the matrix (translation + normalized columns + euler rotation)
// and feeds each piece to the target through its virtual interface.
void func_804CFBC8(EffectScene* self, CFBC8Target* obj, const ml::CMat34* a, Vec* d,
                   const f32* c, bool flag) {
    const f32 zero = lbl_eu_8066B0DC;
    const f32 one = lbl_eu_8066B0D8;
    obj->setFlag(c[3] != zero);
    obj->setAlpha(one - c[3]);
    obj->unk1C(flag);

    Vec trans;
    trans.x = a->m[0][3];
    trans.y = a->m[1][3];
    trans.z = a->m[2][3];
    *obj->getPosSlot() = trans;

    ml::CMat34 m = *a;
    // Only normalize when every element except m[0][0] matches the identity
    // matrix within eps.
    bool isUnit = ((f32)__fabs(m.m[0][1] - ml::CMat34::identity.m[0][1]) <= lbl_eu_8066A208 &&
                   (f32)__fabs(m.m[0][2] - ml::CMat34::identity.m[0][2]) <= lbl_eu_8066A208 &&
                   (f32)__fabs(m.m[0][3] - ml::CMat34::identity.m[0][3]) <= lbl_eu_8066A208 &&
                   (f32)__fabs(m.m[1][0] - ml::CMat34::identity.m[1][0]) <= lbl_eu_8066A208 &&
                   (f32)__fabs(m.m[1][1] - ml::CMat34::identity.m[1][1]) <= lbl_eu_8066A208 &&
                   (f32)__fabs(m.m[1][2] - ml::CMat34::identity.m[1][2]) <= lbl_eu_8066A208 &&
                   (f32)__fabs(m.m[1][3] - ml::CMat34::identity.m[1][3]) <= lbl_eu_8066A208 &&
                   (f32)__fabs(m.m[2][0] - ml::CMat34::identity.m[2][0]) <= lbl_eu_8066A208 &&
                   (f32)__fabs(m.m[2][1] - ml::CMat34::identity.m[2][1]) <= lbl_eu_8066A208 &&
                   (f32)__fabs(m.m[2][2] - ml::CMat34::identity.m[2][2]) <= lbl_eu_8066A208 &&
                   (f32)__fabs(m.m[2][3] - ml::CMat34::identity.m[2][3]) <= lbl_eu_8066A208);
    if (isUnit) {
        // Normalize each column (guarding degenerate/negative squared lengths).
        for (s32 i = 0; i < 3; i++) {
            Vec col;
            col.x = m.m[0][i];
            col.y = m.m[1][i];
            col.z = m.m[2][i];
            f32 sumsq = col.y * col.y + col.x * col.x + col.z * col.z;
            if (sumsq == zero) continue;
            if (sumsq < zero) {
                nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 len = (sumsq > zero) ? sumsq * nw4r::math::FrSqrt(sumsq) : zero;
            f32 inv = one / len;
            col.x *= inv;
            col.y *= inv;
            col.z *= inv;
            m.m[0][i] = col.x;
            m.m[1][i] = col.y;
            m.m[2][i] = col.z;
        }
    }
    // Wrap the Z translation component back into range when it escapes
    // [-B120, one]: truncate to integer and rescale by (1 - eps).
    if (m.m[2][3] > one || m.m[2][3] < lbl_eu_8066B120) {
        m.m[2][3] = (one - lbl_eu_8066A208) * (f32)(s32)m.m[2][3];
    }
    ml::CVec3 rot;
    m.getRotXYZ(rot);
    *obj->getPosSlot() = *(Vec*)&rot;
    obj->applyDir(d);
    obj->finish();
}

// The retail calls these as real functions; -inline auto would otherwise fold
// the stub bodies into the callers (the retail never inlines them).
extern "C" u32 __attribute__((never_inline)) func_804CE378(const SceneSubObj* self) {
    return *(u32*)((u8*)self + 0x4);
}

extern "C" u32 __attribute__((never_inline)) func_804CE380(const SceneSubObj* self) { return *(u32*)((u8*)self + 0x5c); }

// Target 2: us-804d24fc - drive the attached light (field_0x330) when the
// sub-object has a tone table (field_0x110). The 0x29c/0x2a0/0x2a4 values are
// type-punned floats; 0x2a4 is truncated to an int mode.
extern "C" void __attribute__((never_inline)) func_804CE388(EffectScene* self, Vec* p) {
    if (self->field_0x330 != 0 && self->field_0x0c->field_0x110 != 0) {
        func_804D8160((CETrailLight*)self->field_0x330, p,
                      (s32)*(f32*)&self->field_0x2a4,
                      (const CETrailLightParam*)&self->field_0x1cc,
                      *(f32*)&self->field_0x29c, *(f32*)&self->field_0x2a0);
    }
}

// us-804d22d4
extern "C" void __attribute__((never_inline)) func_804CE160(EffectScene* self, const ml::CVec3* a, const ml::CVec3* b) {
    // Direction b - a. If every component already lies inside [c1, c2] the
    // stored direction is left untouched; otherwise it is renormalized (or
    // zeroed when it has no length) and written to field_0x190.
    ml::CVec3 out = *b - *a;
    if (out.x < lbl_eu_8066B108 || lbl_eu_8066B10C < out.x ||
        out.y < lbl_eu_8066B108 || lbl_eu_8066B10C < out.y ||
        out.z < lbl_eu_8066B108 || lbl_eu_8066B10C < out.z) {
        if (out.x * out.x + out.y * out.y + out.z * out.z == lbl_eu_8066B0DC) {
            out = ml::CVec3::zero;
        } else {
            PSVECNormalize((const Vec*)&out, (Vec*)&out);
        }
        *(ml::CVec3*)&self->field_0x190 = out;
    }
}

extern "C" void __attribute__((never_inline)) func_804CE418(EffectScene* self, Vec* out) {
    // Volatile pins the word load order (x then y) and keeps z's load after
    // the first two stores, matching retail's pipelined copy.
    u32 vx = *(u32*)&self->field_0xac.x;
    u32 vy = *(u32*)&self->field_0xac.y;
    ((IVec3*)out)->y = vy;
    ((IVec3*)out)->x = vx;
    ((IVec3*)out)->z = *(u32*)&self->field_0xac.z;
    Vec shifted;
    Vec res;
    PSMTXMultVec(self->field_0x130, out, &res);
    shifted.x = res.x;
    shifted.y = res.y;
    shifted.z = res.z;
    *(IVec3*)out = *(const IVec3*)&shifted;
    nw4r::math::VEC3Add((nw4r::math::VEC3*)out, (const nw4r::math::VEC3*)out,
                        (const nw4r::math::VEC3*)&self->field_0xb8);
}

// Target 7: compute the inherited/dir vector `out` for a node from self position
// and scale by per-sub-node u16 factors based on its type. The two magic-double
// slots below mirror retail's prologue-hoisted 0x43300000 hi words (one per
// conversion site).
// Signed-int -> float through a caller-owned 0x4330 magic slot; keeping the
// subtrahend in a caller local makes every inlined expansion reload the slot
// (retail alternates two slots and shares one register-held constant).
typedef union {
    f64 d;
    u32 w[2];
} FConv;
static inline f32 fconvS32(FConv& c, u32 w, f64 sub) {
    c.w[1] = w;
    return (f32)(c.d - sub);
}

// MSB-first bitfield view of a u16 flag word for func_804CE79C:
// b10 (0x0400) must be set and b9 (0x0200) clear.
struct SubObjFlags {
    u16 : 4;       // bits 15..12
    u16 b10 : 1;   // 0x0400
    u16 b9 : 1;    // 0x0200
    u16 : 10;
};

extern "C" void __attribute__((never_inline)) func_804CE79C(EffectScene* self, Vec* out, u32 type) {
    // Retail copies the punned float words y-before-x.
    u32 xw = self->field_0x29c;
    ((IVec3*)out)->y = self->field_0x2a0;
    ((IVec3*)out)->x = xw;
    ((IVec3*)out)->z = self->field_0x2a4;
    switch ((s32)((SceneSubObj*)self->field_0x0c)->field_0x00) {
    case 0:
    case 9:
    case 10:
    case 11: {
        SceneSubObj* sub = self->field_0x0c;
        SubObjFlags* fl = (SubObjFlags*)&sub->field_0x1c;
        if (!fl->b9 && fl->b10) {
            if (self->field_0x258 >= 0) {
                U16Pair* p = func_804DF2A8(*(const void**)((u8*)self->field_0x08 + 0x0c));
                s32 ib = p->b;
                s32 ia = p->a;
                out->x = out->x * (f32)ib;
                out->y = out->y * (f32)ia;
            } else if (self->field_0x25a >= 0) {
                U16Pair* p = func_804DF2A8(*(const void**)((u8*)self->field_0x08 + 0x0c));
                s32 ib = p->b;
                s32 ia = p->a;
                out->x = out->x * (f32)ib;
                out->y = out->y * (f32)ia;
            }
            out->z = lbl_eu_8066B0DC;
        }
        break;
    }
    case 8: {
        // One constant load, three stores (retail CSEs the lfs).
        f32 v = lbl_eu_8066B0D8;
        out->x = v;
        out->y = v;
        out->z = v;
        break;
    }
    }
    if ((u32)(type - 3) <= 2 || (u32)(type - 9) <= 1) {
        // Swap y/z; retail stores the y slot first.
        f32 fy = out->y;
        out->y = out->z;
        out->z = fy;
    }
    MatFlags* mat = (MatFlags*)self->field_0x08;
    if (((SubObjFlags*)&mat->field_0x00)->b10) {
        u32* m = (u32*)((SceneSubObj*)self->field_0x0c)->field_0xe4;
        s32 b = m ? *(u8*)((u8*)m - 0xe) : 0;
        if (b != 0xb) {
            out->x *= mat->field_0x34;
            out->y *= mat->field_0x38;
            out->z *= mat->field_0x3c;
        }
    }
}

// Target 6: us-804d4308 - reset two placer color globals, set channel 0 material
// color from a shared GXColor constant, then reset two more globals to -1.
// The color is read inside the call expression so its load schedules with the
// argument setup (before the -1 cache stores).
extern "C" void __attribute__((never_inline)) func_804D0194() {
    // Volatile pins the color load ahead of the r31 save/-1 stores.
    GXColor color = *(const GXColor*)&lbl_eu_8066B124;
    lbl_eu_806659A8 = -1;
    lbl_eu_806659AC = -1;
    GXSetChanMatColor(GX_COLOR0A0, color);
    lbl_eu_806659B0 = -1;
    lbl_eu_806659B4 = -1;
}

// Target 1: us-804d4354 - render the scene's quad: copy the node matrix and
// RGBA color from the scene, then depending on the sub-object class either
// concat the node matrix with the material matrix or (for tone-set classes)
// scale it to the framebuffer width; finally load texture matrices selected
// by the 0x258/0x25a texgen indices and emit a 4-vertex quad with byte-scaled
// UVs.
extern "C" void __attribute__((never_inline)) func_804D01E0(EffectScene* self, void* res) {
    // Shared 0x4330 magic slot for all u8/u16 -> float conversions (retail
    // stores the magic once in the prologue and reuses the slot).
    union {
        f64 d;
        u32 w[2];
    } cnv;
    cnv.w[0] = 0x43300000u;
    s32 ok = (self->field_0x0c->field_0x04 == 5 ||
              ((self->field_0x06 & 0x8000) && (self->field_0x06 & 0x0400))) &&
             (self->field_0x06 & 0x0800) &&
             func_804EEACC(&self->field_0xdc) != 0;
    if (!ok) return;

    RenderNode* node = (RenderNode*)func_804E0168(self->field_0x00);
    *(MtxCopy*)&node->mtx = *(MtxCopy*)&self->field_0x19c;
    f32 scale = lbl_eu_8066B11C;
    node->b0 = (u8)(s32)(scale * self->field_0x1cc);
    node->b1 = (u8)(s32)(scale * self->field_0x1d0);
    node->b2 = (u8)(s32)(scale * self->field_0x1d4);
    node->b3 = (u8)(s32)(scale * self->field_0x1d8);

    switch (self->field_0x0c->field_0x00) {
    case 0:
    case 4:
    case 5:
    case 6:
    case 7:
    case 9:
    case 10:
    case 11:
    case 12: {
        u16 sflags = self->field_0x0c->field_0x1c;
        u32 b11 = (sflags >> 11) & 1;
        u32 c11 = (u32)(-b11 | b11) >> 31;
        u32 b10 = (sflags >> 10) & 1;
        u32 c10 = (u32)(-b10 | b10) >> 31;
        if (c11 || c10) {
            u32 b11i = (self->field_0x0c->field_0x1c >> 11) & 1;
            u32 c11i = (u32)(-b11i | b11i) >> 31;
            if (c11i) {
                u8* p = (u8*)self->field_0x0c->field_0xe4;
                u8 bv = p ? *(p - 0x10) : 0;
                if (bv == 0x10) {
                    u16 fbw = CDeviceVI::getRenderModeObj()->fbWidth;
                    cnv.w[1] = fbw;
                    f32 ratio = (f32)(cnv.d - lbl_eu_8066B0E8) / lbl_eu_8066B128;
                    node->mtx[0][3] = node->mtx[0][3] * ratio;
                } else if (bv == 0x11) {
                    u16 fbw = CDeviceVI::getRenderModeObj()->fbWidth;
                    cnv.w[1] = fbw;
                    f32 ratio = (f32)(cnv.d - lbl_eu_8066B0E8) / lbl_eu_8066B128;
                    Vec svec = {ratio, lbl_eu_8066B0D8, lbl_eu_8066B0D8};
                    PSMTXScaleApply(node->mtx, node->mtx, svec.x, svec.y, svec.z);
                }
            }
        } else {
            Mtx stackMtx;
            Mtx mtxA0;
            MatFlags* mat = (MatFlags*)self->field_0x08;
            void* m = func_80496264(mat->field_0x10, -1);
            PSMTXConcat((const float(*)[4])((u8*)m + 0xcc), node->mtx, stackMtx);
            *(MtxCopy*)&node->mtx = *(MtxCopy*)&stackMtx;
            *(MtxCopy*)&mtxA0 = *(MtxCopy*)&stackMtx;
        }
        break;
    }
    default:
        break;
    }

    RenderNode* node2 = (RenderNode*)func_804E0168(self->field_0x00);
    f32 c0 = lbl_eu_8066B0D8;
    f32 c1 = lbl_eu_8066B0D8;
    f32 c2 = lbl_eu_8066B0D8;
    f32 c3 = lbl_eu_8066B0D8;
    s32 texgens = 0;
    if (self->field_0x258 >= 0) {
        func_804DF164(res, self->field_0x258, 0, self->field_0x0c->field_0x40);
        ml::CMat44 m = ml::CMat44::identity;
        if (self->field_0x260 & 0x20) {
            m.m[0][3] = self->field_0x268;
            m.m[1][3] = self->field_0x26c;
        }
        GXLoadTexMtxImm((const float(*)[4])&m, GX_TEXMTX0, GX_MTX_2x4);
        texgens = 1;
    }
    if (self->field_0x25a >= 0) {
        func_804DF164(res, self->field_0x25a, 1, self->field_0x0c->field_0x44);
        ml::CMat44 m2 = ml::CMat44::identity;
        if (self->field_0x260 & 0x10) {
            m2.m[0][3] = self->field_0x270;
            m2.m[1][3] = self->field_0x274;
        }
        GXLoadTexMtxImm((const float(*)[4])&m2, GX_TEXMTX1, GX_MTX_2x4);
        texgens++;
    }
    if (texgens == 0) return;

    u8* pb = (u8*)self->field_0x0c->field_0x108;
    if (pb) {
        cnv.w[1] = pb ? *(pb - 4) : 1;
        c0 = (f32)(cnv.d - lbl_eu_8066B0E8);
        cnv.w[1] = pb ? *(pb - 3) : 1;
        c1 = (f32)(cnv.d - lbl_eu_8066B0E8);
        cnv.w[1] = pb ? *(pb - 2) : 1;
        c2 = (f32)(cnv.d - lbl_eu_8066B0E8);
        cnv.w[1] = pb ? *(pb - 1) : 1;
        c3 = (f32)(cnv.d - lbl_eu_8066B0E8);
    }

    u8 tg = (u8)texgens;
    if (lbl_eu_806659B4 != (s32)tg) {
        GXSetNumTexGens(tg);
        GXSetNumTevStages(tg ? tg : 1);
        switch (tg) {
        case 0:
            GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
            GXSetVtxDesc(GX_VA_TEX1, GX_NONE);
            break;
        case 1:
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX1, GX_NONE);
            break;
        case 2:
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
            break;
        default:
            break;
        }
        lbl_eu_806659B4 = (s32)tg;
    }
    u32 zbit = (self->field_0x0c->field_0x1c >> 7) & 1;
    u32 zb = (u32)(-zbit | zbit) >> 31;
    if (lbl_eu_806659A8 != (s32)zb) {
        if (zb) {
            GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
        } else {
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
        }
        lbl_eu_806659A8 = (s32)zb;
    }
    if (lbl_eu_806659AC != 1) {
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
        lbl_eu_806659AC = 1;
    }
    u32 colorWord = *(u32*)&node2->b0;
    if (lbl_eu_806659B0 != (s32)colorWord) {
        GXColor color;
        *(u32*)&color = colorWord;
        GXSetChanMatColor(GX_COLOR0A0, color);
        lbl_eu_806659B0 = (s32)colorWord;
    }
    GXLoadPosMtxImm(node2->mtx, 0);
    if (texgens == 1) {
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition1x8(0);
        GXTexCoord2f32(lbl_eu_8066B0DC, lbl_eu_8066B0DC);
        GXPosition1x8(1);
        GXTexCoord2f32(c0, lbl_eu_8066B0DC);
        GXPosition1x8(2);
        GXTexCoord2f32(c0, c1);
        GXPosition1x8(3);
        GXTexCoord2f32(lbl_eu_8066B0DC, c1);
    } else if (texgens == 2) {
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition1x8(0);
        GXTexCoord2f32(lbl_eu_8066B0DC, lbl_eu_8066B0DC);
        GXTexCoord2f32(lbl_eu_8066B0DC, lbl_eu_8066B0DC);
        GXPosition1x8(1);
        GXTexCoord2f32(c0, lbl_eu_8066B0DC);
        GXTexCoord2f32(c2, lbl_eu_8066B0DC);
        GXPosition1x8(2);
        GXTexCoord2f32(c0, c1);
        GXTexCoord2f32(c2, c3);
        GXPosition1x8(3);
        GXTexCoord2f32(lbl_eu_8066B0DC, c1);
        GXTexCoord2f32(lbl_eu_8066B0DC, c3);
    }
}

// Target 5: us-804d4c28 - twin of func_804D01E0 but draws a 6-vertex strip
// per chain node: copy the root node matrix/color, optionally concat/scale it
// to the framebuffer width, load the two texture matrices selected by the
// 0x258/0x25a indices, then walk the node chain (field_0x02 links) emitting a
// triangle strip that interpolates the previous vertex state with each node's
// transformed position/direction and byte-scaled color.
void func_804D3DB0(EffectScene* self, f32* out, s32 i5, s32 i6, s32 i7);
extern "C" void __attribute__((never_inline)) func_804D0AB4(EffectScene* self, s32 index) {
    // Shared 0x4330 magic slot for the fbWidth conversions.
    union {
        f64 d;
        u32 w[2];
    } cnv;
    cnv.w[0] = 0x43300000u;
    s32 ok = (self->field_0x0c->field_0x04 == 5 ||
              ((self->field_0x06 & 0x8000) && (self->field_0x06 & 0x0400))) &&
             (self->field_0x06 & 0x0800) &&
             func_804EEACC(&self->field_0xdc) != 0;
    if (!ok) return;

    RenderNode* node = (RenderNode*)func_804E0168(self->field_0x00);
    *(MtxCopy*)&node->mtx = *(MtxCopy*)&self->field_0x19c;
    f32 scale = lbl_eu_8066B11C;
    u8 r = (u8)(s32)(scale * self->field_0x1cc);
    u8 g = (u8)(s32)(scale * self->field_0x1d0);
    u8 b = (u8)(s32)(scale * self->field_0x1d4);
    u8 a = (u8)(s32)(scale * self->field_0x1d8);
    *(u32*)&node->b0 = (u32)r | ((u32)g << 8) | ((u32)b << 16) | ((u32)a << 24);

    switch (self->field_0x0c->field_0x00) {
    case 0:
    case 4:
    case 5:
    case 6:
    case 7:
    case 9:
    case 10:
    case 11:
    case 12: {
        u16 sflags = self->field_0x0c->field_0x1c;
        u32 b11 = (sflags >> 11) & 1;
        u32 c11 = (u32)(-b11 | b11) >> 31;
        u32 b10 = (sflags >> 10) & 1;
        u32 c10 = (u32)(-b10 | b10) >> 31;
        if (c11 || c10) {
            u32 b11i = (self->field_0x0c->field_0x1c >> 11) & 1;
            u32 c11i = (u32)(-b11i | b11i) >> 31;
            if (c11i) {
                u8* p = (u8*)self->field_0x0c->field_0xe4;
                u8 bv = p ? *(p - 0x10) : 0;
                if (bv == 0x10) {
                    u16 fbw = CDeviceVI::getRenderModeObj()->fbWidth;
                    cnv.w[1] = fbw;
                    f32 ratio = (f32)(cnv.d - lbl_eu_8066B0E8) / lbl_eu_8066B128;
                    node->mtx[0][3] = node->mtx[0][3] * ratio;
                } else if (bv == 0x11) {
                    u16 fbw = CDeviceVI::getRenderModeObj()->fbWidth;
                    cnv.w[1] = fbw;
                    f32 ratio = (f32)(cnv.d - lbl_eu_8066B0E8) / lbl_eu_8066B128;
                    Vec svec = {ratio, lbl_eu_8066B0D8, lbl_eu_8066B0D8};
                    PSMTXScaleApply(node->mtx, node->mtx, svec.x, svec.y, svec.z);
                }
            }
        } else {
            Mtx stackMtx;
            Mtx mtxA0;
            MatFlags* mat = (MatFlags*)self->field_0x08;
            void* m = func_80496264(mat->field_0x10, -1);
            PSMTXConcat((const float(*)[4])((u8*)m + 0xcc), node->mtx, stackMtx);
            *(MtxCopy*)&node->mtx = *(MtxCopy*)&stackMtx;
            *(MtxCopy*)&mtxA0 = *(MtxCopy*)&stackMtx;
        }
        break;
    }
    default:
        break;
    }

    func_804E0168(self->field_0x00);
    s32 texgens = 0;
    if (self->field_0x258 >= 0) {
        func_804DF164((void*)(u32)index, self->field_0x258, 0, self->field_0x0c->field_0x40);
        ml::CMat44 m = ml::CMat44::identity;
        if (self->field_0x260 & 0x20) {
            m.m[0][3] = self->field_0x268;
            m.m[1][3] = self->field_0x26c;
        }
        GXLoadTexMtxImm((const float(*)[4])&m, GX_TEXMTX0, GX_MTX_2x4);
        texgens = 1;
    }
    if (self->field_0x25a >= 0) {
        func_804DF164((void*)(u32)index, self->field_0x25a, 1, self->field_0x0c->field_0x44);
        ml::CMat44 m2 = ml::CMat44::identity;
        if (self->field_0x260 & 0x10) {
            m2.m[0][3] = self->field_0x270;
            m2.m[1][3] = self->field_0x274;
        }
        GXLoadTexMtxImm((const float(*)[4])&m2, GX_TEXMTX1, GX_MTX_2x4);
        texgens++;
    }
    if (texgens == 0) return;

    u8 tg = (u8)texgens;
    if (lbl_eu_806659B4 != (s32)tg) {
        GXSetNumTexGens(tg);
        GXSetNumTevStages(tg ? tg : 1);
        switch (tg) {
        case 0:
            GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
            GXSetVtxDesc(GX_VA_TEX1, GX_NONE);
            break;
        case 1:
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX1, GX_NONE);
            break;
        case 2:
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
            break;
        default:
            break;
        }
        lbl_eu_806659B4 = (s32)tg;
    }
    u32 zbit = (self->field_0x0c->field_0x1c >> 7) & 1;
    u32 zb = (u32)(-zbit | zbit) >> 31;
    if (lbl_eu_806659A8 != (s32)zb) {
        if (zb) {
            GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
        } else {
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
        }
        lbl_eu_806659A8 = (s32)zb;
    }
    if (lbl_eu_806659AC != 1) {
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
        lbl_eu_806659AC = 1;
    }

    // Initial vertex state derived from the root node: the {1,0,0} and
    // {B0E4,0,0} transformed directions plus the matrix translation column.
    Vec curOut1, curTrans, curOut2;
    u8 curCol[4];
    Vec v1 = {lbl_eu_8066B12C, 0.0f, 0.0f};
    PSMTXMultVec(self->field_0x19c, &v1, &curOut1);
    curTrans.x = self->field_0x19c[0][3];
    curTrans.y = self->field_0x19c[1][3];
    curTrans.z = self->field_0x19c[2][3];
    Vec v2 = {lbl_eu_8066B0E4, 0.0f, 0.0f};
    PSMTXMultVec(self->field_0x19c, &v2, &curOut2);
    f32 f28 = lbl_eu_8066B11C;
    u8 c0 = (u8)(s32)(f28 * self->field_0x1cc);
    u8 c1 = (u8)(s32)(f28 * self->field_0x1d0);
    u8 c2 = (u8)(s32)(f28 * self->field_0x1d4);
    u8 c3 = (u8)(s32)(f28 * self->field_0x1d8);
    *(u32*)&curCol[0] = (u32)c0 | ((u32)c1 << 8) | ((u32)c2 << 16) | ((u32)c3 << 24);

    s16 count = self->field_0x0c->field_0x2e;
    EffectNode* walk = (EffectNode*)self;
    while ((s16)index < count) {
        walk = func_804E0114(walk->field_0x02);
        if (walk == 0) break;
        EffectScene* node = (EffectScene*)walk;
        Vec nodeOut1, nodeTrans, nodeOut2;
        u8 nodeCol[4];
        Vec nv1 = {lbl_eu_8066B12C, 0.0f, 0.0f};
        PSMTXMultVec(node->field_0x19c, &nv1, &nodeOut1);
        nodeTrans.x = node->field_0x19c[0][3];
        nodeTrans.y = node->field_0x19c[1][3];
        nodeTrans.z = node->field_0x19c[2][3];
        Vec nv2 = {lbl_eu_8066B0E4, 0.0f, 0.0f};
        PSMTXMultVec(node->field_0x19c, &nv2, &nodeOut2);
        u8 n0 = (u8)(s32)(f28 * node->field_0x1cc);
        u8 n1 = (u8)(s32)(f28 * node->field_0x1d0);
        u8 n2 = (u8)(s32)(f28 * node->field_0x1d4);
        u8 n3 = (u8)(s32)(f28 * node->field_0x1d8);
        *(u32*)&nodeCol[0] = (u32)n0 | ((u32)n1 << 8) | ((u32)n2 << 16) | ((u32)n3 << 24);
        if (texgens == 1) {
            f32 mtx208[18];
            func_804D3DB0(self, mtx208, (s16)index, count, 0);
            GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT1, 6);
            GXPosition3f32(curOut1.x, curOut1.y, curOut1.z);
            GXColor4u8(curCol[0], curCol[1], curCol[2], curCol[3]);
            GXTexCoord2f32(mtx208[0], mtx208[1]);
            GXPosition3f32(nodeOut1.x, nodeOut1.y, nodeOut1.z);
            GXColor4u8(nodeCol[0], nodeCol[1], nodeCol[2], nodeCol[3]);
            GXTexCoord2f32(mtx208[9], mtx208[10]);
            GXPosition3f32(curTrans.x, curTrans.y, curTrans.z);
            GXColor4u8(curCol[0], curCol[1], curCol[2], curCol[3]);
            GXTexCoord2f32(mtx208[3], mtx208[4]);
            GXPosition3f32(nodeTrans.x, nodeTrans.y, nodeTrans.z);
            GXColor4u8(nodeCol[0], nodeCol[1], nodeCol[2], nodeCol[3]);
            GXTexCoord2f32(mtx208[12], mtx208[13]);
            GXPosition3f32(curOut2.x, curOut2.y, curOut2.z);
            GXColor4u8(curCol[0], curCol[1], curCol[2], curCol[3]);
            GXTexCoord2f32(mtx208[6], mtx208[7]);
            GXPosition3f32(nodeOut2.x, nodeOut2.y, nodeOut2.z);
            GXColor4u8(nodeCol[0], nodeCol[1], nodeCol[2], nodeCol[3]);
            GXTexCoord2f32(mtx208[15], mtx208[16]);
        } else if (texgens == 2) {
            f32 mtx208[18];
            f32 mtx1c0[18];
            func_804D3DB0(self, mtx208, (s16)index, count, 0);
            func_804D3DB0(self, mtx1c0, (s16)index, count, 1);
            GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT1, 6);
            GXPosition3f32(curOut1.x, curOut1.y, curOut1.z);
            GXColor4u8(curCol[0], curCol[1], curCol[2], curCol[3]);
            GXTexCoord2f32(mtx208[0], mtx208[1]);
            GXTexCoord2f32(mtx1c0[0], mtx1c0[1]);
            GXPosition3f32(nodeOut1.x, nodeOut1.y, nodeOut1.z);
            GXColor4u8(nodeCol[0], nodeCol[1], nodeCol[2], nodeCol[3]);
            GXTexCoord2f32(mtx208[9], mtx208[10]);
            GXTexCoord2f32(mtx1c0[9], mtx1c0[10]);
            GXPosition3f32(curTrans.x, curTrans.y, curTrans.z);
            GXColor4u8(curCol[0], curCol[1], curCol[2], curCol[3]);
            GXTexCoord2f32(mtx208[3], mtx208[4]);
            GXTexCoord2f32(mtx1c0[3], mtx1c0[4]);
            GXPosition3f32(nodeTrans.x, nodeTrans.y, nodeTrans.z);
            GXColor4u8(nodeCol[0], nodeCol[1], nodeCol[2], nodeCol[3]);
            GXTexCoord2f32(mtx208[12], mtx208[13]);
            GXTexCoord2f32(mtx1c0[12], mtx1c0[13]);
            GXPosition3f32(curOut2.x, curOut2.y, curOut2.z);
            GXColor4u8(curCol[0], curCol[1], curCol[2], curCol[3]);
            GXTexCoord2f32(mtx208[6], mtx208[7]);
            GXTexCoord2f32(mtx1c0[6], mtx1c0[7]);
            GXPosition3f32(nodeOut2.x, nodeOut2.y, nodeOut2.z);
            GXColor4u8(nodeCol[0], nodeCol[1], nodeCol[2], nodeCol[3]);
            GXTexCoord2f32(mtx208[15], mtx208[16]);
            GXTexCoord2f32(mtx1c0[15], mtx1c0[16]);
        }
        curOut1 = nodeOut1;
        curTrans = nodeTrans;
        curOut2 = nodeOut2;
        *(u32*)&curCol[0] = *(u32*)&nodeCol[0];
        index++;
    }
}

// Init-region view used by func_804CC3A4 (matrices + color at 0x19c..0x20c).
struct EffInitView {
    ml::CVec3 field_0x190;
    Mtx field_0x19c;
    ml::CCol4 field_0x1cc;
    Mtx field_0x1dc;
};
// Source node: +0/+0x4 u32s, +0x8 s16, +0xa u8 flag selector. The +4 word is
// aliased as both an integer and a sub-object pointer; the distinct access
// types keep MWCC from merging the two reads.
struct EffSrc3 {
    u32 field_0x00;
    union {
        u32 field_0x04;
        SceneSubObj* sub;
    };
    s16 field_0x08;
    u8 field_0x0a;
};

// Anim-item initializers and lookup helpers (defined in code_804DEDA8.cpp).
struct CSchedAnimItem;
extern "C" void func_804E0E48(CSchedAnimItem* item, u8* base, f32* ref);
extern "C" void func_804E17A4(CSchedAnimItem* item, u8* base);
extern "C" void func_804E196C(CSchedAnimItem* item, u8* base);
extern "C" void func_804E1AA8(CSchedAnimItem* item, u8* base);
extern "C" void func_804E1D50(CSchedAnimItem* item, u8* base);
extern "C" void func_804E214C(CSchedAnimItem* item, u8* base, bool sel);
extern "C" void func_804E26D8(CSchedAnimItem* item, u8* base);
extern "C" void func_804E2D8C(CSchedAnimItem* item, u8* base);
// Resource lookup query (code_804DEDA8.cpp).
extern "C" s32 func_804DF2C4(void* lookup, void* a, s32 index);
// CETrail constructor (CETrail.cpp; retail linker name).
extern "C" void* __dt__804D6C60(CETrail* t, u32 count, s32 segCount, void* linkArg,
                                u32 dataA, u32 dataB, bool flag158, u32 id15C);
// Light attachment init (CETrail.cpp).
extern "C" CETrailLight* func_804D807C(CETrailLight* self, void* parent);

// Target 3: initialise an effect scene from a source node entry. Seeds the
// flag word (two fixed bits, one bit from the source entry, one random bit),
// zeroes the body, fills the identity matrices / white color, builds the eight
// animation blocks from the sub-object resource pointers, then runs the
// per-class allocation paths (trail / light / randomizer) and the tone-scale
// randomization tail.

void func_804CC3A4(EffectScene* out, const EffSrc3* src) {
    SceneSubObj* sub = (SceneSubObj*)src->field_0x04;
    u16 flags = (u16)(0xC000 | ((src->field_0x0a << 13) & 0x2000));
    u32 type = sub->field_0x00;
    out->field_0x04 = (u16)src->field_0x08;
    out->field_0x06 = flags;
    // random bit 0 lands in flag bit 7
    out->field_0x06 = (u16)(out->field_0x06 | ((ml::math::mtRand() & 1) << 7));
    out->field_0x08 = src->field_0x00;
    out->field_0x0c = sub;
    *(f32*)&out->field_0x10 = lbl_eu_8066B0D8;
    *(f32*)&out->field_0x14 = lbl_eu_8066B0D8;
    out->field_0x18 = lbl_eu_8066B0DC;
    out->field_0x1c = lbl_eu_8066B0DC;
    memset((u8*)out + 0x20, 0, 0x80);

    EffInitView* iv = (EffInitView*)((u8*)out + 0x190);
    *(ml::CMat34*)iv->field_0x19c = ml::CMat34::identity;
    iv->field_0x1cc = ml::CCol4::white;
    iv->field_0x190 = ml::CVec3(lbl_eu_8066B0DC, lbl_eu_8066B0DC, lbl_eu_8066B0D8);
    *(ml::CMat34*)iv->field_0x1dc = ml::CMat34::identity;

    // Eight anim blocks built from the sub-object's resource pointers; retail
    // caches the sub pointer only for the first call and reloads it afterwards.
    SceneSubObj* subA = (SceneSubObj*)out->field_0x0c;
    func_804E0E48((CSchedAnimItem*)&out->field_0x20c, (u8*)subA->field_0xf8,
                  (f32*)type);
    func_804E17A4((CSchedAnimItem*)&out->field_0x238,
                  (u8*)((SceneSubObj*)out->field_0x0c)->field_0xfc);
    func_804E196C((CSchedAnimItem*)&out->field_0x24c,
                  (u8*)((SceneSubObj*)out->field_0x0c)->field_0x104);
    func_804E1AA8((CSchedAnimItem*)&out->field_0x25c,
                  (u8*)((SceneSubObj*)out->field_0x0c)->field_0x108);
    func_804E1D50((CSchedAnimItem*)&out->field_0x278,
                  (u8*)((SceneSubObj*)out->field_0x0c)->field_0x110);
    func_804E26D8((CSchedAnimItem*)&out->field_0x2e8,
                  (u8*)((SceneSubObj*)out->field_0x0c)->field_0x114);
    func_804E214C((CSchedAnimItem*)&out->field_0x2a8,
                  (u8*)((SceneSubObj*)out->field_0x0c)->field_0x10c,
                  ((((SceneSubObj*)out->field_0x0c)->field_0x1c >> 11) & 1) != 0);
    func_804E2D8C((CSchedAnimItem*)&out->field_0x310,
                  (u8*)((SceneSubObj*)out->field_0x0c)->field_0x118);

    MemManGlob* mg = (MemManGlob*)lbl_eu_8065FC18;
    mtl::ALLOC_HANDLE h = (mtl::ALLOC_HANDLE)mg->field_0x04;

    if (type == 0xe) {
        // class 0xe never fades: clear flag bit 14
        out->field_0x06 &= (u16)~0x4000;
    }
    if ((((SceneSubObj*)out->field_0x0c)->field_0x1c >> 10) & 1) {
        MatFlags* obj = (MatFlags*)out->field_0x08;
        out->field_0x32c =
            (void*)func_804DF2C4(obj->field_0x0c, obj->field_0x10,
                                 (s32)out->field_0x258);
    }

    if (type == 8) {
        // Trail allocation: only when the scheduler heap can hold both the
        // arena guard (0x8000) and the trail block itself.
        if (mtl::MemManager::getMaxAllocSize(h) > 0x8000) {
            void* trail = 0;
            if (mtl::MemManager::getMaxAllocSize(h) >= 0x188) {
                trail = mtl::MemManager::allocate(0x188, h);
                if (trail != 0) {
                    SceneSubObj* sb = (SceneSubObj*)out->field_0x0c;
                    trail = __dt__804D6C60(
                        (CETrail*)trail,
                        (u32)(s32)*(f32*)&out->field_0x2a0,
                        (s32)*(f32*)&out->field_0x2a4,
                        (void*)out->field_0x08, sb->field_0xe4, sb->field_0xe8,
                        ((out->field_0x06 >> 13) & 1) != 0, 0);
                }
            }
            out->field_0x328 = trail;
        }
    }
    if (type == 0xd) {
        CETrailLight* light = 0;
        if (mtl::MemManager::getMaxAllocSize(h) >= 8) {
            light = (CETrailLight*)mtl::MemManager::allocate(8, h);
            if (light != 0) {
                light = func_804D807C(light, ((MatFlags*)out->field_0x08)->field_0x10);
            }
        }
        out->field_0x330 = light;
    }
    if (type == 4) {
        CERandomizerSimple* rnd = 0;
        if (mtl::MemManager::getMaxAllocSize(h) >= 0xc) {
            rnd = (CERandomizerSimple*)mtl::MemManager::allocate(0xc, h);
            if (rnd != 0) {
                new (rnd) CERandomizerSimple();
            }
        }
        out->field_0x334 = rnd;
        rnd->create(-1);
    }

    // Tone-scale tail driven by the sub-object's tone table.
    SceneSubObj* subT = (SceneSubObj*)out->field_0x0c;
    u8* tbl = (u8*)subT->field_0xf4;
    if (tbl != 0) {
        f32 v1c = u16ToF_b0e8(*(u16*)tbl);
        *(f32*)&out->field_0x1c = v1c;
        if (lbl_eu_8066B0DC < v1c) {
            out->field_0x06 &= (u16)~0x4000;
        }
        SceneSubObj* subU = (SceneSubObj*)out->field_0x0c;
        u8* tb = (u8*)subU->field_0xf4;
        s32 r = (s32)ml::math::mtRand();
        f32 v = (s32ToF_b0f0(r) / lbl_eu_8066B0E0 - lbl_eu_8066B0E4) *
                    *(f32*)(tb + 8) + lbl_eu_8066B0D8;
        *(f32*)&out->field_0x14 = v;
        // retail clamps the computed scale when it is not strictly positive
        if (v <= lbl_eu_8066B0DC) {
            *(f32*)&out->field_0x14 = lbl_eu_8066B0D8;
        }
    }
}

// Per-type node-walk advance helpers (retail C-linkage names). Each advances
// the node chain one step; the render-dispatch (func_804D42B8) calls them with
// (node, arg). Stub bodies - separate targets (us-804d5a10 / 6260 / 6804 /
// 6cd4 / 720c / 7790).
extern "C" void __attribute__((never_inline)) func_804D189C(EffectNode* node, void* arg){}
extern "C" void __attribute__((never_inline)) func_804D20EC(EffectNode* nodeRaw, void* res) {
    EffectScene* self = (EffectScene*)nodeRaw;
    union {
        f64 d;
        u32 w[2];
    } cnvA, cnvB;
    cnvA.w[0] = 0x43300000u;
    cnvB.w[0] = 0x43300000u;
    // Visibility gate (same shape as func_804D2B60).
    if (*(s32*)&((SceneSubObj*)self->field_0x0c)->field_0x04 != 5) {
        SceneFlagBits* flg = (SceneFlagBits*)&self->field_0x06;
        if (!flg->b15) return;
        if (!flg->b10) return;
    }
    if (!((SceneFlagBits*)&self->field_0x06)->b11) return;
    if (func_804EEACC(&self->field_0xdc) == 0) return;

    RenderNode* node = (RenderNode*)func_804E0168(self->field_0x00);
    *(MtxCopy*)&node->mtx = *(MtxCopy*)&self->field_0x19c;
    f32 scale = lbl_eu_8066B11C;
    node->b0 = (u8)(s32)(scale * self->field_0x1cc);
    node->b1 = (u8)(s32)(scale * self->field_0x1d0);
    node->b2 = (u8)(s32)(scale * self->field_0x1d4);
    node->b3 = (u8)(s32)(scale * self->field_0x1d8);

    switch (*(s32*)&((SceneSubObj*)self->field_0x0c)->field_0x00) {
    case 0:
    case 4:
    case 5:
    case 6:
    case 7:
    case 9:
    case 10:
    case 11:
    case 12: {
        bool toneBit = ((((SceneSubObj*)self->field_0x0c)->field_0x1c >> 11) & 1) != 0;
        bool fadeBit = ((((SceneSubObj*)self->field_0x0c)->field_0x1c >> 10) & 1) != 0;
        if (toneBit || fadeBit) {
            if ((((SceneSubObj*)self->field_0x0c)->field_0x1c >> 11) & 1) {
                u8* p = (u8*)((SceneSubObj*)self->field_0x0c)->field_0xe4;
                u8 bv = p ? *(p - 0x10) : 0;
                if (bv == 0x10) {
                    u16 fbw = CDeviceVI::getRenderModeObj()->fbWidth;
                    cnvA.w[1] = fbw;
                    f32 ratio = (f32)(cnvA.d - lbl_eu_8066B0E8) / lbl_eu_8066B128;
                    node->mtx[0][3] = node->mtx[0][3] * ratio;
                } else if (bv == 0x11) {
                    u16 fbw = CDeviceVI::getRenderModeObj()->fbWidth;
                    cnvB.w[1] = fbw;
                    f32 ratio = (f32)(cnvB.d - lbl_eu_8066B0E8) / lbl_eu_8066B128;
                    Vec svec = {ratio, lbl_eu_8066B0D8, lbl_eu_8066B0D8};
                    PSMTXScaleApply(node->mtx, node->mtx, svec.x, svec.y, svec.z);
                }
            }
        } else {
            Mtx stackMtx;
            Mtx mtxA0;
            MatFlags* mat = (MatFlags*)self->field_0x08;
            void* m = func_80496264(mat->field_0x10, -1);
            PSMTXConcat((const float(*)[4])((u8*)m + 0xcc), node->mtx, stackMtx);
            *(MtxCopy*)&node->mtx = *(MtxCopy*)&stackMtx;
            *(MtxCopy*)&mtxA0 = *(MtxCopy*)&stackMtx;
        }
        break;
    }
    default:
        break;
    }

    // Re-fetch the render node built above and derive the two screen-space
    // anchor vectors: one from the node's translation column, one from the
    // scene's stored float triple (punned through u32).
    node = (RenderNode*)func_804E0168(self->field_0x00);
    Vec drawPos;
    drawPos.x = node->mtx[0][3];
    drawPos.y = node->mtx[1][3];
    drawPos.z = node->mtx[2][3];
    Vec angBase;
    *(u32*)&angBase.x = self->field_0x29c;
    *(u32*)&angBase.y = self->field_0x2a0;
    *(u32*)&angBase.z = self->field_0x2a4;

    // When the scene-local scale bit is clear, verify the quad is on-screen
    // via the camera helper; bail out when it reports off-screen.
    if (!(((((SceneSubObj*)self->field_0x0c)->field_0x1c >> 11) & 1))) {
        if (!func_804D3B14((D3B14Cam*)self, &drawPos, &angBase)) return;
    }

    // Angle wrap: seed with a per-flag constant times the scene float, then
    // normalize into [limit, limit+step) with additive stepping loops.
    f32 ang;
    if (((SceneFlagBits*)&self->field_0x06)->b7) {
        ang = lbl_eu_8066B130 * *(f32*)&self->field_0x10;
    } else {
        ang = lbl_eu_8066B134 * *(f32*)&self->field_0x10;
    }
    const f32 step = lbl_eu_8066B138;
    const f32 limit = lbl_eu_8066B0DC;
    while (ang < limit) ang += step;
    while (limit <= ang) ang -= step;

    // Z-gate byte from the sample table decides the depth-write flag.
    u8* pz = (u8*)((SceneSubObj*)self->field_0x0c)->field_0x104;
    u32 zb = pz ? *(pz - 2) : 0;
    bool zflag = zb != 0;

    // Stack texture-matrix source payload (same layout as func_804D2B60).
    TexMtxSrc src;
    src.mTex = res;
    src.mIndex = (s8)self->field_0x25a;
    src.field_0x08 = 0;
    f32 zero = lbl_eu_8066B0DC;
    f32 one = lbl_eu_8066B0D8;
    src.field_0x0c = zero;
    src.field_0x10 = zero;
    src.field_0x14 = zero;
    src.field_0x18 = one;
    src.field_0x1c = one;
    src.field_0x20 = zero;
    u8* pb = (u8*)((SceneSubObj*)self->field_0x0c)->field_0x108;
    if (pb != NULL) {
        src.field_0x08 = *(pb - 7);
        src.field_0x0c = self->field_0x270;
        src.field_0x10 = self->field_0x274;
        src.field_0x18 = u8ToF_b0e8(*(pb - 2));
        src.field_0x1c = u8ToF_b0e8(*(pb - 1));
    }
    // Scenes without the local-scale bit inherit the parent transform.
    bool inheritScale = ((((SceneSubObj*)self->field_0x0c)->field_0x1c >> 11) & 1) != 0;
    MatFlags* mat = (MatFlags*)self->field_0x08;
    func_804EEB40(mat->field_0x10, &drawPos, (void*)&self->field_0x1cc,
                  &angBase, !inheritScale, zflag ? 1 : 0, &src, self->field_0x2a4,
                  self->field_0x2d8);
}
// Common prologue shared by the per-type node-walk renderers (targets
// us-804d6804/7790/6cd4/720c): visibility gate, render-node matrix/color
// snapshot, and the per-class tone/framebuffer scaling.

extern "C" void __attribute__((never_inline)) func_804D2B60(EffectNode* nodeRaw, void* res) {
    EffectScene* self = (EffectScene*)nodeRaw;
    union {
        f64 d;
        u32 w[2];
    } cnv;
    cnv.w[0] = 0x43300000u;
    // Visibility gate: type-5 scenes only need the tone bit, others need
    // master+enable too; the link check runs last.
    if (self->field_0x0c->field_0x04 != 5) {
        if (!(self->field_0x06 & 0x8000)) return;
        if (!(self->field_0x06 & 0x0400)) return;
    }
    if (!(self->field_0x06 & 0x0800)) return;
    if (func_804EEACC(&self->field_0xdc) == 0) return;

    RenderNode* node = (RenderNode*)func_804E0168(self->field_0x00);
    *(MtxCopy*)&node->mtx = *(MtxCopy*)&self->field_0x19c;
    f32 scale = lbl_eu_8066B11C;
    node->b0 = (u8)(s32)(scale * self->field_0x1cc);
    node->b1 = (u8)(s32)(scale * self->field_0x1d0);
    node->b2 = (u8)(s32)(scale * self->field_0x1d4);
    node->b3 = (u8)(s32)(scale * self->field_0x1d8);

    switch (self->field_0x0c->field_0x00) {
    case 0:
    case 4:
    case 5:
    case 6:
    case 7:
    case 9:
    case 10:
    case 11:
    case 12: {
        u16 sflags = self->field_0x0c->field_0x1c;
        u32 b11 = (sflags >> 11) & 1;
        u32 c11 = (u32)(-b11 | b11) >> 31;
        u32 b10 = (sflags >> 10) & 1;
        u32 c10 = (u32)(-b10 | b10) >> 31;
        if (c11 || c10) {
            u32 b11i = (self->field_0x0c->field_0x1c >> 11) & 1;
            u32 c11i = (u32)(-b11i | b11i) >> 31;
            if (c11i) {
                u8* p = (u8*)self->field_0x0c->field_0xe4;
                u8 bv = p ? *(p - 0x10) : 0;
                if (bv == 0x10) {
                    u16 fbw = CDeviceVI::getRenderModeObj()->fbWidth;
                    cnv.w[1] = fbw;
                    f32 ratio = (f32)(cnv.d - lbl_eu_8066B0E8) / lbl_eu_8066B128;
                    node->mtx[0][3] = node->mtx[0][3] * ratio;
                } else if (bv == 0x11) {
                    u16 fbw = CDeviceVI::getRenderModeObj()->fbWidth;
                    cnv.w[1] = fbw;
                    f32 ratio = (f32)(cnv.d - lbl_eu_8066B0E8) / lbl_eu_8066B128;
                    Vec svec = {ratio, lbl_eu_8066B0D8, lbl_eu_8066B0D8};
                    PSMTXScaleApply(node->mtx, node->mtx, svec.x, svec.y, svec.z);
                }
            }
        } else {
            Mtx stackMtx;
            Mtx mtxA0;
            MatFlags* mat = (MatFlags*)self->field_0x08;
            void* m = func_80496264(mat->field_0x10, -1);
            PSMTXConcat((const float(*)[4])((u8*)m + 0xcc), node->mtx, stackMtx);
            *(MtxCopy*)&node->mtx = *(MtxCopy*)&stackMtx;
            *(MtxCopy*)&mtxA0 = *(MtxCopy*)&stackMtx;
        }
        break;
    }
    default:
        break;
    }

    // Screen position plus the alpha pair fed to the blit entry point.
    Vec drawPos;
    f32 alpha = self->field_0x2b0;
    f32 alphaMax = self->field_0x2b4;
    // Visibility gate: when the tone bit is clear, verify on-screen state via
    // the camera helper; bail out when it reports off-screen.
    if (!(self->field_0x0c->field_0x1c & 0x800)) {
        if (!func_804D3B14((D3B14Cam*)self, &drawPos, NULL)) return;
    }

    TexMtxSrc src;
    src.mTex = res;
    src.mIndex = (s8)self->field_0x25a;
    src.field_0x08 = 0;
    f32 zero = lbl_eu_8066B0DC;
    f32 one = lbl_eu_8066B0D8;
    src.field_0x0c = zero;
    src.field_0x10 = zero;
    src.field_0x14 = zero;
    src.field_0x18 = one;
    src.field_0x1c = one;
    src.field_0x20 = zero;
    u8* pb = (u8*)self->field_0x0c->field_0x108;
    if (pb != NULL) {
        src.field_0x08 = *(pb - 7);
        src.field_0x0c = self->field_0x270;
        src.field_0x10 = self->field_0x274;
        src.field_0x18 = u8ToF_b0e8(*(pb - 2));
        src.field_0x1c = u8ToF_b0e8(*(pb - 1));
    }
    MatFlags* mat = (MatFlags*)self->field_0x08;
    DrawQuad quad;
    quad.x = self->field_0x19c[0][3];
    quad.y = self->field_0x19c[1][3];
    quad.z = self->field_0x19c[2][3];
    void* material;
    func_804F1F18(mat->field_0x10, (ml::CVec3*)&drawPos, (TexDrawSize*)&self->field_0x1cc,
                  alpha, alphaMax, material, &src);
}

extern "C" void __attribute__((never_inline)) func_804D3098(EffectNode* nodeRaw, void* res) {
    EffectScene* self = (EffectScene*)nodeRaw;
    // Two 0x4330 magic-double int->float conversion slots initialized up front
    // (retail prologue scratch words); they are shared between the widescreen
    // aspect fixes and the trailing texture-source byte conversions.
    union {
        f64 d;
        u32 w[2];
    } cnvA, cnvB;
    cnvA.w[0] = 0x43300000u;
    cnvB.w[0] = 0x43300000u;
    // Visibility gate: type-5 scenes only need the tone bit (bit 10); other
    // types need bits 14 and 9 as well before the link check decides.
    s32 ok = 0;
    if ((s32)self->field_0x0c->field_0x04 == 5 ||
        (((self->field_0x06 >> 14) & 1) && ((self->field_0x06 >> 9) & 1))) {
        if ((self->field_0x06 >> 10) & 1) {
            ok = func_804EEACC(&self->field_0xdc) != 0;
        }
    }
    if (!ok) return;
    // Randomizer tick: drive the attached simple randomizer while the tone
    // table is active and its age parameter is positive.
    if (self->field_0x334 != NULL && self->field_0x0c->field_0x110 != NULL) {
        f32 t = self->field_0x18;
        f32 v = self->field_0x2a4;
        if (lbl_eu_8066B0DC < v) {
            t = t / v;
        }
        ((CERandomizerSimple*)self->field_0x334)->execute(t);
    }
    RenderNode* node = (RenderNode*)func_804E0168((s16)self->field_0x00);
    *(MtxCopy*)&node->mtx = *(MtxCopy*)&self->field_0x19c;
    f32 scale = lbl_eu_8066B11C;
    node->b0 = (u8)(s32)(scale * self->field_0x1cc);
    node->b1 = (u8)(s32)(scale * self->field_0x1d0);
    node->b2 = (u8)(s32)(scale * self->field_0x1d4);
    node->b3 = (u8)(s32)(scale * self->field_0x1d8);

    switch (self->field_0x0c->field_0x00) {
    case 0:
    case 4:
    case 5:
    case 6:
    case 7:
    case 9:
    case 10:
    case 11:
    case 12: {
        u16 sflags = self->field_0x0c->field_0x1c;
        u32 b10 = (sflags >> 10) & 1;
        u32 c10 = (u32)(-b10 | b10) >> 31;
        u32 b09 = (sflags >> 9) & 1;
        u32 c09 = (u32)(-b09 | b09) >> 31;
        if (c10 || c09) {
            u32 b10i = (self->field_0x0c->field_0x1c >> 10) & 1;
            u32 c10i = (u32)(-b10i | b10i) >> 31;
            if (c10i) {
                u8* p = (u8*)self->field_0x0c->field_0xe4;
                u8 bv = p ? *(p - 0x10) : 0;
                if (bv == 0x10) {
                    u16 fbw = CDeviceVI::getRenderModeObj()->fbWidth;
                    cnvA.w[1] = fbw;
                    f32 ratio = (f32)((cnvA.d - lbl_eu_8066B0E8) / lbl_eu_8066B128);
                    node->mtx[0][3] = node->mtx[0][3] * ratio;
                } else if (bv == 0x11) {
                    u16 fbw = CDeviceVI::getRenderModeObj()->fbWidth;
                    cnvB.w[1] = fbw;
                    f32 ratio = (f32)((cnvB.d - lbl_eu_8066B0E8) / lbl_eu_8066B128);
                    PSMTXScaleApply(node->mtx, node->mtx, ratio,
                                    lbl_eu_8066B0D8, lbl_eu_8066B0D8);
                }
            }
        } else {
            Mtx stackMtx;
            Mtx mtxA0;
            MatFlags* mat = (MatFlags*)self->field_0x08;
            void* m = func_80496264(mat->field_0x10, -1);
            PSMTXConcat((const float(*)[4])((u8*)m + 0xcc), node->mtx, stackMtx);
            *(MtxCopy*)&node->mtx = *(MtxCopy*)&stackMtx;
            // Second dead-looking stack copy that retail keeps.
            *(MtxCopy*)&mtxA0 = *(MtxCopy*)&stackMtx;
        }
        break;
    }
    default:
        break;
    }

    // Tone-table payload: while a tone table is bound, re-fetch the render
    // node, derive the screen position/mode from it, and emit the quad.
    if (self->field_0x0c->field_0x110 != NULL) {
        RenderNode* node2 = (RenderNode*)func_804E0168((s16)self->field_0x00);
        f32 alpha = self->field_0x29c;
        Vec drawPos;
        drawPos.x = node2->mtx[0][3];
        drawPos.y = node2->mtx[1][3];
        drawPos.z = node2->mtx[2][3];
        s32 mode = (s32)self->field_0x2a0;
        // When the tone bit is clear, verify on-screen state via the camera.
        if (!((self->field_0x0c->field_0x1c >> 10) & 1)) {
            if (!func_804D3B14((D3B14Cam*)self, &drawPos, NULL)) return;
        }
        // Selector byte behind field_0x104 booleanized without a branch.
        u8* mp = (u8*)self->field_0x0c->field_0x104;
        u32 matFlag = 0;
        if (mp != NULL) {
            u32 bit = *(mp - 2);
            matFlag = (u32)(-bit | bit) >> 31;
        }

        TexMtxSrc src;
        src.mTex = res;
        src.mIndex = (s8)self->field_0x25a;
        src.field_0x08 = 0;
        f32 zero = lbl_eu_8066B0DC;
        f32 one = lbl_eu_8066B0D8;
        src.field_0x0c = zero;
        src.field_0x10 = zero;
        src.field_0x14 = zero;
        src.field_0x18 = one;
        src.field_0x1c = one;
        src.field_0x20 = zero;
        u8* pb = (u8*)self->field_0x0c->field_0x108;
        if (pb != NULL) {
            src.field_0x08 = *(pb - 7);
            src.field_0x0c = self->field_0x270;
            src.field_0x10 = self->field_0x274;
            src.field_0x14 = zero;
        }
        // Byte conversions reuse the prologue magic-double slots; a missing
        // payload defaults each byte to 1.
        cnvA.w[1] = pb ? *(pb - 1) : 1;
        cnvB.w[1] = pb ? *(pb - 2) : 1;
        src.field_0x1c = (f32)(cnvA.d - lbl_eu_8066B0E8);
        src.field_0x18 = (f32)(cnvB.d - lbl_eu_8066B0E8);

        MatFlags* mat = (MatFlags*)self->field_0x08;
        void* material;
        func_804F0F2C(mat->field_0x10, alpha, (const ml::CVec3*)&drawPos,
                      (const TexDrawSize*)&self->field_0x1cc, (const f32*)(u32)mode,
                      (void*)(u32)matFlag, &src, self->field_0x334);
    }
}

extern "C" void __attribute__((never_inline)) func_804D361C(EffectNode* nodeRaw, void* res) {
    EffectScene* self = (EffectScene*)nodeRaw;
    union {
        f64 d;
        u32 w[2];
    } cnv;
    cnv.w[0] = 0x43300000u;
    // Visibility gate (same shape as func_804D2B60).
    if (self->field_0x0c->field_0x04 != 5) {
        if (!(self->field_0x06 & 0x8000)) return;
        if (!(self->field_0x06 & 0x0400)) return;
    }
    if (!(self->field_0x06 & 0x0800)) return;
    if (func_804EEACC(&self->field_0xdc) == 0) return;

    RenderNode* node = (RenderNode*)func_804E0168(self->field_0x00);
    *(MtxCopy*)&node->mtx = *(MtxCopy*)&self->field_0x19c;
    f32 scale = lbl_eu_8066B11C;
    node->b0 = (u8)(s32)(scale * self->field_0x1cc);
    node->b1 = (u8)(s32)(scale * self->field_0x1d0);
    node->b2 = (u8)(s32)(scale * self->field_0x1d4);
    node->b3 = (u8)(s32)(scale * self->field_0x1d8);

    switch (self->field_0x0c->field_0x00) {
    case 0:
    case 4:
    case 5:
    case 6:
    case 7:
    case 9:
    case 10:
    case 11:
    case 12: {
        u16 sflags = self->field_0x0c->field_0x1c;
        u32 b11 = (sflags >> 11) & 1;
        u32 c11 = (u32)(-b11 | b11) >> 31;
        u32 b10 = (sflags >> 10) & 1;
        u32 c10 = (u32)(-b10 | b10) >> 31;
        if (c11 || c10) {
            u32 b11i = (self->field_0x0c->field_0x1c >> 11) & 1;
            u32 c11i = (u32)(-b11i | b11i) >> 31;
            if (c11i) {
                u8* p = (u8*)self->field_0x0c->field_0xe4;
                u8 bv = p ? *(p - 0x10) : 0;
                if (bv == 0x10) {
                    u16 fbw = CDeviceVI::getRenderModeObj()->fbWidth;
                    cnv.w[1] = fbw;
                    f32 ratio = (f32)(cnv.d - lbl_eu_8066B0E8) / lbl_eu_8066B128;
                    node->mtx[0][3] = node->mtx[0][3] * ratio;
                } else if (bv == 0x11) {
                    u16 fbw = CDeviceVI::getRenderModeObj()->fbWidth;
                    cnv.w[1] = fbw;
                    f32 ratio = (f32)(cnv.d - lbl_eu_8066B0E8) / lbl_eu_8066B128;
                    Vec svec = {ratio, lbl_eu_8066B0D8, lbl_eu_8066B0D8};
                    PSMTXScaleApply(node->mtx, node->mtx, svec.x, svec.y, svec.z);
                }
            }
        } else {
            Mtx stackMtx;
            Mtx mtxA0;
            MatFlags* mat = (MatFlags*)self->field_0x08;
            void* m = func_80496264(mat->field_0x10, -1);
            PSMTXConcat((const float(*)[4])((u8*)m + 0xcc), node->mtx, stackMtx);
            *(MtxCopy*)&node->mtx = *(MtxCopy*)&stackMtx;
            *(MtxCopy*)&mtxA0 = *(MtxCopy*)&stackMtx;
        }
        break;
    }
    default:
        break;
    }

    // Second payload: while a tone table is bound, re-fetch the render node
    // and derive the screen quad/mode from it.
    if (self->field_0x0c->field_0x110 != NULL) {
        RenderNode* n2 = (RenderNode*)func_804E0168(self->field_0x00);
        if (n2 != NULL) {
            DrawQuad q2;
            q2.x = *(f32*)((u8*)n2 + 0xc);
            q2.y = *(f32*)((u8*)n2 + 0x1c);
            q2.z = *(f32*)((u8*)n2 + 0x2c);
            s32 mode = (s32)self->field_0x29c;

            TexMtxSrc src;
            src.mTex = res;
            src.mIndex = (s8)self->field_0x25a;
            src.field_0x08 = 0;
            f32 zero = lbl_eu_8066B0DC;
            f32 one = lbl_eu_8066B0D8;
            src.field_0x0c = zero;
            src.field_0x10 = zero;
            src.field_0x14 = zero;
            src.field_0x18 = one;
            src.field_0x1c = one;
            src.field_0x20 = zero;
            u8* pb = (u8*)self->field_0x0c->field_0x108;
            if (pb != NULL) {
                src.field_0x08 = *(pb - 7);
                src.field_0x0c = self->field_0x270;
                src.field_0x10 = self->field_0x274;
                src.field_0x18 = u8ToF_b0e8(*(pb - 2));
                src.field_0x1c = u8ToF_b0e8(*(pb - 1));
            }
            MatFlags* mat = (MatFlags*)self->field_0x08;
            void* material;
            func_804F2E44(mat->field_0x10, (DrawQuad*)&self->field_0x1cc, mode,
                          material, &src);
        }
    }
}

extern "C" void __attribute__((never_inline)) func_804D2690(EffectNode* nodeRaw, void* res) {
    EffectScene* self = (EffectScene*)nodeRaw;
    // Two 0x4330 integer-to-float conversion slots, initialized up front like
    // retail's prologue stores (slots 0xe0/0xe8).
    union {
        f64 d;
        u32 w[2];
    } convA, convB;
    convA.w[0] = 0x43300000u;
    convB.w[0] = 0x43300000u;

    // Visibility gate: type-5 scenes only need flag bit 10; other types need
    // bits 15 and 9 as well before the link check decides.
    int ok = 0;
    if ((s32)self->field_0x0c->field_0x04 == 5 ||
        (((self->field_0x06 >> 15) & 1) && ((self->field_0x06 >> 10) & 1))) {
        if ((self->field_0x06 >> 11) & 1) {
            ok = func_804EEACC(&self->field_0xdc) != 0;
        }
    }
    if (!ok) return;

    RenderNode* node = (RenderNode*)func_804E0168((s16)self->field_0x00);
    *(MtxCopy*)&node->mtx = *(MtxCopy*)&self->field_0x19c;
    f32 scale = lbl_eu_8066B11C;
    node->b0 = (u8)(scale * self->field_0x1cc);
    node->b1 = (u8)(scale * self->field_0x1d0);
    node->b2 = (u8)(scale * self->field_0x1d4);
    node->b3 = (u8)(scale * self->field_0x1d8);

    switch (self->field_0x0c->field_0x00) {
    case 0:
    case 4:
    case 5:
    case 6:
    case 7:
    case 9:
    case 10:
    case 11:
    case 12: {
        if ((((self->field_0x0c->field_0x1c >> 11) & 1) != 0) ||
            (((self->field_0x0c->field_0x1c >> 10) & 1) != 0)) {
            if (((self->field_0x0c->field_0x1c >> 11) & 1) != 0) {
                u8* p = (u8*)self->field_0x0c->field_0xe4;
                u8 bv = p ? *(p - 0x10) : 0;
                if (bv == 0x10) {
                    u16 fbw = CDeviceVI::getRenderModeObj()->fbWidth;
                    convA.w[1] = fbw;
                    f32 ratio = (f32)(convA.d - lbl_eu_8066B0E8) / lbl_eu_8066B128;
                    node->mtx[0][3] = node->mtx[0][3] * ratio;
                } else if (bv == 0x11) {
                    u16 fbw = CDeviceVI::getRenderModeObj()->fbWidth;
                    convB.w[1] = fbw;
                    f32 ratio = (f32)(convB.d - lbl_eu_8066B0E8) / lbl_eu_8066B128;
                    PSMTXScaleApply(node->mtx, node->mtx, ratio, lbl_eu_8066B0D8,
                                    lbl_eu_8066B0D8);
                }
            }
        } else {
            // Concat with the material transform; the second copy mirrors the
            // dead-looking duplicate store retail keeps on the stack.
            Mtx out1;
            Mtx out2;
            MatFlags* mat = (MatFlags*)self->field_0x08;
            void* m = func_80496264(mat->field_0x10, -1);
            PSMTXConcat((const float(*)[4])((u8*)m + 0xcc), node->mtx, out1);
            *(MtxCopy*)&node->mtx = *(MtxCopy*)&out1;
            // Retail keeps a second (dead-looking) copy of the result on the
            // stack; volatile defeats MWCC's dead-store elimination.
            {
                volatile MtxCopy* vdst = (volatile MtxCopy*)&out2;
                const MtxCopy* vsrc = (const MtxCopy*)&out1;
                vdst->w0 = vsrc->w0;
                vdst->w1 = vsrc->w1;
                vdst->w2 = vsrc->w2;
                vdst->w3 = vsrc->w3;
                vdst->w4 = vsrc->w4;
                vdst->w5 = vsrc->w5;
            }
        }
        break;
    }
    default:
        break;
    }

    // Selector byte behind field_0x104 becomes the draw call's material arg.
    u8* mp = (u8*)self->field_0x0c->field_0x104;
    s32 material = mp ? (*(mp - 2) != 0) : 0;

    // Stack texture-matrix source payload for the gradient quad emitter.
    TexMtxSrc src;
    src.mTex = res;
    src.mIndex = (s8)self->field_0x25a;
    src.field_0x08 = 0;
    f32 zero = lbl_eu_8066B0DC;
    f32 one = lbl_eu_8066B0D8;
    src.field_0x0c = zero;
    src.field_0x10 = zero;
    src.field_0x14 = zero;
    src.field_0x18 = one;
    src.field_0x1c = one;
    src.field_0x20 = zero;
    u8* pb = (u8*)self->field_0x0c->field_0x108;
    if (pb != NULL) {
        src.field_0x08 = *(pb - 7);
        src.field_0x0c = self->field_0x270;
        src.field_0x10 = self->field_0x274;
        src.field_0x14 = zero;
        convA.w[1] = *(pb - 1);
        src.field_0x18 = (f32)(convA.d - lbl_eu_8066B0E8);
        convB.w[1] = *(pb - 2);
        src.field_0x1c = (f32)(convB.d - lbl_eu_8066B0E8);
        src.field_0x20 = zero;
    }
    DrawQuad quad;
    quad.x = self->field_0x19c[0][3];
    quad.y = self->field_0x19c[1][3];
    quad.z = self->field_0x19c[2][3];
    MatFlags* mat = (MatFlags*)self->field_0x08;
    func_804F0258(mat->field_0x10, &quad, (ml::CVec3*)&self->field_0x29c,
                  (TexDrawSize*)&self->field_0x1cc, (void*)material, &src);
}
// Target 6: build an 18-float matrix from two scale bytes (selected by i7)
// and two ints. A scales the B0E4 constant terms; B weights the direction
// terms built from i5 and the reciprocal of i6. Both 0x4330 conversion slots
// are initialized up front (retail stores the magic twice in the prologue).
void func_804D3DB0(EffectScene* self, f32* out, s32 i5, s32 i6, s32 i7) {
    FConv cB, cA;
    f32 A = lbl_eu_8066B0D8;
    f32 B = lbl_eu_8066B0D8;
    SceneSubObj* sub = self->field_0x0c;
    cA.w[0] = 0x43300000u;
    u8* p = (u8*)sub->field_0x108;
    cB.w[0] = 0x43300000u;
    if (p != NULL) {
        if (i7 == 0) {
            cA.w[1] = (u32)(p ? p[-4] : 1);
            A = (f32)(cA.d - lbl_eu_8066B0E8);
            cB.w[1] = (u32)(p ? p[-3] : 1);
            B = (f32)(cB.d - lbl_eu_8066B0E8);
        } else {
            cA.w[1] = (u32)(p ? p[-2] : 1);
            A = (f32)(cA.d - lbl_eu_8066B0E8);
            cB.w[1] = (u32)(p ? p[-1] : 1);
            B = (f32)(cB.d - lbl_eu_8066B0E8);
        }
    }
    f64 convSub = lbl_eu_8066B0F0;
    f32 fi6 = fconvS32(cA, (u32)i6 ^ 0x80000000u, convSub);
    f32 inv = lbl_eu_8066B0D8 / fi6;
    f32 zero = lbl_eu_8066B0DC;
    out[0] = zero;
    out[1] = B * (fconvS32(cB, (u32)i5 ^ 0x80000000u, convSub) * inv);
    out[2] = zero;
    f32 ka = lbl_eu_8066B0E4 * A;
    out[3] = ka;
    out[4] = B * (fconvS32(cA, (u32)i5 ^ 0x80000000u, convSub) * inv);
    out[5] = zero;
    out[6] = A;
    out[7] = B * (fconvS32(cB, (u32)i5 ^ 0x80000000u, convSub) * inv);
    out[8] = zero;
    out[9] = zero;
    out[10] = B * (fconvS32(cA, (u32)i5 ^ 0x80000000u, convSub) * inv + inv);
    out[11] = zero;
    out[12] = ka;
    out[13] = B * (fconvS32(cB, (u32)i5 ^ 0x80000000u, convSub) * inv + inv);
    out[14] = zero;
    out[15] = A;
    out[16] = B * (fconvS32(cA, (u32)i5 ^ 0x80000000u, convSub) * inv + inv);
    out[17] = zero;
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
// then clear the head list. old is declared before idx so it takes the higher
// saved register (retail: old in r31, idx in r30).
void func_804D3F94(EffectNode* self) {
    s32 old = 0;
    s32 idx = self->field_0x06;
    while (idx >= 0) {
        old = idx;
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

extern "C" s32 func_804D41F8(ListOwner* self, const Vend* target);
extern "C" void func_804D5E90(EffectNode* self);

// Target 7: per-node per-frame update. Only the head (visible frame-advance) is
// reconstructed; the sprite/sub-effect emission body is elided from the brief.
// Target 7: per-node per-frame update. Scales the frame delta into the
// node's accumulated weight, refreshes the tone-table state, then drives the
// five light/material processor blocks and the attached virtual applier.
extern "C" void __attribute__((never_inline)) func_804CCA64(void* nodeptr, f32 f) {
    EffectScene* node = (EffectScene*)nodeptr;
    node->field_0x18 = f * *(f32*)&node->field_0x14;

    // Tone-gate refresh: when the master bit and the owner's 0x40 flag are
    // set, clear the 0x4000 gate and reset the accumulated value.
    u16 h = node->field_0x06;
    if (h & 0x8000) {
        u16* owner = (u16*)node->field_0x08;
        if (*owner & 0x40) {
            node->field_0x06 = h & ~0x4000;
            *(f32*)&node->field_0x1c = lbl_eu_8066B0DC;
        }
    }

    // Per-material random-phase reseed: the gate byte selects a bit from the
    // signed shift count in the material object.
    SceneSubObj* sub = node->field_0x0c;
    u8* pf4 = (u8*)sub->field_0xf4;
    if (pf4 != NULL) {
        u8 sel = *(pf4 + 2);
        if (sel != 0 && !(node->field_0x06 & 0x1000)) {
            s32 cnt = *(s8*)((u8*)node->field_0x08 + 0x59);
            if (cnt > 0 && (sel & (1 << (cnt - 1)))) {
                node->field_0x06 &= ~0x4000;
                u16 hw = *(u16*)(pf4 + 4);
                node->field_0x06 |= 0x1000;
                *(f32*)&node->field_0x1c = u16ToF_b0e8(hw);
            }
        }
    }

    if (!(node->field_0x06 & 0x4000)) {
        *(f32*)&node->field_0x1c -= *(f32*)&node->field_0x18;
    }

    f32 val = *(f32*)&node->field_0x18;
    s32 clamped = (*(f32*)&node->field_0x10 <= lbl_eu_8066B0D8);
    f32 lim = CDeviceVI::isTvFormatPal() ? lbl_eu_8066B0F8 : lbl_eu_8066B0D8;
    if (lim < val) {
        if (clamped) {
            clamped = 0;
            val -= lim;
        }
    }

    void* mat = (MatFlags*)node->field_0x08;
    if (!clamped) {
        // Five guarded processor blocks; each fires while its top-bit gate is
        // set, passing its own storage block and the current weight.
        u8 g = node->field_0x210;
        if (((g >> 7) & 1) || ((g >> 6) & 3) != 0) {
            func_804E1044(&node->field_0x20c, (void*)sub->field_0xf8, mat, val);
        }
        if (node->field_0x23c & 0x80) {
            func_804E18CC(&node->field_0x238, (void*)sub->field_0xf8, mat, val);
        }
        if (node->field_0x250 & 0x80) {
            func_804E1A44(&node->field_0x24c, (void*)sub->field_0x104, mat, val);
        }
        if (node->field_0x260 & 0x80) {
            func_804E1C1C(&node->field_0x25c, (void*)sub->field_0x108, mat, val);
        }
        if (node->field_0x314 & 0x80) {
            func_804E2EAC(&node->field_0x310, (void*)sub->field_0x118, mat, val);
        }
    }

    // First tone block: either pull the processor result or snapshot the
    // material's tone colors into node+0x278..0x2a7.
    u8* p110 = (u8*)sub->field_0x110;
    u8 tb = p110 ? *(p110 - 0x1c) : 0;
    if (tb == 0) {
        if (!clamped && (node->field_0x27c & 0x80)) {
            func_804E2088(&node->field_0x278, (void*)sub->field_0x110, mat, val);
        }
    } else {
        node->field_0x278 = NULL;
        node->field_0x27c = sub->field_0x198;
        node->field_0x27d = sub->field_0x199;
        node->field_0x27e = sub->field_0x19a;
        node->field_0x27f = sub->field_0x19b;
        u32 w = sub->field_0x1a0;
        u32 v = sub->field_0x1a4;
        node->field_0x288 = v;
        node->field_0x284 = w;
        node->field_0x28c = sub->field_0x1a8;
        w = sub->field_0x1ac;
        v = sub->field_0x1b0;
        node->field_0x294 = v;
        node->field_0x290 = w;
        node->field_0x298 = sub->field_0x1b4;
        w = sub->field_0x1b8;
        v = sub->field_0x1bc;
        node->field_0x2a0 = v;
        node->field_0x29c = w;
        node->field_0x2a4 = sub->field_0x1c0;
    }

    // Second tone block (same shape, node+0x2e8..0x30f).
    u8* p114 = (u8*)sub->field_0x114;
    u8 tb2 = p114 ? *(p114 - 0x18) : 0;
    if (tb2 == 0) {
        if (!clamped && (node->field_0x2ec & 0x80)) {
            func_804E2A5C(&node->field_0x2e8, (void*)sub->field_0x114, mat, val);
        }
    } else {
        node->field_0x2e8 = 0;
        node->field_0x2ec = sub->field_0x208;
        u32 w = sub->field_0x210;
        u32 v = sub->field_0x214;
        node->field_0x2f8 = v;
        node->field_0x2f4 = w;
        w = sub->field_0x218;
        v = sub->field_0x21c;
        node->field_0x300 = v;
        node->field_0x2fc = w;
        node->field_0x304 = sub->field_0x220;
        node->field_0x308 = sub->field_0x224;
        node->field_0x30c = sub->field_0x228;
    }

    // Apply the accumulated weight through the attached object's slot-5
    // virtual when both the 0x400 gate and the object pointer are live.
    if ((sub->field_0x1c & 0x400) && node->field_0x32c != NULL) {
        ((CCA64VirtObj*)node->field_0x32c)->apply(*(f32*)&node->field_0x18);
    }
}

// Target 4: walk the node chain updating each; advance via the step.
void func_804D401C(EffectNode* self, f32 f) {
    EffectNode* node;
    if (self->field_0x0a <= 0) return;
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
        if (!(f <= lim)) {
            if (!((EffectSceneFlags*)&node->field_0x06)->b15) {
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
    f32 lim = lbl_eu_8066B140;
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
extern "C" s32 __attribute__((never_inline)) func_804D41F8(ListOwner* self, const Vend* target) {
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

// Render sub-object at EffectInit+0x14: type id at +0, sub-mode at +4.
struct RenderTypeObj {
    s32 field_0x00;   // type id
    s32 field_0x04;   // sub-mode
};

// TEV swap-index holder at EffectInit+0x10: the swap index lives at +0x5c.
// (Mirrors MtxHolder below; declared here so the dispatch can use it.)
struct SwapHolder {
    u8 pad_0x00[0x5c];
    s32 field_0x5c;   // tev swap index
};

// func_804D8AA4's retail reads only r3 (the fog toggle); the shared header
// declares a second unused param. Cast through a 1-arg pointer type so the
// call sites emit only `li r3, N` like retail.
typedef void (*FogPushFn)(int);

struct RenderObj;
extern "C" void func_804D5764(RenderObj* self);
extern "C" void func_804D513C(RenderObj* self);

// Target 1 (us-804d842c): render-dispatch for a linked effect node chain.
// Guards on the 0x0a frame counter, clears the GX vertex-descriptor state,
// then switches on the sub-object type at self+0x14: types 8/0xc/5/6/4/7 run a
// per-type node walk (each with its own advance helper); the default runs one
// of two render helpers selected by the sub-mode at +4. Cases 0xc/5/6/4/7
// refresh the TEV swap tables from the 0x5c swap index when the 0x18 cache is
// stale. The final block resets the swap tables when the cache is non-zero and
// pushes the fog state.
void func_804D42B8(EffectInit* self, void* arg) {
    if (self->field_0x0a <= 0) return;
    GXClearVtxDesc();
    RenderTypeObj* t = (RenderTypeObj*)self->field_0x14;
    switch (t->field_0x00) {
    case 8: {
        func_804D5764((RenderObj*)self);
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
            func_804D189C(node, arg);
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
        break;
    }
    case 0xc: {
        SwapHolder* mh = (SwapHolder*)self->field_0x10;
        s32 cache = self->field_0x18;
        s32 idx = mh->field_0x5c;
        if (cache != idx) {
            switch (idx) {
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
            case 3:
                GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                break;
            }
            self->field_0x18 = idx;
        }
        ((FogPushFn)func_804D8AA4)(0);
        func_804D0194();
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
            func_804D20EC(node, arg);
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
        break;
    }
    case 5: {
        SwapHolder* mh = (SwapHolder*)self->field_0x10;
        s32 cache = self->field_0x18;
        s32 idx = mh->field_0x5c;
        if (cache != idx) {
            switch (idx) {
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
            case 3:
                GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                break;
            }
            self->field_0x18 = idx;
        }
        ((FogPushFn)func_804D8AA4)(0);
        func_804D0194();
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
            func_804D2690(node, arg);
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
        break;
    }
    case 6: {
        SwapHolder* mh = (SwapHolder*)self->field_0x10;
        s32 cache = self->field_0x18;
        s32 idx = mh->field_0x5c;
        if (cache != idx) {
            switch (idx) {
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
            case 3:
                GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                break;
            }
            self->field_0x18 = idx;
        }
        ((FogPushFn)func_804D8AA4)(0);
        func_804D0194();
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
            func_804D2B60(node, arg);
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
        break;
    }
    case 4: {
        SwapHolder* mh = (SwapHolder*)self->field_0x10;
        s32 cache = self->field_0x18;
        s32 idx = mh->field_0x5c;
        if (cache != idx) {
            switch (idx) {
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
            case 3:
                GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                break;
            }
            self->field_0x18 = idx;
        }
        ((FogPushFn)func_804D8AA4)(0);
        func_804D0194();
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
            func_804D3098(node, arg);
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
        break;
    }
    case 7: {
        SwapHolder* mh = (SwapHolder*)self->field_0x10;
        s32 cache = self->field_0x18;
        s32 idx = mh->field_0x5c;
        if (cache != idx) {
            switch (idx) {
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
            case 3:
                GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
                break;
            }
            self->field_0x18 = idx;
        }
        ((FogPushFn)func_804D8AA4)(0);
        func_804D0194();
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
            func_804D361C(node, arg);
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
        break;
    }
    default: {
        if (t->field_0x04 == 5) {
            func_804D5764((RenderObj*)self);
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
            if (node) {
                EffectNode* node2;
                if (self->field_0x06 >= 0) {
                    node2 = func_804E0114(self->field_0x06);
                    self->field_0x00 = -1;
                    self->field_0x02 = self->field_0x06;
                    self->field_0x04 = node2->field_0x02;
                    self->field_0x0c = 0;
                } else {
                    node2 = 0;
                }
                func_804D0AB4((EffectScene*)(void*)node2, (s32)arg);
            }
        } else {
            func_804D513C((RenderObj*)self);
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
                func_804D01E0((EffectScene*)(void*)node, arg);
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
        break;
    }
    }
    if (self->field_0x18 != 0) {
        GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
        self->field_0x18 = 0;
    }
    ((FogPushFn)func_804D8AA4)(1);
}

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
extern "C" void func_804D5764(RenderObj* self) {
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
        // Ambient color: 4-word copy of the source block (retail uses lwz/stw),
        // scaled by the material factor then clamped into [B140, B14C].
        ColorSrc* cs = self->field_0x10->field_0x10->field_0x5c;
        ml::CCol4 s = *(ml::CCol4*)&cs->field_0xac;
        void* q2 = self->field_0x14->field_0x114;
        f32 f7 = q2 ? *(f32*)((u8*)q2 - 4) : lbl_eu_8066B144;
        f32 d[4];
        d[0] = s.r * f7;
        d[1] = s.g * f7;
        d[2] = s.b * f7;
        d[3] = s.a;
        f32 c[4];
        c[0] = d[0] * lbl_eu_8066B148;
        c[1] = d[1] * lbl_eu_8066B148;
        c[2] = d[2] * lbl_eu_8066B148;
        c[3] = d[3];
        if (c[0] > lbl_eu_8066B14C) c[0] = lbl_eu_8066B14C;
        else if (c[0] < lbl_eu_8066B140) c[0] = lbl_eu_8066B140;
        if (c[1] > lbl_eu_8066B14C) c[1] = lbl_eu_8066B14C;
        else if (c[1] < lbl_eu_8066B140) c[1] = lbl_eu_8066B140;
        if (c[2] > lbl_eu_8066B14C) c[2] = lbl_eu_8066B14C;
        else if (c[2] < lbl_eu_8066B140) c[2] = lbl_eu_8066B140;
        if (c[3] > lbl_eu_8066B14C) c[3] = lbl_eu_8066B14C;
        else if (c[3] < lbl_eu_8066B140) c[3] = lbl_eu_8066B140;
        GXColor col;
        col.r = (u8)(s32)(lbl_eu_8066B150 * c[0]);
        col.g = (u8)(s32)(lbl_eu_8066B150 * c[1]);
        col.b = (u8)(s32)(lbl_eu_8066B150 * c[2]);
        col.a = (u8)(s32)(lbl_eu_8066B150 * c[3]);
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
    // extrwi 1,20 = bit 10; extrwi 1,25 = bit 5.
    if (dd0 == 0 && !((self->field_0x14->field_0x1c >> 10) & 1)) bfl = 1;
    s32 tp = 0;
    if (bfl && !((self->field_0x14->field_0x1c >> 5) & 1)) tp = 1;
    func_804D8AA4(tp, bfl);
    func_804D0194();
}

// Target 2 (us-804d92b0): near-twin of func_804D5764. Same GX pass but uses
// a palette array for POS vertex data, no position matrix load, and table
// based blends.
extern "C" void func_804D513C(RenderObj* self) {
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_CLR0, GX_NONE);
    // Palette select: retail extracts bit 10 (0x0400) of the flag word and
    // materializes (bit != 0) as a 0/1 value before branching on it.
    u32 bit = (self->field_0x14->field_0x1c >> 10) & 1;
    u32 cond = bit != 0;
    const u8* arr;
    if (cond) {
        arr = lbl_eu_80524520;
    } else {
        arr = lbl_eu_80524500;
    }
    GXSetArray(GX_VA_POS, (void*)arr, 3);
    void* q = self->field_0x14->field_0x114;
    u8 b = q ? *(u8*)((u8*)q - 0x1c) : 0;
    if (b == 0) {
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    } else {
        GXSetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        // Ambient color: 4-word copy of the source block (retail uses lwz/stw),
        // scaled by the material factor then clamped into [B140, B14C].
        ColorSrc* cs = self->field_0x10->field_0x10->field_0x5c;
        ml::CCol4 s = *(ml::CCol4*)&cs->field_0xac;
        void* q2 = self->field_0x14->field_0x114;
        f32 f7 = q2 ? *(f32*)((u8*)q2 - 4) : lbl_eu_8066B144;
        f32 d[4];
        d[0] = s.r * f7;
        d[1] = s.g * f7;
        d[2] = s.b * f7;
        d[3] = s.a;
        f32 c[4];
        c[0] = d[0] * lbl_eu_8066B148;
        c[1] = d[1] * lbl_eu_8066B148;
        c[2] = d[2] * lbl_eu_8066B148;
        c[3] = d[3];
        if (c[0] > lbl_eu_8066B14C) c[0] = lbl_eu_8066B14C;
        else if (c[0] < lbl_eu_8066B140) c[0] = lbl_eu_8066B140;
        if (c[1] > lbl_eu_8066B14C) c[1] = lbl_eu_8066B14C;
        else if (c[1] < lbl_eu_8066B140) c[1] = lbl_eu_8066B140;
        if (c[2] > lbl_eu_8066B14C) c[2] = lbl_eu_8066B14C;
        else if (c[2] < lbl_eu_8066B140) c[2] = lbl_eu_8066B140;
        if (c[3] > lbl_eu_8066B14C) c[3] = lbl_eu_8066B14C;
        else if (c[3] < lbl_eu_8066B140) c[3] = lbl_eu_8066B140;
        GXColor col;
        col.r = (u8)(s32)(lbl_eu_8066B150 * c[0]);
        col.g = (u8)(s32)(lbl_eu_8066B150 * c[1]);
        col.b = (u8)(s32)(lbl_eu_8066B150 * c[2]);
        col.a = (u8)(s32)(lbl_eu_8066B150 * c[3]);
        GXSetChanAmbColor(GX_COLOR0A0, col);
        GXSetChanAmbColor(GX_COLOR1A1, col);
    }
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e, GX_FALSE, 0x7d);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x21, GX_FALSE, 0x7d);
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
    // Retail booleanizes each bit test (extrwi/neg/or/srwi.) before branching.
    // extrwi 1,20 = bit 10; extrwi 1,25 = bit 5.
    u32 b20 = (self->field_0x14->field_0x1c >> 10) & 1;
    u32 c20 = b20 != 0;
    s32 bfl = 0;
    if (dd0 == 0 && !c20) bfl = 1;
    u32 b25 = (self->field_0x14->field_0x1c >> 5) & 1;
    u32 c25 = b25 != 0;
    s32 tp = 0;
    if (bfl && !c25) tp = 1;
    func_804D8AA4(tp, bfl);
    func_804D0194();
}

// Target 8: us-804d9ebc - attach node list entry by index.
EffectNode* func_804D5D48(EffectNode* self) {
    s32 idx = self->field_0x06;
    if (idx >= 0) {
        EffectNode* node = func_804E0114(idx);
        self->field_0x00 = -1;
        self->field_0x02 = self->field_0x06;
        self->field_0x04 = node->field_0x02;
        self->field_0x0c = 0;
        return node;
    }
    return 0;
}

// Target 9: us-804d9f20 - step to next node slot.
EffectNode* func_804D5DAC(EffectNode* self) {
    s32 cur = self->field_0x04;
    if (cur >= 0) {
        self->field_0x00 = self->field_0x02;
        self->field_0x02 = cur;
        EffectNode* node = func_804E0114(cur);
        self->field_0x04 = node->field_0x02;
        self->field_0x0c = self->field_0x0c + 1;
        return node;
    }
    return 0;
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

extern "C" void __attribute__((never_inline)) func_804D5E90(EffectNode* self) {
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
// cubic basis across t in [start,end) stepping by end/count.
f32* func_804D5F54(void* this_, s32 count) {
    MemManGlob* mg = (MemManGlob*)lbl_eu_8065FC18;
    u32 max = mtl::MemManager::getMaxAllocSize((mtl::ALLOC_HANDLE)mg->field_0x04);
    u32 req = (u32)count << 4;
    f32* buf;
    if (req <= max) {
        buf = (f32*)mtl::MemManager::allocate_array(
            req, (mtl::ALLOC_HANDLE)mg->field_0x04);
    } else {
        buf = 0;
    }
    if (!buf) return 0;
    // Retail divides end by the raw int count: MWCC's int->float conversion
    // (magic 2^52 pool entry) feeds the single-precision divide directly.
    s16 i = 0;
    f32 f8 = lbl_eu_8066B158;
    f32 t = lbl_eu_8066B15C;
    f32 f7 = lbl_eu_8066B164;
    f32 f5 = lbl_eu_8066B160;
    f32 f1 = lbl_eu_8066B168;
    f32 dt = f8 / count;
    while (t < f8) {
        f32 a2 = t * t;
        f32 a3 = a2 * t;
        buf[i++] = f8 + (f5 * a3 - f7 * a2);
        buf[i++] = t + (a3 - f5 * a2);
        buf[i++] = a3 - a2;
        buf[i++] = f1 * a3 + f7 * a2;
        t += dt;
    }
    return buf;
}

void func_804D6070(void) {}

void func_804D6074(){}

s32 func_804D6BC0(void* unused, Node2Control* c, Node2** p5, Node2** p6,
               Node2* volatile* p7,
               Node2* volatile* p8, Node2** p9) {
    // Retail reloads *p5 for every output slot (the stores may alias p5), so
    // keep one local per slot pair; the p9 walk chains through the temp.
    // p7/p8 take volatile pointees so each *p7/*p8 read is a real load,
    // matching retail's reloads inside the branches.
    Node2* a = *p5;
    *p6 = a;
    *p6 = a->field_0x04;
    *p7 = *p5;
    Node2* b = *p5;
    *p8 = b;
    *p8 = b->field_0x00;
    Node2* e = *p5;
    *p9 = e;
    e = e->field_0x00;
    *p9 = e;
    *p9 = e->field_0x00;
    // Volatile pointee forces the fresh *p7 load retail emits in the body
    // (each *p7 read is a real load, no caching).
    if (*p7 == c->field_0x04->field_0x00) {
        *p6 = *p7;
    }
    if (*p8 == c->field_0x04) {
        *p8 = *p7;
        *p9 = *p7;
        return 1;
    }
    if ((u32)*p9 == (u32)c->field_0x04) *p9 = *p8;
    return 0;
}

// Target 1: us-804d8184 - simple setter: stores two params at offsets 0x10/0x14
void func_804D4010(EffectStruct* self, u32 a, u32 b) {
    self->field_0x10 = a;
    self->field_0x14 = b;
}

// Target 2: us-804d22b4 - if field_0x1c <= 0.0f, clear bit 15 (0x8000) of field_0x06
// A bitfield clear reproduces the exact wrap mask (same pattern as
// CScheduleFlags::b15 in code_804E36DC.cpp).
extern "C" void __attribute__((never_inline)) func_804CE140(EffectScene* self) {
    if (lbl_eu_8066B0DC < self->field_0x1c) return;
    ((EffectSceneFlags*)&self->field_0x06)->b15 = 0;
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
extern "C" void __attribute__((never_inline)) func_804CE3E8(EffectScene* self) {
    if (self->field_0x328 == 0) return;
    if (self->field_0x18 <= lbl_eu_8066B0DC) return;
    self->field_0x06 |= 0x40;
}

// Camera-side layout for func_804D3B14: a pointer at 0x08 to an object whose
// +0x10 is the projection-matrix source.
struct D3B14CamObj {
    u8 pad_0x00[0x10];
    void* field_0x10;
};
struct D3B14Cam {
    u8 pad_0x00[0x08];
    D3B14CamObj* field_0x08;
};

// Screen-space output position: x/y/z at 0x00/0x04/0x08 (also read as raw
// words by the word-wise copy).
struct ScreenPos {
    Vec pos;
};

// Project a world position to screen space: transform by the camera matrix,
// apply the perspective divide (1/-z) to x/y, then scale by the framebuffer
// dimensions. Returns 0 when the depth is negative. Note the asymmetric
// mapping: outA gets the centered (*half + half) viewport map while outB is
// only scaled by the raw framebuffer size.
s32 func_804D3B14(D3B14Cam* cam, Vec* outA, Vec* outB) {
    f32 z = outA->z;
    // fcmpo(constant, z) + cror merge -> constant-first <= form.
    if (lbl_eu_8066B0DC <= z) {
        Mtx m;
        f32 s = lbl_eu_8066B0D8 / -z;
        func_80496120(cam->field_0x08->field_0x10, m, -1);

        Vec v;
        v.y = outA->x * m[1][0] + outA->y * m[1][1] + outA->z * m[1][2] + m[1][3];
        v.x = outA->x * m[0][0] + outA->y * m[0][1] + outA->z * m[0][2] + m[0][3];
        v.z = outA->x * m[2][0] + outA->y * m[2][1] + outA->z * m[2][2] + m[2][3];
        *outA = v;
        outA->x *= s;
        outA->y *= s;

        u16 w = CDeviceVI::getRenderModeObj()->fbWidth;
        outA->x = (outA->x * lbl_eu_8066B0E4 + lbl_eu_8066B0E4) * (f32)w;
        u16 h = CDeviceVI::getRenderModeObj()->efbHeight;
        outA->z = lbl_eu_8066B0DC;
        outA->y = (outA->y * lbl_eu_8066B12C + lbl_eu_8066B0E4) * (f32)h;

        if (outB != 0) {
            Vec v2;
            v2.y = outB->x * m[1][0] + outB->y * m[1][1] + outB->z * m[1][2] + m[1][3];
            v2.x = outB->x * m[0][0] + outB->y * m[0][1] + outB->z * m[0][2] + m[0][3];
            v2.z = outB->x * m[2][0] + outB->y * m[2][1] + outB->z * m[2][2] + m[2][3];
            *outB = v2;
            outB->x *= s;
            outB->y *= s;

            u16 w2 = CDeviceVI::getRenderModeObj()->fbWidth;
            outB->x = outB->x * (f32)w2;
            u16 h2 = CDeviceVI::getRenderModeObj()->efbHeight;
            outB->z = lbl_eu_8066B0DC;
            outB->y = outB->y * (f32)h2;
        }
        return 1;
    }
    return 0;
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056FC10-0x8056FC40 (0x30): func_804CE4C0 switch jumptable
// (function + case offset, retail relocs carry the +164/+192/+216/+468
// addends).
extern "C" u32 jumptable_eu_8056FC10[12] = {
    (u32)((char*)&func_804CE4C0 + 164), (u32)((char*)&func_804CE4C0 + 164),
    (u32)((char*)&func_804CE4C0 + 192), (u32)((char*)&func_804CE4C0 + 192),
    (u32)((char*)&func_804CE4C0 + 216), (u32)((char*)&func_804CE4C0 + 468),
    (u32)((char*)&func_804CE4C0 + 164), (u32)((char*)&func_804CE4C0 + 164),
    (u32)((char*)&func_804CE4C0 + 192), (u32)((char*)&func_804CE4C0 + 192),
    (u32)((char*)&func_804CE4C0 + 216), 0x00000000,
};

// [.rodata] 0x80524500-0x80524530 (0x30): palette arrays (raw retail bytes).
extern "C" const u8 lbl_eu_80524500[32] = {
    0xFF,0x01,0x00,0x01, 0x01,0x00,0x01,0xFF, 0x00,0xFF,0xFF,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
};
extern "C" const u8 lbl_eu_80524520[16] = {
    0xFF,0xFF,0x00,0x01, 0xFF,0x00,0x01,0x01, 0x00,0xFF,0x01,0x00,
    0x00,0x00,0x00,0x00,
};

// [.sbss] 0x806659A8-0x806659BC (0x14 = 20B): 5 cache-state words.
extern "C" { s32 lbl_eu_806659A8; s32 lbl_eu_806659AC; s32 lbl_eu_806659B0;
              s32 lbl_eu_806659B4; s32 lbl_eu_806659B8; }