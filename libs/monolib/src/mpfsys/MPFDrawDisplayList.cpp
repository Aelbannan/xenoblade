// Auto-scaffolded catalog TU for monolib/src/mpfsys/MPFDrawDisplayList
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/mpfsys/MPFDrawDisplayList.hpp"
#include "monolib/mpfsys/UnkClass_80471EC8.hpp"

#include <nw4r/math/math_types.h>
#include <nw4r/math/math_arithmetic.h>
#include <revolution/os/OSFastCast.h>
#include <revolution/os/OSCache.h>
#include <revolution/GX.h>
#include <monolib/math/CVec3.hpp>
#include <monolib/math/Utility.hpp>

#include <stdlib.h>
#include <string.h>

extern const f64 lbl_eu_8066A7F8;  // u8/u16->f32 magic
extern const f64 lbl_eu_8066A808;  // s32->f32 magic

// nw4r diagnostics (retail-named; see UnkClass_80471EC8.cpp).
extern "C" void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

// Builtin (f32) casts pool TU-local magic doubles; the retail object
// references the blob pool entries (lbl_eu_8066A7F8 u8/u16, lbl_eu_8066A808
// s32) instead. Union helpers keep this TU's .sdata2 empty (retail shape).
static inline f32 u8ToF_a7f8(u8 v) {
    union { double d; u32 w[2]; } c;
    c.w[0] = 0x43300000u;
    c.w[1] = v;
    return (f32)(c.d - lbl_eu_8066A7F8);
}
static inline f32 s32ToF_a808(s32 v) {
    union { double d; u32 w[2]; } c;
    c.w[0] = 0x43300000u;
    c.w[1] = (u32)v ^ 0x80000000u;
    return (f32)(c.d - lbl_eu_8066A808);
}
// Caller-owned bit-cast slot: w[0] is written once by the caller, only
// w[1] changes per conversion (matches the retail store/lfd shape).
union S32Cast {
    f64 d;
    u32 w[2];
};
static inline f64 s32ToF64_a808(s32 v, S32Cast* c) {
    c->w[1] = (u32)v ^ 0x80000000u;
    return c->d - lbl_eu_8066A808;
}
// f32 flavor over the same caller-owned slot (header pre-stored once).
static inline f32 s32ToF32_a808(s32 v, S32Cast* c) {
    c->w[1] = (u32)v ^ 0x80000000u;
    return (f32)(c->d - lbl_eu_8066A808);
}

// ---------------------------------------------------------------------------
// Data layouts recovered from the retail assembly (func_804753B4 walks the
// 2D map triangle grid and interpolates height + color at the current
// position).
//
//   self+0x04 : axis-aligned bounds (used by sibling functions)
//   self+0x08 : vertex array (2D float pairs, 8-byte stride)
//   self+0x0C : triangle-height array ({s16, s16, f32}, 8-byte stride)
//   self+0x10 : vertex-color array (RGB bytes, 3-byte stride)
//   self+0x14 : current position + interpolated color
//   self+0x30 : entry array (16-byte stride) - this function's argument
//   self+0x38 : flag mask gate
// ---------------------------------------------------------------------------

struct MPFDrawEntry {          // 0x10-byte walk entries (self+0x30)
    u16 i0;                    // +0x00 vertex index A
    u16 i1;                    // +0x02 vertex index B
    u16 i2;                    // +0x04 third vertex index = i1 + i2
    u16 c0;                    // +0x06 color index A
    u16 c1;                    // +0x08 color index B
    u16 c2;                    // +0x0A color index C
    u16 tri;                   // +0x0C height-plane index
    u16 mask;                  // +0x0E flag mask
};

struct MPFDrawVert {           // 8-byte 2D vertex
    f32 x;
    f32 y;
};

struct MPFDrawTri {            // 8-byte height plane: y = h0*x + h1*y + off
    s16 h0;
    s16 h1;
    f32 off;
};

struct MPFDrawCol {            // 3-byte RGB color
    u8 r;
    u8 g;
    u8 b;
};

struct MPFDrawPos {            // position + interpolated vertex color
    f32 x;                     // +0x00
    f32 y;                     // +0x04
    f32 z;                     // +0x08
    u8 r;                      // +0x0C
    u8 g;                      // +0x0D
    u8 b;                      // +0x0E
    u8 field_0xf;              // +0x0F
    f32 field_0x10;            // +0x10 second position slot x
    f32 field_0x14;            // +0x14 second position slot y
    f32 field_0x18;            // +0x18 second position slot z
};

struct MPFColor {              // float color used for the interpolation
    f32 r;
    f32 g;
    f32 b;
};

struct MPFDrawDisplayListLayout {
    u8 gap00[4];              // +0x00
    u8 gap04[4];              // +0x04 bounds data
    MPFDrawVert* verts;       // +0x08
    MPFDrawTri* tris;         // +0x0C
    MPFDrawCol* cols;         // +0x10
    MPFDrawPos* pos;          // +0x14
    u8 gap18[0x18];           // +0x18
    MPFDrawEntry* entries;    // +0x30
    u8 gap34[4];              // +0x34
    u32 flags;                // +0x38
};

// Draw-data chain handed to setPlainDrawNode / setColorDrawNode (retail r4).  The
// pointer is stored at self+0x04 and forwarded to the sibling draw walkers
// (func_804795BC / drawColoredList) when the chain carries data.
struct MPFDrawNode {
    u32 field_0x0;            // +0x00 arena offset of the batch data
    u32 field_0x4;            // +0x04 non-zero when the chain has draw data
    u32 field_0x8;            // +0x08 arena offset
    u32 field_0xC;            // +0x0C arena offset
    u32 field_0x10;           // +0x10 arena offset
    u32 field_0x14;           // +0x14 arena offset
    u32 field_0x18;           // +0x18 arena offset
    u32 field_0x1C;           // +0x1C arena offset
    u32 field_0x20;           // +0x20 arena offset
    u32 field_0x24;           // +0x24 arena offset
    u32 field_0x28;           // +0x28 arena offset
    f32 field_0x2c;           // +0x2C bounds min X
    f32 field_0x30;           // +0x30 bounds min Z
    f32 field_0x34;           // +0x34 bounds max X
    f32 field_0x38;           // +0x38 bounds max Z
};

// Global slot chain (head at lbl_eu_80665870) browsed by allocateDrawSlot.  The
// key at +0x00 is an arena offset expressed in 16-byte units with a 0x2c
// base; +0x0e bit 1 marks a slot as occupied.
struct MPFDrawSlot {
    u32 field_0x0;            // +0x00 key
    MPFDrawSlot* next;        // +0x04 global chain
    MPFDrawSlot* field_0x8;   // +0x08 bucket-chain link
    u8 field_0xc;             // +0x0c
    u8 field_0xd;             // +0x0d
    u8 field_0xe;             // +0x0e flags (bit 1 = in use)
    u8 gap0f;                 // +0x0f
    u16 field_0x10;           // +0x10
    u16 field_0x12;           // +0x12
    f32 field_0x14;           // +0x14
    f32 field_0x18;           // +0x18
    f32 field_0x1c;           // +0x1c
    f32 field_0x20;           // +0x20
    f32 field_0x24;           // +0x24
    u16 field_0x28;           // +0x28
    u16 field_0x2a;           // +0x2a
};

// 0xC-byte walk cells (self+0x34).  The grid at self+0x20 holds indices into
// this array; even values recurse into another cell, odd values select a
// draw-list run from the u16 index array at self+0x24.
struct MPFDrawCell {
    u32 offset;               // +0x00 base offset into the cell grid
    u16 i0;                   // +0x04 vertex index A
    u16 i1;                   // +0x06 vertex index B
    u8 w;                     // +0x08 grid width
    u8 h;                     // +0x09 grid height
    u16 mask;                 // +0x0A flag mask
};

// 0x4c-byte per-item data (self+0x18 cursor advances by 0x4c per entry).
struct MPFDrawItem {
    u8 gap00[0x16];           // +0x00
    u8 field_0x16;            // +0x16
    u8 field_0x17;            // +0x17 scatter mode (1 = jittered, 2 = random)
    u8 field_0x18;            // +0x18
    u8 gap19[0xF];            // +0x19..+0x28
    f32 field_0x28;           // +0x28
    u8 gap2C[8];              // +0x2C..+0x34
    u8 field_0x34;            // +0x34
    u8 gap35[0x17];           // +0x35..+0x4C
};

// Global draw state at lbl_eu_80665838 (pointer).  The render-state object
// spans +0x00..+0x2E08 (UnkClass_80471EC8); the current cell index lives at
// +0x2E12 (advanced by func_80477F80).
struct MPFDrawGlobal {
    u8 gap00[0x2DFC];             // +0x00
    f32 field_0x2DFC;             // +0x2DFC
    f32 field_0x2E00;             // +0x2E00
    u8 gap2E04[0x2E12 - 0x2E04];  // +0x2E04
    s16 field_0x2E12;             // +0x2E12 current cell index
};

// Per-billboard item data (0x4c stride) consumed by func_804783D0 and
// func_80478C94.  The position is at +0x00/+0x04, the flag word at +0x10,
// the layer index at +0x1b and the spread value at +0x20; the sibling draw
// walkers (func_804795BC etc.) read the remaining fields.
struct MPFBillItem {
    f32 x;                    // +0x00
    f32 y;                    // +0x04
    u8 gap08[8];              // +0x08
    u32 flags;                // +0x10
    u8 gap14[7];              // +0x14
    u8 layer;                 // +0x1b
    u8 gap1c[4];              // +0x1c
    f32 spread;               // +0x20
    u8 gap24[8];              // +0x24
    f32 field_0x2c;           // +0x2c near radius
    f32 field_0x30;           // +0x30 far radius
    u8 field_0x34;            // +0x34 mask-table index
    f32 field_0x38;           // +0x38 fallback radius
    u8 gap3c[0x4c - 0x3c];    // +0x3c
};

// 0x10-byte probe entry inside each slot (slot + 0x2c); the walk in
// func_80478C94 advances it by 0x10 per billboard.
struct MPFDrawProbe {
    f32 x;                    // +0x00
    f32 y;                    // +0x04
    f32 z;                    // +0x08
    u8 r;                     // +0x0C
    u8 g;                     // +0x0D
    u8 b;                     // +0x0E
    u8 counter;               // +0x0F proximity counter
};

// Display-list entry written by func_80478C94 (0xcc0-byte stride).
// field_0x10/field_0x14 are the position/color cursors; field_0x4 counts
// the written quads and doubles as the copy format for func_804782C4.
struct MPFDispEntryFull {
    u16 field_0x0;            // +0x00 layer
    u16 field_0x2;            // +0x02 flags
    u32 field_0x4;            // +0x04 quad count / copy format
    u32 field_0x8;            // +0x08 quad byte size
    u32 field_0xc;            // +0x0C display-list byte count
    u32 field_0x10;           // +0x10 position cursor
    u32 field_0x14;           // +0x14 color cursor
    u32 field_0x18;           // +0x18 display-list pointer
    u8 arrayA[0xbc0];         // +0x20 position array (12-byte stride)
    u8 arrayB[0xe0];          // +0xbe0 color array (3-byte stride)
};

// 0xb4-byte per-layer draw matrix entry returned by UnkClass_80471EC8::
// func_804734F4.  The transform matrix sits at +0x54 and each entry advances
// by 0xb4.
struct MPFBillMtx {
    u8 gap00[0x54];           // +0x00
    Mtx mtx;                  // +0x54
    u8 gap84[0x30];           // +0x84
};

// Layout used by the draw-list management functions (setPlainDrawNode,
// setColorDrawNode, allocateDrawSlot, checkProbeRange).  Kept separate from
// MPFDrawDisplayListLayout because those functions read +0x04/+0x0e/+0x14 as
// pointer/flag/vector fields rather than the map-walk layout.
struct MPFDrawMgrLayout {
    u8 gap00[4];              // +0x00
    MPFDrawNode* field_0x4;   // +0x04 current node chain
    u8 gap08[6];              // +0x08
    u8 field_0xe;             // +0x0e flags
    u8 gap0f[5];              // +0x0f
    ml::CVec3 field_0x14;     // +0x14 probe vector (x,y,z at +0x14..+0x1c)
    u8 gap20[0x34];           // +0x20
    MPFDrawSlot* field_0x54;  // +0x54 best-fit insertion slot
};

// self layout for updateSlotChain: only the +0x3c gate field is read.
struct MPFDrawGateLayout {
    u8 gap00[0x3c];           // +0x00
    u32 field_0x3c;           // +0x3c
};

// Draw-list buffer header used by createSlotFromPool.  Slots (0x2c bytes each)
// are carved out of the buffer at 16-byte-unit offsets past the header.
struct MPFDrawListHdr {
    u32 field_0x0;            // +0x00 free-space / arena offset
    MPFDrawSlot* field_0x4;   // +0x04 last allocated slot
    u8 gap08[6];              // +0x08
    u8 field_0xe;             // +0x0e flags
    u8 gap0f[0x19];           // +0x0f
    u16 field_0x28;           // +0x28 slot count (16-byte units)
    u16 field_0x2a;           // +0x2a
};

// Draw-list entry (0xcc0-byte stride) iterated by the display-list walkers.
struct MPFDispEntry {
    u16 field_0x0;            // +0x00 billboard index
    u16 field_0x2;            // +0x02 flags (bit 1 = enabled for the color pass)
    u8 gap04[8];              // +0x04
    u32 field_0xc;            // +0x0c display-list byte count
    u8 gap10[8];              // +0x10
    u8* field_0x18;           // +0x18 display-list pointer
    u8 pad1c[4];              // +0x1c
    u8 arrayA[0xbc0];         // +0x20 position array (12-byte stride)
    u8 arrayB[0xe0];          // +0xbe0 color array (3-byte stride)
};

