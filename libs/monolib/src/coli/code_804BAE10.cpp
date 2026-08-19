// Auto-scaffolded catalog TU for monolib/src/coli/code_804BAE10
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// --- shared collision-query state ----------------------------------------
// The bss block at 0x8065F3F0 holds a world-space AABB pair (see
// func_804BAF34 / func_804BB2C0): min corner at lbl_eu_8065F3F0, max corner
// at lbl_eu_8065F3FC, half-extents at +0x18.  lbl_eu_80665960 is the query
// radius (sbss).
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};

extern Vec3 lbl_eu_8065F3F0;
extern Vec3 lbl_eu_8065F3FC;
extern f32 lbl_eu_80665960;
extern const f32 lbl_eu_8066AEF4;  // sdata2: slab max init
extern const f32 lbl_eu_8066AEF8;  // sdata2: slab min init
extern const f32 lbl_eu_8066AEFC;  // sdata2: degenerate-axis epsilon

// 12-byte triangle element inside the collision object's element array
// (layout matches func_804BB768 / func_804BB2C0).
struct ColiTri {
    u8 field_0x0;
    u8 field_0x1;
    u16 field_0x2;
    u16 field_0x4;
    u16 field_0x6;
    u32 field_0x8;
};

// Collision processing object (first arg of func_804BB768 / func_804BB2C0).
struct ColiObj {
    ColiTri* tris;    // 0x00 array of 12-byte elements
    Vec3* verts;      // 0x04 vertex array (12-byte stride)
    u16* indices;     // 0x08 index array
    ColiTri* tris2;   // 0x0C secondary triangle array
    u16* indices2;    // 0x10 secondary index array
};

// Collision walk helpers in the sibling coli unit (code_804B59C8): take the
// processing object plus a pointer into its u16 index array. C linkage so the
// bl relocs carry the retail names.
extern "C" void func_804B791C(ColiObj* self, const u16* list);
extern "C" void func_804B7ACC(ColiObj* self, const u16* list);
extern "C" void func_804B7B6C(ColiObj* self, const u16* list);

extern "C" void func_804BAE10(void* self) { *(u32*)self = 0; }

// Clamp the query box (a = max corner, b = min corner) to the shared world
// AABB origin per-axis, then test the squared distance of the clamped point
// against the query radius stored in lbl_eu_80665960.
bool func_804BAE1C(const Vec3* a, const Vec3* b) {
    Vec3 v;
    v.x = lbl_eu_8065F3F0.x;
    v.y = lbl_eu_8065F3F0.y;
    v.z = lbl_eu_8065F3F0.z;
    if (a->x < lbl_eu_8065F3F0.x) v.x = a->x;
    if (a->y < lbl_eu_8065F3F0.y) v.y = a->y;
    if (a->z < lbl_eu_8065F3F0.z) v.z = a->z;
    if (b->x > lbl_eu_8065F3F0.x) v.x = b->x;
    if (b->y > lbl_eu_8065F3F0.y) v.y = b->y;
    if (b->z > lbl_eu_8065F3F0.z) v.z = b->z;
    v.x -= lbl_eu_8065F3F0.x;
    v.y -= lbl_eu_8065F3F0.y;
    v.z -= lbl_eu_8065F3F0.z;
    return v.x * v.x + v.y * v.y + v.z * v.z < lbl_eu_80665960;
}

void func_804BAF34(){}

// Segment-vs-AABB slab test over the three axes.  Returns false as soon as
// the running [min,max] interval (initialized from the sdata2 constants)
// collapses; axes whose scale equals the epsilon constant fall back to a
// point-in-range check of the AABB corner.
bool func_804BB0C8(const f32* a, const f32* b) {
    const f32* minCorner = &lbl_eu_8065F3F0.x;  // r6
    const f32* scale = &lbl_eu_8065F3FC.x;      // r5
    f32 maxVal = lbl_eu_8066AEF4;
    f32 minVal = lbl_eu_8066AEF8;
    f32 eps = lbl_eu_8066AEFC;
    for (int i = 0; i < 3; i++) {
        f32 s = scale[i];
        if (s != eps) {
            f32 t1 = s * (b[i] - minCorner[i]);
            f32 t0 = s * (a[i] - minCorner[i]);
            if (t1 > t0) {
                if (t0 > minVal) minVal = t0;
                if (t1 < maxVal) maxVal = t1;
            } else {
                if (t1 > minVal) minVal = t1;
                if (t0 < maxVal) maxVal = t0;
            }
            if (minVal > maxVal) return false;
        } else {
            if (minCorner[i] < b[i]) return false;
            if (minCorner[i] > a[i]) return false;
        }
    }
    return true;
}

bool func_804BB1A0(const Vec3* a, const Vec3* b) {
    if (b->x <= lbl_eu_8065F3F0.x && lbl_eu_8065F3F0.x <= a->x &&
        b->z <= lbl_eu_8065F3F0.z && lbl_eu_8065F3F0.z <= a->z &&
        a->y >= lbl_eu_8065F3F0.y && lbl_eu_80665960 >= b->y) {
        return true;
    }
    return false;
}

