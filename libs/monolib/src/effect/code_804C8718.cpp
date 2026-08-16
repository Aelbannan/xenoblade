// Auto-scaffolded catalog TU for monolib/src/effect/code_804C8718
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include <revolution/GX.h>
#include <revolution/os/OSFastCast.h>
#include "monolib/util/MemManager.hpp"
#include "monolib/core/CScheduleItem.hpp"
#include "monolib/effect/CERand.hpp"
extern "C" void func_804E4DD4(f32);
#include "monolib/effect/code_804CC2B8.hpp"
#include <revolution/MTX.h>
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/math/CMat34.hpp"

void sinit_804CC2B4(void) {}

// ---------------------------------------------------------------------------
// Effect-manager singleton (lbl_eu_8065FC18) partial layout. The retail class
// is anonymous (plain C free functions), so unknown fields keep field_0xNN
// names. Only fields touched by the targets below are declared.
// ---------------------------------------------------------------------------

// Per-scene effect node-chain head at EffectRoot+0x24 (initialized by
// func_804D3F60, advanced by func_804D401C). Layout mirrors the EffectInit
// view used in code_804CC2B8.cpp.
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

struct Field08Obj {
    u16 field_0x00;          // 0x00 flags
    u8 pad_0x02[0xc - 0x02];
    void* field_0x0c;        // 0x0c
};

struct EffectRoot {
    u16 flags0;              // 0x00
    u8 pad_0x02[2];
    u32 allocHandle;         // 0x04
    Field08Obj* field_0x08;  // 0x08
    f32 field_0x0c;          // 0x0c
    u16 field_0x10;          // 0x10
    u16 field_0x12;          // 0x12
    f32 field_0x14;          // 0x14
    f32 field_0x18;          // 0x18
    u16 flags1;              // 0x1c
    s16 field_0x1e;          // 0x1e
    s16 field_0x20;          // 0x20
    u8 field_0x22;           // 0x22
    u8 field_0x23;           // 0x23
    u8 renderArg[0x24];      // 0x24
    u32 field_0x48;          // 0x48
    u8 link0[0x48];          // 0x4c
    u8 link1[0x10];          // 0x94
    u8 pad_0xa4[0xdc - 0xa4];
    u32 field_0xdc[16];      // 0xdc
};

// Registered-effect list used by the render passes: one node type is shared by
// both levels. Outer nodes chain by 0x00 with the child list head at 0x04;
// inner nodes chain by 0x00 with the effect object at 0x08.
// lbl_eu_80665A30 is the outer head.
struct EffectListNode {
    EffectListNode* next;       // 0x00
    EffectListNode* field_0x04; // 0x04 (child list head on outer nodes)
    EffectRoot* obj;            // 0x08 (effect object on inner nodes)
    u8 pad_0x0c[0x10 - 0x0c];
    u16 field_0x10;             // 0x10 (child-list enabled flag)
};

// Full-layout effect object used by func_804CB5FC (the shared CEffectObj in
// CScheduleItem.hpp only covers the fields the schedule TUs touch).
struct BoundScene {
    u8 pad_0x00[0x10];
    void* field_0x10;       // 0x10: scene handle passed to func_80496264
};

struct EffObj {
    u8 pad_0x00[0x08];
    BoundScene* field_0x08; // 0x08: bound scene/desktop
    u8 pad_0x0c[0x1c - 0x0c];
    u16 field_0x1c;         // 0x1c flags (bit 0x800 = direct-priority path)
    u8 pad_0x1e[0x23 - 0x1e];
    s8 field_0x23;          // 0x23 signed priority byte
    u8 pad_0x24[0x64 - 0x24];
    ml::CMat34 mtx;         // 0x64 transform matrix
    u8 pad_0x94[0x128 - 0x94];
    ml::CVec3 pos;          // 0x128 world position
};

// Object returned by func_80496264: view matrix at +0xcc, depth scale at +0x1f0.
struct SceneViewObj {
    u8 pad_0x00[0xcc];
    Mtx field_0xcc;         // 0xcc view matrix
    u8 pad_0xfc[0x1f0 - 0xfc];
    f32 field_0x1f0;        // 0x1f0 depth scale
};

// Object with a type id at +0x00 (compared to 0xe by func_804CB274).
struct EffTypeObj {
    u32 field_0x00;         // 0x00
};

// Init blob (the `data` parameter of func_804C8830): two u16 offsets into the
// resource base select the offset tables read for the 0xdc tail block.
struct EffInitBlob {
    s16 field_0x00;          // 0x00
    s16 field_0x02;          // 0x02
    u16 field_0x04;          // 0x04
    u16 field_0x06;          // 0x06
    u8 field_0x08;           // 0x08 (2 flag bits -> flags1 bits 8-9)
    u8 pad_0x09[0x0d - 0x09];
    u8 field_0x0d;           // 0x0d
    u8 field_0x0e;           // 0x0e (bit -> flags1 bit 7)
    u8 field_0x0f;           // 0x0f (bit -> flags1 bit 6)
    u16 field_0x10;          // 0x10
    u16 field_0x12;          // 0x12
};

// u16 offset table read through the resolved p04/p06 pointers.
struct EffOffList {
    u16 field_0x00;          // 0x00
    u16 field_0x02;          // 0x02
    u16 field_0x04;          // 0x04
    u16 field_0x06;          // 0x06
    u16 field_0x08;          // 0x08
    u16 field_0x0a;          // 0x0a
    u16 field_0x0c;          // 0x0c
    u16 field_0x0e;          // 0x0e
    u16 field_0x10;          // 0x10
    u16 field_0x12;          // 0x12
    u16 field_0x14;          // 0x14
};

// Full effect-object view used by func_804C8830 / func_804CAC08 /
// func_804C8D90 (same object as EffectRoot but with a u32 type word at 0x00
// and the resolved-pointer tail block typed as u32).
struct EffObject {
    u32 field_0x00;          // 0x00 type
    u32 field_0x04;          // 0x04
    Field08Obj* field_0x08;  // 0x08
    f32 field_0x0c;          // 0x0c
    s16 field_0x10;          // 0x10
    s16 field_0x12;          // 0x12
    f32 field_0x14;          // 0x14
    f32 field_0x18;          // 0x18
    u16 flags1;              // 0x1c
    s16 field_0x1e;          // 0x1e
    s16 field_0x20;          // 0x20
    u8 field_0x22;           // 0x22
    u8 field_0x23;           // 0x23
    u8 renderArg[0x0a];      // 0x24
    s16 field_0x2e;          // 0x2e
    u8 pad_0x30[0x40 - 0x30];
    u32 field_0x40;          // 0x40
    u32 field_0x44;          // 0x44
    u32 field_0x48;          // 0x48
    u8 link0[0x48];          // 0x4c
    u8 link1[0x10];          // 0x94
    u8 pad_0xa4[0xdc - 0xa4];
    u32 field_0xdc[16];      // 0xdc..0x118 resolved pointers
    f32 sub_0x11c[6];        // 0x11c..0x134 sub-region (func_804E0580/E06B4 target)
    ml::CVec3 field_0x134;   // 0x134 (emission vector + y read at 0x138)
    u8 pad_0x140[0x14c - 0x140];
    f32 sub_0x14c[6];        // 0x14c..0x164 (func_804E0788/E08BC target)
    ml::CVec3 field_0x164;   // 0x164 (emission vector)
    u8 pad_0x170[0x17c - 0x170];
    f32 sub_0x17c[6];        // 0x17c..0x194 (func_804E0990/E0B94 target)
    f32 sub_0x194[12];       // 0x194..0x1c4 (func_804E1D50/E2088 target)
    f32 sub_0x1c4[16];       // 0x1c4..0x204 (func_804E214C/E24A8 target)
    f32 sub_0x204[8];        // 0x204..0x224 (func_804E26D8/E2A5C target)
};