// Billboard data entry (0x4c-byte stride) indexed by MPFDispEntry.field_0x0.
struct MPFDrawData {
    u8 gap00[0x10];           // +0x00
    u32 field_0x10;           // +0x10 flags
    s16 field_0x14;           // +0x14 tex index
    u8 gap16[3];              // +0x16
    u8 field_0x19;            // +0x19 tex map id
    u8 gap1a[2];              // +0x1a
    f32 field_0x1c;           // +0x1c tex scale
    u8 gap20[0x4c - 0x20];    // +0x20 tail
};

// Display-list walk config (lbl_eu_80665874): two counts, flags and the
// entry-array base used by the draw walkers.
struct MPFDrawCfg {
    s32 field_0x0;            // +0x00
    s32 field_0x4;            // +0x04
    u16 field_0x8;            // +0x08 flags
    u8 gap0a[2];              // +0x0a
    s32 field_0xc;            // +0x0c
    MPFDispEntry* field_0x10; // +0x10
    u8 gap14[0xC];            // +0x14..+0x20
    u8 field_0x20;            // +0x20
};

// Draw command used by func_804782C4: index, format, size and buffer.
struct MPFDrawCmd {
    u16 field_0x0;            // +0x00 index
    u16 field_0x2;            // +0x02 flags (bit 1)
    s32 field_0x4;            // +0x04 format (0xfc = direct copy)
    u8 gap08[4];              // +0x08
    s32 field_0xc;            // +0x0c byte size
    u8 gap10[8];              // +0x10
    u8* field_0x18;           // +0x18 destination buffer
};

// Source buffers for func_804782C4 (self+0x28 / self+0x2c).  Kept as u32 so
// the offset arithmetic is plain integer adds (byte-identical to the retail
// pointer math without pointer-add canonicalization).
struct MPFDrawSrcLayout {
    u8 gap00[0x28];           // +0x00
    u32 field_0x28;           // +0x28 arena address
    u32 field_0x2c;           // +0x2c arena address
};

// Full recovered layout of MPFDrawDisplayList (offsets from the retail
// assembly; the class header has no data members yet).
struct MPFDrawListLayout {
    u8 gap00[4];              // +0x00
    MPFDrawNode* field_0x4;   // +0x04 current node chain
    MPFDrawVert* field_0x8;   // +0x08 vertex array
    MPFDrawTri* field_0xC;    // +0x0C triangle-height array
    MPFDrawCol* field_0x10;   // +0x10 vertex-color array
    MPFDrawPos* field_0x14;   // +0x14 current position + color
    MPFDrawItem* field_0x18;  // +0x18 per-item cursor
    MPFDrawVert* field_0x1C;  // +0x1C vertex array (cell walk)
    u32* field_0x20;          // +0x20 cell grid
    u16* field_0x24;          // +0x24 cell index array
    u32 field_0x28;           // +0x28 arena base A
    u32 field_0x2C;           // +0x2C arena base B
    MPFDrawEntry* field_0x30; // +0x30 draw entries (0x10 stride)
    MPFDrawCell* field_0x34;  // +0x34 walk cells (0xC stride)
    u32 field_0x38;           // +0x38 flag mask
    u32 field_0x3c;           // +0x3C gate
    f32 field_0x40;           // +0x40 bounds min X
    f32 field_0x44;           // +0x44 bounds min Z
    f32 field_0x48;           // +0x48 bounds max X
    f32 field_0x4C;           // +0x4C bounds max Z
    MPFDrawSlot* field_0x50;  // +0x50 current slot
    MPFDrawSlot* field_0x54;  // +0x54 best-fit insertion slot
    MPFDrawSlot* field_0x58;  // +0x58 chained slot head
};

// Singleton block (.bss, 0x60 bytes = 0x18 pointer slots): the first slot is
// the instance pointer returned by getInstance.  Declared at the full retail
// size so MWCC keeps absolute lis/addi addressing (small externs default to
// sda21).
// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.bss] 0x80658488-0x806584E8 (0x60 = 96B): global instance slot array.
mpfsys::MPFDrawDisplayList* lbl_eu_80658488[0x18];

// [.sbss] 0x80665888-0x80665890 (8B): getInstance init flag + pad.
s8 lbl_eu_80665888[8];  // flag at [0], pad to the retail 8B

// [.rodata] 0x80523D98-0x80523DC8 (0x30 = 48B): class name strings.
extern "C" __declspec(align(8)) const char lbl_eu_80523D98[0x20] = {
    0x6D,0x70,0x66,0x73,0x79,0x73,0x3A,0x3A,0x4D,0x50,0x46,0x44,0x72,0x61,0x77,0x44,
    0x69,0x73,0x70,0x6C,0x61,0x79,0x4C,0x69,0x73,0x74,0x00,0x00,0x00,0x00,0x00,0x00,
};
extern "C" __declspec(align(8)) const char lbl_eu_80523DB8[0x10] = {
    0x6D,0x70,0x66,0x73,0x79,0x73,0x3A,0x3A,0x4D,0x50,0x46,0x44,0x72,0x61,0x77,0x00,
};

// [.data] 0x8056DBA0-0x8056DBC0 (0x20 = 32B): prototype instance vtables.
namespace MPFDrawBlob {
extern "C" void deallocate__Q26mpfsys18MPFDrawDisplayListFv();
extern "C" void destroy__Q26mpfsys18MPFDrawDisplayListFv();
}
extern "C" u32 lbl_eu_80663868[2];  // .sdata RTTI locators (defined below)
extern "C" u32 lbl_eu_80663870[2];
extern "C" u32 lbl_eu_8056DBA0[4] = {
    (u32)&lbl_eu_80663868, 0x00000000,
    (u32)&MPFDrawBlob::deallocate__Q26mpfsys18MPFDrawDisplayListFv,
    (u32)&MPFDrawBlob::destroy__Q26mpfsys18MPFDrawDisplayListFv,
};
extern "C" u32 lbl_eu_8056DBB0[4] = {
    (u32)&lbl_eu_80663870, 0x00000000, 0x00000000, 0x00000000,
};
extern "C" u32 lbl_eu_80663868[2] = { (u32)&lbl_eu_80523D98, (u32)&lbl_eu_8056DBB0 };
extern "C" u32 lbl_eu_80663870[2] = { (u32)&lbl_eu_80523DB8, 0x00000000 };

// Prototype instance body defined by monolibdata2 (.data, 0x10 bytes).
struct MPFDrawProto {
    u8 body[0x10];                    // +0x00
};

// ---------------------------------------------------------------------------
// Imports (defined by other units / monolibdata2; global-scope names are not
// mangled by MWCC, so plain extern declarations emit the retail symbols).
// ---------------------------------------------------------------------------
extern MPFDrawSlot* lbl_eu_80665870;                // global slot-chain head (.sbss)
extern ml::CVec3 lbl_eu_80658410;                   // probe reference vector (.bss)
extern ml::CVec3 lbl_eu_8065841C;                   // probe coefficient vector (.bss)
extern u32 lbl_eu_8066586C;                         // shared TEV/Z-mode flag state
// Draw-data arena base (MPFDrawMdlColor / UnkClass_80471EC8 declare the same).
extern u8* lbl_eu_80665840;
extern MPFDrawCfg* lbl_eu_80665874;                 // display-list walk config
extern MPFDrawGlobal* lbl_eu_80665838;              // global draw state (current cell index)
extern u32* lbl_eu_80665864;                        // global bit-mask table
extern Mtx lbl_eu_80658428;                        // shared billboard matrix
extern s32 lbl_eu_8066A728;                        // billboard count
extern f32 lbl_eu_80665880;                        // billboard probe scale A
extern f32 lbl_eu_80665884;                        // billboard probe scale B

namespace mpfsys {

MPFDrawDisplayList* MPFDrawDisplayList::getInstance() {
    if (!lbl_eu_80665888[0]) {
        // First call: point the instance slot at the prototype instance blob.
        lbl_eu_80658488[0] = (MPFDrawDisplayList*)&lbl_eu_8056DBA0;
        lbl_eu_80665888[0] = 1;
    }
    return (MPFDrawDisplayList*)&lbl_eu_80658488;
}

void MPFDrawDisplayList::destroy() {}

void MPFDrawDisplayList::deallocate() {}

} // namespace mpfsys

// SDA2 float/double constants referenced by the retail body (pooled in the
// retail build; referencing the named symbols avoids TU-local pool labels).
extern "C" {

extern const f32 lbl_eu_8066A7E0;  // slot fill constant
extern const f32 lbl_eu_8066A7E4;  // slot fill constant
extern const f32 lbl_eu_8066A7E8;  // 0.0f
extern const f32 lbl_eu_8066A7EC;  // -0.001f
extern const f32 lbl_eu_8066A7F0;  // 1.0f
extern const f32 lbl_eu_8066A7F4;  // 0.001f
extern const f32 lbl_eu_8066A800;  // 0.0f
extern const f32 lbl_eu_8066A804;  // 0.5f
extern const f64 lbl_eu_8066A808;  // 2^52 (double)
extern const f32 lbl_eu_8066A810;  // probe range scale
extern const f32 lbl_eu_8066A730;  // billboard spread scale (flags&2 path)
extern const f32 lbl_eu_8066A83C;  // billboard spread scale (10-entry ramp)
extern const f32 lbl_eu_8066A814;  // probe fallback scale
extern const f32 lbl_eu_8066A818;  // grid reset value
extern const f32 lbl_eu_8066A81C;  // probe scatter scale
extern const f32 lbl_eu_8066A820;  // probe scatter scale 2
extern const f32 lbl_eu_8066A824;  // fallback probe radius
extern const f64 lbl_eu_8066A7F8;  // u8/u16->f32 magic (0x4330000000000000)


extern const f32 lbl_eu_8066A828;  // probe fallback radius A
extern const f32 lbl_eu_8066A82C;  // probe fallback radius B
extern const f32 lbl_eu_8066A830;  // probe fallback radius C
extern const f32 lbl_eu_8066A834;  // probe fallback radius D
extern const f32 lbl_eu_8066A838;  // fast-path probe radius
extern const f32 lbl_eu_8066A83C;  // billboard spread scale (10-entry ramp)
extern const f32 lbl_eu_8066A840;  // mid-path probe radius
extern const f32 lbl_eu_8066A844;  // billboard probe color scale

// TEV / texture pipeline helpers implemented by mpfsys::UnkClass_80471EC8.
// Retail mangles them as Fv/Fif members but they are invoked with the
// arguments below (see the sibling MPFDrawBillLayTex / MPFDrawMdlColor
// units).
void bindTexture__Q26mpfsys17UnkClass_80471EC8Fif(s16 texIdx, f32 texScale);
void* getLayerRecord__Q26mpfsys17UnkClass_80471EC8FUc(mpfsys::UnkClass_80471EC8* self, u8 layer);
void* func_804B5A68(void);
void setupGfxMode5__Q26mpfsys17UnkClass_80471EC8Fv(void);
void enableAlphaBlend__Q26mpfsys17UnkClass_80471EC8Fv(void);
void disableAlphaBlend__Q26mpfsys17UnkClass_80471EC8Fv(void);
void setFogIndex__Q26mpfsys17UnkClass_80471EC8Fv(u8 texMapId);
void resetAmbient__Q26mpfsys17UnkClass_80471EC8Fv(void);
void applyAmbient__Q26mpfsys17UnkClass_80471EC8Fv(void);
void disableZMode__Q26mpfsys17UnkClass_80471EC8Fv(void);
void enableZMode__Q26mpfsys17UnkClass_80471EC8Fv(void);

// Sibling draw walkers (retail Fv names; called with the node chain in r4).
void func_804795BC__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawNode* node);
void drawColoredList__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawNode* node);

// Per-item draw helpers (retail Fv names; still stubs - called from
// func_80477F80 with the item index / scale values below).
bool func_80476E50__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, s32 index, s32 mode, f32 scale);
bool func_80476344__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, s32 index, s32 mode, f32 scale);
void func_80478C94__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDispEntryFull* dst, s32* counter, s32 limit);
void func_804783D0__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, Vec* dst, s32* countOut, s32* flagsOut);

// Height/color walker + position advance (defined below in this unit).
bool func_804753B4__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, const MPFDrawEntry* e);
bool func_80476104__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self);
MPFDrawSlot* allocateDrawSlot__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, u32 index, s32 v1, s32 v2, u16 v3, u16 v4, f32 f1, f32 f2);
bool func_80475E64__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawCell* cell);
void createSlotFromPool__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawListHdr* hdr);

// collectActiveSlots: collect the in-use, non-empty slots from the global chain
// into 16 buckets keyed by slot->field_0xc, then splice the buckets into a
// single list whose head is stored at self+0x58.
void collectActiveSlots__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self) {
    MPFDrawListLayout* d = (MPFDrawListLayout*)self;
    MPFDrawSlot* buckets[16];
    MPFDrawSlot* tails[16];
    MPFDrawSlot* s = lbl_eu_80665870;
    memset(buckets, 0, 0x40);

    while (s != 0) {
        if ((s->field_0xe & 2) != 0 && s->field_0x28 != 0) {
            u32 idx = s->field_0xc;
            if (buckets[idx] == 0) {
                buckets[idx] = s;
            } else {
                tails[idx]->field_0x8 = s;
            }
            tails[idx] = s;
        }
        s = s->next;
    }

    d->field_0x58 = 0;
    MPFDrawSlot* prev = 0;
    for (u32 i = 0; i < 16; i++) {
        if (buckets[i] != 0) {
            if (d->field_0x58 == 0) {
                d->field_0x58 = buckets[i];
            }
            if (prev != 0) {
                prev->field_0x8 = buckets[i];
            }
            prev = tails[i];
            prev->field_0x8 = 0;
        }
    }
}

