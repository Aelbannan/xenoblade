// Auto-scaffolded catalog TU for monolib/src/effect/code_804C8718
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include <revolution/GX.h>
#include <revolution/os/OSFastCast.h>
#include "monolib/util/MemManager.hpp"
#include "monolib/core/CScheduleItem.hpp"
#include "monolib/effect/CERand.hpp"
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (lbl_eu_8066A208)
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
    u16 field_0x00;         // 0x00 flags
    u8 pad_0x02[0x10 - 0x02];
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
    u8 pad_0xfc[0x10c - 0xfc];
    f32 field_0x10c[2];     // 0x10c projection-center xy
    f32 field_0x114[2];     // 0x114 projection z reference
    u8 pad_0x11c[0x1f0 - 0x11c];
    f32 field_0x1f0;        // 0x1f0 depth scale
};

// Object with a type id at +0x00 (compared to 0xe by func_804CB274).
struct EffTypeObj {
    s32 field_0x00;         // 0x00
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
void func_804D9364(ml::CVec3* out, const ml::CVec3* a, const ml::CVec3* b, double radius);
void func_804D9A10(ml::CVec3* out, const ml::CVec3* a, const ml::CVec3* b, double radius);
void func_804D9B70(ml::CVec3* out, const ml::CVec3* a, const ml::CVec3* b, double radius);
void func_804D9F00(ml::CVec3* out, const ml::CVec3* a, const ml::CVec3* b);
}

struct CScnRootEnv;
extern CScnRootEnv* lbl_eu_806659B8;
extern u8 lbl_eu_80663B3C;
extern f32 lbl_eu_8066B0A8;
extern f32 lbl_eu_8066B0C0;
extern f32 lbl_eu_8066B0C4;
extern u32 lbl_eu_8066B0D0;
extern const f64 lbl_eu_8066B0C8;   // 0x4330000000000000 (u->f magic)
extern const f64 lbl_eu_8066B0B0;   // 0x4330000000000000 (s16->f magic)
extern const f64 lbl_eu_8066B0B8;   // 0x4330000000000000 (u16 range-table base)

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
// from the 0xdc tail block; the func_804E06B4-family take the float THIRD
// and the scene/desktop pointer last (retail schedules the f1 argument load
// between the r4 and r5 loads).
void func_804E0580(void* dst, const void* src);
void func_804E0788(void* dst, const void* src);
void func_804E0990(void* dst, const void* src);
void func_804E1D50(void* dst, const void* src);
void func_804E214C(void* dst, const void* src, u32 a3);
void func_804E26D8(void* dst, const void* src);
void func_804E06B4(void* dst, const void* src, f32 f, const void* a3);
void func_804E08BC(void* dst, const void* src, f32 f, const void* a3);
void func_804E0B94(void* dst, const void* src, f32 f, const void* a3);
void func_804E2088(void* dst, const void* src, f32 f, const void* a3);
void func_804E24A8(void* dst, const void* src, f32 f, const void* a3);
void func_804E2A5C(void* dst, const void* src, f32 f, const void* a3);
void func_804E0CF0(void* dst, const void* src);
int func_804DD6E8(int val);
void func_804D4010(void* self, const void* a, void* b);

// Scene-list iteration helpers (defined in code_804CC2B8.cpp).
void func_804CCF84(void* self);
void* func_804D5DAC(void* self);
EffectNode* func_804D5D48(EffectNode* self);
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