// Tail-state view used by func_804C8D90 (0x180..0x18c, inside the 0x17c
// sub-region range).
struct EffFrameTail {
    u8 field_0x180;          // 0x180
    u8 pad_0x181[0x188 - 0x181];
    u16 field_0x188;         // 0x188
    u16 field_0x18a;         // 0x18a
};

// Cross-TU imports for the targets below (the shared headers are read-only
// in this session, so the declarations live here next to the existing block).
extern "C" {
u32 func_804E53D8(void);
void func_804E4E8C(u32 x);
u32 func_804DFE8C(void);
void func_804DF7A4(void);
void func_804F3B4C(void* obj, void* arg);
void func_804F3B60(void* obj, int flag, f32 w, f32 h);
void func_804D854C(void);
void func_804DF150(void);
void func_804D9364(ml::CVec3* out, const ml::CVec3* a, const ml::CVec3* b, f32 radius);
void func_804D9A10(ml::CVec3* out, const ml::CVec3* a, const ml::CVec3* b, f32 radius);
void func_804D9B70(ml::CVec3* out, const ml::CVec3* a, const ml::CVec3* b, f32 radius);
void func_804D9F00(ml::CVec3* out, const ml::CVec3* a, const ml::CVec3* b);
}

struct CScnRootEnv;
extern CScnRootEnv* lbl_eu_806659B8;
extern u8 lbl_eu_80663B3C;
extern f32 lbl_eu_8066A208;
extern f32 lbl_eu_8066B0A8;
extern f32 lbl_eu_8066B0C0;
extern f32 lbl_eu_8066B0C4;
extern u32 lbl_eu_8066B0D0;
extern const f64 lbl_eu_8066B0C8;   // 0x4330000000000000 (u->f magic)
extern const f64 lbl_eu_8066B0B0;   // 0x4330000000000000 (s16->f magic)

// u16 -> float through the shared 0x4330000000000000 magic double. The
// explicit extern reference pins the sdata2 reloc to the retail constant
// name (a plain (f32) cast would emit the TU's own unnamed pool entry).
inline f32 u16ToF_b0c8(u16 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[1] = v;
    c.w[0] = 0x43300000u;
    return (f32)(c.d - lbl_eu_8066B0C8);
}

// Signed value -> float through the 0x4330000080000000 magic
// (lbl_eu_8066B0B0). The xor folds the sign into the low word.
inline f32 s32ToF_b0b0(u32 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[1] = v ^ 0x80000000u;
    c.w[0] = 0x43300000u;
    return (f32)(c.d - lbl_eu_8066B0B0);
}

// Cross-TU imports + in-TU C-linkage declarations (retail symbol names -
// extern "C" keeps the unmangled reloc names; the definitions further down
// inherit this linkage so their symbols match the retail names).
extern "C" {
void func_804DF7FC(void);
void __dt__804DF744(void);
void func_804E4D58(u32 x);
u32 func_804DF4BC(void* key);
void func_804C868C(void);
void func_804C8684(void* scene);
u32 func_804DFE9C(void);
void func_804DF690(void);
void __ct__804E4B64(int arg);
void func_804D82DC(void* scene);
u32 func_804EEACC(void* link);
void func_804D42B8(void* obj, void* arg);
void func_804D401C(void* a, float f);
void func_804D3F60(EffectInit* self);
void func_804F4620(void* mtx);
void func_804F4D74(void* mtx);
void func_804CBEEC(void);
void func_804CB3E4(EffectRoot* self);
void func_804CB4F4(EffectRoot* self);
void func_804C8718(EffectRoot* self);
void func_804CBC90(EffectRoot* self);
void func_804CBDB4(EffectRoot* self);
void func_804CB458(EffectRoot* self);
void func_804CB560(EffectRoot* self);
void func_804D3F94(EffectNode* self);
void func_804DD754(void);
u32 func_804DF344(u32 p, u32 size);
u32 func_804DFA08(u32 p, u32 size);
u32 func_804DFC48(u32 p, u32 size);
u32 func_804DFF00(u32 p, u32 size);
u32 func_804DF5F8(u32* outC, CScheduleEntryData** out8, u32 a, u32 c);
u32 func_804E4EF8(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4, u32 p5);
extern EffectListNode* lbl_eu_80665A30;
extern f32 lbl_eu_8066B0A0;
extern f32 lbl_eu_8066B0A4;

// Effect sub-region bind/refresh helpers (stubs in code_804DEDA8.cpp): first
// arg is a sub-object inside the effect object, second the bound data pointer
// from the 0xdc tail block; the func_804E06B4-family take a scene/desktop
// pointer plus a float.
void func_804E0580(void* dst, const void* src);
void func_804E0788(void* dst, const void* src);
void func_804E0990(void* dst, const void* src);
void func_804E1D50(void* dst, const void* src);
void func_804E214C(void* dst, const void* src, u32 a3);
void func_804E26D8(void* dst, const void* src);
void func_804E06B4(void* dst, const void* src, const void* a3, f32 f);
void func_804E08BC(void* dst, const void* src, const void* a3, f32 f);
void func_804E0B94(void* dst, const void* src, const void* a3, f32 f);
void func_804E2088(void* dst, const void* src, const void* a3, f32 f);
void func_804E24A8(void* dst, const void* src, const void* a3, f32 f);
void func_804E2A5C(void* dst, const void* src, const void* a3, f32 f);
void func_804E0CF0(void* dst, const void* src);
int func_804DD6E8(int val);
void func_804D4010(void* self, const void* a, void* b);

// Scene-list iteration helpers (defined in code_804CC2B8.cpp).
void func_804CCF84(void* self);
void* func_804D5DAC(void* self);
void func_804D4144(void* self);
void func_804D5E10(void* self, s32 index);
void func_804CC808(void* self, const void* src);

// Scene/resource helpers (defined in code_804BD8E8.cpp).
int func_804BE398(Vec* v, u32 a, u32 b, u32 c, f32 f1, f32 f2);
void func_804BE4B4(void* dst, int index);
CEffectObj* func_804DFEAC(s16 handle);
void func_804CB274(EffTypeObj* self, ml::CVec3* out, s32 mode, s32 r6,
                   const ml::CVec3* in, const ml::CVec3* r8);
s32 func_804CAAD4(EffectRoot* self, void* scene, void* node);
struct EffSceneRef;
struct EffMtx;
void func_804CAC08(EffObject* obj, EffSceneRef* scene, const Mtx* m1,
                   const Mtx* m2, const EffMtx* m3, const EffMtx* m4);

// Effect-node view used by func_804CAC08: a float at +0x10.
struct EffNode10 {
    u8 pad_0x00[0x10];
    f32 field_0x10;          // 0x10
};

// Scene handle view used by func_804CAC08: an s16 index at +0x04.
struct EffSceneRef {
    u8 pad_0x00[0x04];
    s16 field_0x04;          // 0x04
};

// 12-word matrix view (word-copied into the emission packet).
struct EffMtx {
    u32 w[12];
};

// Emission packet built by func_804CAC08 and passed to func_804CC808 as a
// SrcMtx (first 0x54 bytes: three vectors + one matrix); the trailing matrix
// is copied along but not part of SrcMtx.
struct EmitData {
    ml::CVec3 va;            // 0x00
    ml::CVec3 vb;            // 0x0c
    ml::CVec3 d;             // 0x18
    EffMtx m3;               // 0x24
    EffMtx m4;               // 0x54
};
}