// func_80475C78: walk the cell grid from the node bounds.  Each cell
// interpolates a quantized grid coordinate from the vertex pair, then either
// descends into a sub-cell (even grid value) or runs its draw-list entries
// through the height/color walker (func_804753B4) and advances the position
// (func_80476104).  The x coordinate re-reads the i1 vertex x as its own
// multiplier (retail shape).
bool func_80475C78__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self) {
    MPFDrawListLayout* d = (MPFDrawListLayout*)self;
    // Declarations in MWCC scratch-color priority (ascending); assignments
    // keep the retail load/statement order.
    u32 mask;
    s32 ix;
    s32 iz;
    const MPFDrawVert* v1;
    MPFDrawNode* node;
    MPFDrawCell* cells;
    MPFDrawPos* pos;
    pos = d->field_0x14;
    node = d->field_0x4;

    // Gate: the walk position must lie inside the node's X/Z extents.
    if (node->field_0x34 > pos->x || node->field_0x2c < pos->x) return false;
    if (node->field_0x38 > pos->z || node->field_0x30 < pos->z) return false;

    mask = d->field_0x38;
    cells = d->field_0x34;

    MPFDrawCell* e = cells;
    while (true) {
        if (!(e->mask & mask)) break;
        v1 = &d->field_0x1C[e->i1];
        ix = (s32)((pos->x - d->field_0x1C[e->i1].x) * d->field_0x1C[e->i0].x);
        iz = (s32)((pos->z - v1->y) * d->field_0x1C[e->i0].y);
        if (ix < 0 || ix >= e->w) return false;
        if (iz < 0 || iz >= e->h) return false;

        u32 cell = d->field_0x20[e->offset + ix * e->w + iz];
        if (cell != 0) {
            s32 odd = cell & 1;
            if (odd != 0) {
                // Declaration order drives saved-reg colors: n -> r30,
                // p -> r29, i -> r28.
                s32 n = d->field_0x24[cell];
                const u16* p = &d->field_0x24[cell];
                p++;
                s32 i = 0;
                while (i < n) {
                    if (func_804753B4__Q26mpfsys18MPFDrawDisplayListFv(self, &d->field_0x30[*p])) {
                        if (func_80476104__Q26mpfsys18MPFDrawDisplayListFv(self)) return true;
                    }
                    i++;
                    p++;
                }
                return false;
            }
            e = &cells[cell];
            continue;
        }
        return false;
    }
    return false;
}

// func_80476104: advance the walk position by one step.  The current slot's
// bounds are grown to include the position; when the slot's step counter
// exceeds its limit the position is moved to the embedded second slot, and
// when the slot list is exhausted a fresh slot is allocated from the arena.
//
// All state is reached through d->... on every use (no caching): retail
// reloads the walk position and the current slot around every call/store.
// Declaration order drives scratch colors to r27/r26 and the third clamp
// result stays in r0.
bool func_80476104__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self) {
    MPFDrawListLayout* d = (MPFDrawListLayout*)self;
    s32 n;
    s32 rnd;
    s32 off;
    s32 r;
    s32 g;
    s32 b;
    f32 height;

    d->field_0x14->field_0xf = (u8)d->field_0x3c;

    if (d->field_0x18->field_0x18 != 0) {
        // Jitter the interpolated vertex color by +/- n over each channel,
        // clamped to the u8 range.  rnd = 2n, off = 4n.
        n = d->field_0x18->field_0x18;
        rnd = n << 1;
        off = n << 2;
        r = d->field_0x14->r + (rand() % rnd) * 4 - off;
        g = d->field_0x14->g + (rand() % rnd) * 4 - off;
        b = d->field_0x14->b + (rand() % rnd) * 4 - off;
        if (r > 0xff) r = 0xff;
        else if (r < 0) r = 0;
        if (g > 0xff) g = 0xff;
        else if (g < 0) g = 0;
        if (b > 0xff) b = 0xff;
        else if (b < 0) b = 0;
        d->field_0x14->r = (u8)r;
        d->field_0x14->g = (u8)g;
        d->field_0x14->b = (u8)b;
    }

    // Grow the slot's Y bounds to include the walk position.
    if (d->field_0x50->field_0x18 >= d->field_0x14->y) d->field_0x50->field_0x18 = d->field_0x14->y;
    if (d->field_0x50->field_0x20 <= d->field_0x14->y) d->field_0x50->field_0x20 = d->field_0x14->y;

    d->field_0x50->field_0x28++;
    // Unsigned compare (cmplw).  if/else keeps the allocation path as the
    // fall-through and shares one `return false` exit with the slide path,
    // matching the retail branch layout.
    if ((u32)d->field_0x50->field_0x2a <= (u32)d->field_0x50->field_0x28) {
        // Slot exhausted: grow the buffer chain, then claim a fresh slot
        // carrying over the color, dimensions and height of the filled one.
        createSlotFromPool__Q26mpfsys18MPFDrawDisplayListFv(self, (MPFDrawListHdr*)d->field_0x50);
        height = d->field_0x50->field_0x24;
        // Reuses `off` (dead after the jitter block) to hold the carried
        // slot color across the allocation call - this is what pins it to
        // the same saved register (r29) as retail.
        off = d->field_0x50->field_0xc;
        MPFDrawSlot* slot = allocateDrawSlot__Q26mpfsys18MPFDrawDisplayListFv(
            self, 0x20, off, d->field_0x50->field_0xd + 1, d->field_0x50->field_0x10,
            d->field_0x50->field_0x12, d->field_0x50->field_0x14, d->field_0x50->field_0x1c);
        d->field_0x50 = slot;
        if (slot != NULL) {
            // Carry the walk position into the new slot's embedded position
            // area; x/z are read from the old position after the allocation.
            f32 x = d->field_0x14->x;
            f32 z = d->field_0x14->z;
            d->field_0x14 = (MPFDrawPos*)((u8*)slot + 0x2c);
            slot->field_0xc = (u8)off;
            d->field_0x50->field_0x24 = height;
            d->field_0x14->x = x;
            d->field_0x14->z = z;
        } else {
            return true;
        }
    } else {
        // Slot not full yet: slide to the embedded second position slot.
        d->field_0x14->field_0x10 = d->field_0x14->x;
        d->field_0x14->field_0x18 = d->field_0x14->z;
        d->field_0x14 = (MPFDrawPos*)((u8*)d->field_0x14 + 0x10);
    }
    return false;
}

// setPlainDrawNode: attach a node chain and hand it to the plain draw walker
// when the chain carries data.
void setPlainDrawNode__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawNode* node) {
    MPFDrawMgrLayout* d = (MPFDrawMgrLayout*)self;
    d->field_0x4 = node;
    if (node->field_0x4 != 0) {
        func_804795BC__Q26mpfsys18MPFDrawDisplayListFv(self, node);
    }
}

// setColorDrawNode: same as setPlainDrawNode but for the colored draw walker.
void setColorDrawNode__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawNode* node) {
    MPFDrawMgrLayout* d = (MPFDrawMgrLayout*)self;
    d->field_0x4 = node;
    if (node->field_0x4 != 0) {
        drawColoredList__Q26mpfsys18MPFDrawDisplayListFv(self, node);
    }
}

// allocateDrawSlot: scan the global slot chain for a free slot at/after the
// given 16-byte-unit position and fill it; otherwise record the best-fit
// insertion slot in self+0x54 and report failure.
MPFDrawSlot* allocateDrawSlot__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, u32 index, s32 v1, s32 v2, u16 v3, u16 v4, f32 f1, f32 f2) {
    MPFDrawMgrLayout* d = (MPFDrawMgrLayout*)self;
    u32 key;
    MPFDrawSlot* best;
    MPFDrawSlot* slot;
    u32 limit;

    d->field_0x54 = 0;
    limit = (index << 4) + 0x2c;
    slot = lbl_eu_80665870;
    while (slot != 0) {
        if ((slot->field_0xe & 0x2) == 0) {
            key = slot->field_0x0;
            if (key >= limit) {
                slot->field_0x28 = 0;
                slot->field_0xc = v1;
                slot->field_0xd = v2;
                slot->field_0x10 = v3;
                slot->field_0x12 = v4;
                slot->field_0x14 = f1;
                slot->field_0x1c = f2;
                slot->field_0x18 = lbl_eu_8066A7E0;
                slot->field_0x20 = lbl_eu_8066A7E4;
                slot->field_0x2a = index + ((slot->field_0x0 - limit) >> 4);
                return slot;
            }
            best = d->field_0x54;
            if (best == 0) {
                d->field_0x54 = slot;
            } else if (best->field_0x0 > key) {
                d->field_0x54 = slot;
            }
        }
        slot = slot->next;
    }
    return 0;
}

// checkProbeRange: probe test - is the probe vector (self+0x14) still inside
// the moving display range?  The VEC3Sub inline lowers to paired-single
// psq_l/ps_sub/psq_st so the x/z deltas are computed in one pass.
bool checkProbeRange__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self) {
    MPFDrawMgrLayout* d = (MPFDrawMgrLayout*)self;
    ml::CVec3 diff;
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff, (const nw4r::math::VEC3*)&d->field_0x14,
                       (const nw4r::math::VEC3*)&lbl_eu_80658410);
    f32 val = lbl_eu_8065841C.x * diff.x + lbl_eu_8065841C.z * diff.z;
    if (val <= lbl_eu_8066A7E8) {
        f32 d2 = diff.x * diff.x + diff.z * diff.z;
        f32 sel;
        if (d->field_0xe & 0x40) {
            sel = lbl_eu_8066A838;
        } else if (d->field_0xe & 0x20) {
            sel = lbl_eu_8066A840;
        } else {
            sel = lbl_eu_8066A824;
        }
        sel *= lbl_eu_8066A810;
        if (d2 > sel * sel) {
            return false;
        }
    }
    return true;
}

