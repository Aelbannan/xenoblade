// Auto-scaffolded catalog TU for monolib/src/mpfsys/MPFDrawDisplayList
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/mpfsys/MPFDrawDisplayList.hpp"

#include <nw4r/math/math_types.h>
#include <revolution/os/OSFastCast.h>
#include <monolib/math/CVec3.hpp>
#include <monolib/math/Utility.hpp>

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

namespace mpfsys {

MPFDrawDisplayList* MPFDrawDisplayList::getInstance() { return nullptr; }

void MPFDrawDisplayList::func_80474FB0() {}

void MPFDrawDisplayList::func_8047509C() {}

void MPFDrawDisplayList::func_80475238() {}

void MPFDrawDisplayList::func_804752EC() {}

void MPFDrawDisplayList::func_80475C78() {}

void MPFDrawDisplayList::func_80475E64() {}

void MPFDrawDisplayList::func_80476104() {}

void MPFDrawDisplayList::func_80476344() {}

void MPFDrawDisplayList::func_80476E50() {}

void MPFDrawDisplayList::func_80477F80() {}

void MPFDrawDisplayList::func_804782C4() {}

void MPFDrawDisplayList::func_804783D0() {}

void MPFDrawDisplayList::func_80478BDC() {}

void MPFDrawDisplayList::func_80478C94() {}

void MPFDrawDisplayList::func_8047958C() {}

void MPFDrawDisplayList::func_804795A4() {}

void MPFDrawDisplayList::func_804795BC() {}

void MPFDrawDisplayList::func_804796F0() {}

void MPFDrawDisplayList::func_8047983C() {}

void MPFDrawDisplayList::func_80479840() {}

} // namespace mpfsys

// SDA2 float/double constants referenced by the retail body (pooled in the
// retail build; referencing the named symbols avoids TU-local pool labels).
extern "C" {

extern const f32 lbl_eu_8066A7E8;  // 0.0f
extern const f32 lbl_eu_8066A7EC;  // -0.001f
extern const f32 lbl_eu_8066A7F0;  // 1.0f
extern const f32 lbl_eu_8066A7F4;  // 0.001f

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

} // extern "C"