// Effect memory-region name string (.rodata at lbl_eu_805244E0).
extern const char lbl_eu_805244E0[];

// func_804C9D24: advance the per-effect node chain head (0x24) using the
// accumulated frame fraction at 0x14.
extern "C" void func_804C9D24(void* self) { func_804D401C((u8*)self + 0x24, *(float*)((u8*)self + 0x14)); }

// func_804C8718: effect-manager constructor. Zero the tail state block
// (0xdc..0x11c), set the per-effect defaults (float at 0x18 from sdata2,
// sentinel -1 pairs at 0x1e/0x20, count 4 at 0x48), then chain-init the node
// head at 0x24 via tail call.
void func_804C8718(EffectRoot* self) {
    f32 f = lbl_eu_8066B0A0;
    u32 zero = 0;
    s16 neg = -1;
    u32 four = 4;
    self->field_0xdc[0] = zero;
    self->field_0xdc[1] = zero;
    self->field_0xdc[2] = zero;
    self->field_0xdc[3] = zero;
    self->field_0xdc[4] = zero;
    self->field_0xdc[5] = zero;
    self->field_0xdc[6] = zero;
    self->field_0xdc[7] = zero;
    self->field_0xdc[8] = zero;
    self->field_0xdc[9] = zero;
    self->field_0xdc[10] = zero;
    self->field_0xdc[11] = zero;
    self->field_0xdc[12] = zero;
    self->field_0xdc[13] = zero;
    self->field_0xdc[14] = zero;
    self->field_0xdc[15] = zero;
    self->flags1 = zero;
    self->field_0x10 = zero;
    self->field_0x12 = zero;
    self->field_0x18 = f;
    self->field_0x1e = neg;
    self->field_0x20 = neg;
    self->field_0x23 = zero;
    self->field_0x48 = four;
    func_804D3F60((EffectInit*)self->renderArg);
}

// func_804C8790: effect-manager constructor tail. Releases the per-effect node
// chain head (0x24), zeroes the 16-word tail state block (0xdc..0x11c), then
// sets the per-effect defaults (floats at 0x0c/0x18 from sdata2, -1 sentinels
// at 0x1e/0x20, cleared 0x23 flag).
void func_804C8790(EffectRoot* self) {
    func_804D3F94((EffectNode*)self->renderArg);
    f32 f1 = lbl_eu_8066B0A4;
    u32 zero = 0;
    f32 f0 = lbl_eu_8066B0A0;
    s16 neg = -1;
    self->field_0xdc[0] = zero;
    self->field_0xdc[1] = zero;
    self->field_0xdc[2] = zero;
    self->field_0xdc[3] = zero;
    self->field_0xdc[4] = zero;
    self->field_0xdc[5] = zero;
    self->field_0xdc[6] = zero;
    self->field_0xdc[7] = zero;
    self->field_0xdc[8] = zero;
    self->field_0xdc[9] = zero;
    self->field_0xdc[10] = zero;
    self->field_0xdc[11] = zero;
    self->field_0xdc[12] = zero;
    self->field_0xdc[13] = zero;
    self->field_0xdc[14] = zero;
    self->field_0xdc[15] = zero;
    self->flags1 = zero;
    self->field_0x0c = f1;
    self->field_0x10 = zero;
    self->field_0x12 = zero;
    self->field_0x18 = f0;
    self->field_0x1e = neg;
    self->field_0x20 = neg;
    self->field_0x23 = zero;
}

// func_804CB458: bind-linked render pass. Requires both links, bits 5 and 10
// of flags1 clear and bit 11 set; skips the render when bit 7 is set on
// either the effect or its Field08Obj.
void func_804CB458(EffectRoot* self) {
    if (func_804EEACC(self->link0) == 0) return;
    if (func_804EEACC(self->link1) == 0) return;
    u16 flags = self->flags1;
    if ((flags >> 10) & 1) return;
    if ((flags >> 5) & 1) return;
    if (!((flags >> 11) & 1)) return;
    u32 hide = ((flags >> 7) & 1) || ((self->field_0x08->field_0x00 >> 7) & 1);
    if (hide != 0) return;
    func_804D42B8(self->renderArg, self->field_0x08->field_0x0c);
}

// func_804CB560: mirror of func_804CB458 - render only when the bit-7 hide
// flag is clear on both the effect and its Field08Obj.
void func_804CB560(EffectRoot* self) {
    if (func_804EEACC(self->link0) == 0) return;
    if (func_804EEACC(self->link1) == 0) return;
    u16 flags = self->flags1;
    if ((flags >> 10) & 1) return;
    if ((flags >> 5) & 1) return;
    if (!((flags >> 11) & 1)) return;
    u32 hide = ((flags >> 7) & 1) || ((self->field_0x08->field_0x00 >> 7) & 1);
    if (hide == 0) return;
    func_804D42B8(self->renderArg, self->field_0x08->field_0x0c);
}

// func_804CB5FC: resolve the draw depth of an effect object. When the 0x800
// flag is set, the signed byte at 0x23 is returned directly. Otherwise the
// world position (0x128) is transformed by the object's matrix (0x64, skipped
// when it equals the identity matrix within epsilon) and then by the bound
// view matrix, and (priority * depth scale + transformed z) is truncated to
// int. The per-element identity flags are declared in reverse so MWCC keeps
// them in the retail registers (r29..r0).
void* func_804CB5FC(EffObj* obj) {
    if ((obj->field_0x1c >> 11) & 1) {
        return (void*)(s32)obj->field_0x23;
    }
    ml::CVec3 pos(obj->pos.x, obj->pos.y, obj->pos.z);
    u32 f10 = 0;
    u32 f9 = 0;
    u32 f8 = 0;
    u32 f7 = 0;
    u32 f6 = 0;
    u32 f5 = 0;
    u32 f4 = 0;
    u32 f3 = 0;
    u32 f2 = 0;
    u32 f1 = 0;
    u32 f0 = 0;
    if (__fabs(obj->mtx.m[0][0] - ml::CMat34::identity.m[0][0]) <= lbl_eu_8066A208 &&
        __fabs(obj->mtx.m[0][1] - ml::CMat34::identity.m[0][1]) <= lbl_eu_8066A208) {
        f0 = 1;
    }
    if (f0 && __fabs(obj->mtx.m[0][2] - ml::CMat34::identity.m[0][2]) <= lbl_eu_8066A208) f1 = 1;
    if (f1 && __fabs(obj->mtx.m[0][3] - ml::CMat34::identity.m[0][3]) <= lbl_eu_8066A208) f2 = 1;
    if (f2 && __fabs(obj->mtx.m[1][0] - ml::CMat34::identity.m[1][0]) <= lbl_eu_8066A208) f3 = 1;
    if (f3 && __fabs(obj->mtx.m[1][1] - ml::CMat34::identity.m[1][1]) <= lbl_eu_8066A208) f4 = 1;
    if (f4 && __fabs(obj->mtx.m[1][2] - ml::CMat34::identity.m[1][2]) <= lbl_eu_8066A208) f5 = 1;
    if (f5 && __fabs(obj->mtx.m[1][3] - ml::CMat34::identity.m[1][3]) <= lbl_eu_8066A208) f6 = 1;
    if (f6 && __fabs(obj->mtx.m[2][0] - ml::CMat34::identity.m[2][0]) <= lbl_eu_8066A208) f7 = 1;
    if (f7 && __fabs(obj->mtx.m[2][1] - ml::CMat34::identity.m[2][1]) <= lbl_eu_8066A208) f8 = 1;
    if (f8 && __fabs(obj->mtx.m[2][2] - ml::CMat34::identity.m[2][2]) <= lbl_eu_8066A208) f9 = 1;
    if (f9 && __fabs(obj->mtx.m[2][3] - ml::CMat34::identity.m[2][3]) <= lbl_eu_8066A208) f10 = 1;
    if (f10 == 0) {
        Vec out;
        PSMTXMultVec(obj->mtx.mtx, (const Vec*)&pos, &out);
        *(Vec*)&pos = out;
    }
    SceneViewObj* view = (SceneViewObj*)func_80496264(obj->field_0x08->field_0x10, -1);
    Vec out2;
    PSMTXMultVec(view->field_0xcc, (const Vec*)&pos, &out2);
    *(Vec*)&pos = out2;
    SceneViewObj* view2 = (SceneViewObj*)func_80496264(obj->field_0x08->field_0x10, -1);
    return (void*)(s32)((f32)(s32)obj->field_0x23 * view2->field_0x1f0 + pos.z);
}