// Height/color walker: test the current position against the map triangle
// grid, then interpolate the height plane and vertex colors at the position.
bool func_804753B4__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, const MPFDrawEntry* e) {
    MPFDrawDisplayListLayout& d = *(MPFDrawDisplayListLayout*)self;
    const MPFDrawVert* v0;
    const MPFDrawVert* v1;
    const MPFDrawVert* v2;
    f32 v0x, v1x, v2x, v0y, v1y, v2y;
    f32 c01, c12, c20;
    MPFColor outSlot[2];
    MPFColor c0, c1, c2;
    MPFColor tmp;
    ml::CVec3 out;
    f32 yCross[2];
    s32 i;

    if (!(d.flags & e->mask)) return false;

    v0 = &d.verts[e->i0];
    v1 = &d.verts[e->i1];
    v2 = &d.verts[e->i1 + e->i2];

    v0x = v0->x - d.pos->x;
    v0y = v0->y - d.pos->z;
    v1x = v1->x - d.pos->x;
    v1y = v1->y - d.pos->z;
    v2x = v2->x - d.pos->x;
    v2y = v2->y - d.pos->z;

    // 2D cross products of (vertex - position); all must share a sign for the
    // position to lie inside (or on) the triangle.
    c01 = v0y * v1x - v0x * v1y;
    c12 = v1y * v2x - v1x * v2y;
    c20 = v2y * v0x - v2x * v0y;

    if (c01 * c12 < lbl_eu_8066A7E8 || c01 * c20 < lbl_eu_8066A7E8 || c20 * c12 < lbl_eu_8066A7E8) return false;

    // Height plane of the containing triangle (quantized s16 gradients via
    // the Gekko fast-cast, plus a full-precision offset).
    f32 h0, h1;
    OSs16tof32(&d.tris[e->tri].h0, &h0);
    OSs16tof32(&d.tris[e->tri].h1, &h1);
    f32 off = d.tris[e->tri].off;
    d.pos->y = h1 * v0y + (h0 * v0x + off);

    // Vertex colors as floats (pointer cached for the degenerate-copy paths).
    const MPFDrawCol* pc0 = &d.cols[e->c0];
    const MPFDrawCol* pc1 = &d.cols[e->c1];
    const MPFDrawCol* pc2 = &d.cols[e->c2];
    c0.b = u8ToF_a7f8(pc0->b);
    c0.g = u8ToF_a7f8(pc0->g);
    c0.r = u8ToF_a7f8(pc0->r);
    c1.b = u8ToF_a7f8(pc1->b);
    c1.g = u8ToF_a7f8(pc1->g);
    c1.r = u8ToF_a7f8(pc1->r);
    c2.b = u8ToF_a7f8(pc2->b);
    c2.g = u8ToF_a7f8(pc2->g);
    c2.r = u8ToF_a7f8(pc2->r);

    out = ml::CVec3::zero;
    i = 0;

    // The vertical line x = pos.x crosses edge (v0, v1) when v0x and v1x have
    // opposite signs.  Depending on which edge is crossed, interpolate either
    // along y (horizontal crossing -> final color) or along x (vertical
    // crossing -> intermediate color + crossing height, blended at the end).
    f32 p01 = v0x * v1x;
    if (p01 > lbl_eu_8066A7E8) goto check12;  // no x-crossing on edge (v0,v1)

    if (p01 >= lbl_eu_8066A7EC) {
        // horizontal-line crossing of edge (v0, v1): final color
        f32 a0 = ml::math::abs(v0y);
        f32 a1 = ml::math::abs(v1y);
        f32 s = a0 + a1;
        if (s != lbl_eu_8066A7E8) {
            f32 t = lbl_eu_8066A7F0 / s;
            nw4r::math::VEC3Scale((nw4r::math::VEC3*)&out, (const nw4r::math::VEC3*)&c0, lbl_eu_8066A7F0 - a0 * t);
            nw4r::math::VEC3Scale((nw4r::math::VEC3*)&tmp, (const nw4r::math::VEC3*)&c1, lbl_eu_8066A7F0 - a1 * t);
            nw4r::math::VEC3Add((nw4r::math::VEC3*)&out, (const nw4r::math::VEC3*)&out, (const nw4r::math::VEC3*)&tmp);
            d.pos->r = (u8)out.x;
            d.pos->g = (u8)out.y;
            d.pos->b = (u8)out.z;
            return true;
        }
        d.pos->r = pc0->r;
        d.pos->g = pc0->g;
        d.pos->b = pc0->b;
        return true;
    }

    // vertical-line crossing of edge (v0, v1)
    {
        f32 a0 = ml::math::abs(v0x);
        f32 a1 = ml::math::abs(v1x);
        f32 t = lbl_eu_8066A7F0 / (a0 + a1);
        f32 w0 = lbl_eu_8066A7F0 - a0 * t;
        f32 w1 = lbl_eu_8066A7F0 - a1 * t;
        nw4r::math::VEC3Scale((nw4r::math::VEC3*)&outSlot[i], (const nw4r::math::VEC3*)&c0, w0);
        nw4r::math::VEC3Scale((nw4r::math::VEC3*)&tmp, (const nw4r::math::VEC3*)&c1, w1);
        nw4r::math::VEC3Add((nw4r::math::VEC3*)&outSlot[i], (const nw4r::math::VEC3*)&outSlot[i], (const nw4r::math::VEC3*)&tmp);
        yCross[i] = v0->y + (a0 * t) * (v1->y - v0->y);
        i = 1;
    }

check12:
    {
        f32 p12 = v1x * v2x;
        if (p12 >= lbl_eu_8066A7E8) goto check_i;  // no x-crossing on edge (v1,v2)

        if (p12 >= lbl_eu_8066A7EC) {
            // horizontal-line crossing of edge (v1, v2): final color
            f32 a0 = ml::math::abs(v1y);
            f32 a1 = ml::math::abs(v2y);
            f32 s = a0 + a1;
            if (s != lbl_eu_8066A7E8) {
                f32 t = lbl_eu_8066A7F0 / s;
                nw4r::math::VEC3Scale((nw4r::math::VEC3*)&out, (const nw4r::math::VEC3*)&c1, lbl_eu_8066A7F0 - a0 * t);
                nw4r::math::VEC3Scale((nw4r::math::VEC3*)&tmp, (const nw4r::math::VEC3*)&c2, lbl_eu_8066A7F0 - a1 * t);
                nw4r::math::VEC3Add((nw4r::math::VEC3*)&out, (const nw4r::math::VEC3*)&out, (const nw4r::math::VEC3*)&tmp);
                d.pos->r = (u8)out.x;
                d.pos->g = (u8)out.y;
                d.pos->b = (u8)out.z;
                return true;
            }
            d.pos->r = pc1->r;
            d.pos->g = pc1->g;
            d.pos->b = pc1->b;
            return true;
        }

        // vertical-line crossing of edge (v1, v2)
        {
            f32 a0 = ml::math::abs(v1x);
            f32 a1 = ml::math::abs(v2x);
            f32 t = lbl_eu_8066A7F0 / (a0 + a1);
            f32 w0 = lbl_eu_8066A7F0 - a0 * t;
            f32 w1 = lbl_eu_8066A7F0 - a1 * t;
            nw4r::math::VEC3Scale((nw4r::math::VEC3*)&outSlot[i], (const nw4r::math::VEC3*)&c1, w0);
            nw4r::math::VEC3Scale((nw4r::math::VEC3*)&tmp, (const nw4r::math::VEC3*)&c2, w1);
            nw4r::math::VEC3Add((nw4r::math::VEC3*)&outSlot[i], (const nw4r::math::VEC3*)&outSlot[i], (const nw4r::math::VEC3*)&tmp);
            yCross[i] = v1->y + (a0 * t) * (v2->y - v1->y);
            i++;
        }
    }

check_i:
    if (i < 2) {
        f32 p12 = v1x * v2x;
        if (p12 <= lbl_eu_8066A7F4 && p12 >= lbl_eu_8066A7EC) {
            // degenerate (vertical line through a vertex): horizontal crossing
            // of edge (v0, v2)
            f32 a0 = ml::math::abs(v0y);
            f32 a2 = ml::math::abs(v2y);
            f32 s = a0 + a2;
            if (s != lbl_eu_8066A7E8) {
                f32 t = lbl_eu_8066A7F0 / s;
                nw4r::math::VEC3Scale((nw4r::math::VEC3*)&out, (const nw4r::math::VEC3*)&c2, lbl_eu_8066A7F0 - a2 * t);
                nw4r::math::VEC3Scale((nw4r::math::VEC3*)&tmp, (const nw4r::math::VEC3*)&c0, lbl_eu_8066A7F0 - a0 * t);
                nw4r::math::VEC3Add((nw4r::math::VEC3*)&out, (const nw4r::math::VEC3*)&out, (const nw4r::math::VEC3*)&tmp);
                d.pos->r = (u8)out.x;
                d.pos->g = (u8)out.y;
                d.pos->b = (u8)out.z;
                return true;
            }
            d.pos->r = pc2->r;
            d.pos->g = pc2->g;
            d.pos->b = pc2->b;
            return true;
        }
        // vertical-line crossing of edge (v0, v2)
        f32 a0 = ml::math::abs(v0x);
        f32 a2 = ml::math::abs(v2x);
        f32 t = lbl_eu_8066A7F0 / (a0 + a2);
        f32 w2 = lbl_eu_8066A7F0 - a2 * t;
        f32 w0 = lbl_eu_8066A7F0 - a0 * t;
        nw4r::math::VEC3Scale((nw4r::math::VEC3*)&outSlot[i], (const nw4r::math::VEC3*)&c2, w2);
        nw4r::math::VEC3Scale((nw4r::math::VEC3*)&tmp, (const nw4r::math::VEC3*)&c0, w0);
        nw4r::math::VEC3Add((nw4r::math::VEC3*)&outSlot[i], (const nw4r::math::VEC3*)&outSlot[i], (const nw4r::math::VEC3*)&tmp);
        yCross[i] = v2->y + (a2 * t) * (v0->y - v2->y);
        i++;
    }

    // Blend the two vertical-crossing colors by the position of pos.z between
    // their crossing heights.
    {
        f32 d0 = ml::math::abs(yCross[0] - d.pos->z);
        f32 d1 = ml::math::abs(yCross[1] - d.pos->z);
        f32 t = lbl_eu_8066A7F0 / (d0 + d1);
        nw4r::math::VEC3Scale((nw4r::math::VEC3*)&out, (const nw4r::math::VEC3*)&outSlot[0], lbl_eu_8066A7F0 - d0 * t);
        nw4r::math::VEC3Scale((nw4r::math::VEC3*)&tmp, (const nw4r::math::VEC3*)&outSlot[1], lbl_eu_8066A7F0 - d1 * t);
        nw4r::math::VEC3Add((nw4r::math::VEC3*)&out, (const nw4r::math::VEC3*)&out, (const nw4r::math::VEC3*)&tmp);
        d.pos->r = (u8)out.x;
        d.pos->g = (u8)out.y;
        d.pos->b = (u8)out.z;
    }
    return true;
}

// createSlotFromPool: allocate a 0x2c-byte slot from the draw-list buffer when at
// least 0x12c bytes remain, prepend it to the buffer's chain, then set the
// buffer's flag byte from the shared state global (bit 2 -> 0x20, bit 3 ->
// 0x40, base 2).
void createSlotFromPool__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawListHdr* hdr) {
    u32 off = (hdr->field_0x28 << 4) + 0x2c;
    if (hdr->field_0x0 >= off + 0x12c) {
        MPFDrawSlot* node = (MPFDrawSlot*)((u8*)hdr + off);
        memset(node, 0, 0x2c);
        node->field_0x0 = hdr->field_0x0 - off;
        if (hdr->field_0x4 != 0) {
            node->next = hdr->field_0x4;
        } else {
            node->next = 0;
        }
        hdr->field_0x4 = node;
        hdr->field_0x0 = off;
    }
    hdr->field_0xe = 2;
    if (lbl_eu_8066586C & 4) {
        hdr->field_0xe = hdr->field_0xe | 0x20;
    } else if (lbl_eu_8066586C & 8) {
        hdr->field_0xe = hdr->field_0xe | 0x40;
    }
}

// updateSlotChain: walk the global slot chain (lbl_eu_80665870).  Slots marked
// in-use (bit 0) are freed unless bit 3 is set or the self+0x3c gate is open
// (then the slot key is merged into the previous slot); free slots are
// re-marked in-use, with the bit-2 transition gated on the mask argument.
void updateSlotChain__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, u32 mask) {
    MPFDrawGateLayout* g = (MPFDrawGateLayout*)self;
    MPFDrawSlot* slot = lbl_eu_80665870;
    MPFDrawSlot* prev = 0;
    while (slot != 0) {
        u8 flags = slot->field_0xe;
        if (flags & 1) {
            if (flags & 4) {
                if (!(flags & 8) || g->field_0x3c != 0) {
                    slot->field_0xe = 0;
                    if (prev && !(prev->field_0xe & 2)) {
                        prev->next = slot->next;
                        prev->field_0x0 += slot->field_0x0;
                        slot = prev;
                    }
                } else {
                    slot->field_0xe = flags & 0xF7;
                }
            } else {
                if (flags & 2) {
                    if (mask & (1 << slot->field_0xc)) {
                        slot->field_0xe = flags & 0xF3;
                    } else {
                        slot->field_0xe = flags | 4;
                    }
                } else {
                    slot->field_0xe = flags | 4;
                }
            }
        } else {
            if (flags & 2) {
                slot->field_0xe = flags & 0xF3;
            }
            slot->field_0xe |= 1;
        }
        prev = slot;
        slot = slot->next;
    }
}

// func_804782C4: prepare a draw-list command.  Format 0xfc copies the whole
// 0x300-byte slot; otherwise the destination buffer is offset into the
// self+0x2c arena, the size is rounded up to a 32-byte boundary, the source
// is memcpy'd from the self+0x28 arena and the tail is zero-filled.
void func_804782C4__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawCmd* arg) {
    MPFDrawSrcLayout* s = (MPFDrawSrcLayout*)self;
    if (arg->field_0x4 == 0xfc) {
        u16 idx = arg->field_0x0;
        u32 sz = 0x300;
        u32 prod = idx * sz;
        u8* src = (u8*)s->field_0x28;
        arg->field_0xc = sz;
        arg->field_0x18 = src + prod;
    } else {
        if (arg->field_0x2 & 2) {
            // Parenthesised scaled term keeps MWCC's add operand order.
            arg->field_0x18 = (u8*)(s->field_0x2c + arg->field_0x0 * 0x600) + 0x300;        } else {
            arg->field_0x18 = (u8*)(s->field_0x2c + arg->field_0x0 * 0x600);
        }
        s32 n = (arg->field_0x4 + 1) * 3;
        if (n & 0x1f) {
            arg->field_0xc = n + (0x20 - (n & 0x1f));
        } else {
            arg->field_0xc = n;
        }
        memcpy(arg->field_0x18, (const void*)(s->field_0x28 + arg->field_0x0 * 0x300), arg->field_0xc);
        while (n < arg->field_0xc) {
            arg->field_0x18[n] = 0;
            n++;
        }
        arg->field_0x18[2] = (u8)arg->field_0x4;
    }
}

// func_804795BC: draw the plain (no vertex color) display list.  Walks the
// MPFDrawCfg entry array (0xcc0-byte stride) for the node chain attached at
// self+0x04, submitting each enabled entry's position/color arrays and its
// display list.
//
// Residual vs retail (17 mismatches @ last build): retail recomputes the
// entry base each iteration (add r24,r27,r31) while MWCC insistently turns
// this source into a loop-carried pointer (or-copy + +=1 step).  Recompute
// forms instead trigger strength reduction (cursor steals r31); declaration
// order here is required for the retail register map (off=r31 .. entry=r24).
void func_804795BC__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawNode* node) {
    // Declaration order drives MWCC's preserved-register assignment (first
    // declared takes the highest GPR), so the walk state is declared in
    // retail allocation order: off, posOff, clrOff, dataBase, entries,
    // count, i, entry.
    MPFDrawCfg* cfg = (MPFDrawCfg*)lbl_eu_80665874;
    MPFDrawNode* chain = ((MPFDrawMgrLayout*)self)->field_0x4;
    u8* base = lbl_eu_80665840;
    u32 off;
    u32 posOff;
    u32 clrOff;
    MPFDrawData* dataBase = (MPFDrawData*)(base + chain->field_0x0);
    MPFDispEntry* entries = cfg->field_0x10;
    s32 count;
    s32 i;
    MPFDispEntry* entry;

    if (cfg->field_0x8 & 1) {
        count = cfg->field_0x0;
    } else {
        if (!(cfg->field_0x8 & 4)) return;
        count = cfg->field_0x4;
        // Element-pointer scaling emits the plain mulli/add shape.
        entries += cfg->field_0xc;
    }
    if (count == 0) return;

    setupGfxMode5__Q26mpfsys17UnkClass_80471EC8Fv();
    disableAlphaBlend__Q26mpfsys17UnkClass_80471EC8Fv();
    disableZMode__Q26mpfsys17UnkClass_80471EC8Fv();

    off = 0;
    entry = entries;
    for (i = 0; i < count; i++) {
        posOff = off + 0x20;
        clrOff = posOff + 0xbe0;
        if (!(entry->field_0x2 & 2)) {
            // The per-item data address is recomputed at every use (matches retail).
            bindTexture__Q26mpfsys17UnkClass_80471EC8Fif(
                dataBase[entry->field_0x0].field_0x14, dataBase[entry->field_0x0].field_0x1c);
            setFogIndex__Q26mpfsys17UnkClass_80471EC8Fv(dataBase[entry->field_0x0].field_0x19);
            if (dataBase[entry->field_0x0].field_0x10 & 8) {
                applyAmbient__Q26mpfsys17UnkClass_80471EC8Fv();
            } else {
                resetAmbient__Q26mpfsys17UnkClass_80471EC8Fv();
            }
            GXSetArray(GX_VA_POS, (u8*)entries + posOff, 12);
            GXSetArray(GX_VA_CLR0, (u8*)entries + clrOff, 3);
            GXCallDisplayList(entry->field_0x18, entry->field_0xc);
        }
        off += sizeof(MPFDispEntry);
        entry += 1;
    }
}

