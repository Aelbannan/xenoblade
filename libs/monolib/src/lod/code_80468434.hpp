#pragma once

#include <types.h>
#include <revolution/MTX.h>
#include "nw4r/math/math_types.h"
#include "monolib/math/CVec3.hpp"
#include "monolib/math/CVec4.hpp"

// Global-scope LOD distance-cull helper (defined in UnkClass_8046368C.cpp).
// Returns non-zero when the distance range [a, b] straddles the shared LOD
// distance limit (lbl_eu_806657B8).
s32 func_8046451C(s32 a, s32 b);

// LOD render-state helpers defined in sibling LOD TUs.  The retail symbol map
// keeps their C++ mangled names, so plain global-scope declarations with the
// matching signatures resolve to the retail symbols.
s32 func_80464128(u32 arg);
s32 func_804642BC(s32 a, u32 b);
s32 func_804643D8(s32 a, s32 b, s32 da, s32 db);
void func_80465314(s32 value);
void func_80463C24(s32 index, s32 offset, const f32 (*mtx)[3][4]);
void func_8046406C(s32 count);

// Scene-resource current-data getter (monolib/src/scn/code_804BC9EC.cpp).
// C linkage so the 0-arg call emits the retail name unmangled (a C++ 0-arg
// declaration would mangle to getScnHandle__Fv__Fv).
extern "C" void* getScnHandle__Fv(void);

// Fv-named entry points of sibling LOD classes (retail Fv names carry extra
// ABI values in r3, per the "Fv ABI note" - model them as explicit args).
extern "C" {
void resetLodFlags__Q23LOD17UnkClass_804645CCFv(void);
void func_8046534C__Q23LOD17UnkClass_804645CCFv(s32 value);
void setCurrentMtxCached__Q23LOD17UnkClass_80466348Fv(s32 value);
}