void* func_804CB9F4(void){ return (void*)0x2FDA00; }

// func_804CBA00: effect-system init. Reserves four schedule pools at the given
// arena base (each setup helper returns its input pointer for chaining), arms
// the GQR fast-cast registers, builds the sin/cos table, seeds the CE random
// generator, marks the 0x8800 flags, then creates the effect memory region.
void func_804CBA00(EffectRoot* self, u32 memAlloc, u32 size2) {
    u32 p = memAlloc;
    p = func_804DF344(p, 0x80);
    p = func_804DFA08(p, 0x200);
    p = func_804DFC48(p, 0x200);
    p = func_804DFF00(p, 0xc80);
    OSInitFastCast();
    func_804DD754();
    CERand::init();
    self->flags0 = (u16)(self->flags0 | 0x8800);
    self->allocHandle =
        mtl::MemManager::create(mtl::MemManager::getHandleMEM1(), 0x60000,
                                lbl_eu_805244E0);
}

// func_804CBAA8: when the 0x8000 flag is set, fold clz(arg) into flag bit 10,
// set bit 13, then (re)initialize the schedule lists and scene snapshot.
void func_804CBAA8(EffectRoot* self, void* scene, u32 arg) {
    u16 flags = self->flags0;
    if ((flags >> 15) & 1) {
        self->flags0 =
            (u16)(((flags | 0x2000) & ~0x400) | ((__cntlzw(arg) << 5) & 0x400));
        func_804DFE9C();
        func_804DF690();
        __ct__804E4B64(0);
        func_804DF4BC(0);
        func_804D82DC(scene);
        func_804C8684(scene);
    }
}

// When bit 12 or bit 11 of the u16 flag is set, run the shared randomizer
// execute (CERand::execute) and the anim driver with the incoming float.
extern "C" void func_804CBB14(u16* self, f32 f1) {
    u16 flag = *self;
    if (((flag >> 13) & 1) != 0 || ((flag >> 12) & 1) != 0) {
        CERand::execute(f1);
        func_804E4DD4(f1);
    }
}

// func_804CBB60: no-arg per-frame hook - restore the fog state, then advance
// the two global schedule lists (retail: two bare bl's in a 16-byte frame).
extern "C" void func_804D83D0(void);
extern "C" void func_804E4E38(void);
void func_804CBB60() {
    func_804D83D0();
    func_804E4E38();
}

// func_804CBB84: on the 0x800 flag (with 0x2000 or 0x1000 also set), rebind
// the active scene: hand it to both schedule lists, refresh the fog-enable
// byte from the current scene pointer, refresh the ortho projection (0x48)
// from the render mode, and set the 0x4000 flag. Always ends with the
// fog-state refresh.
void func_804CBB84(EffectRoot* self, void* scene) {
    if ((self->flags0 >> 11) & 1) {
        if (((self->flags0 >> 13) & 1) || ((self->flags0 >> 12) & 1)) {
            u32 a = func_804E53D8();
            u32 b = func_804DFE8C();
            if (a != 0 && b != 0) {
                func_804DF7A4();
                func_804E4E8C((u32)scene);
                lbl_eu_80663B3C = (scene == lbl_eu_806659B8);
                func_804F3B4C(&self->field_0x08, scene);
                f32 h = (f32)CDeviceVI::getRenderModeObj()->efbHeight;
                f32 w = (f32)CDeviceVI::getRenderModeObj()->fbWidth;
                func_804F3B60(&self->field_0x48, 1, w, h);
                self->flags0 = (u16)(self->flags0 | 0x4000);
            }
            func_804D854C();
        }
    }
}

// func_804CBC90: first render pass. When the 0x4000 flag is set and the
// registered-effect list exists, refresh the projection from the 0x08 matrix,
// then run func_804CB3E4 over every registered effect.
void func_804CBC90(EffectRoot* self) {
    if (!((self->flags0 >> 14) & 1)) return;
    if (lbl_eu_80665A30 == 0) return;
    func_804CBEEC();
    func_804F4620(&self->field_0x08);
    EffectListNode* c;
    EffectListNode* n;
    for (n = lbl_eu_80665A30; n != 0; n = n->next) {
        for (c = n->field_0x04; c != 0; c = c->next) {
            func_804CB3E4(c->obj);
        }
    }
}

// func_804CB3E4: bind-linked render pass. Runs only when both CLytBind links
// resolve and flags1 has bits 5, 10 and 11 all clear. Kept outlined
// (__declspec(noinline)): retail calls it via bl (different original TU), but
// -ipa file would inline this same-cpp body into func_804CBC90.
void __declspec(noinline) func_804CB3E4(EffectRoot* self) {
    if (func_804EEACC(self->link0) == 0) return;
    if (func_804EEACC(self->link1) == 0) return;
    u16 flags = self->flags1;
    if ((flags >> 10) & 1) return;
    if ((flags >> 5) & 1) return;
    if ((flags >> 11) & 1) return;
    func_804D42B8(self->renderArg, self->field_0x08->field_0x0c);
}

// func_804CBD14: render pass over the registered effect list. Mirrors
// func_804CBDB4 but skips outer nodes whose 0x10 child flag is clear and runs
// the bit-7-hide render path (func_804CB458) per inner node.
void func_804CBD14(EffectRoot* self) {
    if (!((self->flags0 >> 14) & 1)) return;
    if (lbl_eu_80665A30 == 0) return;
    func_804CBEEC();
    func_804F4D74(&self->field_0x48);
    EffectListNode* c;
    EffectListNode* n;
    for (n = lbl_eu_80665A30; n != 0; n = n->next) {
        if (n->field_0x10 == 0) continue;
        for (c = n->field_0x04; c != 0; c = c->next) {
            func_804CB458(c->obj);
        }
    }
    func_804F4620(&self->field_0x08);
}

// func_804CBDB4: second render pass (func_804CB4F4 over the registered list),
// with the projection refreshes in the opposite order (perspective at 0x48
// first, orthographic at 0x08 last).
void func_804CBDB4(EffectRoot* self) {
    if (!((self->flags0 >> 14) & 1)) return;
    if (lbl_eu_80665A30 == 0) return;
    func_804CBEEC();
    func_804F4D74(&self->field_0x48);
    EffectListNode* c;
    EffectListNode* n;
    for (n = lbl_eu_80665A30; n != 0; n = n->next) {
        for (c = n->field_0x04; c != 0; c = c->next) {
            func_804CB4F4(c->obj);
        }
    }
    func_804F4620(&self->field_0x08);
}