// drawColoredList: draw the vertex-colored display list.  Same walk as
// func_804795BC but for entries with bit 1 set, with the color TEV alpha
// inputs reconfigured up front.
//
// Declaration order drives MWCC's preserved-register assignment (first
// declared takes the highest GPR); retail map is off=r31 .. entry=r24.
void drawColoredList__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawNode* node) {
    s32 flags;
    MPFDrawCfg* cfg = (MPFDrawCfg*)lbl_eu_80665874;
    MPFDrawNode* chain = ((MPFDrawMgrLayout*)self)->field_0x4;
    u8* base = lbl_eu_80665840;
    flags = cfg->field_0x8;
    u32 off;
    u32 posOff;
    u32 clrOff;
    MPFDrawData* dataBase = (MPFDrawData*)(base + chain->field_0x0);
    // Integer base keeps the per-iteration recompute as a plain add (MWCC
    // otherwise turns the pointer sum into a loop-carried cursor).
    u32 entriesAddr = (u32)cfg->field_0x10;
    s32 count;
    s32 i;

    if (flags & 1) {
        count = cfg->field_0x0;
    } else {
        if (!(flags & 4)) return;
        count = cfg->field_0x4;
        // Element scaling emits the plain mulli/add shape.
        entriesAddr += cfg->field_0xc * sizeof(MPFDispEntry);
    }
    if (count == 0) return;

    setupGfxMode5__Q26mpfsys17UnkClass_80471EC8Fv();
    enableAlphaBlend__Q26mpfsys17UnkClass_80471EC8Fv();
    enableZMode__Q26mpfsys17UnkClass_80471EC8Fv();
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);

    i = 0;
    off = 0;
    while (i < count) {
        MPFDispEntry* entry = (MPFDispEntry*)(entriesAddr + off);
        // Array offsets are formed before the enable test (matches retail).
        posOff = off + 0x20;
        clrOff = posOff + 0xbe0;
        if (entry->field_0x2 & 2) {
            // The per-item data address is recomputed at every use (matches retail).
            bindTexture__Q26mpfsys17UnkClass_80471EC8Fif(
                dataBase[entry->field_0x0].field_0x14, dataBase[entry->field_0x0].field_0x1c);
            setFogIndex__Q26mpfsys17UnkClass_80471EC8Fv(dataBase[entry->field_0x0].field_0x19);
            if (dataBase[entry->field_0x0].field_0x10 & 8) {
                applyAmbient__Q26mpfsys17UnkClass_80471EC8Fv();
            } else {
                resetAmbient__Q26mpfsys17UnkClass_80471EC8Fv();
            }
            GXSetArray(GX_VA_POS, (u8*)(entriesAddr + posOff), 12);
            GXSetArray(GX_VA_CLR0, (u8*)(entriesAddr + clrOff), 3);
            GXCallDisplayList(entry->field_0x18, entry->field_0xc);
        }
        off += sizeof(MPFDispEntry);
        i++;
    }
}

// func_80478C94: walk the slot chain (self+0x58), probe-test each slot's
// position against the moving display range, and emit per-layer display-list
// quads.  The retail Fv symbol actually receives (self, dst entries, entry
// counter, counter limit) - see the call site in func_80477F80.
//
// Each probe's proximity counter (MPFDrawProbe.counter) decays toward the
// band edges; the emitted quad corners are probe + arena[0..1] and
// probe + arena[i .. i+1] with the arena index derived from the probe colour
// sum, so consecutive quads tile the arena buffer.
void func_80478C94__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDispEntryFull* dst, s32* counter, s32 limit) {
    MPFDrawMgrLayout* d = (MPFDrawMgrLayout*)self;
    MPFDrawNode* node = d->field_0x4;
    s32 prevLayer = -1;
    // volatile: retail spills the item base to the stack (sp+0x3c) and
    // reloads it every outer iteration.
    volatile MPFBillItem* itemBase = (MPFBillItem*)(lbl_eu_80665840 + node->field_0x0);
    Vec* arena = (Vec*)func_804B5A68();
    s32 countB = 0;
    s32 countA = 8;
    Vec diff;
    Vec tmp;
    MPFDrawSlot* slot = ((MPFDrawListLayout*)self)->field_0x58;
    f32 f28 = lbl_eu_80665838->field_0x2DFC;
    f32 f27 = lbl_eu_80665838->field_0x2E00;
    MPFDispEntryFull* pendingA = 0;
    MPFDispEntryFull* pendingB = 0;
    f32 f31 = lbl_eu_8066A7E8;
    f32 f30 = lbl_eu_8066A7F0;
    f32 f20 = lbl_eu_8066A844;
    const nw4r::math::VEC3* ref = (const nw4r::math::VEC3*)&lbl_eu_80658410;
    const nw4r::math::VEC3* coeff = (const nw4r::math::VEC3*)&lbl_eu_8065841C;

    while (slot != 0) {
        if (checkProbeRange__Q26mpfsys18MPFDrawDisplayListFv((mpfsys::MPFDrawDisplayList*)slot)) {
            s32 layer = slot->field_0xc;
            MPFBillItem* item = (MPFBillItem*)&itemBase[layer]; 
            if (!(lbl_eu_80665864[item->field_0x34 >> 5] & (1u << (item->field_0x34 & 31)))) {
                f32 f1 = item->field_0x30;
                f32 f26;
                if (f28 > f1) {
                    f1 = f28;
                    f26 = f27;
                } else {
                    f26 = item->field_0x38;
                }
                f32 f0 = f1 - f26;
                f32 f24 = f1 * f1;
                f32 f23 = f26 * f26;
                f32 f25 = f30 / f0;

                if (prevLayer != layer) {
                    if (pendingA != 0) {
                        func_804782C4__Q26mpfsys18MPFDrawDisplayListFv(self, (MPFDrawCmd*)pendingA);
                        pendingA = 0;
                    }
                    if (pendingB != 0) {
                        func_804782C4__Q26mpfsys18MPFDrawDisplayListFv(self, (MPFDrawCmd*)pendingB);
                        pendingB = 0;
                    }
                    if (limit <= *counter && pendingA == 0 && pendingB == 0) {
                        return;
                    }
                    func_804783D0__Q26mpfsys18MPFDrawDisplayListFv((mpfsys::MPFDrawDisplayList*)item, arena, &countA, &countB);
                    prevLayer = layer;
                    u32 fv = item->flags;
                    u32 r22 = (fv & 1) ? 0 : 1;   // odd flag word selects the alternate entry
                    (void)r22;
                }

                // Inner walk over the slot's probe array (0x10-byte stride).
                f32 f22 = slot->field_0x24 * slot->field_0x24;
                MPFDrawProbe* probe = (MPFDrawProbe*)((u8*)slot + 0x2c);
                f32 f21 = item->field_0x2c;
                s32 k = 0;
                u16 probeCount = slot->field_0x28;
                while (k < probeCount) {
                    s32 r19;
                    MPFDispEntryFull* entry;
                    if (slot->field_0xe & 4) {
                        if (probe->counter != 0) {
                            nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff, (const nw4r::math::VEC3*)probe, ref);
                            f32 dot = nw4r::math::VEC3Dot(coeff, (const nw4r::math::VEC3*)&diff);
                            if (dot <= f31) {
                                probe->counter = 0;
                            } else {
                                f32 f5 = dot * lbl_eu_80665880;
                                f32 rad = f5 * lbl_eu_80665884 + f21;
                                nw4r::math::VEC3Scale((nw4r::math::VEC3*)&tmp, coeff, f5);
                                nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff, (const nw4r::math::VEC3*)&diff, (const nw4r::math::VEC3*)&tmp);
                                f32 d2 = nw4r::math::VEC3LenSq((const nw4r::math::VEC3*)&diff);
                                if (d2 <= rad * rad) {
                                    probe->counter = 0;
                                } else {
                                    probe->counter--;
                                    r19 = ((u8)probe->counter << 4) + (u8)probe->counter;
                                    slot->field_0xe |= 8;
                                    goto emit;
                                }
                            }
                        }
                        goto nextProbe;
                    } else {
                        nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff, (const nw4r::math::VEC3*)probe, ref);
                        f32 f29 = nw4r::math::VEC3LenSq((const nw4r::math::VEC3*)&diff);
                        if (f22 > f29) {
                            if (f21 < f29) {
                                f32 dot = nw4r::math::VEC3Dot(coeff, (const nw4r::math::VEC3*)&diff);
                                if (dot <= f31) {
                                    probe->counter = 15;
                                    goto nextProbe;
                                }
                                f32 f5 = dot * lbl_eu_80665880;
                                f32 rad = f5 * lbl_eu_80665884 + f21;
                                nw4r::math::VEC3Scale((nw4r::math::VEC3*)&tmp, coeff, f5);
                                nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff, (const nw4r::math::VEC3*)&diff, (const nw4r::math::VEC3*)&tmp);
                                f32 d2 = nw4r::math::VEC3LenSq((const nw4r::math::VEC3*)&diff);
                                if (d2 <= rad * rad) {
                                    probe->counter = 15;
                                    goto nextProbe;
                                }
                            }
                            // Inside the band: grow the counter, or compute the
                            // depth-shaded alpha from the ring distance.
                            if (probe->counter < 15) {
                                probe->counter++;
                                r19 = ((u8)probe->counter << 4) + (u8)probe->counter;
                            } else {
                                r19 = 0xff;
                            }
                            if (f29 >= f24) {
                                goto emit;
                            }
                            if (f29 <= f23) {
                                goto nextProbe;
                            }
                            // Guard the square root: warn and clamp when the
                            // squared distance went negative.
                            if (!(f29 >= f31)) {
                                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273,
                                                             lbl_eu_80526300);
                            }
                            f32 sq = f31;
                            if (!(f29 <= f31)) {
                                sq = f29 * nw4r::math::FrSqrt(f29);
                            }
                            f0 = sq;
                            r19 = (s32)(f20 * f25 * (f0 - f26));
                            goto emit;
                        } else {
                            // Outside the outer ring: decay the counter.
                            if (probe->counter != 0) {
                                f32 dot = nw4r::math::VEC3Dot(coeff, (const nw4r::math::VEC3*)&diff);
                                if (dot <= f31) {
                                    probe->counter = 0;
                                } else {
                                    f32 f5 = dot * lbl_eu_80665880;
                                    f32 rad = f5 * lbl_eu_80665884 + f21;
                                    nw4r::math::VEC3Scale((nw4r::math::VEC3*)&tmp, coeff, f5);
                                    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff, (const nw4r::math::VEC3*)&diff, (const nw4r::math::VEC3*)&tmp);
                                    f32 d2 = nw4r::math::VEC3LenSq((const nw4r::math::VEC3*)&diff);
                                    if (d2 <= rad * rad) {
                                        probe->counter = 0;
                                    } else {
                                        probe->counter--;
                                        r19 = ((u8)probe->counter << 4) + (u8)probe->counter;
                                        goto emit;
                                    }
                                }
                            }
                            goto nextProbe;
                        }
                    }

                emit:
                    // Acquire the pending entry (main or alternate) for this
                    // alpha class, then write one quad.
                    if (r19 == 0xff && (item->flags & 1) == 0) {
                        if (pendingA != 0) {
                            entry = pendingA;
                        } else {
                            if (limit <= *counter) {
                                if (pendingB != 0) {
                                    goto nextProbe;
                                }
                                return;
                            }
                            (*counter)++;
                            pendingA = dst;
                            dst->field_0x4 = 0;
                            dst->field_0x2 = 0;
                            dst->field_0x10 = (u32)dst->arrayA;
                            dst->field_0x14 = (u32)((u8*)dst + 0xc00);
                            dst->field_0x8 = 3;
                            dst->field_0x0 = (u16)layer;
                            dst++;
                            entry = pendingA;
                        }
                    } else {
                        if (pendingB != 0) {
                            entry = pendingB;
                        } else {
                            if (limit <= *counter) {
                                if (pendingA != 0) {
                                    goto nextProbe;
                                }
                                return;
                            }
                            (*counter)++;
                            pendingB = dst;
                            dst->field_0x4 = 0;
                            dst->field_0x10 = (u32)dst->arrayA;
                            dst->field_0x14 = (u32)((u8*)dst + 0xc00);
                            dst->field_0x8 = 3;
                            dst->field_0x0 = (u16)layer;
                            dst->field_0x2 = 2;
                            dst++;
                            entry = pendingB;
                        }
                    }

                    // Quad emission: colour-summed arena index + rotated corner
                    // adds, then the packed RGB colour word.
                    {
                        u8 pr = probe->r;
                        u8 pg = probe->g;
                        u8 pb = probe->b;
                        s32 idx = (pg + k) + (pb + pr);
                        u32 color = ((u32)(pr >> 2) << 26) | ((u32)pg << 18) | ((u32)pb << 12) | (((u32)r19 & 0xFFFFFF) << 6);
                        union { u32 w; u8 b[4]; } col;
                        s32 aidx = (countB & ((idx % countA) << 1)) + 2;
                        Vec* pos = (Vec*)entry->field_0x10;
                        if (idx & 1) {
                            nw4r::math::VEC3Add((nw4r::math::VEC3*)&pos[0], (const nw4r::math::VEC3*)probe, (const nw4r::math::VEC3*)&arena[1]);
                            nw4r::math::VEC3Add((nw4r::math::VEC3*)&pos[1], (const nw4r::math::VEC3*)probe, (const nw4r::math::VEC3*)&arena[aidx + 1]);
                            nw4r::math::VEC3Add((nw4r::math::VEC3*)&pos[2], (const nw4r::math::VEC3*)probe, (const nw4r::math::VEC3*)&arena[aidx]);
                            nw4r::math::VEC3Add((nw4r::math::VEC3*)&pos[3], (const nw4r::math::VEC3*)probe, (const nw4r::math::VEC3*)&arena[0]);
                        } else {
                            nw4r::math::VEC3Add((nw4r::math::VEC3*)&pos[0], (const nw4r::math::VEC3*)probe, (const nw4r::math::VEC3*)&arena[0]);
                            nw4r::math::VEC3Add((nw4r::math::VEC3*)&pos[1], (const nw4r::math::VEC3*)probe, (const nw4r::math::VEC3*)&arena[aidx]);
                            nw4r::math::VEC3Add((nw4r::math::VEC3*)&pos[2], (const nw4r::math::VEC3*)probe, (const nw4r::math::VEC3*)&arena[aidx + 1]);
                            nw4r::math::VEC3Add((nw4r::math::VEC3*)&pos[3], (const nw4r::math::VEC3*)probe, (const nw4r::math::VEC3*)&arena[1]);
                        }
                        entry->field_0x10 = (u32)(pos + 4);
                        col.w = color;
                        u8* cptr = (u8*)entry->field_0x14;
                        cptr[0] = col.b[0];
                        cptr[1] = col.b[1];
                        cptr[2] = col.b[2];
                        entry->field_0x14 = (u32)(cptr + 3);
                        entry->field_0x4 += 4;
                        entry->field_0x8 += 0xc;
                        if (entry->field_0x4 >= 0xfc) {
                            func_804782C4__Q26mpfsys18MPFDrawDisplayListFv(self, (MPFDrawCmd*)entry);
                            if (r19 == 0xff && (item->flags & 1) == 0) {
                                pendingA = 0;
                            } else {
                                pendingB = 0;
                            }
                        }
                    }
                nextProbe:
                    k++;
                    probe++;
                }
            }
        }
        slot = slot->next;
    }

    if (pendingA != 0) {
        func_804782C4__Q26mpfsys18MPFDrawDisplayListFv(self, (MPFDrawCmd*)pendingA);
    }
    if (pendingB != 0) {
        func_804782C4__Q26mpfsys18MPFDrawDisplayListFv(self, (MPFDrawCmd*)pendingB);
    }
}

