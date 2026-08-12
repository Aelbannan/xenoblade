// Auto-scaffolded catalog TU for monolib/src/mpfsys/MPFDrawDisplayList
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/mpfsys/MPFDrawDisplayList.hpp"

#include <nw4r/math/math_types.h>
#include <revolution/os/OSFastCast.h>
#include <revolution/GX.h>
#include <monolib/math/CVec3.hpp>
#include <monolib/math/Utility.hpp>

#include <string.h>

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
    f32 x;
    f32 y;
    f32 z;
    u8 r;
    u8 g;
    u8 b;
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

// Draw-data chain handed to func_8047958C / func_804795A4 (retail r4).  The
// pointer is stored at self+0x04 and forwarded to the sibling draw walkers
// (func_804795BC / func_804796F0) when the chain carries data.
struct MPFDrawNode {
    u32 field_0x0;            // +0x00 arena offset of the batch data
    u32 field_0x4;            // +0x04 non-zero when the chain has draw data
};

// Global slot chain (head at lbl_eu_80665870) browsed by func_80475238.  The
// key at +0x00 is an arena offset expressed in 16-byte units with a 0x2c
// base; +0x0e bit 1 marks a slot as occupied.
struct MPFDrawSlot {
    u32 field_0x0;            // +0x00 key
    MPFDrawSlot* next;        // +0x04
    u8 gap08[4];              // +0x08
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
    u8 gap24[4];              // +0x24
    u16 field_0x28;           // +0x28
    u16 field_0x2a;           // +0x2a
};

// Layout used by the draw-list management functions (func_8047958C,
// func_804795A4, func_80475238, func_80478BDC).  Kept separate from
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

// self layout for func_80474FB0: only the +0x3c gate field is read.
struct MPFDrawGateLayout {
    u8 gap00[0x3c];           // +0x00
    u32 field_0x3c;           // +0x3c
};

// Draw-list buffer header used by func_804752EC.  Slots (0x2c bytes each)
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

// Singleton block (.bss, 0x60 bytes = 0x18 pointer slots): the first slot is
// the instance pointer returned by getInstance.  Declared at the full retail
// size so MWCC keeps absolute lis/addi addressing (small externs default to
// sda21).
extern mpfsys::MPFDrawDisplayList* lbl_eu_80658488[0x18];

// Prototype instance body defined by monolibdata2 (.data, 0x10 bytes).
struct MPFDrawProto {
    u8 body[0x10];                    // +0x00
};

// ---------------------------------------------------------------------------
// Imports (defined by other units / monolibdata2; global-scope names are not
// mangled by MWCC, so plain extern declarations emit the retail symbols).
// ---------------------------------------------------------------------------
extern s8 lbl_eu_80665888;                          // getInstance init flag (.sbss)
extern MPFDrawProto lbl_eu_8056DBA0;                // prototype instance (.data)
extern MPFDrawSlot* lbl_eu_80665870;                // global slot-chain head (.sbss)
extern ml::CVec3 lbl_eu_80658410;                   // probe reference vector (.bss)
extern ml::CVec3 lbl_eu_8065841C;                   // probe coefficient vector (.bss)
extern u32 lbl_eu_8066586C;                         // shared TEV/Z-mode flag state
// Draw-data arena base (MPFDrawMdlColor / UnkClass_80471EC8 declare the same).
extern u8* lbl_eu_80665840;
extern MPFDrawCfg* lbl_eu_80665874;                 // display-list walk config