// Effect memory-region name string (.rodata at lbl_eu_805244E0, 0x20B
// incl. zero padding; retail shape).
extern "C" __declspec(align(8)) const char lbl_eu_805244E0[0x20] = {
    0x43,0x45,0x66,0x66,0x65,0x63,0x74,0x48,0x65,0x61,0x70,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

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
    // Load order/register naming: A0's temp must be born first (f0), A4's
    // second (f1); MWCC then schedules both sdata2 loads right after the call.
    // Load order/register naming: A0's temp must be born first (f0), A4's
    // second (f1); MWCC then schedules both sdata2 loads right after the call.
    f32 f0 = lbl_eu_8066B0A0;
    f32 f1 = lbl_eu_8066B0A4;
    u32 zero = 0;
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

s32 func_804CB5FC(EffObj* obj) {
    // Direct-priority path: return the signed priority byte unchanged.
    if ((obj->field_0x1c >> 11) & 1) {
        return obj->field_0x23;
    }

    Vec pos;
    pos.x = obj->pos.x;
    pos.y = obj->pos.y;
    pos.z = obj->pos.z;

    // Chained identity-matrix test over all 12 elements: flag k is only
    // computed when flag k-1 passed; all eleven flags stay live in registers.
    // Declaration order controls MWCC's flag->register assignment (alloc is
    // in declaration order over [r0, r29, r30, r12..r5]); c11 leads so the
    // chain lands in the retail registers.
    s32 c11 = 0;
    s32 c1 = 0;
    s32 c2 = 0;
    s32 c3 = 0;
    s32 c4 = 0;
    s32 c5 = 0;
    s32 c6 = 0;
    s32 c7 = 0;
    s32 c8 = 0;
    s32 c9 = 0;
    s32 c10 = 0;

    c1 = lbl_eu_8066A208 >= __fabs(obj->mtx.m[0][0] - ml::CMat34::identity.m[0][0]) &&
         lbl_eu_8066A208 >= __fabs(obj->mtx.m[0][1] - ml::CMat34::identity.m[0][1]);
    if (c1 && lbl_eu_8066A208 >= __fabs(obj->mtx.m[0][2] - ml::CMat34::identity.m[0][2])) c2 = 1;
    if (c2 && lbl_eu_8066A208 >= __fabs(obj->mtx.m[0][3] - ml::CMat34::identity.m[0][3])) c3 = 1;
    if (c3 && lbl_eu_8066A208 >= __fabs(obj->mtx.m[1][0] - ml::CMat34::identity.m[1][0])) c4 = 1;
    if (c4 && lbl_eu_8066A208 >= __fabs(obj->mtx.m[1][1] - ml::CMat34::identity.m[1][1])) c5 = 1;
    if (c5 && lbl_eu_8066A208 >= __fabs(obj->mtx.m[1][2] - ml::CMat34::identity.m[1][2])) c6 = 1;
    if (c6 && lbl_eu_8066A208 >= __fabs(obj->mtx.m[1][3] - ml::CMat34::identity.m[1][3])) c7 = 1;
    if (c7 && lbl_eu_8066A208 >= __fabs(obj->mtx.m[2][0] - ml::CMat34::identity.m[2][0])) c8 = 1;
    if (c8 && lbl_eu_8066A208 >= __fabs(obj->mtx.m[2][1] - ml::CMat34::identity.m[2][1])) c9 = 1;
    if (c9 && lbl_eu_8066A208 >= __fabs(obj->mtx.m[2][2] - ml::CMat34::identity.m[2][2])) c10 = 1;
    if (c10 && lbl_eu_8066A208 >= __fabs(obj->mtx.m[2][3] - ml::CMat34::identity.m[2][3])) c11 = 1;

    if (c11 == 0) {
        // Non-identity rotation: transform the world position by the object matrix.
        ml::CVec3 out;
        PSMTXMultVec(obj->mtx.mtx, (Vec*)&pos, (Vec*)&out);
        pos = *(Vec*)&out;
    }

    // Project through the bound scene's view matrix.
    SceneViewObj* view = (SceneViewObj*)func_80496264(obj->field_0x08->field_0x10, -1);
    ml::CVec3 out2;
    PSMTXMultVec(view->field_0xcc, (Vec*)&pos, (Vec*)&out2);
    pos = *(Vec*)&out2;

    SceneViewObj* view2 = (SceneViewObj*)func_80496264(obj->field_0x08->field_0x10, -1);
    // Depth = projected z + priority byte scaled by the scene depth scale.
    f32 depth = s32ToF_b0b0((u32)(s32)obj->field_0x23) * view2->field_0x1f0 + pos.z;
    return (s32)depth;
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
        // clz folded straight into flag bit 10 via a single rlwimi
        // (retail shape: ori 0x2000; rlwimi r0, clz, 5, 26, 26; sth).
        u32 v = self->flags0;
        self->flags0 =
            (u16)__rlwimi(v | 0x2000, __cntlzw(arg), 5, 21, 21);
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
                f32 h = u16ToF_b0c8(CDeviceVI::getRenderModeObj()->efbHeight);
                f32 w = u16ToF_b0c8(CDeviceVI::getRenderModeObj()->fbWidth);
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
namespace {
// End offset resolver. Retail computes this through an inlined helper, so the
// mOffset==0 select branch survives next to the caller's own guard.
inline u32 EntryEnd(const CScheduleEntryData* e, u16 off) {
    return off != 0 ? (u32)e + off : 0;
}
} // namespace

u32 func_804CC1F4(EffectRoot* self, u32 a, u32 b, u32 c, u32 d, u32 e) {
    u16 flags = self->flags0;
    if (!((flags >> 13) & 1)) {
        if (!((flags >> 12) & 1)) return 0;
    }
    u32 outC;
    void* pv;
    if (func_804DF5F8(&outC, (CScheduleEntryData**)&pv, a, c) == 0) return 0;
    // Typed alias lives in a register for guard/select; the raw void* stays
    // memory-resident and is re-loaded for the call argument (retail shape).
    CScheduleEntryData* entry = (CScheduleEntryData*)pv;
    if (entry->mOffset == 0) return 0;
    return func_804E4EF8((u32)pv, EntryEnd(entry, entry->mOffset), outC, d,
                         e, b);
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
    if (data->field_0x04 == 0) return 0;
    // volatile read: retail re-loads field_0x06 here instead of reusing the
    // guard's register value; the volatile blocks the CSE merge.
    if (*(const volatile u16*)&data->field_0x06 == 0) return 0;
    u32 p04 = data->field_0x04 ? (u32)(base + data->field_0x04) : 0;
    u32 p06 = data->field_0x06 ? (u32)(base + data->field_0x06) : 0;

    u32 zero = 0;
    self->flags1 = (u16)zero;
    u16 f12 = data->field_0x12;
    u32 two = 2;
    u16 w = *(const u16*)(p06 + 0x14);
    u32 four = 4;
    f32 fA0 = lbl_eu_8066B0A0;
    f32 fA4 = lbl_eu_8066B0A4;
    s16 s0 = data->field_0x00;
    s16 s2 = data->field_0x02;
    u8 bD = data->field_0x0d;

    self->field_0x00 = w;
    self->field_0x04 = (u32)two;
    self->field_0x0c = fA4;
    self->field_0x14 = fA0;
    self->field_0x10 = s0;
    self->field_0x12 = s2;
    self->field_0x18 = fA0;
    self->field_0x20 = arg7;
    self->field_0x23 = bD;
    self->field_0x22 = arg8;
    self->field_0x08 = (Field08Obj*)arg6;
    self->field_0x48 = four;
    self->field_0x40 = zero;
    self->field_0x44 = zero;
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

    // Pack flags1: 0xC000 base plus sign/type/select bits from the blob via
    // rlwimi inserts (operand order mirrors retail).
    u32 sign = (u32)(arg7 >= 0);
    u32 fl = __rlwimi(self->flags1 | 0xc000, sign, 12, 19, 19);
    fl = __rlwimi(fl, data->field_0x08 & 3, 8, 22, 23);
    fl = __rlwimi(fl, data->field_0x0e & 1, 7, 24, 24);
    u32 d0f = data->field_0x0f & 1;
    fl = __rlwimi(fl, d0f, 6, 25, 25);
    self->flags1 = (u16)fl;
    u32 type = self->field_0x00;
    if (type == 0xc || type == 4 || type == 5 || type == 6 || type == 7) {
        self->flags1 = self->flags1 | 0x20;
    } else {
        self->flags1 = (u16)(fl & ~0x20);
    }

    u32 e4b = self->field_0xdc[2];
    if (e4b != 0) {
        u32 v = e4b ? *(const u8*)((const u8*)e4b - 0x10) : 0;
        s32 b = (s32)v >= 0x10 && (s32)v <= 0x14;
        self->flags1 = __rlwimi(self->flags1, b, 11, 20, 20);
    }

    if (self->field_0x00 == 8) {
        self->flags1 = self->flags1 | 0x300;
    }

    func_804EE558(self->link0, (u32)arg6,
                  self->field_0xdc[2] ? *(const u8*)((const u8*)self->field_0xdc[2] - 0x10) : 0,
                  self->field_0xdc[2] ? self->field_0xdc[2] - 8 : 0,
                  (u32)(self->field_0x00 == 0xe));
    func_804EE558(self->link1, (u32)arg6,
                  self->field_0xdc[4] ? *(const u8*)((const u8*)self->field_0xdc[4] - 0x10) : 0,
                  self->field_0xdc[4] ? self->field_0xdc[4] - 8 : 0,
                  (u32)(self->field_0x00 == 0xe));

    u32 p104b = self->field_0xdc[10];
    if (p104b != 0) {
        self->field_0x04 = *(const u8*)((const u8*)p104b - 1);
    }
    if ((s32)self->field_0x04 >= 6 && (s32)self->field_0x04 <= 10) {
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
        u32 p108b = self->field_0xdc[11];
        if (p108b != 0) self->field_0x44 = *(const u8*)((const u8*)p108b - 7);
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
// u16 -> float through the lbl_eu_8066B0B8 magic-base double (same shape as
// u16ToF_b0c8).
inline f32 u16ToF_b0b8(u16 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[1] = v;
    c.w[0] = 0x43300000u;
    return (f32)(c.d - lbl_eu_8066B0B8);
}

static inline double s32ToF64_b0b0(s32 v);
// Node-chain view used by func_804C8D90: type at 0x00, flags at 0x06 and a
// float time at 0x10; per-node timer slots are addressed as node + (0x22<<2).
struct EffNodeUpd {
    s16 field_0x00;
    u8 pad_0x02[0x06 - 0x02];
    u16 field_0x06;
    u8 pad_0x08[0x10 - 0x08];
    f32 field_0x10;
};

// Depth-range table bound at field_0xdc[0]: three u16 entries driving the
// spawn-rate interpolation.
struct DepthTable {
    u16 f0;
    u16 f2;
    u16 f4;
};

// Spawn argument built on the stack for func_804D5E10.
struct SpawnArg {
    void* scene;
    EffObject* obj;
    u16 type;
    u8 b0;
};

// Identity-rotation test shared by both update paths: chained element-wise
// comparison of the object matrix against ml::CMat34::identity. Declaration
// order controls MWCC's flag->register assignment (same shape as
// func_804CB5FC). Kept inline: retail duplicates this block textually at both
// sites.
static inline s32 EffMtxNearIdentity(EffObj* eo) {
    s32 c11 = 0;
    s32 c1 = 0;
    s32 c2 = 0;
    s32 c3 = 0;
    s32 c4 = 0;
    s32 c5 = 0;
    s32 c6 = 0;
    s32 c7 = 0;
    s32 c8 = 0;
    s32 c9 = 0;
    s32 c10 = 0;

    c1 = lbl_eu_8066A208 >= __fabs(eo->mtx.m[0][0] - ml::CMat34::identity.m[0][0]) &&
         lbl_eu_8066A208 >= __fabs(eo->mtx.m[0][1] - ml::CMat34::identity.m[0][1]);
    if (c1 && lbl_eu_8066A208 >= __fabs(eo->mtx.m[0][2] - ml::CMat34::identity.m[0][2])) c2 = 1;
    if (c2 && lbl_eu_8066A208 >= __fabs(eo->mtx.m[0][3] - ml::CMat34::identity.m[0][3])) c3 = 1;
    if (c3 && lbl_eu_8066A208 >= __fabs(eo->mtx.m[1][0] - ml::CMat34::identity.m[1][0])) c4 = 1;
    if (c4 && lbl_eu_8066A208 >= __fabs(eo->mtx.m[1][1] - ml::CMat34::identity.m[1][1])) c5 = 1;
    if (c5 && lbl_eu_8066A208 >= __fabs(eo->mtx.m[1][2] - ml::CMat34::identity.m[1][2])) c6 = 1;
    if (c6 && lbl_eu_8066A208 >= __fabs(eo->mtx.m[1][3] - ml::CMat34::identity.m[1][3])) c7 = 1;
    if (c7 && lbl_eu_8066A208 >= __fabs(eo->mtx.m[2][0] - ml::CMat34::identity.m[2][0])) c8 = 1;
    if (c8 && lbl_eu_8066A208 >= __fabs(eo->mtx.m[2][1] - ml::CMat34::identity.m[2][1])) c9 = 1;
    if (c9 && lbl_eu_8066A208 >= __fabs(eo->mtx.m[2][2] - ml::CMat34::identity.m[2][2])) c10 = 1;
    if (c10 && lbl_eu_8066A208 >= __fabs(eo->mtx.m[2][3] - ml::CMat34::identity.m[2][3])) c11 = 1;
    return c11;
}

// Depth interpolation against the bound scene's projection center: ratio from
// entry f0, clamped/lerped between entries f2/f4 by the projected distance.
// Inline: retail duplicates it at both spawn paths.
static inline f32 EffDepthFactor(EffObject* obj, Vec* pos) {
    f32 factor = lbl_eu_8066B0A4;
    if (obj->field_0xdc[0] == 0) {
        return factor;
    }
    SceneViewObj* view = (SceneViewObj*)func_80496264(((EffObj*)obj)->field_0x08->field_0x10, -1);
    ml::CVec3 diff;
    diff.x = pos->x - view->field_0x10c[0];
    diff.y = pos->y - view->field_0x10c[1];
    diff.z = pos->z - view->field_0x114[0];
    f32 mag = PSVECMag((Vec*)&diff);
    DepthTable* dd = (DepthTable*)obj->field_0xdc[0];
    f32 v0 = u16ToF_b0b8(dd->f0);
    f32 ratio = v0 / lbl_eu_8066B0A8;
    f32 v2 = u16ToF_b0b8(dd->f4);
    if (v2 <= mag) {
        factor = ratio;
    } else {
        f32 v1 = u16ToF_b0b8(dd->f2);
        if (v1 < mag) {
            factor = (1.0f - ratio) * (1.0f - (mag - v1) / (v2 - v1)) + ratio;
        }
    }
    return factor;
}

void func_804C8D90(EffObject* obj, f32 delta) {
    EffFrameTail* ft = (EffFrameTail*)&obj->sub_0x17c[1];
    EffObj* eo = (EffObj*)obj;

    if (obj->field_0x0c <= lbl_eu_8066B0A4) {
        func_804CAAD4((EffectRoot*)obj, (void*)1, 0);
    }
    obj->field_0x14 = delta;
    if ((f32)delta <= lbl_eu_8066B0A0) {
        return;
    }

    if ((obj->flags1 >> 14) & 1) {
        // Stale-flag sweep for the 0x4000 bit.
        if ((eo->field_0x08->field_0x00 >> 6) & 1) {
            obj->flags1 = (u16)(obj->flags1 & ~0x4000);
        }
        if (!((obj->flags1 >> 12) & 1)) {
            if (obj->field_0x12 > 0) {
                // Lifetime gate: clear once field_0x12 passes into the object lifetime.
                double t = s32ToF64_b0b0(obj->field_0x12);
                if (lbl_eu_8066B0A0 < t && t <= (double)obj->field_0x0c) {
                    obj->flags1 = (u16)(obj->flags1 & ~0x4000);
                }
            }
            if ((ft->field_0x180 & 0x08) != 0) {
                if (ft->field_0x18a == 0) {
                    obj->flags1 = (u16)(obj->flags1 & ~0x4000);
                }
            }
        } else {
            CEffectObj* parent = func_804DFEAC(obj->field_0x20);
            if (parent != 0 && !(parent->mFlags1C & 0x8000)) {
                obj->flags1 = (u16)(obj->flags1 & ~0x4000);
            }
        }
    }
    if (!((obj->flags1 >> 14) & 1)) {
        if (obj->field_0x2e <= 0) {
            obj->flags1 = (u16)(obj->flags1 & ~0x8000);
        }
    }

    if (!((obj->flags1 >> 12) & 1)) {
        // ---- main path (bit 0x1000 clear): refresh regions, then self-spawn ----
        s32 ok;
        if (obj->field_0x10 <= 0) {
            ok = 1;
        } else if (obj->field_0x0c - lbl_eu_8066B0A4 >= s32ToF64_b0b0(obj->field_0x10)) {
            ok = 1;
        } else {
            ok = 0;
        }
        if (ok) {
            func_804E06B4(obj->sub_0x11c, (const void*)obj->field_0xdc[2], obj->field_0x14,
                          (const void*)obj->field_0x08);
            func_804E08BC(obj->sub_0x14c, (const void*)obj->field_0xdc[4], obj->field_0x14,
                          (const void*)obj->field_0x08);
            func_804E0B94(obj->sub_0x17c, (const void*)obj->field_0xdc[5], obj->field_0x14,
                          (const void*)obj->field_0x08);
            if (obj->field_0xdc[13] ? *(const u8*)((const u8*)obj->field_0xdc[13] - 0x1c) : 0) {
                func_804E2088(obj->sub_0x194, (const void*)obj->field_0xdc[13], obj->field_0x14,
                              (const void*)obj->field_0x08);
            }
            if (obj->field_0xdc[12] ? *(const u8*)((const u8*)obj->field_0xdc[12] - 0x19) : 0) {
                func_804E24A8(obj->sub_0x1c4, (const void*)obj->field_0xdc[12], obj->field_0x14,
                              (const void*)obj->field_0x08);
            }
            if (obj->field_0xdc[14] ? *(const u8*)((const u8*)obj->field_0xdc[14] - 0x18) : 0) {
                func_804E2A5C(obj->sub_0x204, (const void*)obj->field_0xdc[14], obj->field_0x14,
                              (const void*)obj->field_0x08);
            }
        }

        if (!((obj->flags1 >> 14) & 1)) {
            goto end;
        }
        if (obj->field_0x10 > 0 && s32ToF64_b0b0(obj->field_0x10) > obj->field_0x0c) {
            goto end;
        }
        if (obj->field_0x12 > 0 && s32ToF64_b0b0(obj->field_0x12) <= obj->field_0x0c) {
            goto end;
        }
        {
            // Burn-down of field_0x18; only spawn once it hits zero.
            f32 old = obj->field_0x18;
            if (!(old <= lbl_eu_8066B0A0)) {
                old = old - obj->field_0x14;
                obj->field_0x18 = old;
                if (!(old <= lbl_eu_8066B0A0)) {
                    goto end;
                }
            }
        }
        if (func_804CAAD4((EffectRoot*)obj, 0, 0) == 0) {
            goto end;
        }

        Vec pos;
        pos.x = eo->pos.x;
        pos.y = eo->pos.y;
        pos.z = eo->pos.z;
        if (!EffMtxNearIdentity(eo)) {
            ml::CVec3 out;
            PSMTXMultVec(ml::CMat34::identity.mtx, (Vec*)&pos, (Vec*)&out);
            pos = *(Vec*)&out;
        }

        f32 factor = EffDepthFactor(obj, &pos);

        s32 count = (s32)(u16ToF_b0b8(ft->field_0x188) * delta);
        u8 mode = obj->field_0xdc[2] ? *(const u8*)((const u8*)obj->field_0xdc[2] - 0xe) : 0;
        int isB = (mode == 7);
        for (u16 i = 0; i < (u16)count; i++) {
            SpawnArg arg;
            arg.scene = obj->field_0x08;
            arg.obj = obj;
            arg.type = (u16)-1;
            arg.b0 = 0;
            func_804D5E10((void*)obj->renderArg, (s32)(u32)&arg);
            if (isB) {
                arg.b0 = 1;
                func_804D5E10((void*)obj->renderArg, (s32)(u32)&arg);
            }
        }
        func_804E0CF0(obj->sub_0x17c, (const void*)obj->field_0xdc[5]);
        obj->field_0x18 = (f32)u16ToF_b0b8(ft->field_0x188);
    } else {
        // ---- alternate path (bit 0x1000 set): walk the parent effect's node chain ----
        if (!((obj->flags1 >> 14) & 1)) {
            goto end;
        }
        CEffectObj* parent = func_804DFEAC(obj->field_0x20);
        EffNodeUpd* node = (EffNodeUpd*)func_804D5D48((EffectNode*)((u8*)parent + 0x24));
        while (node != 0) {
            s16 idx = obj->field_0x10;
            f32 nt = node->field_0x10;
            int go = 1;
            if (idx == -1) {
                go = ((node->field_0x06 >> 6) & 1);
            } else {
                if (!(node->field_0x06 & 0x8000)) {
                    go = 0;
                } else if (idx > 0 && nt < s32ToF64_b0b0(idx)) {
                    go = 0;
                } else if (obj->field_0x12 > 0 && s32ToF64_b0b0(obj->field_0x12) > nt) {
                    go = 0;
                } else {
                    // Per-node timer slot burn-down (slot offset = field_0x22 * 4).
                    f32* slot = (f32*)((u8*)node + ((u32)obj->field_0x22 << 2));
                    f32 nv = *slot;
                    if (lbl_eu_8066B0A0 < nv) {
                        nv = nv - obj->field_0x14;
                        *slot = nv;
                        if (lbl_eu_8066B0A0 < nv) {
                            go = 0;
                        }
                    }
                }
            }
            if (go) {
                f32 d = nt - lbl_eu_8066B0A4;
                obj->sub_0x11c[0] = d;
                func_804E06B4(obj->sub_0x11c, (const void*)obj->field_0xdc[2], lbl_eu_8066B0A4,
                              (const void*)obj->field_0x08);
                obj->sub_0x14c[0] = d;
                func_804E08BC(obj->sub_0x14c, (const void*)obj->field_0xdc[4], lbl_eu_8066B0A4,
                              (const void*)obj->field_0x08);
                obj->sub_0x17c[0] = d;
                func_804E0B94(obj->sub_0x17c, (const void*)obj->field_0xdc[5], lbl_eu_8066B0A4,
                              (const void*)obj->field_0x08);
                if (obj->field_0xdc[13] ? *(const u8*)((const u8*)obj->field_0xdc[13] - 0x1c) : 0) {
                    obj->sub_0x194[0] = d;
                    func_804E2088(obj->sub_0x194, (const void*)obj->field_0xdc[13], lbl_eu_8066B0A4,
                                  (const void*)obj->field_0x08);
                }
                if (obj->field_0xdc[12] ? *(const u8*)((const u8*)obj->field_0xdc[12] - 0x19) : 0) {
                    obj->sub_0x1c4[0] = d;
                    func_804E24A8(obj->sub_0x1c4, (const void*)obj->field_0xdc[12], lbl_eu_8066B0A4,
                                  (const void*)obj->field_0x08);
                }
                if (obj->field_0xdc[14] ? *(const u8*)((const u8*)obj->field_0xdc[14] - 0x18) : 0) {
                    obj->sub_0x204[0] = d;
                    func_804E2A5C(obj->sub_0x204, (const void*)obj->field_0xdc[14], lbl_eu_8066B0A4,
                                  (const void*)obj->field_0x08);
                }
                if (func_804CAAD4((EffectRoot*)obj, (void*)1, node) != 0) {
                    Vec pos;
                    pos.x = eo->pos.x;
                    pos.y = eo->pos.y;
                    pos.z = eo->pos.z;
                    if (!EffMtxNearIdentity(eo)) {
                        ml::CVec3 out;
                        PSMTXMultVec(ml::CMat34::identity.mtx, (Vec*)&pos, (Vec*)&out);
                        pos = *(Vec*)&out;
                    }

                    f32 factor = EffDepthFactor(obj, &pos);

                    s32 count = (s32)(u16ToF_b0b8(ft->field_0x188) * factor);
                    u8 mode = obj->field_0xdc[2] ? *(const u8*)((const u8*)obj->field_0xdc[2] - 0xe) : 0;
                    int isB = (mode == 7);
                    for (u16 i = 0; i < (u16)count; i++) {
                        SpawnArg arg;
                        arg.scene = obj->field_0x08;
                        arg.obj = obj;
                        arg.type = (u16)node->field_0x00;
                        arg.b0 = 0;
                        func_804D5E10((void*)obj->renderArg, (s32)(u32)&arg);
                        if (isB) {
                            arg.b0 = 1;
                            func_804D5E10((void*)obj->renderArg, (s32)(u32)&arg);
                        }
                    }
                    func_804E0CF0(obj->sub_0x17c, (const void*)obj->field_0xdc[5]);
                    *((f32*)((u8*)node + ((u32)obj->field_0x22 << 2))) = (f32)u16ToF_b0b8(ft->field_0x188);
                }
            }
            node = (EffNodeUpd*)func_804D5DAC((EffectNode*)((u8*)parent + 0x24));
        }
    }
end:;
}
// Object bound at effect+0x08 (scene/desktop): scale factors at 0x34..0x3c.
struct Field08Scale {
    u16 field_0x00;          // 0x00 flags (bit 0x10 = scale emission translation)
    u8 pad_0x02[0x34 - 0x02];
    f32 scale_x;             // 0x34
    f32 scale_y;             // 0x38
    f32 scale_z;             // 0x3c
};

// Frame-update view of the effect object used by func_804C9D30: the 0x64/0xac
// emission matrices, the 0x128 position and the 0x140/0x170 anchor columns.
struct EffFrameObj {
    u32 field_0x00;          // 0x00 type
    u8 pad_0x04[0x08 - 0x04];
    Field08Scale* field_0x08;  // 0x08
    f32 field_0x0c;          // 0x0c
    u8 pad_0x10[0x1c - 0x10];
    u16 flags1;              // 0x1c
    u8 pad_0x1e[0x24 - 0x1e];
    u8 renderArg[0x2e - 0x24]; // 0x24 node-chain head
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
    f32 field_0x158[6];      // 0x158..0x170
    f32 field_0x170[8];      // 0x170..0x190
    u8 pad_0x190[0x204 - 0x190];
    f32 field_0x204[8];      // 0x204
};

// Word/float-viewed 48-byte matrix used for the emission transforms. The u32
// view makes the retail whole-matrix word copies expressible as assignments.
struct EffWMat {
    union {
        u32 w[12];
        f32 f[12];
        Mtx mtx;
    };
};

// Node sink view used by func_804C9D30: two 12-word matrix slots at 0x130 and
// 0x160 receive the emission transforms.
struct EffNodeSink {
    u8 pad_0x00[0x130];
    EffWMat slotA;           // 0x130
    EffWMat slotB;           // 0x160
};

extern "C" EffectNode* func_804D5D48(EffectNode* self);
extern "C" void func_804CCF84(void* self);

// Build one emission transform pair for the 0x64 (anchor 0x140, position
// 0x128) or 0xac (anchor 0x170, vector 0x158) matrices: negate the anchor
// column, build a matrix with func_804DB980, add the (optionally scaled)
// position, then concatenate. When the bound scene's 0x800 flag is clear the
// scaled variant is just a copy of the unscaled result.
// NOTE: deliberately NOT a helper - retail duplicates this block textually at
// four sites so each instance owns distinct stack slots.
#define EFF_BUILD_PAIR(emMain, emAlt, mtxObj, anchorSrc, addSrc) \
    { \
        Vec neg; \
        EffWMat anchor; \
        neg.x = -(anchorSrc)[0]; \
        neg.y = -(anchorSrc)[1]; \
        neg.z = -(anchorSrc)[2]; \
        func_804DB980(&anchor.mtx, &neg, (s32)obj->field_0x48); \
        (emMain) = anchor; \
        (emMain).f[3] += (addSrc)[0]; \
        (emMain).f[7] += (addSrc)[1]; \
        (emMain).f[11] += (addSrc)[2]; \
        EffWMat tmp; \
        PSMTXConcat((mtxObj).mtx, (emMain).mtx, tmp.mtx); \
        (emMain) = tmp; \
        if (((obj->field_0x08->field_0x00 >> 11) & 1) == 0) { \
            (emAlt) = (emMain); \
        } else { \
            (emAlt) = anchor; \
            (emAlt).f[3] += (addSrc)[0] * obj->field_0x08->scale_x; \
            (emAlt).f[7] += (addSrc)[1] * obj->field_0x08->scale_y; \
            (emAlt).f[11] += (addSrc)[2] * obj->field_0x08->scale_z; \
            EffWMat tmp2; \
            PSMTXConcat((mtxObj).mtx, (emAlt).mtx, tmp2.mtx); \
            (emAlt) = tmp2; \
        } \
    }

// func_804C9D30: per-frame effect-object update. When the lifetime (0x2e) is
// positive, builds the two emission transforms (negated anchor columns via
// func_804DB980, position added, concatenated with the 0x64/0xac matrices;
// optionally rescaled by the bound scene's 0x34 factors) once up front and
// again per node inside the chain walk, calling func_804CAC08 and folding the
// resulting matrices back into each node.
void func_804C9D30(EffFrameObj* obj) {
    f32 cZero;
    if (obj->field_0x2e <= 0) return;
    s32 ok = 0;

    EffWMat em68;            // transform pair 1: main / scaled
    EffWMat em98;
    EffWMat emC8;            // transform pair 2
    EffWMat emF8;

    if (((obj->flags1 >> 12) & 1) == 0) {
        cZero = lbl_eu_8066B0A4;
        if (func_804CAAD4((EffectRoot*)obj,
                          (void*)(u32)(obj->field_0x0c <= cZero), 0) != 0) {
            // Emission transform 1: negated 0x140 anchor + position 0x128,
            // concatenated with the 0x64 matrix.
            EFF_BUILD_PAIR(em98, em68, obj->mtx, &obj->field_0x134[3], &obj->pos.x)
            // Emission transform 2: negated 0x170 anchor + vector 0x158 with
            // the 0xac matrix.
            EFF_BUILD_PAIR(emF8, emC8, obj->mtx2, &obj->field_0x170[0], &obj->field_0x158[0])
            ok = 1;
        }
    }

    EffectNode* node = func_804D5D48((EffectNode*)obj->renderArg);
    while (node != 0) {
        if (((obj->flags1 >> 12) & 1) == 0) {
            s32 ok2;
            if (node->field_0x04 >= 0) {
                // Node references another effect node by index: both must be
                // active before the emission update runs for this node.
                EffectNode* ref = func_804E0114(node->field_0x04);
                if (ref == 0 || ref->field_0x00 < 0) {
                    ok2 = 0;
                } else {
                    ok2 = func_804CAAD4((EffectRoot*)obj, (void*)1, ref);
                }
            } else {
                ok2 = func_804CAAD4((EffectRoot*)obj,
                                    (void*)(u32)(obj->field_0x0c <= cZero), 0);
            }
            if (ok2) {
                // Per-node rebuild of both emission transforms (same shape as
                // the pre-loop build).
                EFF_BUILD_PAIR(em98, em68, obj->mtx, &obj->field_0x134[3], &obj->pos.x)
                EFF_BUILD_PAIR(emF8, emC8, obj->mtx2, &obj->field_0x170[0], &obj->field_0x158[0])
                ok = 1;
            }
        }
        if (ok != 0) {
            if (((node->field_0x06 >> 11) & 1) == 0) {
                func_804CAC08((EffObject*)obj, (EffSceneRef*)node,
                              &em68.mtx, &emC8.mtx, (const EffMtx*)&em98,
                              (const EffMtx*)&emF8);
            }
            // Fold the emission transforms back into the node's two matrix
            // slots: wide (whole-matrix) copies for effect types 9-11, else
            // sparse copies of the translation column (slot A) and the 3x3
            // rotation block (slot A again).
            EffNodeSink* sink = (EffNodeSink*)node;
            u32 fl = (obj->flags1 >> 8) & 3;
            if (fl != 0) {
                if ((u32)(obj->field_0x00 - 9) <= 2) {
                    if ((fl & 1) != 0) sink->slotA = em98;
                    if ((fl & 2) != 0) sink->slotB = emF8;
                } else {
                    if ((fl & 1) != 0) {
                        sink->slotA.f[3] = em98.f[3];
                        sink->slotA.f[7] = em98.f[7];
                        sink->slotA.f[11] = em98.f[11];
                    }
                    if ((fl & 2) != 0) {
                        sink->slotA.f[0] = emF8.f[0];
                        sink->slotA.f[1] = emF8.f[1];
                        sink->slotA.f[2] = emF8.f[2];
                        sink->slotA.f[4] = emF8.f[4];
                        sink->slotA.f[5] = emF8.f[5];
                        sink->slotA.f[6] = emF8.f[6];
                        sink->slotA.f[8] = emF8.f[8];
                        sink->slotA.f[9] = emF8.f[9];
                        sink->slotA.f[10] = emF8.f[10];
                    }
                }
            }
        }
        func_804CCF84(node);
        node = (EffectNode*)func_804D5DAC(obj->renderArg);
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
        func_804E06B4(obj->sub_0x11c, (const void*)obj->field_0xdc[2], lbl_eu_8066B0A0, (const void*)obj->field_0x08);
        obj->sub_0x14c[0] = f31 - lbl_eu_8066B0A4;
        func_804E08BC(obj->sub_0x14c, (const void*)obj->field_0xdc[4], lbl_eu_8066B0A0, (const void*)obj->field_0x08);
        obj->sub_0x17c[0] = f31 - lbl_eu_8066B0A4;
        func_804E0B94(obj->sub_0x17c, (const void*)obj->field_0xdc[5], lbl_eu_8066B0A0, (const void*)obj->field_0x08);
        if ((obj->field_0xdc[13] ? *(const u8*)((const u8*)obj->field_0xdc[13] - 0x1c) : 0) != 0) {
            obj->sub_0x194[0] = f31 - lbl_eu_8066B0A4;
            func_804E2088(obj->sub_0x194, (const void*)obj->field_0xdc[13], lbl_eu_8066B0A0, (const void*)obj->field_0x08);
        }
        if ((obj->field_0xdc[12] ? *(const u8*)((const u8*)obj->field_0xdc[12] - 0x19) : 0) != 0) {
            obj->sub_0x1c4[0] = f31 - lbl_eu_8066B0A4;
            func_804E24A8(obj->sub_0x1c4, (const void*)obj->field_0xdc[12], lbl_eu_8066B0A0, (const void*)obj->field_0x08);
        }
        if ((obj->field_0xdc[14] ? *(const u8*)((const u8*)obj->field_0xdc[14] - 0x18) : 0) != 0) {
            obj->sub_0x204[0] = f31 - lbl_eu_8066B0A4;
            func_804E2A5C(obj->sub_0x204, (const void*)obj->field_0xdc[14], lbl_eu_8066B0A0, (const void*)obj->field_0x08);
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

// s32 -> f64 through the shared signed magic double lbl_eu_8066B0B0 (retail
// xoris + stack-double shape, no frsp - result stays double).
static inline double s32ToF64_b0b0(s32 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[1] = (u32)v ^ 0x80000000u;
    c.w[0] = 0x43300000u;
    return c.d - lbl_eu_8066B0B0;
}

// func_804CB274: fill `out` with a random emission vector selected by `mode`
// (7/1 = zero, 0 = direction, 2 = box, 4 = arc, 3 = cylinder). The radius
// (1.0 - r6/maxScale) is computed up front and stays in f1 as the 4th
// argument of the 4-arg callees (func_804D9F00 ignores it). When the object
// type is 0xe the y component is post-processed.
void func_804CB274(EffTypeObj* self, ml::CVec3* out, s32 mode, s32 r6,
                   const ml::CVec3* in, const ml::CVec3* r8) {
    // Radius stays in double precision end-to-end (no frsp); the signed
    // argument converts through the shared 2^52+2^31 magic (helper above).
    double t = s32ToF64_b0b0(r6);
    double div = t / lbl_eu_8066B0A8;
    double radius = lbl_eu_8066B0A4 - div;
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