// func_80476344: regenerate the display-list quads for the given map cell.
// The retail Fv symbol actually receives (self, cell index, density mode,
// scale) - see the call sites in func_80477F80.  When the cell aligns to the
// density spacing the cell index is advanced and the caller retries;
// otherwise a fresh slot is allocated for every in-range grid position and
// the walk is handed to func_80475E64.
bool func_80476344__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, s32 index, s32 mode, f32 scale) {
    MPFDrawListLayout* d = (MPFDrawListLayout*)self;
    MPFDrawGlobal* g = lbl_eu_80665838;
    f32* cell = (f32*)((u8*)g + 0x2d20 + index * 0xc);
    f32 oldx = cell[0];
    f32 oldz = cell[2];
    cell[0] = lbl_eu_80658410.x;
    s32 ix = (s32)lbl_eu_80658410.x;
    s32 iz = (s32)lbl_eu_80658410.z;
    s32 step = 1;
    s32 ox = (s32)oldx;
    s32 oz = (s32)oldz;
    cell[2] = lbl_eu_80658410.z;

    // Density-gate bits (2/3) in the shared TEV state, selected by mode.
    u32 flag = lbl_eu_8066586C & ~0xCu;
    lbl_eu_8066586C = flag;
    s32 span;
    s32 spacing;
    if (mode < 3) {
        lbl_eu_8066586C = flag | 8;
        span = mode + 1;
        step = 4;
        spacing = 0x28;
    } else if (mode < 6) {
        lbl_eu_8066586C = flag | 4;
        span = (mode - 2) * 2;
        step = 2;
        spacing = 0x14;
    } else {
        span = mode - 2;
        spacing = 0xa;
    }

    // If the reference position aligns to the density spacing on both axes
    // the cell index is advanced and this call bails out early.
    if (ix - (ix / spacing) * spacing == ox - (ox / spacing) * spacing &&
        iz - (iz / spacing) * spacing == oz - (oz / spacing) * spacing) {
        MPFDrawNode* node = d->field_0x4;
        s16 next = g->field_0x2E12 + 1;
        if ((u32)node->field_0x4 <= (u32)next) {
            next = 0;
        }
        g->field_0x2E12 = next;
        return true;
    }

    // Two alternating fast-cast slots (retail pre-stores both 0x43300000
    // headers once and reuses slot A/B for every int->float conversion).
    S32Cast cvtA;
    S32Cast cvtB;
    cvtA.w[0] = 0x43300000u;
    cvtB.w[0] = 0x43300000u;

    // Fast-cast the spacing and validate the probe disc; degenerate spacing
    // raises the retail diagnostic and the normalized radius falls back to 0.
    f32 f26 = s32ToF32_a808(spacing, &cvtA);
    f32 sq = f26 * f26;
    f32 dd = f26 * f26 + sq;
    if (dd < lbl_eu_8066A7E8) {
        Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
    }
    f32 norm = lbl_eu_8066A7E8;
    sq = f26 * f26;
    dd = f26 * f26 + sq;
    if (dd > lbl_eu_8066A7E8) {
        f32 inv = nw4r::math::FrSqrt(dd);
        sq = f26 * f26;
        norm = (f26 * f26 + sq) * inv;
    }
    f32 f27 = norm * lbl_eu_8066A810;

    s32 r22 = ((s32)(scale / f26) + 4) * 2;
    f32 f25 = (scale + f27) * (scale + f27);
    s32 r23 = span * span;
    s32 r7 = spacing * (r22 / 2);
    s32 r5 = ix / 10;
    s32 rz = iz / 10;
    s32 r6 = ix - r7;
    s32 r4 = iz - r7;
    s32 rhalf = (r22 * step) / 2;
    s32 r14 = r5 + 0x2710 - rhalf;
    s32 r25 = rz + 0x2710 - rhalf;
    f32 f24 = s32ToF32_a808(r6, &cvtB);
    f32 f23 = s32ToF32_a808(r4, &cvtA);

    // Bit array of grid positions claimed by surviving slots.
    u32* bits = (u32*)func_804B5A68();
    DCZeroRange((void*)bits, (((r22 * r22) >> 5) + 1) * 4);

    // Claim every in-range slot's grid position, then deactivate the slots
    // whose probe disc falls inside the refreshed radius.
    {
        const u32 one = 1;
        MPFDrawSlot* slot = lbl_eu_80665870;
        while (slot != 0) {
            if ((slot->field_0xe & 2) && slot->field_0xc == (u8)index) {
                f32 dx = slot->field_0x1c - lbl_eu_80658410.z;
                f32 dz = slot->field_0x14 - lbl_eu_80658410.x;
                f32 rr = slot->field_0x24 + f27;
                f32 d2 = dx * dx + dz * dz;
                if (d2 <= rr * rr) {
                    slot->field_0xe &= (u8)~one;
                }
                s32 bx = (r14 - slot->field_0x10) / step;
                s32 bz = (r25 - slot->field_0x12) / step;
                s32 bit = r22 * bx + bz;
                bits[bit >> 5] |= one << (bit & 31);
            }
            slot = slot->next;
        }
    }

    f32 f22 = f26 / s32ToF32_a808(span, &cvtB);
    f32 f21 = lbl_eu_8066A814 * f22 - f26;
    f32 f28 = lbl_eu_8066A814;
    f32 f30 = lbl_eu_8066A820;
    f32 f29 = lbl_eu_8066A81C;
    s32 r28 = 0;
    s32 r29 = 0;
    s32 r20 = 0;
    s32 r26 = 0;
    s32 rowOff = 0;

    for (s32 row = 0; row < r22; row++) {
        f32 f20 = s32ToF32_a808(row, &cvtA) * f26 + f24;
        f32 f19 = f28 * f26 + f20 - lbl_eu_80658410.x;
        s32 rowBase = r28;
        s32 vx = r14 + rowOff;
        for (s32 col = 0; col < r22; col++) {
            f32 f18 = s32ToF32_a808(col, &cvtB) * f26 + f23;
            f32 dz = f28 * f26 + f18 - lbl_eu_80658410.z;
            s32 bitOff = col + rowBase;
            if (bits[bitOff >> 5] & (1u << (bitOff & 31))) {
                goto nextCol;
            }
            if (f19 * f19 + dz * dz > f25) {
                goto nextCol;
            }
            {
                MPFDrawSlot* slot = allocateDrawSlot__Q26mpfsys18MPFDrawDisplayListFv(
                    self, r23, index, 0, (u16)vx, (u16)(r25 + r26), f20, f18);
                d->field_0x50 = slot;
                if (slot == 0) {
                    goto failReset;
                }
                d->field_0x14 = (MPFDrawPos*)((u8*)slot + 0x2c);
                slot->field_0x24 = scale;
                f32 f2 = f20 + f26;
                f32 f1 = f18 + f26;
                MPFDrawNode* node = d->field_0x4;
                s32 walked = 0;
                if (node->field_0x34 <= f2 && node->field_0x2c <= f20 &&
                    node->field_0x38 <= f1 && node->field_0x30 <= f18) {
                    d->field_0x40 = f20;
                    d->field_0x44 = f18;
                    d->field_0x48 = f2;
                    d->field_0x4C = f1;
                    walked = func_80475E64__Q26mpfsys18MPFDrawDisplayListFv(self, d->field_0x34);
                }
                if (walked) {
                    // Grid walk hit: scatter billboards over the fresh quad.
                    s32 kind = d->field_0x18->field_0x17;
                    if (kind == 1) {
                        // Jittered scatter: two draws per billboard, the jitter
                        // scaled by the span-normalized step.
                        f32 f31 = f30 * (f28 * f22);
                        for (s32 i = 0; i < r23; i++) {
                            s32 rv = rand();
                            f32 base = f22 * s32ToF32_a808(i % span, &cvtB) + f20 - f21;
                            d->field_0x14->x = f31 * s32ToF32_a808(rv % 100, &cvtA) + (base - f29 * f22);
                            s32 rv2 = rand();
                            f32 base2 = f22 * s32ToF32_a808(i % span, &cvtB) + f18 - f21;
                            d->field_0x14->z = f31 * s32ToF32_a808(rv2 % 100, &cvtA) + (base2 - f29 * f22);
                            if (func_80475C78__Q26mpfsys18MPFDrawDisplayListFv(self)) {
                                break;
                            }
                        }
                    } else if (kind == 2) {
                        // Uniform random scatter across the quad.
                        f32 f31 = f30 * f22;
                        for (s32 i = 0; i < r23; i++) {
                            d->field_0x14->x = f31 * s32ToF32_a808(rand() % (span * 100), &cvtA) + f20 - f21;
                            d->field_0x14->z = f31 * s32ToF32_a808(rand() % (span * 100), &cvtB) + f18 - f21;
                            if (func_80475C78__Q26mpfsys18MPFDrawDisplayListFv(self)) {
                                break;
                            }
                        }
                    } else {
                        // Deterministic sweep along the span diagonal.
                        for (s32 i = 0; i < r23; i++) {
                            d->field_0x14->x = f22 * s32ToF32_a808(i % span, &cvtA) + f20 - f21;
                            d->field_0x14->z = f22 * s32ToF32_a808(i / span, &cvtB) + i - f21;
                            if (func_80475C78__Q26mpfsys18MPFDrawDisplayListFv(self)) {
                                break;
                            }
                        }
                    }
                    if (d->field_0x50 == 0) {
                        goto failReset;
                    }
                    if (d->field_0x50->field_0x28 == 0 && d->field_0x54 != 0) {
                        MPFDrawSlot* cur = d->field_0x50;
                        MPFDrawSlot* best = d->field_0x54;
                        best->field_0x28 = (u16)r29;
                        best->field_0x2a = (u16)r29;
                        best->field_0xc = cur->field_0xc;
                        best->field_0xd = cur->field_0xd;
                        best->field_0x10 = cur->field_0x10;
                        best->field_0x12 = cur->field_0x12;
                        best->field_0x14 = cur->field_0x14;
                        best->field_0x1c = cur->field_0x1c;
                        best->field_0x18 = cur->field_0x18;
                        best->field_0x20 = cur->field_0x20;
                        best->field_0x24 = cur->field_0x24;
                        d->field_0x50 = best;
                    }
                    createSlotFromPool__Q26mpfsys18MPFDrawDisplayListFv(self, (MPFDrawListHdr*)d->field_0x50);
                } else {
                    // Walk missed: splice in the best-fit slot and advance it.
                    if (d->field_0x54 != 0) {
                        MPFDrawSlot* best = d->field_0x54;
                        best->field_0x28 = (u16)r29;
                        best->field_0x2a = (u16)r29;
                        best->field_0xc = slot->field_0xc;
                        best->field_0xd = slot->field_0xd;
                        best->field_0x10 = slot->field_0x10;
                        best->field_0x12 = slot->field_0x12;
                        best->field_0x14 = slot->field_0x14;
                        best->field_0x1c = slot->field_0x1c;
                        best->field_0x18 = slot->field_0x18;
                        best->field_0x20 = slot->field_0x20;
                        best->field_0x24 = slot->field_0x24;
                        d->field_0x50 = best;
                    }
                    createSlotFromPool__Q26mpfsys18MPFDrawDisplayListFv(self, (MPFDrawListHdr*)d->field_0x50);
                }
                r20++;
                if (r20 > 0x14 && d->field_0x3c == 0) {
                    goto failReset;
                }
            }
        nextCol:
            r26 += step;
        }
        r28 += r22;
        rowOff += step;
    }

failReset:
    cell[0] = lbl_eu_8066A818;
    cell[2] = lbl_eu_8066A818;
    {
        MPFDrawNode* node = d->field_0x4;
        s16 next = g->field_0x2E12 + 1;
        if ((u32)node->field_0x4 <= (u32)next) {
            next = 0;
        }
        g->field_0x2E12 = next;
    }
    return false;
}