bool func_804BB228(const Vec3* a, const Vec3* b) {
    if (b->x <= lbl_eu_8065F3F0.x && lbl_eu_8065F3FC.x <= a->x &&
        b->z <= lbl_eu_8065F3F0.z && lbl_eu_8065F3FC.z <= a->z &&
        b->y <= lbl_eu_8065F3F0.y && lbl_eu_8065F3FC.y <= a->y) {
        return true;
    }
    return false;
}

// Sphere-vs-triangle-tree walker. Clamp the query point (lbl_eu_8065F3F0)
// into the AABB spanned by the two vertex indices, test the squared distance
// against the query radius, then descend into the node's children (branch
// nodes) or iterate its child-triangle range (leaf nodes).
void func_804BB2C0(ColiObj* self, ColiTri* tri) {
    Vec3 v;
    v.x = lbl_eu_8065F3F0.x;
    v.y = lbl_eu_8065F3F0.y;
    v.z = lbl_eu_8065F3F0.z;
    const Vec3* vA = &self->verts[tri->field_0x4];
    const Vec3* vB = &self->verts[tri->field_0x6];
    if (vA->x < lbl_eu_8065F3F0.x) v.x = vA->x;
    if (vA->y < lbl_eu_8065F3F0.y) v.y = vA->y;
    if (vA->z < lbl_eu_8065F3F0.z) v.z = vA->z;
    if (vB->x > lbl_eu_8065F3F0.x) v.x = vB->x;
    if (vB->y > lbl_eu_8065F3F0.y) v.y = vB->y;
    if (vB->z > lbl_eu_8065F3F0.z) v.z = vB->z;
    f32 dx = v.x - lbl_eu_8065F3F0.x;
    f32 dy = v.y - lbl_eu_8065F3F0.y;
    f32 dz = v.z - lbl_eu_8065F3F0.z;
    if (dx * dx + dy * dy + dz * dz < lbl_eu_80665960) {
        if ((tri->field_0x0 & 6) != 0) {
            u32 idx;
            if ((tri->field_0x0 & 2) != 0) {
                func_804B791C(self, &self->indices[tri->field_0x8 + 1]);
                idx = tri->field_0x8 + self->indices[tri->field_0x8] + 1;
            } else {
                idx = tri->field_0x8;
            }
            if ((tri->field_0x0 & 4) != 0) {
                func_804B7ACC(self, &self->indices[idx + 1]);
            }
        } else {
            for (u32 i = tri->field_0x8; i < tri->field_0x8 + tri->field_0x1; i++) {
                func_804BB2C0(self, &self->tris[self->indices[i]]);
            }
        }
    }
}

void func_804BB4EC(){}

void func_804BB768(ColiObj* self, ColiTri* tri);

// Guard the not-yet-recovered stub so MWCC does not inline the empty body and
// drop the call sites (func_804BC134 tail-calls it; see MWCC_CASES
// empty-stub pattern).
#pragma push
#pragma auto_inline off
void func_804BB768(ColiObj* self, ColiTri* tri) {}
#pragma pop

void func_804BB904(){}

void func_804BBAB0(){}

// Record the query box (src) + squared radius, then dispatch to the
// sphere-vs-object walker with the triangle selected by idx. The box is
// copied as raw u32 words (retail lwz/stw, not float loads).
extern "C" void func_804BBFA0(ColiObj* self, const Vec3* src, int idx, f32 radius) {
    u32* dst = (u32*)&lbl_eu_8065F3F0;
    const u32* s = (const u32*)src;
    dst[0] = s[0];
    dst[1] = s[1];
    dst[2] = s[2];
    lbl_eu_80665960 = radius * radius;
    func_804BB2C0(self, &self->tris[idx]);
}

void func_804BBFD4(){}

// Record the query box (src) + radius, then dispatch to the box-vs-object
// walker with the triangle selected by idx. The box is copied as raw u32
// words (retail lwz/stw, not float loads).
extern "C" void func_804BC134(ColiObj* self, const Vec3* src, int idx, f32 radius) {
    u32* dst = (u32*)&lbl_eu_8065F3F0;
    const u32* s = (const u32*)src;
    dst[0] = s[0];
    dst[1] = s[1];
    dst[2] = s[2];
    lbl_eu_80665960 = radius;
    func_804BB768(self, &self->tris[idx]);
}

void func_804BC164(){}

void func_804BC494(){}

extern "C" u32 func_804BC9A0(u32* self) { return *(u32*)self != 0; }

void func_804BC9B4(int *dest, int offset, int *src) {
    dest[0] = offset + src[1];
    dest[1] = offset + src[2];
    dest[2] = offset + src[3];
}

extern "C" void func_804BC9DC(void* self, u32 a, u32 b) { *(u32*)((u8*)self + 0xC) = a; *(u32*)((u8*)self + 0x10) = b; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_804BC9E8(void) {}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.sbss] 0x80665960-0x80665968 (8 bytes)
f32 lbl_eu_80665960;  // zero-fill, word 0
f32 lbl_eu_80665960_hi;  // zero-fill, word 1 (synthetic)