// func_804CB4F4: bind-linked render pass. Runs when both links resolve and
// flags1 has bit 5 set and bit 10 clear. Kept outlined (__declspec(noinline))
// like func_804CB3E4 - retail calls it via bl.
void __declspec(noinline) func_804CB4F4(EffectRoot* self) {
    if (func_804EEACC(self->link0) == 0) return;
    if (func_804EEACC(self->link1) == 0) return;
    u16 flags = self->flags1;
    if ((flags >> 10) & 1) return;
    if (!((flags >> 5) & 1)) return;
    func_804D42B8(self->renderArg, self->field_0x08->field_0x0c);
}

// func_804CBE48: render pass twin of func_804CBD14, running the other hide
// path (func_804CB560) per inner node over the same filtered list.
void func_804CBE48(EffectRoot* self) {
    if (!((self->flags0 >> 14) & 1)) return;
    if (lbl_eu_80665A30 == 0) return;
    func_804CBEEC();
    func_804F4D74(&self->field_0x48);
    EffectListNode* c;
    EffectListNode* n;
    for (n = lbl_eu_80665A30; n != 0; n = n->next) {
        if (n->field_0x10 == 0) continue;
        for (c = n->field_0x04; c != 0; c = c->next) {
            func_804CB560(c->obj);
        }
    }
    func_804F4620(&self->field_0x08);
}

extern "C" void func_804C8688();
extern "C" void func_804CBEE8(void) { func_804C8688(); }

// func_804CBEEC: set up the GX vertex formats, TEV stage 1, channel/alpha
// state and ambient color used by the effect render passes. noinline keeps
// callers emitting a real bl (retail calls it) instead of inlining.
void __declspec(noinline) func_804CBEEC() {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S8, 1);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT2, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT2, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT2, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT2, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXColor amb;
    *(u32*)&amb = lbl_eu_8066B0D0;
    GXSetChanAmbColor(GX_COLOR0A0, amb);
    GXSetCurrentMtx(GX_PNMTX0);
    func_804DF150();
}

// func_804CC104: reset the global alpha-compare state (GREATER/AND) and clear
// the 0x4000 flag on the effect singleton.
void func_804CC104(EffectRoot* self) {
    func_804DF7FC();
    self->flags0 = (u16)(self->flags0 & ~0x4000);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_GREATER, 0);
}

// func_804CC154: tear down the schedule lists and mem region when the 0x2000
// flag is set, then mark the alloc handle invalid.
void func_804CC154(EffectRoot* self) {
    if ((self->flags0 >> 13) & 1) {
        __dt__804DF744();
        func_804E4D58(0);
        func_804DF4BC(0);
        func_804C868C();
        self->flags0 = (u16)(self->flags0 & ~0x2000);
        mtl::MemManager::erase(self->allocHandle);
        self->allocHandle = 0xFFFFFFFF;
    }
}

// func_804CC1BC/804CC1D8: when the 0x8000 (bit 15) schedule flag is set,
// forward the key to the matching schedule-list lookup and tail-return its
// result; otherwise 0. The retail halfword is read once (lhz) and the flag
// extracted with rlwinm. r0,r0,17,31,31 ((flags0 >> 15) & 1).
extern "C" u32 func_804DF3D0(void* key);
u32 func_804CC1BC(EffectRoot* self, void* key) {
    if ((self->flags0 >> 15) & 1) {
        return func_804DF3D0(key);
    }
    return 0;
}

u32 func_804CC1D8(EffectRoot* self, void* key) {
    if ((self->flags0 >> 15) & 1) {
        return func_804DF4BC(key);
    }
    return 0;
}

// func_804CC1F4: add a schedule request. When either the 0x2000 or 0x1000
// flag is set, resolve an entry range via func_804DF5F8 and forward it to the
// schedule lists; returns 0 when no flag is set or the entry carries no data.
u32 func_804CC1F4(EffectRoot* self, u32 a, u32 b, u32 c, u32 d, u32 e) {
    u16 flags = self->flags0;
    if (!((flags >> 13) & 1) && !((flags >> 12) & 1)) return 0;
    u32 outC;
    CScheduleEntryData* out8;
    if (func_804DF5F8(&outC, &out8, a, c) == 0) return 0;
    u16 off = out8->mOffset;
    if (off == 0) return 0;
    u32 end = off ? (u32)out8 + off : 0;
    return func_804E4EF8((u32)out8, end, outC, d, e, b);
}