// func_80476E50: regenerate the display-list quads for the given map cell,
// with an interpolated half-step refinement between the density rows.  The
// retail Fv symbol actually receives (self, cell index, density mode, scale)
// - see the call site in func_80477F80.
bool func_80476E50__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, s32 index, s32 mode, f32 scale) {
    MPFDrawListLayout* d = (MPFDrawListLayout*)self;
    MPFDrawGlobal* g = lbl_eu_80665838;
    // Two persistent int->float bit-cast slots (retail stores both 0x43300000
    // headers once in the prologue and only rewrites the low word).
    S32Cast cvtA;
    S32Cast cvtB;
    cvtA.w[0] = 0x43300000u;
    cvtB.w[0] = 0x43300000u;
    f32* cell = (f32*)((u8*)g + 0x2d20 + index * 0xc);
    f32 refx = lbl_eu_80658410.x;
    f32 refz = lbl_eu_80658410.z;
    f32 oldx = cell[0];
    f32 oldz = cell[2];
    cell[0] = refx;
    cell[2] = refz;
    s32 ix = (s32)refx;
    s32 iz = (s32)refz;
    s32 ox = (s32)oldx;
    s32 oz = (s32)oldz;
    f32 f18 = scale;

    f32 f27 = lbl_eu_8066A824;
    f32 f1 = lbl_eu_8066A828;
    f32 f30 = f1 * nw4r::math::FrSqrt(f1);
    f32 f22 = scale - lbl_eu_8066A82C;

    if (ix - ix % 10 == ox - ox % 10 && iz % 10 == oz % 10) {
        s16 next = g->field_0x2E12 + 1;
        if (next >= (s32)d->field_0x4->field_0x4) {
            next = 0;
        }
        g->field_0x2E12 = next;
        return true;
    }

    // Density step selection from the scale/fallback comparison.
    s32 r22;
    if (f22 <= lbl_eu_8066A7E8) {
        r22 = 2;
    } else if (f22 <= lbl_eu_8066A830) {
        r22 = (mode > 8) ? 4 : 3;
    } else if (f18 <= lbl_eu_8066A834) {
        r22 = (mode > 8) ? 5 : 4;
    } else {
        s32 v = mode ^ 6;
        r22 = 4 + (((v >> 1) - (v & mode)) < 0 ? 1 : 0);
    }

    // Probe spacing factor for the step-count rows.
    f32 f21 = lbl_eu_8066A7E8;
    if (r22 >= 3) {
        f21 = lbl_eu_8066A814 * s32ToF32_a808(r22, &cvtA) /
              s32ToF32_a808(r22 - 2, &cvtB);
    }

    s32 r23 = ((s32)(f18 / f27) + 1) * 2;
    s32 r24 = mode * mode;
    s32 r28 = mode * 100;
    f32 f15 = lbl_eu_8066A808;
    s32 r14b = ix - ix % 10;
    s32 r17b = iz - iz % 10;
    s32 r18b = ix % 10;
    s32 r19b = iz % 10;

    // Bit array of already-placed quads (size in bytes, rounded up to a word).
    u32* bits = (u32*)func_804B5A68();
    s32 bitBytes = (((r22 * r23 * r23) >> 5) + 1) * 4;
    DCZeroRange((void*)bits, bitBytes);
    // Scatter stride/count for the index chain below.
    s32 r29 = mode * (mode / 2 + 1) + 1;

    // Clear existing slots inside the new probe radius.
    {
        MPFDrawSlot* slot = lbl_eu_80665870;
        while (slot != 0) {
            if ((slot->field_0xe & 2) && slot->field_0xc == (u8)index) {
                s32 u = slot->field_0x10 - r14b;
                s32 v = slot->field_0x12 - r17b;
                if (u >= 0 && v >= 0 && u < r23 && v < r23) {
                    if (slot->field_0xe & 0x10) {
                        s32 n = r22;
                        for (s32 i = 0; i < n; i++) {
                            s32 bit = (v + i * r23) * r23 + u;
                            bits[bit >> 5] |= 1u << (bit & 31);
                        }
                    } else {
                        s32 bit = v * r23 + u;
                        bits[bit >> 5] |= 1u << (bit & 31);
                    }
                    // Probe the quad distance and clear the slot if inside.
                    f32 dx = slot->field_0x1c - lbl_eu_80658410.z;
                    f32 dz = slot->field_0x14 - lbl_eu_80658410.x;
                    f32 d2 = dx * dx + dz * dz;
                    f32 rr = slot->field_0x24 + f30;
                    if (d2 <= rr * rr) {
                        slot->field_0xe &= ~1u;
                    }
                }
            }
            slot = slot->next;
        }
    }

    // Grid walk.  Position bases mirror the retail stack-slot scheme:
    // slot40 = A814*A824 + (float)floor10(ix), f25 = A824/(rem) ramp bases,
    // xb/zb are the 10000-offset integer bases shared by both axes.
    f32 f16 = lbl_eu_8066A820;
    f32 f14 = lbl_eu_8066A81C;
    f32 f31 = lbl_eu_8066A814;
    f32 f40 = f31 * f27 + (f32)r14b;
    f32 f25 = f31 * f27 + (f32)(r17b - r23 / 2 * 10);
    f32 f26 = (f32)r17b;
    f32 f24 = f27 / (f32)(r17b - r23 / 2 * 10);
    f32 f23 = -(f31 * f24) + f27;
    s32 xb = r14b / 10 + 10000 - r23 / 2;
    s32 zb = r17b / 10 + 10000 - r23 / 2;
    s32 t0 = r24 >> 1;
    s32 placed = 0;

    for (s32 zz = 0; zz < r23; zz++) {
        cvtA.w[1] = (u32)zz ^ 0x80000000u;
        f32 f20 = (f32)(cvtA.d - f15) * f27 + f40;
        f32 f29 = f20 - (f32)r14b;
        s32 s60 = xb + zz;
        for (s32 xx = 0; xx < r23; xx++) {
            cvtB.w[1] = (u32)xx ^ 0x80000000u;
            f32 f19 = (f32)(cvtB.d - f15) * f27 + f25;
            f32 f28 = f19 - f26;
            s32 s5c = zb + xx;
            s32 r14c = 0;
            s32 r26 = 0;
            for (s32 s = 0; s < r22; s++) {
                s32 bit = (zz + r14c) * r23 + xx;
                if (bits[bit >> 5] & (1u << (bit & 31))) {
                    goto next_step;
                }
                // Interpolated position for this sub-step.
                f32 f17;
                if (s == 0) {
                    f17 = f18;
                } else if (s == r22 - 1) {
                    f17 = lbl_eu_8066A82C;
                } else {
                    cvtA.w[1] = (u32)(s - 1) ^ 0x80000000u;
                    f17 = -f31 * f22 + f18 + f21 * (f32)(cvtA.d - f15);
                }
                if (f29 * f29 + f28 * f28 > (f17 + f30) * (f17 + f30)) {
                    goto next_step;
                }
                {
                    // Row reduction: two halve-and-subtract chains over the
                    // squared density mode produce the quad index pair.
                    s32 rr;
                    s32 rv;
                    if (s == r22 - 1) {
                        rr = r24;
                        s32 t = t0;
                        for (s32 i = 1; i < r22; i++) {
                            t >>= 1;
                            rr -= t;
                        }
                        rv = r24 - rr;
                    } else {
                        rr = r24;
                        rv = 0;
                        for (s32 i = s; i < r22; i++) {
                            rr >>= 1;
                            rv += rr;
                        }
                        rv = rr + t0 - rv;
                    }
                    MPFDrawSlot* slot = allocateDrawSlot__Q26mpfsys18MPFDrawDisplayListFv(
                        self, rr, index, r26, (u16)s60,
                        (u16)s5c, f20, f19);
                    d->field_0x50 = slot;
                    if (slot == 0) {
                        cell[0] = lbl_eu_8066A818;
                        cell[2] = lbl_eu_8066A818;
                        return false;
                    }
                    if (s == 0) {
                        // Row start: bound the walk and hand it over.
                        f32 f2 = f20 + f27;
                        MPFDrawNode* node = d->field_0x4;
                        if (node->field_0x34 <= f2 && node->field_0x2c <= f20 &&
                            node->field_0x38 <= f19 && node->field_0x30 <= f19) {
                            d->field_0x40 = f20;
                            d->field_0x44 = f19;
                            d->field_0x48 = f2;
                            d->field_0x4C = f19;
                            if (func_80475E64__Q26mpfsys18MPFDrawDisplayListFv(self, d->field_0x34)) {
                                placed++;
                                if (placed > 0x14 && d->field_0x3c == 0) {
                                    cell[0] = lbl_eu_8066A818;
                                    cell[2] = lbl_eu_8066A818;
                                    return false;
                                }
                                goto next_step;
                            }
                        }
                        // Merge the best-fit slot and advance the walk.
                        if (d->field_0x54 != 0) {
                            MPFDrawSlot* best = d->field_0x54;
                            best->field_0x28 = 0;
                            best->field_0x2a = 0;
                            best->field_0xc = slot->field_0xc;
                            best->field_0xd = slot->field_0xd;
                            best->field_0x10 = slot->field_0x10;
                            best->field_0x12 = slot->field_0x12;
                            best->field_0x14 = slot->field_0x14;
                            best->field_0x1c = slot->field_0x1c;
                            best->field_0x18 = slot->field_0x18;
                            best->field_0x20 = slot->field_0x20;
                            best->field_0x24 = slot->field_0x24;
                            d->field_0x50 = best;
                        }
                        createSlotFromPool__Q26mpfsys18MPFDrawDisplayListFv(self, (MPFDrawListHdr*)d->field_0x50);
                        if (d->field_0x18->field_0x17 != 1 && d->field_0x18->field_0x17 != 2) {
                            // Deterministic sub-grid: index v picks a cell;
                            // its /mode and %mode parts give the z/x offsets.
                            s32 cnt = 0;
                            while (cnt < rr) {
                                s32 v = s60 + (cnt + rv) * r29;
                                s32 m = v % r24;
                                d->field_0x14->x = f24 * (m % mode) + f20 - f23;
                                d->field_0x14->z = f24 * (m / mode) + f19 - f23;
                                if (func_80475C78__Q26mpfsys18MPFDrawDisplayListFv(self)) {
                                    break;
                                }
                                cnt++;
                            }
                        } else if (d->field_0x18->field_0x17 == 1) {
                            // Jittered: deterministic base plus a
                            // rand()%100 term scaled by A820*A814*step.
                            f32 ramp = f16 * f31 * f24;
                            s32 cnt = 0;
                            while (cnt < rr) {
                                s32 v = s60 + (cnt + rv) * r29;
                                s32 m = v % r24;
                                s32 q = m % mode;
                                s32 rnd1 = rand() % 100;
                                d->field_0x14->x = f24 * q + f20 - f23 - f14 * f24 +
                                                   ramp * s32ToF_a808(rnd1);
                                s32 rnd2 = rand() % 100;
                                d->field_0x14->z = f24 * q + f19 - f23 - f14 * f24 +
                                                   ramp * s32ToF_a808(rnd2);
                                if (func_80475C78__Q26mpfsys18MPFDrawDisplayListFv(self)) {
                                    break;
                                }
                                cnt++;
                            }
                        } else {
                            // Fully random over mode*100 positions.
                            f32 ramp = f16 * f24;
                            s32 cnt = 0;
                            while (cnt < rr) {
                                s32 rnd1 = rand() % r28;
                                d->field_0x14->x = ramp * s32ToF_a808(rnd1) + f20 - f23;
                                s32 rnd2 = rand() % r28;
                                d->field_0x14->z = ramp * s32ToF_a808(rnd2) + f19 - f23;
                                if (func_80475C78__Q26mpfsys18MPFDrawDisplayListFv(self)) {
                                    break;
                                }
                                cnt++;
                            }
                        }
                        placed++;
                        if (placed > 0x14 && d->field_0x3c == 0) {
                            cell[0] = lbl_eu_8066A818;
                            cell[2] = lbl_eu_8066A818;
                            return false;
                        }
                    } else {
                        // Sub-step: just position and store the slot.
                        d->field_0x14 = (MPFDrawPos*)((u8*)slot + 0x2c);
                        slot->field_0x24 = f17;
                        createSlotFromPool__Q26mpfsys18MPFDrawDisplayListFv(self, (MPFDrawListHdr*)slot);
                    }
                }
            next_step: ;
            }
        }
    }

    cell[0] = lbl_eu_8066A818;
    cell[2] = lbl_eu_8066A818;
    s16 next = g->field_0x2E12 + 1;
    if (next >= (s32)d->field_0x4->field_0x4) {
        next = 0;
    }
    g->field_0x2E12 = next;
    return false;
}

// func_80475E64: recursively walk the cell grid region bounded by the
// current bounds (self+0x40..+0x4C).  The four corner coordinates are
// converted from the vertex-pair gradient form (same (bnd - v1)*v0 shape the
// walker uses) into integer grid ranges, then every grid cell in the range
// is tested: empty cells are skipped, odd values report a hit and even
// values recurse into the referenced sub-cell.
bool func_80475E64__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawCell* cell) {
    MPFDrawListLayout* d = (MPFDrawListLayout*)self;
    // Two alternating bit-cast slots (retail pre-stores both 0x43300000
    // headers up front, then reuses slot A for the min bounds and slot B
    // for the max bounds).
    S32Cast cvtA;
    S32Cast cvtB;
    cvtA.w[0] = 0x43300000u;
    cvtB.w[0] = 0x43300000u;
    if (!(cell->mask & d->field_0x38)) return false;

    const MPFDrawVert* v1 = &d->field_0x1C[cell->i1];
    const MPFDrawVert* v0 = &d->field_0x1C[cell->i0];
    u8 h = cell->h;

    // Grid X start from the min-X bound (floor with an inclusive-equal step).
    f32 fx = (d->field_0x40 - v1->x) * v0->x;
    s32 ix = (s32)fx;
    if (fx - s32ToF64_a808(ix, &cvtA) <= lbl_eu_8066A800) ix--;
    if (ix > cell->w - 1) return false;
    if (ix < 0) ix = 0;

    // Grid Z start from the min-Z bound.
    f32 fz = (d->field_0x44 - v1->y) * v0->y;
    s32 iz0 = (s32)fz;
    if (fz - s32ToF64_a808(iz0, &cvtB) <= lbl_eu_8066A800) iz0--;
    if (iz0 > h - 1) return false;
    if (iz0 < 0) iz0 = 0;

    // Grid X end from the max-X bound (rounded to nearest).
    f32 gx = (d->field_0x48 - v1->x) * v0->x;
    s32 ex = (s32)gx;
    if (gx - s32ToF64_a808(ex, &cvtA) >= lbl_eu_8066A804) ex++;
    if (ex < 0) return false;
    if (ex > cell->w - 1) ex = cell->w - 1;

    // Grid Z end from the max-Z bound.
    f32 gz = (d->field_0x4C - v1->y) * v0->y;
    s32 ez = (s32)gz;
    if (gz - s32ToF64_a808(ez, &cvtB) >= lbl_eu_8066A804) ez++;
    if (ez < 0) return false;
    if (ez > h - 1) ez = h - 1;

    s32 stride = ix * h;
    for (; ix <= ex; ix++) {
        for (s32 iz = iz0; iz <= ez; iz++) {
            u32 val = d->field_0x20[cell->offset + stride + iz];
            if (val == 0) continue;
            if (val & 1) return true;
            if (func_80475E64__Q26mpfsys18MPFDrawDisplayListFv(self, &d->field_0x34[val])) return true;
        }
        stride += h;
    }
    return false;
}