namespace mpfsys {

MPFDrawDisplayList* MPFDrawDisplayList::getInstance() {
    if (!lbl_eu_80665888) {
        lbl_eu_80658488[0] = (MPFDrawDisplayList*)&lbl_eu_8056DBA0;
        lbl_eu_80665888 = 1;
    }
    return (MPFDrawDisplayList*)lbl_eu_80658488;
}

void MPFDrawDisplayList::func_8047509C() {}

void MPFDrawDisplayList::func_80475C78() {}

void MPFDrawDisplayList::func_80475E64() {}

void MPFDrawDisplayList::func_80476104() {}

void MPFDrawDisplayList::func_80476344() {}

void MPFDrawDisplayList::func_80476E50() {}

void MPFDrawDisplayList::func_80477F80() {}

void MPFDrawDisplayList::func_804783D0() {}

void MPFDrawDisplayList::func_80478C94() {}

void MPFDrawDisplayList::func_8047983C() {}

void MPFDrawDisplayList::func_80479840() {}

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
extern const f32 lbl_eu_8066A810;  // probe range scale
extern const f32 lbl_eu_8066A824;  // fallback probe radius
extern const f32 lbl_eu_8066A838;  // fast-path probe radius
extern const f32 lbl_eu_8066A840;  // mid-path probe radius

// TEV / texture pipeline helpers implemented by mpfsys::UnkClass_80471EC8.
// Retail mangles them as Fv/Fif members but they are invoked with the
// arguments below (see the sibling MPFDrawBillLayTex / MPFDrawMdlColor
// units).
void func_804737CC__Q26mpfsys17UnkClass_80471EC8Fif(s16 texIdx, f32 texScale);
void func_8047491C__Q26mpfsys17UnkClass_80471EC8Fv(void);
void func_80474A40__Q26mpfsys17UnkClass_80471EC8Fv(void);
void func_80474AA0__Q26mpfsys17UnkClass_80471EC8Fv(void);
void func_80474DF8__Q26mpfsys17UnkClass_80471EC8Fv(u8 texMapId);
void func_80474E24__Q26mpfsys17UnkClass_80471EC8Fv(void);
void func_80474E68__Q26mpfsys17UnkClass_80471EC8Fv(void);
void func_80474F2C__Q26mpfsys17UnkClass_80471EC8Fv(void);
void func_80474F54__Q26mpfsys17UnkClass_80471EC8Fv(void);

// Sibling draw walkers (retail Fv names; called with the node chain in r4).
void func_804795BC__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawNode* node);
void func_804796F0__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawNode* node);

// func_8047958C: attach a node chain and hand it to the plain draw walker
// when the chain carries data.
void func_8047958C__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawNode* node) {
    MPFDrawMgrLayout* d = (MPFDrawMgrLayout*)self;
    d->field_0x4 = node;
    if (node->field_0x4 != 0) {
        func_804795BC__Q26mpfsys18MPFDrawDisplayListFv(self, node);
    }
}

// func_804795A4: same as func_8047958C but for the colored draw walker.
void func_804795A4__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawNode* node) {
    MPFDrawMgrLayout* d = (MPFDrawMgrLayout*)self;
    d->field_0x4 = node;
    if (node->field_0x4 != 0) {
        func_804796F0__Q26mpfsys18MPFDrawDisplayListFv(self, node);
    }
}