namespace LOD {

// Shared LOD spawn-direction table entry (retail stride 0x24).  The direction
// vector lives at +0x0C; copySpawnPos spreads a spawn position along it.
struct LodDirEntry {
    u8 pad_00[0x0C];
    nw4r::math::VEC3 dir;   // 0x0C direction
    f32 field_0x18;         // 0x18
    u8 pad_1C[0x24 - 0x1C];
};

// Per-type LOD pointer tables: lbl_eu_80665768 / lbl_eu_80665778 are the
// first entries; the helpers index them at stride 0x18 with byte arithmetic.
extern "C" {
extern s32 lbl_eu_806657A0;          // spawn-direction table index bias
extern LodDirEntry* lbl_eu_806657AC; // spawn table / matrix records (stride 0xB4)
extern const f32 lbl_eu_8066A650;    // spawn spread scale
extern u8* lbl_eu_8066576C;          // most recent object pointer
extern s32 lbl_eu_80665794;          // object index / record selector
extern const u32 lbl_eu_8066A5F0;    // visibility flag mask
extern u8* lbl_eu_80665768;          // per-type pointer table base (stride 0x18)
extern u8* lbl_eu_80665778;          // per-type pointer table base (stride 0x18)
extern const f32 lbl_eu_80665754;    // shared LOD distance constant
extern f32 lbl_eu_806657A4;          // cached LOD distance value
extern f32 lbl_eu_80658048[][3][4];  // per-record matrix work area (stride 0x30)
extern f32 lbl_eu_806581C8[][3][4];  // shared billboard matrix table (stride 0x30)
extern f32 lbl_eu_806583E0[][3][4];  // fixed LOD matrix table (stride 0x30)
extern f32 lbl_eu_80658008[][3][4];  // shared view matrix (stride 0x30)
extern f32 lbl_eu_80657FD8[][3][4];  // fixed LOD view matrix
extern u32 lbl_eu_80665788;          // matrix-index pointer table base
extern u32 lbl_eu_8066578C;          // matrix-data pointer table base
extern u32 lbl_eu_806657B0;          // LOD dirty flags
extern s16 lbl_eu_806657C8;          // vertex-array stride
extern const f32 lbl_eu_8066A64C;    // 0.0f
extern const f32 lbl_eu_8066A648;    // 1.0f
}

// Per-record draw dispatch table (target func_80469138): entry index is the
// record's field_0x29; called with (r3, r4, r5, r6).
typedef void (*LodD9B0Fn)(void*, void*, void*, u32);
extern "C" LodD9B0Fn lbl_eu_8056D9B0[];

// Record-draw dispatch table (target func_80468434): called with the byte-
// stream pointer in r3; the callee advances the stream in place.
typedef void (*LodD980Fn)(void*);
extern "C" LodD980Fn lbl_eu_8056D980[];

// Cross-TU LOD/scene entry points referenced by the record-draw helpers.
// func_804A6C60 / func_804BCC48 have unmangled retail symbols (C linkage).
extern "C" {
void func_804A6C60(void* mtx, void* record);
void func_804BCC48(void* res, void* viewData, void* renderParams);
}

// Object + records used by func_8046A3B4 (stride-0x68 record array selected
// through the per-type pointer tables).
struct LodObj1 {
    u8  pad_00[0x4];
    u16 field_0x4;    // 0x04 record count
    u8  pad_06[0x28 - 0x06];
    u32 field_0x28;   // 0x28 byte offset to the record array
};

struct LodRec1 {      // stride 0x68
    u16 field_0x0;    // 0x00 flags (bit 2 = concat + submit)
    u16 field_0x2;    // 0x02 (unused by this helper)
    u16 field_0x4;    // 0x04 parent matrix index (1-based; 0 = none)
    u8  pad_06[0x68 - 0x06];
};

// Object-header view used by func_80468434 (u32 offsets at +0x10/0x14/0x18).
struct LodHeader34 {
    u16 field_0x0;    // 0x00
    u16 field_0x2;    // 0x02
    u16 field_0x4;    // 0x04
    u8  pad_06[0x10 - 0x06];
    u32 field_0x10;   // 0x10
    u32 field_0x14;   // 0x14
    u32 field_0x18;   // 0x18
    u8  pad_1C[0x28 - 0x1C];
    u32 field_0x28;   // 0x28
};

// Object-header view used by the record-draw helpers func_80469138 /
// func_80469620 / func_80469B5C.  Reads the header bytes with the widths
// those helpers use; aliases records[0] of UnkClass_80468434.
struct LodDrawHeader {
    u8  field_0x0;    // 0x00
    u8  field_0x1;    // 0x01 flags (bit1 = distance cull, bit7 = group fade)
    u16 field_0x2;    // 0x02 record index / loop count
    u16 field_0x4;    // 0x04 cull distance
    u16 field_0x6;    // 0x06
    u16 field_0x8;    // 0x08 cull distance
    u16 field_0xA;    // 0x0A cull distance
    u8  field_0xC;    // 0x0C
    u8  field_0xD;    // 0x0D
    union {
        struct {
            u8 field_0xE;    // 0x0E (byte view)
            u8 field_0xF;    // 0x0F
        };
        u16 field_0xE_u16;   // 0x0E 16-bit matrix-select index
    };
    u16 field_0x10;   // 0x10 group id
    u16 field_0x12;   // 0x12 group id
    u8  field_0x14;   // 0x14
    u8  field_0x15;   // 0x15
    u16 field_0x16;   // 0x16 flags
    u8  field_0x18;   // 0x18 (matrix bytes)
    u8  field_0x19;   // 0x19
    u16 field_0x1A;   // 0x1A
    u16 field_0x1C;   // 0x1C
    u8  field_0x1E[2];// 0x1E
    u16 field_0x20;   // 0x20
    u16 field_0x22;   // 0x22
    u8  field_0x24;   // 0x24
    u8  field_0x25[0x2C - 0x25];
};

// 0x2C-stride LOD draw record viewed by the billboard renderers (the +0x1C
// / +0x48 record views).  The same memory is also addressed through
// LodRenderRecord's byte/word fields by the cull helpers.
struct LodDrawRec {
    f32 field_0x0;    // 0x00
    f32 field_0x4;    // 0x04
    f32 field_0x8;    // 0x08
    f32 field_0xC;    // 0x0C
    f32 field_0x10;   // 0x10
    f32 field_0x14;   // 0x14
    f32 field_0x18;   // 0x18
    u16 field_0x1C[4];// 0x1C..0x22 colour indices
    u8  field_0x24[4];// 0x24..0x27 texcoord indices
    u8  field_0x28;   // 0x28
    u8  field_0x29;   // 0x29
    u8  field_0x2A;   // 0x2A billboard matrix index
    u8  field_0x2B;   // 0x2B
};

// Per-type render dispatch table (.data): the visibility-cull entry points
// (dispatchLodRecords walks it).  Each entry takes (self, out) and returns the
// next record pointer.
typedef u8* (*LodDispatchFn)(u8*, u8*);
extern "C" LodDispatchFn lbl_eu_8056D998[];

/**
 * One record of the LOD object's record array (retail stride 0x2C).  The
 * object header (offsets 0x00..0x1A) aliases record[0]; only the fields
 * touched by the matched helpers are declared, the rest is opaque padding.
 */
struct LodRenderRecord {
    u8  field_0x0;         // 0x00
    u8  field_0x1;         // 0x01 flags (bit 1 = distance-cull enabled)
    u16 field_0x2;         // 0x02 record index / count
    u16 field_0x4;         // 0x04 cull distance
    u8  field_0x6;         // 0x06
    u8  field_0x7;         // 0x07
    u16 field_0x8;         // 0x08 cull distance
    u16 field_0xA;         // 0x0A cull distance
    u32 field_0xC;         // 0x0C display-list offset
    u32 field_0x10;        // 0x10 display-list size
    u16 field_0x14;        // 0x14 current-matrix index
    u16 field_0x16;        // 0x16 flags (bit 1 = distance cull, bit 7 = group fade)
    u8  field_0x18;        // 0x18
    u8  field_0x19;        // 0x19
    u16 field_0x1A;        // 0x1A
    u16 field_0x1C;        // 0x1C colour index / matrix words
    u8  field_0x1E[2];     // 0x1E
    u16 field_0x20;        // 0x20
    u16 field_0x22;        // 0x22
    u8  field_0x24;        // 0x24 fog/alpha byte
    u8  field_0x25[0x2C - 0x25];
};

class UnkClass_80468434 {
public:
    LodRenderRecord records[8];  // record array; header fields alias record[0]