// func_804783D0: build the per-billboard display vectors.  The retail Fv
// symbol actually receives (item data, dst vectors, count out, flags out) -
// see the call site in func_80478C94.
//
// Bit 0 of item->flags (flags & 2) selects the two-vector billboard shape;
// otherwise a four-vector fan (spread == 0) or a 10-entry ramp is emitted.
// When item->spread is non-zero the y offset ramps with the iteration
// counter through the probe-range scale (lbl_eu_8066A810) and a spread
// scale (lbl_eu_8066A730 / lbl_eu_8066A83C).  Bit 1 (flags & 4) mirrors the
// emitted vectors' y components at the end.
void func_804783D0__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, Vec* dst, s32* countOut, s32* flagsOut) {
    MPFBillItem* item = (MPFBillItem*)self;
    Vec a;
    Vec b;
    // Single shared int->float bit-cast slot (retail reuses one stack slot
    // at sp+0x20 for every conversion in every path).
    S32Cast cvt;

    if (item->flags & 2) {
        // Two base vectors, transformed by the shared matrix, then by each
        // layer's draw matrix (func_804734F4 result).
        dst[0].y = lbl_eu_8066A7E8;
        dst[0].z = lbl_eu_8066A7E8;
        dst[1].y = lbl_eu_8066A7E8;
        dst[1].z = lbl_eu_8066A7E8;
        dst[0].x = -item->x;
        dst[1].x = item->x;
        PSMTXMultVec(lbl_eu_80658428, &dst[0], &dst[0]);
        PSMTXMultVec(lbl_eu_80658428, &dst[1], &dst[1]);

        MPFBillMtx* m = (MPFBillMtx*)getLayerRecord__Q26mpfsys17UnkClass_80471EC8FUc((mpfsys::UnkClass_80471EC8*)lbl_eu_80665838, item->layer);
        Vec* out = &dst[2];

        if (item->spread != lbl_eu_8066A7E8) {
            // Ramp the y offset with the iteration counter (Gekko fast-cast
            // int->float through a shared stack bit-cast slot).
            s32 count = lbl_eu_8066A728;
            for (s32 i = 0; i < count; i++) {
                f32 sp = item->spread;
                f32 t = lbl_eu_8066A810 * sp;
                cvt.w[0] = 0x43300000u;
                cvt.w[1] = (u32)i ^ 0x80000000u;
                f32 fi = (f32)(cvt.d - lbl_eu_8066A808);
                f32 k = item->y * (fi * t * lbl_eu_8066A730 + lbl_eu_8066A7F0 - sp);
                a.x = -item->x;
                a.y = k;
                a.z = lbl_eu_8066A7E8;
                b.x = item->x;
                b.y = k;
                b.z = lbl_eu_8066A7E8;
                PSMTXMultVec(lbl_eu_80658428, &a, &a);
                PSMTXMultVec(lbl_eu_80658428, &b, &b);
                PSMTXMultVec(m->mtx, &a, &out[0]);
                PSMTXMultVec(m->mtx, &b, &out[1]);
                out += 2;
                m++;
            }
            *countOut = count;
            if (item->flags & 4) {
                Vec* n = &dst[2];
                for (s32 j = 0; j < count; j++) {
                    n[0].y = -n[0].y;
                    n[1].y = -n[1].y;
                    n += 2;
                }
            }
        } else {
            // Fixed base pair, reused for every layer entry.
            a.x = -item->x;
            a.y = item->y;
            a.z = lbl_eu_8066A7E8;
            b.x = item->x;
            b.y = item->y;
            b.z = lbl_eu_8066A7E8;
            PSMTXMultVec(lbl_eu_80658428, &a, &a);
            PSMTXMultVec(lbl_eu_80658428, &b, &b);
            s32 count = lbl_eu_8066A728;
            for (s32 i = 0; i < count; i++) {
                PSMTXMultVec(m->mtx, &a, &out[0]);
                PSMTXMultVec(m->mtx, &b, &out[1]);
                out += 2;
                m++;
            }
            *countOut = count;
            if (item->flags & 4) {
                Vec* n = &dst[2];
                for (s32 j = 0; j < count; j++) {
                    n[0].y = -n[0].y;
                    n[1].y = -n[1].y;
                    n += 2;
                }
            }
        }
        *flagsOut = -1;
    } else {
        if (item->spread != lbl_eu_8066A7E8) {
            // Fixed 10-iteration ramp, transformed directly into the output.
            dst[0].y = lbl_eu_8066A7E8;
            dst[0].z = lbl_eu_8066A7E8;
            dst[1].y = lbl_eu_8066A7E8;
            dst[1].z = lbl_eu_8066A7E8;
            dst[0].x = -item->x;
            dst[1].x = item->x;
            PSMTXMultVec(lbl_eu_80658428, &dst[0], &dst[0]);
            PSMTXMultVec(lbl_eu_80658428, &dst[1], &dst[1]);
            Vec* out = &dst[2];
            for (s32 i = 0; i < 10; i++) {
                f32 sp = item->spread;
                f32 t = lbl_eu_8066A810 * sp;
                cvt.w[0] = 0x43300000u;
                cvt.w[1] = ((u32)(i % 10)) ^ 0x80000000u;
                f32 fi = (f32)(cvt.d - lbl_eu_8066A808);
                f32 k = item->y * (lbl_eu_8066A83C * (fi * t) + lbl_eu_8066A7F0 - sp);
                a.x = -item->x;
                a.y = k;
                a.z = lbl_eu_8066A7E8;
                b.x = item->x;
                b.y = k;
                b.z = lbl_eu_8066A7E8;
                PSMTXMultVec(lbl_eu_80658428, &a, &out[0]);
                PSMTXMultVec(lbl_eu_80658428, &b, &out[1]);
                out += 2;
            }
            *countOut = 10;
            *flagsOut = -1;
            if (item->flags & 4) {
                for (s32 j = 0; j < 20; j++) {
                    dst[2 + j].y = -dst[2 + j].y;
                }
            }
        } else {
            // Four-vector fan: two mirrored pairs, the second pair carrying
            // the y position.
            *flagsOut = 0;
            dst[0].y = lbl_eu_8066A7E8;
            dst[0].z = lbl_eu_8066A7E8;
            dst[1].y = lbl_eu_8066A7E8;
            dst[1].z = lbl_eu_8066A7E8;
            dst[2].y = item->y;
            dst[3].y = item->y;
            dst[2].z = lbl_eu_8066A7E8;
            dst[3].z = lbl_eu_8066A7E8;
            dst[0].x = -item->x;
            dst[1].x = item->x;
            dst[2].x = -item->x;
            dst[3].x = item->x;
            PSMTXMultVec(lbl_eu_80658428, &dst[0], &dst[0]);
            PSMTXMultVec(lbl_eu_80658428, &dst[1], &dst[1]);
            PSMTXMultVec(lbl_eu_80658428, &dst[2], &dst[2]);
            PSMTXMultVec(lbl_eu_80658428, &dst[3], &dst[3]);
            if (item->flags & 4) {
                dst[0].y = -dst[0].y;
                dst[1].y = -dst[1].y;
            }
        }
    }
}

// func_80477F80: per-frame billboard regeneration driver.  Chains the arena
// region bases off the attached draw node, then for each active map cell that
// has come due (round-robin index in the global state) regenerates its display
// list quads via func_80476E50 / func_80476344, collecting a completion mask.
// The mask drives the slot retirement walk (updateSlotChain), after which the
// slots are bucketed (collectActiveSlots) and the accumulated display-list buffers
// are flushed into the configured entry array and cache-stored.
//
// The retail Fv symbol actually receives (self, draw node, flagA, flagB).
void func_80477F80__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawNode* node,
                                                    s32 flagA, s32 flagB) {
    // Declarations in retail scratch-color order.
    s32 count;
    s32 i;
    s32 cellIdx;
    u32 doneMask;
    u32 k;
    MPFDrawListLayout* d = (MPFDrawListLayout*)self;

    d->field_0x4 = node;
    if (node->field_0x4 == 0) {
        return;
    }

    // Default single pass (retail hoists this into the chain schedule);
    // multi-cell mode opens the gate and iterates every cell instead.
    count = 1;
    // Each region sits at arena-base + its node offset; the base global is
    // re-read per statement (retail emits one SDA reference each).
    d->field_0x10 = (MPFDrawCol*)(lbl_eu_80665840 + node->field_0x14);
    d->field_0x1C = (MPFDrawVert*)(lbl_eu_80665840 + node->field_0x28);
    d->field_0x20 = (u32*)(lbl_eu_80665840 + node->field_0x20);
    d->field_0x24 = (u16*)(lbl_eu_80665840 + node->field_0x1C);
    d->field_0x30 = (MPFDrawEntry*)(lbl_eu_80665840 + node->field_0x8);
    d->field_0x34 = (MPFDrawCell*)(lbl_eu_80665840 + node->field_0x18);
    d->field_0x8 = (MPFDrawVert*)(lbl_eu_80665840 + node->field_0xC);
    d->field_0xC = (MPFDrawTri*)(lbl_eu_80665840 + node->field_0x10);
    d->field_0x18 = (MPFDrawItem*)(lbl_eu_80665840 + node->field_0x0);

    // Multi-cell mode: open the gate.
    if (flagA != 0) {
        d->field_0x3c = 0xf;
        count = node->field_0x4;
    } else {
        d->field_0x3c = 0;
    }

    f32 threshold = lbl_eu_8066A838;

    for (i = 0; i < count; i++) {
        cellIdx = lbl_eu_80665838->field_0x2E12;
        doneMask = 0;
        for (k = 0; k < (u32)((MPFDrawNode*)d->field_0x4)->field_0x4; k++) {
            // Cells outside the current round-robin slot are only touched in
            // multi-cell mode (mask bit alone).
            if (cellIdx == (s32)k || flagA != 0) {
                MPFDrawItem* item = d->field_0x18;
                if (lbl_eu_80665864[item->field_0x34 >> 5] & (1u << (item->field_0x34 & 31))) {
                    // Due for regeneration: advance the round-robin index.
                    s16 next = (s16)(lbl_eu_80665838->field_0x2E12 + 1);
                    if (next >= (s32)((MPFDrawNode*)d->field_0x4)->field_0x4) {
                        next = 0;
                    }
                    lbl_eu_80665838->field_0x2E12 = next;
                    doneMask |= 1u << k;
                } else if (flagB != 0) {
                    // Regenerate now; the density split picks the interpolated
                    // walker when the scatter mode is dense and the scale is
                    // under the fast-path threshold.
                    d->field_0x38 = 1u << k;
                    f32 scale = item->field_0x28;
                    bool ok;
                    if (item->field_0x16 >= 6 && scale < threshold) {
                        ok = func_80476E50__Q26mpfsys18MPFDrawDisplayListFv(self, k, item->field_0x16 - 2, scale);
                    } else {
                        ok = func_80476344__Q26mpfsys18MPFDrawDisplayListFv(self, k, item->field_0x16, scale);
                    }
                    if (ok) {
                        doneMask |= 1u << k;
                    }
                } else {
                    doneMask |= 1u << k;
                }
            }
            d->field_0x18 = (MPFDrawItem*)((u8*)d->field_0x18 + sizeof(MPFDrawItem));
        }
        updateSlotChain__Q26mpfsys18MPFDrawDisplayListFv(self, doneMask);
        collectActiveSlots__Q26mpfsys18MPFDrawDisplayListFv(self);
    }

    // Flush the accumulated quads into the configured display-list entry
    // array.  Bit 0 of the config flags selects appending after the entries
    // already written this frame (counter at cfg+4); otherwise the full array
    // is rewritten from scratch (counter at cfg+0) and the position/color
    // arena gets the larger 0x900-per-cell reservation.
    MPFDrawCfg* cfg = lbl_eu_80665874;
    u8* entries = (u8*)cfg->field_0x10;
    d->field_0x28 = (u32)((u8*)cfg + 0x20);
    u32 flags = cfg->field_0x8;
    if (flags & 1) {
        cfg->field_0x8 = (u16)((flags & ~0xFFFFu) | 4);
        cfg->field_0x4 = 0;
        entries += cfg->field_0xc * sizeof(MPFDispEntryFull);
        d->field_0x2C = d->field_0x28 + ((MPFDrawNode*)d->field_0x4)->field_0x4 * 0x300;
        func_80478C94__Q26mpfsys18MPFDrawDisplayListFv(self, (MPFDispEntryFull*)entries, &cfg->field_0x4,
                                                       cfg->field_0xc);
        DCStoreRange(entries, cfg->field_0x4 * 0xcc0);
    } else {
        cfg->field_0x8 = (u16)(flags | 3);
        cfg->field_0x0 = 0;
        d->field_0x2C = d->field_0x28 + ((MPFDrawNode*)d->field_0x4)->field_0x4 * 0x900;
        func_80478C94__Q26mpfsys18MPFDrawDisplayListFv(self, (MPFDispEntryFull*)entries, &cfg->field_0x0,
                                                       cfg->field_0xc);
        DCStoreRange(entries, cfg->field_0x0 * 0xcc0);
    }
    DCStoreRange((void*)d->field_0x2C, ((MPFDrawNode*)d->field_0x4)->field_0x4 * 0x600);
}

} // extern "C"