extern "C" void __ct__804E4F9C();
extern "C" void func_804CC2B0(void) { __ct__804E4F9C(); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---

// func_804C8830: effect-object constructor. Resolves the u16 offset tables
// selected by `data` against `base` into the 0xdc tail block, binds the six
// sub-regions (0x11c/0x14c/0x17c/0x194/0x1c4/0x204), builds flags1 from the
// data byte fields plus the sign of arg7, then wires the two scene links and
// the renderArg header. Returns 1 on success, 0 when the data is empty.
s32 func_804C8830(EffObject* self, const u8* base, const EffInitBlob* data,
                  void* arg6, s32 arg7, u8 arg8) {
    u16 f04 = data->field_0x04;
    if (f04 == 0) return 0;
    if (data->field_0x06 == 0) return 0;
    u32 p04 = f04 ? (u32)(base + f04) : 0;
    u32 p06 = data->field_0x06 ? (u32)(base + data->field_0x06) : 0;

    self->flags1 = 0;
    u16 f12 = data->field_0x12;
    u16 w = *(const u16*)(p06 + 0x14);
    f32 fA0 = lbl_eu_8066B0A0;
    f32 fA4 = lbl_eu_8066B0A4;
    s16 s0 = data->field_0x00;
    s16 s2 = data->field_0x02;
    u8 bD = data->field_0x0d;
    self->field_0x00 = w;
    self->field_0x04 = 2;
    self->field_0x0c = fA4;
    self->field_0x14 = fA0;
    self->field_0x10 = s0;
    self->field_0x12 = s2;
    self->field_0x18 = fA0;
    self->field_0x20 = arg7;
    self->field_0x23 = bD;
    self->field_0x22 = arg8;
    self->field_0x08 = (Field08Obj*)arg6;
    self->field_0x48 = 4;
    self->field_0x40 = 0;
    self->field_0x44 = 0;
    self->field_0xdc[0] = f12 ? (u32)(base + f12) : 0;
    self->field_0xdc[1] = data->field_0x10 ? (u32)(base + data->field_0x10) : 0;

    const EffOffList* l04 = (const EffOffList*)p04;
    const EffOffList* l06 = (const EffOffList*)p06;
    self->field_0xdc[2] = l04->field_0x00 ? (u32)(base + l04->field_0x00) : 0;
    self->field_0xdc[4] = l04->field_0x02 ? (u32)(base + l04->field_0x02) : 0;
    self->field_0xdc[5] = l04->field_0x04 ? (u32)(base + l04->field_0x04) : 0;
    self->field_0xdc[6] = l06->field_0x00 ? (u32)(base + l06->field_0x00) : 0;
    self->field_0xdc[7] = l06->field_0x02 ? (u32)(base + l06->field_0x02) : 0;
    self->field_0xdc[8] = l06->field_0x04 ? (u32)(base + l06->field_0x04) : 0;
    self->field_0xdc[9] = l06->field_0x06 ? (u32)(base + l06->field_0x06) : 0;
    self->field_0xdc[10] = l06->field_0x0a ? (u32)(base + l06->field_0x0a) : 0;
    self->field_0xdc[12] = l06->field_0x0c ? (u32)(base + l06->field_0x0c) : 0;
    self->field_0xdc[13] = l06->field_0x0e ? (u32)(base + l06->field_0x0e) : 0;
    self->field_0xdc[14] = l06->field_0x10 ? (u32)(base + l06->field_0x10) : 0;
    self->field_0xdc[15] = l06->field_0x12 ? (u32)(base + l06->field_0x12) : 0;

    u32 e4 = self->field_0xdc[2];
    if (e4 != 0) {
        u16 off = *(const u16*)((const u8*)e4 - 0xa);
        self->field_0xdc[3] = off ? (u32)(base + off) : 0;
    }
    u32 p104 = self->field_0xdc[10];
    if (p104 != 0) {
        u16 off = *(const u16*)((const u8*)p104 - 0x4);
        self->field_0xdc[11] = off ? (u32)(base + off) : 0;
    }

    func_804E0580(self->sub_0x11c, (const void*)self->field_0xdc[2]);
    func_804E0788(self->sub_0x14c, (const void*)self->field_0xdc[4]);
    func_804E0990(self->sub_0x17c, (const void*)self->field_0xdc[5]);
    func_804E1D50(self->sub_0x194, (const void*)self->field_0xdc[13]);
    func_804E214C(self->sub_0x1c4, (const void*)self->field_0xdc[12], 0);
    func_804E26D8(self->sub_0x204, (const void*)self->field_0xdc[14]);

    u32 flags = self->flags1 | 0xc000;
    flags = (flags & ~0x1000) | (((u32)(arg7 >= 0)) << 12);
    flags = (flags & ~0x300) | (((u32)(data->field_0x08 & 3)) << 8);
    flags = (flags & ~0x80) | (((u32)(data->field_0x0e & 1)) << 7);
    flags = (flags & ~0x40) | (((u32)(data->field_0x0f & 1)) << 6);
    self->flags1 = (u16)flags;
    if (self->field_0x00 == 0xc || self->field_0x00 == 0x4 || self->field_0x00 == 0x5 ||
        self->field_0x00 == 0x6 || self->field_0x00 == 0x7) {
        self->flags1 = self->flags1 | 0x20;
    } else {
        self->flags1 = (u16)(flags & ~0x20);
    }

    u32 e4b = self->field_0xdc[2];
    if (e4b != 0) {
        u8 v = e4b ? *(const u8*)((const u8*)e4b - 0x10) : 0;
        int b = (v >= 0x10 && v <= 0x14);
        self->flags1 = (u16)((self->flags1 & ~0x800) | (b << 11));
    }

    func_804EE558((void*)self->link0, (u32)arg6,
                  (u32)(self->field_0xdc[2] ? *(const u8*)((const u8*)self->field_0xdc[2] - 0x10) : 0),
                  (u32)(self->field_0xdc[2] ? self->field_0xdc[2] - 8 : 0),
                  (u32)(self->field_0x00 == 0xe));
    func_804EE558((void*)self->link1, (u32)arg6,
                  (u32)(self->field_0xdc[4] ? *(const u8*)((const u8*)self->field_0xdc[4] - 0x10) : 0),
                  (u32)(self->field_0xdc[4] ? self->field_0xdc[4] - 8 : 0),
                  (u32)(self->field_0x00 == 0xe));

    u32 p104b = self->field_0xdc[10];
    if (p104b != 0) {
        self->field_0x04 = p104b ? *(const u8*)((const u8*)p104b - 1) : 0;
    }
    if (self->field_0x04 >= 6 && self->field_0x04 <= 10) {
        self->flags1 = self->flags1 | 0x400;
    }
    if (self->field_0x00 == 1) {
        self->field_0x04 = 2;
    }
    if (self->field_0x00 == 6) {
        if (self->field_0x10 > 0) self->field_0x12 = self->field_0x10 + 1;
    }
    u32 p10c = self->field_0xdc[12];
    if (p10c != 0) {
        self->field_0x48 = func_804DD6E8(p10c ? *(const u8*)((const u8*)p10c - 0x1c) : 2);
    }
    u32 p108 = self->field_0xdc[11];
    if (p108 != 0) {
        self->field_0x40 = p108 ? *(const u8*)((const u8*)p108 - 8) : 0;
        self->field_0x44 = self->field_0xdc[11] ? *(const u8*)((const u8*)self->field_0xdc[11] - 7) : 0;
    }
    func_804D4010(self->renderArg, arg6, self);
    return 1;
}

// func_804C8D90: per-frame effect-object update. Refreshes the six bound
// sub-regions with the current lifetime value (delta), clears stale flags
// based on the field_0x08 bitmask / parent handle / lifetime, then when the
// 0x4000 flag is set runs the spawn loop: computes the remaining-duration
// count, spawns nodes via func_804D5E10, refreshes the 0x17c region and the
// per-node alpha position, advancing through func_804D5DAC.
void func_804C8D90(EffObject* obj, f32 delta) {
    if (obj->field_0x0c <= lbl_eu_8066B0A4) {
        func_804CAAD4((EffectRoot*)obj, (void*)1, 0);
    }
    obj->field_0x14 = delta;
    if (delta <= lbl_eu_8066B0A0) return;
    EffFrameTail* ft = (EffFrameTail*)&obj->sub_0x17c[1];

    if (obj->flags1 & 0x4000) {
        if ((obj->field_0x08->field_0x00 >> 6) & 1) {
            obj->flags1 = (u16)(obj->flags1 & ~0x4000);
        }
        if (!(obj->flags1 & 0x1000)) {
            if (obj->field_0x12 > 0) {
                f32 t = s32ToF_b0b0((u32)(s32)obj->field_0x12);
                if (t < obj->field_0x0c) {
                    obj->flags1 = (u16)(obj->flags1 & ~0x4000);
                }
            }
            if ((ft->field_0x180 >> 4) & 1) {
                if (ft->field_0x18a == 0) {
                    obj->flags1 = (u16)(obj->flags1 & ~0x4000);
                }
            }
        } else {
            CEffectObj* p = func_804DFEAC(obj->field_0x20);
            if (p != 0 && !(p->mFlags1C & 0x8000)) {
                obj->flags1 = (u16)(obj->flags1 & ~0x4000);
            }
        }
    }
    if (!(obj->flags1 & 0x4000)) {
        if (obj->field_0x2e <= 0) {
            obj->flags1 = (u16)(obj->flags1 & ~0x8000);
        }
    }

    if (!(obj->flags1 & 0x1000)) {
        // Refresh the six bound sub-regions with the current lifetime.
        if (obj->field_0x10 > 0 && (f32)obj->field_0x10 < obj->field_0x0c) {
            obj->sub_0x11c[0] = obj->field_0x14;
            func_804E06B4(obj->sub_0x11c, (const void*)obj->field_0xdc[2], (const void*)obj->field_0x08, obj->field_0x14);
            obj->sub_0x14c[0] = obj->field_0x14;
            func_804E08BC(obj->sub_0x14c, (const void*)obj->field_0xdc[4], (const void*)obj->field_0x08, obj->field_0x14);
            obj->sub_0x17c[0] = obj->field_0x14;
            func_804E0B94(obj->sub_0x17c, (const void*)obj->field_0xdc[5], (const void*)obj->field_0x08, obj->field_0x14);
            if (obj->field_0xdc[13] ? *(const u8*)((const u8*)obj->field_0xdc[13] - 0x1c) : 0) {
                obj->sub_0x194[0] = obj->field_0x14;
                func_804E2088(obj->sub_0x194, (const void*)obj->field_0xdc[13], (const void*)obj->field_0x08, obj->field_0x14);
            }
            if (obj->field_0xdc[12] ? *(const u8*)((const u8*)obj->field_0xdc[12] - 0x19) : 0) {
                obj->sub_0x1c4[0] = obj->field_0x14;
                func_804E24A8(obj->sub_0x1c4, (const void*)obj->field_0xdc[12], (const void*)obj->field_0x08, obj->field_0x14);
            }
            if (obj->field_0xdc[14] ? *(const u8*)((const u8*)obj->field_0xdc[14] - 0x18) : 0) {
                obj->sub_0x204[0] = obj->field_0x14;
                func_804E2A5C(obj->sub_0x204, (const void*)obj->field_0xdc[14], (const void*)obj->field_0x08, obj->field_0x14);
            }
        }
        if (obj->flags1 & 0x4000) {
            if (obj->field_0x10 > 0 && (f32)obj->field_0x10 < obj->field_0x0c) {
                if (obj->field_0x12 <= 0 || (f32)obj->field_0x12 >= obj->field_0x0c) {
                } else if (obj->field_0x18 > lbl_eu_8066B0A0) {
                    obj->field_0x18 = obj->field_0x18 - obj->field_0x14;
                    if (obj->field_0x18 > lbl_eu_8066B0A0) {
                        func_804CAAD4((EffectRoot*)obj, 0, 0);
                    }
                }
            }
        }
    }

    // Spawn loop: refresh the 0x17c region, then walk the node chain.
    func_804E0CF0(obj->sub_0x17c, (const void*)obj->field_0xdc[5]);
    s32 count = (s32)(s32ToF_b0b0((u32)ft->field_0x188) * delta);
    u32 mode = obj->field_0xdc[2] ? *(const u8*)((const u8*)obj->field_0xdc[2] - 0xe) : 0;
    u32 isB = (mode == 7);
    u32 i = 0;
    while (i < (u32)count) {
        struct SpawnArg {
            void* scene;
            EffObject* obj;
            u16 type;
            u8 b0;
        };
        SpawnArg arg;
        arg.scene = obj->field_0x08;
        arg.obj = obj;
        arg.type = (u16)obj->field_0x00;
        arg.b0 = (u8)obj->field_0x23;
        func_804D5E10((void*)((u8*)obj + 0x24), (s32)(u32)&arg);
        if (isB) {
            arg.b0 = (u8)obj->field_0x22;
            func_804D5E10((void*)((u8*)obj + 0x24), (s32)(u32)&arg);
        }
        i++;
    }
    EffectNode* n = (EffectNode*)func_804D5DAC((u8*)obj + 0x24);
    while (n != 0) {
        n = (EffectNode*)func_804D5DAC((u8*)obj + 0x24);
    }
}
// Frame-update view of the effect object used by func_804C9D30: the 0x64/0xac
// emission matrices, the 0x128 position and the 0x140/0x170 anchor columns.
struct EffFrameObj {
    u8 pad_0x00[0x0c];
    f32 field_0x0c;          // 0x0c
    u8 pad_0x10[0x1c - 0x10];
    u16 flags1;              // 0x1c
    u8 pad_0x1e[0x2e - 0x1e];
    s16 field_0x2e;          // 0x2e lifetime
    u8 pad_0x30[0x48 - 0x30];
    u32 field_0x48;          // 0x48
    u8 pad_0x4c[0x64 - 0x4c];
    ml::CMat34 mtx;          // 0x64
    u8 pad_0x94[0xac - 0x94];
    ml::CMat34 mtx2;         // 0xac
    u32 field_0xdc[16];      // 0xdc
    u8 pad_0x11c[0x128 - 0x11c];
    ml::CVec3 pos;           // 0x128
    f32 field_0x134[9];      // 0x134..0x158
    f32 field_0x158[8];      // 0x158..0x178
    f32 field_0x170[8];      // 0x170..0x190
    u8 pad_0x190[0x204 - 0x190];
    f32 field_0x204[8];      // 0x204
};

// func_804C9D30: per-frame effect-object update. When the lifetime (0x2e) is
// positive and the 0x1000 flag is clear, rebuilds the two emission transforms
// (matrix + negated anchor columns via func_804DB980, position added, then
// PSMTXConcat), then walks the per-object node chain (func_804D5DAC)
// invoking func_804CAC08 per node and folding the results back into the node.
void func_804C9D30(EffFrameObj* obj) {
    if (obj->field_0x2e <= 0) return;
    s32 loopFlag = 0;
    if (!(obj->flags1 & 0x1000)) {
        if (func_804CAAD4((EffectRoot*)obj, (void*)(u32)(obj->field_0x0c <= lbl_eu_8066B0A4), 0) != 0) {
            // emission transform 1: negated anchor 0x140 -> func_804DB980 ->
            // position 0x128 added -> concat with the 0x64 matrix.
            Mtx m0;
            Vec neg;
            neg.x = -obj->field_0x134[3];
            neg.y = -obj->field_0x134[4];
            neg.z = -obj->field_0x134[5];
            func_804DB980(&m0, &neg, obj->field_0x48);
            Mtx m1;
            for (u32 i = 0; i < 12; i++) ((u32*)&m1)[i] = ((const u32*)&m0)[i];
            m1[0][3] += obj->pos.x;
            m1[1][3] += obj->pos.y;
            m1[2][3] += obj->pos.z;
            Mtx m2;
            PSMTXConcat(obj->mtx.mtx, m1, m2);

            // emission transform 2: same for the 0x170 anchor and 0xac matrix.
            Mtx n0;
            neg.x = -obj->field_0x170[0];
            neg.y = -obj->field_0x170[1];
            neg.z = -obj->field_0x170[2];
            func_804DB980(&n0, &neg, obj->field_0x48);
            Mtx n1;
            for (u32 i = 0; i < 12; i++) ((u32*)&n1)[i] = ((const u32*)&n0)[i];
            n1[0][3] += obj->field_0x158[0];
            n1[1][3] += obj->field_0x158[1];
            n1[2][3] += obj->field_0x158[2];
            Mtx n2;
            PSMTXConcat(obj->mtx2.mtx, n1, n2);
            loopFlag = 1;
        }
    }
    EffectNode* node = (EffectNode*)func_804D5DAC((u8*)obj + 0x24);
    while (node != 0) {
        if (loopFlag != 0) {
            if (!(node->field_0x06 & 0x800)) {
                Mtx m1;
                Mtx m2;
                Mtx m3;
                Mtx m4;
                func_804CAC08((EffObject*)obj, (EffSceneRef*)node, &m1, &m2,
                              (const EffMtx*)&m3, (const EffMtx*)&m4);
            }
        }
        func_804CCF84(node);
        node = (EffectNode*)func_804D5DAC((u8*)obj + 0x24);
    }
}
extern "C" void func_804CAA94(EffObject* self) {
    func_804D4144((EffectNode*)self->renderArg);
    self->field_0x0c += self->field_0x14;
}
// func_804CAC08: effect-object emission update. When the scene node index is
// valid, refreshes the six bound sub-regions with (nodeTime - 1.0), then
// emits two emission vectors (mode/scale from the 0xdc tail block), builds a
// direction vector (either the difference of the two matrix translations or
// of the emission vectors), normalizes it, and hands the packet to
// func_804CC808.
void func_804CAC08(EffObject* obj, EffSceneRef* scene, const Mtx* m1,
                   const Mtx* m2, const EffMtx* m3, const EffMtx* m4) {
    f32 f31;
    if (scene->field_0x04 >= 0) {
        EffectNode* node = func_804E0114(scene->field_0x04);
        if (node == 0) return;
        f31 = ((const EffNode10*)node)->field_0x10;
        obj->sub_0x11c[0] = f31 - lbl_eu_8066B0A4;
        func_804E06B4(obj->sub_0x11c, (const void*)obj->field_0xdc[2], (const void*)obj->field_0x08, lbl_eu_8066B0A0);
        obj->sub_0x14c[0] = f31 - lbl_eu_8066B0A4;
        func_804E08BC(obj->sub_0x14c, (const void*)obj->field_0xdc[4], (const void*)obj->field_0x08, lbl_eu_8066B0A0);
        obj->sub_0x17c[0] = f31 - lbl_eu_8066B0A4;
        func_804E0B94(obj->sub_0x17c, (const void*)obj->field_0xdc[5], (const void*)obj->field_0x08, lbl_eu_8066B0A0);
        if ((obj->field_0xdc[13] ? *(const u8*)((const u8*)obj->field_0xdc[13] - 0x1c) : 0) != 0) {
            obj->sub_0x194[0] = f31 - lbl_eu_8066B0A4;
            func_804E2088(obj->sub_0x194, (const void*)obj->field_0xdc[13], (const void*)obj->field_0x08, lbl_eu_8066B0A0);
        }
        if ((obj->field_0xdc[12] ? *(const u8*)((const u8*)obj->field_0xdc[12] - 0x19) : 0) != 0) {
            obj->sub_0x1c4[0] = f31 - lbl_eu_8066B0A4;
            func_804E24A8(obj->sub_0x1c4, (const void*)obj->field_0xdc[12], (const void*)obj->field_0x08, lbl_eu_8066B0A0);
        }
        if ((obj->field_0xdc[14] ? *(const u8*)((const u8*)obj->field_0xdc[14] - 0x18) : 0) != 0) {
            obj->sub_0x204[0] = f31 - lbl_eu_8066B0A4;
            func_804E2A5C(obj->sub_0x204, (const void*)obj->field_0xdc[14], (const void*)obj->field_0x08, lbl_eu_8066B0A0);
        }
    }

    EmitData data;
    u32 isE = (obj->field_0x00 == 0xe);
    u32 e4 = obj->field_0xdc[2];
    u32 mode1 = e4 ? *(const u8*)((const u8*)e4 - 0xe) : 0;
    u32 scale1 = e4 ? *(const u16*)((const u8*)e4 - 0xc) : 100;
    ml::CVec3 out1;
    func_804CB274((EffTypeObj*)obj, &out1, mode1, scale1, &obj->field_0x134,
                  (const ml::CVec3*)((const u8*)obj->field_0x08 + 0x34));
    Vec tmp1;
    PSMTXMultVec(&(*m1)[0], (const Vec*)&out1, &tmp1);
    data.va = ml::CVec3(tmp1.x, tmp1.y, tmp1.z);
    if (isE) {
        if (mode1 != 0) {
            f31 = obj->field_0x134.y;
        } else {
            f31 = lbl_eu_8066B0C0 * out1.y;
        }
        if (func_804BE398((Vec*)&data.va, 0x4801, 0, 0, -f31, lbl_eu_8066B0A0) != 0) {
            func_804BE4B4(&data.va, 0);
        } else {
            data.va.y -= f31;
        }
    }
    u32 ec = obj->field_0xdc[4];
    u32 mode2 = ec ? *(const u8*)((const u8*)ec - 0xe) : 0;
    u32 scale2 = ec ? *(const u16*)((const u8*)ec - 0xc) : 100;
    ml::CVec3 out2;
    func_804CB274((EffTypeObj*)obj, &out2, mode2, scale2, &obj->field_0x164,
                  (const ml::CVec3*)((const u8*)obj->field_0x08 + 0x34));
    Vec tmp2;
    PSMTXMultVec(&(*m2)[0], (const Vec*)&out2, &tmp2);
    data.vb = ml::CVec3(tmp2.x, tmp2.y, tmp2.z);
    u32 emode = obj->field_0xdc[4] ? *(const u8*)((const u8*)obj->field_0xdc[4] - 0xe) : 0;
    if (emode == 1) {
        ml::CVec3 a((*m1)[0][3], (*m1)[1][3], (*m1)[2][3]);
        ml::CVec3 b((*m2)[0][3], (*m2)[1][3], (*m2)[2][3]);
        data.d = b - a;
    } else {
        data.d = data.vb - data.va;
    }
    u32 bxy = (__fabs(data.d.x) <= lbl_eu_8066A208) && (__fabs(data.d.y) <= lbl_eu_8066A208);
    if (bxy && (__fabs(data.d.z) <= lbl_eu_8066A208)) {
    } else if (data.d.x * data.d.x + data.d.y * data.d.y + data.d.z * data.d.z == lbl_eu_8066B0A0) {
        data.d = ml::CVec3::zero;
    } else {
        PSVECNormalize((const Vec*)&data.d, (Vec*)&data.d);
    }
    data.m3 = *m3;
    data.m4 = *m4;
    func_804CC808(scene, &data);
}

// func_804CAAD4: update both scene-bind links (0x4c and 0x94) for a scene
// change. The per-link state is a flag byte 0xd before the bound object
// pointer (word 2/4 of the 0xdc tail block): forced to 3 when the scene arg
// is non-null, bit 1 selects func_804EE658, bit 0 selects func_804EE8FC, and
// a non-positive field_0x14 clears the second link's state. Returns 1 when
// both links resolve.
s32 func_804CAAD4(EffectRoot* self, void* scene, void* node) {
    s32 state;
    u32 bound = self->field_0xdc[2];   // 0xe4
    if (bound != 0) {
        state = *(u8*)((u8*)bound - 0xd);
    } else {
        state = 0;
    }
    if (scene != 0) state = 3;
    func_804EE60C(&self->link0);
    if (state & 2) func_804EE658(&self->link0, node);
    if (state & 1) func_804EE8FC(&self->link0, node);

    bound = self->field_0xdc[4];       // 0xec
    if (bound != 0) {
        state = *(u8*)((u8*)bound - 0xd);
    } else {
        state = 0;
    }
    if (self->field_0x14 <= lbl_eu_8066B0A0) state = 0;
    if (scene != 0) state = 3;
    func_804EE60C(&self->link1);
    if (state & 2) func_804EE658(&self->link1, node);
    if (state & 1) func_804EE8FC(&self->link1, node);

    s32 result = 0;
    if (func_804EEACC(&self->link0) != 0 && func_804EEACC(&self->link1) != 0) {
        result = 1;
    }
    return result;
}

// func_804CB274: fill `out` with a random emission vector selected by `mode`
// (7/1 = zero, 0 = direction, 2 = box, 4 = arc, 3 = cylinder). The radius
// (1.0 - r6/maxScale) is computed up front and stays in f1 as the 4th
// argument of the 4-arg callees (func_804D9F00 ignores it). When the object
// type is 0xe the y component is post-processed.
void func_804CB274(EffTypeObj* self, ml::CVec3* out, s32 mode, s32 r6,
                   const ml::CVec3* in, const ml::CVec3* r8) {
    f32 radius = lbl_eu_8066B0A4 - (f32)r6 / lbl_eu_8066B0A8;
    switch (mode) {
    case 7:
    case 1:
        *out = ml::CVec3::zero;
        break;
    case 0:
        func_804D9364(out, in, r8, radius);
        if (self->field_0x00 == 0xe) out->y = __fabs(out->y);
        break;
    case 2:
        func_804D9A10(out, in, r8, radius);
        if (self->field_0x00 == 0xe) out->y = __fabs(in->y) * lbl_eu_8066B0C4;
        break;
    case 4:
        func_804D9F00(out, in, r8);
        break;
    case 3:
        func_804D9B70(out, in, r8, radius);
        if (self->field_0x00 == 0xe) out->y = __fabs(in->y) * lbl_eu_8066B0C4;
        break;
    }
}