    u16* func_80468BDC();
};

// Retail entry points for the LOD cull-check helpers.  The DOL is stripped,
// so the symbol map records the shortened "...Fv" names even though these
// entry points take explicit ABI arguments (MWCC_CASES "Fv ABI note");
// the linker names must stay verbatim, hence C linkage.
extern "C" {
u8* cullLodRecordA__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self, u8* out);
u8* cullLodRecordB__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self, u8* out);
u16* cullLodEntry__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self, u8* out);
u16* cullLodEntryAlt__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self, u8* out);
void copySpawnPos__Q23LOD17UnkClass_80468434Fv(nw4r::math::VEC3* out, u32 unused,
                                               const ml::CVec4* in, s32 idx);
s32 dispatchLodRecords__Q23LOD17UnkClass_80468434Fv(u8* self);
u8* renderLodBatch__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self);
u8* renderLodBatchAlt__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self);
void transformSpawnPos__Q23LOD17UnkClass_80468434Fv(Vec* self,
                                                Vec* vec,
                                                const Vec* src, s32 idx);
void transformSpawnPosAlt__Q23LOD17UnkClass_80468434Fv(Vec* self,
                                                Vec* vec,
                                                const Vec* src, s32 idx);

// The Fv-named record-draw entry points carry their real ABI values in the
// argument registers (MWCC_CASES "Fv ABI note"); the record index lives
// in r3 (u32) for func_8046A3B4, and the object pointer in r3 for the rest.
void func_8046A3B4__Q23LOD17UnkClass_80468434Fv(u32 idx, const f32* srcMtx,
                                                u8* arg2);
void func_80468434__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self,
                                                u8* arg2);
u16* func_80469138__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self);
u16* func_80469620__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self);
u16* func_80469B5C__Q23LOD17UnkClass_80468434Fv(UnkClass_80468434* self);
}
} // namespace LOD