// func_80475238: scan the global slot chain for a free slot at/after the
// given 16-byte-unit position and fill it; otherwise record the best-fit
// insertion slot in self+0x54 and report failure.
MPFDrawSlot* func_80475238__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, u32 index, u8 v1, u8 v2, u16 v3, u16 v4, f32 f1, f32 f2) {
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

// func_80478BDC: probe test - is the probe vector (self+0x14) still inside
// the moving display range?  The VEC3Sub inline lowers to paired-single
// psq_l/ps_sub/psq_st so the x/z deltas are computed in one pass.
bool func_80478BDC__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self) {
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
        f32 th = sel * lbl_eu_8066A810;
        if (d2 > th * th) {
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
    c0.b = (f32)pc0->b;
    c0.g = (f32)pc0->g;
    c0.r = (f32)pc0->r;
    c1.b = (f32)pc1->b;
    c1.g = (f32)pc1->g;
    c1.r = (f32)pc1->r;
    c2.b = (f32)pc2->b;
    c2.g = (f32)pc2->g;
    c2.r = (f32)pc2->r;

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

// func_804752EC: allocate a 0x2c-byte slot from the draw-list buffer when at
// least 0x12c bytes remain, prepend it to the buffer's chain, then set the
// buffer's flag byte from the shared state global (bit 2 -> 0x20, bit 3 ->
// 0x40, base 2).
void func_804752EC__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawListHdr* hdr) {
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

// func_80474FB0: walk the global slot chain (lbl_eu_80665870).  Slots marked
// in-use (bit 0) are freed unless bit 3 is set or the self+0x3c gate is open
// (then the slot key is merged into the previous slot); free slots are
// re-marked in-use, with the bit-2 transition gated on the mask argument.
void func_80474FB0__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, u32 mask) {
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
            u32 p2 = arg->field_0x0 * 0x600;
            u32 b = s->field_0x2c;
            u32 off = 0x300;
            arg->field_0x18 = (u8*)(p2 + b + off);
        } else {
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
void func_804795BC__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawNode* node) {
    MPFDrawCfg* cfg = (MPFDrawCfg*)lbl_eu_80665874;
    MPFDrawMgrLayout* mgr = (MPFDrawMgrLayout*)self;
    MPFDrawNode* chain = mgr->field_0x4;
    u16 flags = cfg->field_0x8;
    u8* base = lbl_eu_80665840;
    MPFDrawData* dataBase = (MPFDrawData*)(base + chain->field_0x0);
    MPFDispEntry* entries = cfg->field_0x10;
    s32 count;
    if (flags & 1) {
        count = cfg->field_0x0;
    } else if (flags & 4) {
        count = cfg->field_0x4;
        entries += cfg->field_0xc;
    } else {
        return;
    }
    if (count == 0) return;
    func_8047491C__Q26mpfsys17UnkClass_80471EC8Fv();
    func_80474AA0__Q26mpfsys17UnkClass_80471EC8Fv();
    func_80474F2C__Q26mpfsys17UnkClass_80471EC8Fv();
    for (s32 i = 0; i < count; i++) {
        MPFDispEntry* entry = &entries[i];
        u8* posArr = entry->arrayA;
        u8* clrArr = entry->arrayB;
        if (entry->field_0x2 & 2) continue;
        MPFDrawData* data = &dataBase[entry->field_0x0];
        func_804737CC__Q26mpfsys17UnkClass_80471EC8Fif(data->field_0x14, data->field_0x1c);
        func_80474DF8__Q26mpfsys17UnkClass_80471EC8Fv(data->field_0x19);
        if (data->field_0x10 & 8) {
            func_80474E68__Q26mpfsys17UnkClass_80471EC8Fv();
        } else {
            func_80474E24__Q26mpfsys17UnkClass_80471EC8Fv();
        }
        GXSetArray(GX_VA_POS, posArr, 12);
        GXSetArray(GX_VA_CLR0, clrArr, 3);
        GXCallDisplayList(entry->field_0x18, entry->field_0xc);
    }
}

// func_804796F0: draw the vertex-colored display list.  Same walk as
// func_804795BC but for entries with bit 1 set, with the color TEV alpha
// inputs reconfigured up front.
void func_804796F0__Q26mpfsys18MPFDrawDisplayListFv(mpfsys::MPFDrawDisplayList* self, MPFDrawNode* node) {
    MPFDrawCfg* cfg = (MPFDrawCfg*)lbl_eu_80665874;
    MPFDrawMgrLayout* mgr = (MPFDrawMgrLayout*)self;
    MPFDrawNode* chain = mgr->field_0x4;
    u16 flags = cfg->field_0x8;
    u8* base = lbl_eu_80665840;
    MPFDrawData* dataBase = (MPFDrawData*)(base + chain->field_0x0);
    MPFDispEntry* entries = cfg->field_0x10;
    s32 count;
    if (flags & 1) {
        count = cfg->field_0x0;
    } else if (flags & 4) {
        count = cfg->field_0x4;
        entries += cfg->field_0xc;
    } else {
        return;
    }
    if (count == 0) return;
    func_8047491C__Q26mpfsys17UnkClass_80471EC8Fv();
    func_80474A40__Q26mpfsys17UnkClass_80471EC8Fv();
    func_80474F54__Q26mpfsys17UnkClass_80471EC8Fv();
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    for (s32 i = 0; i < count; i++) {
        MPFDispEntry* entry = &entries[i];
        u8* posArr = entry->arrayA;
        u8* clrArr = entry->arrayB;
        if (!(entry->field_0x2 & 2)) continue;
        MPFDrawData* data = &dataBase[entry->field_0x0];
        func_804737CC__Q26mpfsys17UnkClass_80471EC8Fif(data->field_0x14, data->field_0x1c);
        func_80474DF8__Q26mpfsys17UnkClass_80471EC8Fv(data->field_0x19);
        if (data->field_0x10 & 8) {
            func_80474E68__Q26mpfsys17UnkClass_80471EC8Fv();
        } else {
            func_80474E24__Q26mpfsys17UnkClass_80471EC8Fv();
        }
        GXSetArray(GX_VA_POS, posArr, 12);
        GXSetArray(GX_VA_CLR0, clrArr, 3);
        GXCallDisplayList(entry->field_0x18, entry->field_0xc);
    }
}

} // extern "C"
