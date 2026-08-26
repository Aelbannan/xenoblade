// Auto-scaffolded catalog TU for monolib/src/coli/code_804BAE10
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/math/math_types.h>
#include <cmath>

// --- shared collision-query state ----------------------------------------
// The bss block at 0x8065F3F0 holds a world-space AABB pair (see
// func_804BAF34 / func_804BB2C0): min corner at lbl_eu_8065F3F0, max corner
// at lbl_eu_8065F3FC, half-extents at +0x18.  lbl_eu_80665960 is the query
// radius (sbss).
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
    // float-wise copy so struct copies compile to lfs/stfs triples
    Vec3() {}
    Vec3(const Vec3& o) : x(o.x), y(o.y), z(o.z) {}
};

extern Vec3 lbl_eu_8065F3F0;
extern Vec3 lbl_eu_8065F3FC;
extern f32 lbl_eu_80665960;
extern const f32 lbl_eu_8066AEF4;  // sdata2: slab max init
extern const f32 lbl_eu_8066AEF8;  // sdata2: slab min init
extern const f32 lbl_eu_8066AEFC;  // sdata2: degenerate-axis epsilon
// sdata2 sweep tables: entries consumed with a 12-byte stride; the additive
// base table sits 16 bytes into the same region and is indexed identically.
struct SweepTab {
    f32 scale;
    f32 field_0x4;
    f32 field_0x8;
};
extern const f32 lbl_eu_8066AEF0;  // sdata2: segment direction scale
extern const f32 lbl_eu_8066AF00;  // sdata2: sweep additive base
extern const f32 lbl_eu_8066AF04;  // sdata2: box-min init value
extern const f32 lbl_eu_8066AF08;  // sdata2: box-max init value
extern Vec3 lbl_eu_8065F408;       // box extents (lbl_eu_8065F3F0 + 0x18)

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

// Collision BVH node in the secondary triangle array (12-byte stride).
// field_0x8 selects the split axis (0-2) or 3 for a leaf.
struct ColiNode {
    u32 field_0x0;  // child/start index into indices2
    f32 field_0x4;  // split plane position
    u16 field_0x8;  // axis or 3 = leaf
};

// Collision processing object (first arg of func_804BB768 / func_804BB2C0).
struct ColiObj {
    ColiTri* tris;    // 0x00 array of 12-byte elements
    Vec3* verts;      // 0x04 vertex array (12-byte stride)
    u16* indices;     // 0x08 index array
    ColiNode* tris2;  // 0x0C secondary node array (12-byte stride)
    u16* indices2;    // 0x10 secondary index array
};


// Collision walk helpers in the sibling coli unit (code_804B59C8): take the
// processing object plus a pointer into its u16 index array. C linkage so the
// bl relocs carry the retail names.
extern "C" void func_804B791C(ColiObj* self, const u16* list, u16 value);
extern "C" void func_804B7ACC(ColiObj* self, const u16* list, int count);
extern "C" bool func_804B7B6C(ColiObj* self, const u16* list, int count);

extern "C" void func_804BAE10(void* self) { *(u32*)self = 0; }

// Clamp the query box (a = max corner, b = min corner) to the shared world
// AABB origin per-axis, then test the squared distance of the clamped point
// against the query radius stored in lbl_eu_80665960.
bool func_804BAE1C(const Vec3* a, const Vec3* b) {
    Vec3 v = lbl_eu_8065F3F0;
    if (a->x < lbl_eu_8065F3F0.x) v.x = a->x;
    if (a->y < lbl_eu_8065F3F0.y) v.y = a->y;
    if (a->z < lbl_eu_8065F3F0.z) v.z = a->z;
    if (b->x > lbl_eu_8065F3F0.x) v.x = b->x;
    if (b->y > lbl_eu_8065F3F0.y) v.y = b->y;
    if (b->z > lbl_eu_8065F3F0.z) v.z = b->z;
    // in-place subtract + dot via the SDK ps kernels (retail paired singles)
    nw4r::math::VEC3& vv = *reinterpret_cast<nw4r::math::VEC3*>(&v);
    const nw4r::math::VEC3& gv = *reinterpret_cast<const nw4r::math::VEC3*>(&lbl_eu_8065F3F0);
    nw4r::math::VEC3Sub(&vv, &vv, &gv);
    return nw4r::math::VEC3Dot(&vv, &vv) < lbl_eu_80665960;
}

// Shared query state: three adjacent VEC3s at lbl_eu_8065F3F0 (min corner,
// max corner, half-extents) - retail addresses them through one base pointer.
struct QueryState {
    nw4r::math::VEC3 min;
    nw4r::math::VEC3 max;
    nw4r::math::VEC3 ext;
};

// Segment-vs-AABB test helper (same culling shape as the first half of
// func_804BB4EC): shrink the segment [b -> a] toward its midpoint by the
// sdata2 scale, form D = query-box min corner - shrunken point, then reject
// if any axis slab test or edge-plane cross test fails.
bool func_804BAF34(const Vec3* a, const Vec3* b) {
    QueryState& qs = *reinterpret_cast<QueryState*>(&lbl_eu_8065F3F0);
    const nw4r::math::VEC3& va = *reinterpret_cast<const nw4r::math::VEC3*>(a);
    const nw4r::math::VEC3& vb = *reinterpret_cast<const nw4r::math::VEC3*>(b);

    nw4r::math::VEC3 mid;
    nw4r::math::VEC3 diff;
    nw4r::math::VEC3Sub(&diff, &va, &vb);
    // shrink toward b in place; retail reuses diff's stack slot for the result
    nw4r::math::VEC3Scale(&diff, &diff, lbl_eu_8066AEF0);
    nw4r::math::VEC3Add(&mid, &vb, &diff);

    // vector from the shrunken point to the box min corner
    f32 nx = qs.min.x - mid.x;
    f32 ny = qs.min.y - mid.y;
    f32 nz = qs.min.z - mid.z;
    if (__fabsf(nx) > diff.x + qs.ext.x) return false;
    if (__fabsf(ny) > diff.y + qs.ext.y) return false;
    if (__fabsf(nz) > diff.z + qs.ext.z) return false;
    // edge-plane tests against the box max corner
    if (__fabsf(nz * qs.max.y - ny * qs.max.z) > ny * qs.ext.z + nz * qs.ext.y)
        return false;
    if (__fabsf(nx * qs.max.z - nz * qs.max.x) > nx * qs.ext.z + nz * qs.ext.x)
        return false;
    if (__fabsf(ny * qs.max.x - nx * qs.max.y) > nx * qs.ext.y + ny * qs.ext.x)
        return false;
    return true;
}

// Segment-vs-AABB slab test over the three axes.  Returns false as soon as
// the running [min,max] interval (initialized from the sdata2 constants)
// collapses; axes whose scale equals the epsilon constant fall back to a
// point-in-range check of the AABB corner.
bool func_804BB0C8(const f32* a, const f32* b) {
    const f32* scale = &lbl_eu_8065F3FC.x;      // r5
    const f32* minCorner = &lbl_eu_8065F3F0.x;  // r6
    f32 s;
    f32 eps;
    f32 maxVal = lbl_eu_8066AEF4;
    f32 minVal = lbl_eu_8066AEF8;
    eps = lbl_eu_8066AEFC;
    for (int i = 3; i != 0; i--) {
        s = *scale;
        if (eps != s) {
            f32 tB = s * (*b - *minCorner);
            f32 tA = s * (*a - *minCorner);
            if (tB > tA) {
                if (tA > minVal) minVal = tA;
                if (tB < maxVal) maxVal = tB;
            } else {
                if (tB > minVal) minVal = tB;
                if (tA < maxVal) maxVal = tA;
            }
            if (minVal > maxVal) return false;
        } else {
            // degenerate axis: box must straddle the slab position
            if (*minCorner < *b || *minCorner > *a) return false;
        }
        ++scale;
        ++minCorner;
        ++b;
        ++a;
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
    // both vertex indices are read before any address math (retail lhz pair)
    const Vec3* vA = &self->verts[tri->field_0x4];
    const Vec3* vB = &self->verts[tri->field_0x6];
    Vec3 v;
    v.x = lbl_eu_8065F3F0.x;
    v.y = lbl_eu_8065F3F0.y;
    v.z = lbl_eu_8065F3F0.z;
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
                func_804B791C(self, &self->indices[tri->field_0x8 + 1],
                              self->indices[tri->field_0x8]);
                idx = tri->field_0x8 + self->indices[tri->field_0x8] + 1;
            } else {
                idx = tri->field_0x8;
            }
            if ((tri->field_0x0 & 4) != 0) {
                func_804B7ACC(self, &self->indices[idx + 1], self->indices[idx]);
            }
        } else {
            for (u32 i = tri->field_0x8; i < tri->field_0x8 + tri->field_0x1; i++) {
                func_804BB2C0(self, &self->tris[self->indices[i]]);
            }
        }
    }
}

// Segment-vs-triangle-tree walker. Scale the segment (vertex A toward
// vertex B), step from B by the scaled difference, and reject the node when
// the point escapes the box expanded by the extents or fails one of the
// three edge-plane tests (each compared against the two perpendicular
// extent axes); otherwise dispatch into children/leaves.
void func_804BB4EC(ColiObj* self, ColiTri* tri) {
    QueryState& qs = *reinterpret_cast<QueryState*>(&lbl_eu_8065F3F0);
    const nw4r::math::VEC3* vA =
        reinterpret_cast<const nw4r::math::VEC3*>(&self->verts[tri->field_0x4]);
    const nw4r::math::VEC3* vB =
        reinterpret_cast<const nw4r::math::VEC3*>(&self->verts[tri->field_0x6]);
    // scaled segment vector, then step point from vertex B
    nw4r::math::VEC3 d;
    nw4r::math::VEC3Sub(&d, vA, vB);
    nw4r::math::VEC3Scale(&d, &d, lbl_eu_8066AEF0);
    nw4r::math::VEC3 p;
    nw4r::math::VEC3Add(&p, vB, &d);
    const nw4r::math::VEC3& mn = qs.min;
    // vector from the moving point to the box min corner
    Vec3 n;
    n.x = mn.x - p.x;
    n.y = mn.y - p.y;
    n.z = mn.z - p.z;
    const nw4r::math::VEC3& ex = qs.ext;
    if (__fabsf(n.x) > d.x + ex.x) return;
    if (__fabsf(n.y) > d.y + ex.y) return;
    if (__fabsf(n.z) > d.z + ex.z) return;
    // edge-plane tests against the box max corner
    const nw4r::math::VEC3& mx = qs.max;
    f32 rhs1 = d.y * ex.z + d.z * ex.y;
    if (__fabsf(n.y * mx.z - n.z * mx.y) > rhs1)
        return;
    f32 rhs2 = d.x * ex.z + d.z * ex.x;
    if (__fabsf(n.x * mx.z - n.z * mx.x) > rhs2)
        return;
    f32 rhs3 = d.x * ex.y + d.y * ex.x;
    if (__fabsf(n.y * mx.x - n.x * mx.y) > rhs3)
        return;
    if ((tri->field_0x0 & 6) != 0) {
        u32 idx;
        if ((tri->field_0x0 & 2) != 0) {
            func_804B791C(self, &self->indices[tri->field_0x8 + 1],
                          self->indices[tri->field_0x8]);
            idx = tri->field_0x8 + self->indices[tri->field_0x8] + 1;
        } else {
            idx = tri->field_0x8;
        }
        if ((tri->field_0x0 & 4) != 0) {
            func_804B7ACC(self, &self->indices[idx + 1], self->indices[idx]);
        }
    } else {
        // bounds hoisted; signed ints so retail emits cmpw/blt
        int start = tri->field_0x8;
        int end = start + tri->field_0x1;
        for (int i = start; i < end; i++) {
            func_804BB4EC(self, &self->tris[self->indices[i]]);
        }
    }
}

// Box-vs-triangle-tree walker. Culls the subtree as soon as one of the
// node's bounding planes puts the query box entirely on the near side
// (vertex B below the min corner on x/z, vertex A above it, or the y span /
// radius test failing); otherwise dispatches into branch children or the
// leaf triangle range, mirroring func_804BB2C0.
extern "C" void func_804BB768(ColiObj* self, ColiTri* tri);

void func_804BB768(ColiObj* self, ColiTri* tri) {
    // vertex indices are cached; descend only when the query box lies
    // inside the node's bounds (x/z span contains min corner, y below A,
    // B above the query sphere)
    const Vec3* vA = &self->verts[tri->field_0x4];
    const Vec3* vB = &self->verts[tri->field_0x6];
    if (vB->x <= lbl_eu_8065F3F0.x && lbl_eu_8065F3F0.x <= vA->x &&
        vB->z <= lbl_eu_8065F3F0.z && lbl_eu_8065F3F0.z <= vA->z &&
        vA->y >= lbl_eu_8065F3F0.y && lbl_eu_80665960 >= vB->y) {
        if ((tri->field_0x0 & 6) != 0) {
            u32 idx;
            if ((tri->field_0x0 & 2) != 0) {
                func_804B791C(self, &self->indices[tri->field_0x8 + 1],
                              self->indices[tri->field_0x8]);
                idx = tri->field_0x8 + self->indices[tri->field_0x8] + 1;
            } else {
                idx = tri->field_0x8;
            }
            if ((tri->field_0x0 & 4) != 0) {
                func_804B7ACC(self, &self->indices[idx + 1], self->indices[idx]);
            }
        } else {
            int start = tri->field_0x8;
            int end = start + tri->field_0x1;
            for (int i = start; i < end; i++) {
                func_804BB768(self, &self->tris[self->indices[i]]);
            }
        }
    }
}

// Box-vs-triangle-tree walker. Culls the subtree as soon as the query box
// (lbl_eu_8065F3F0 min / lbl_eu_8065F3FC max) lies entirely on the near side
// of one of the node's bounding planes (vertex A far side / vertex B near
// side); otherwise dispatches into branch children or the leaf triangle
// range, mirroring func_804BB2C0.
// Retail emits this walker with C linkage (reloc name func_804BB904), so
// the recursive call site must reference the unmangled symbol.
extern "C" void func_804BB904(ColiObj* self, ColiTri* tri);

void func_804BB904(ColiObj* self, ColiTri* tri) {
    const Vec3* vA = &self->verts[tri->field_0x4];
    const Vec3* vB = &self->verts[tri->field_0x6];
    if (vB->x <= lbl_eu_8065F3F0.x && lbl_eu_8065F3FC.x <= vA->x &&
        vB->z <= lbl_eu_8065F3F0.z && lbl_eu_8065F3FC.z <= vA->z &&
        vB->y <= lbl_eu_8065F3F0.y && lbl_eu_8065F3FC.y <= vA->y) {
        if ((tri->field_0x0 & 6) != 0) {
            u32 idx;
            if ((tri->field_0x0 & 2) != 0) {
                func_804B791C(self, &self->indices[tri->field_0x8 + 1],
                              self->indices[tri->field_0x8]);
                idx = tri->field_0x8 + self->indices[tri->field_0x8] + 1;
            } else {
                idx = tri->field_0x8;
            }
            if ((tri->field_0x0 & 4) != 0) {
                func_804B7ACC(self, &self->indices[idx + 1], self->indices[idx]);
            }
        } else {
            // bounds hoisted: retail keeps start/end/2*i live across the loop
            // (signed ints: retail emits cmpw/blt)
            int start = tri->field_0x8;
            int end = start + tri->field_0x1;
            for (int i = start; i < end; i++) {
                func_804BB904(self, &self->tris[self->indices[i]]);
            }
        }
    }
}

// Axis-aligned BVH walker over the secondary arrays: split nodes test the
// query box (lbl_eu_8065F3F0 min / lbl_eu_8065F3FC max) against the node's
// split plane on its axis; if the box lies entirely on one side only that
// side's child is visited, otherwise the near child is visited first and the
// far child visited only when the near side reported no hit.
// Axis-aligned BVH walker over the secondary arrays: split nodes test the
// query box (lbl_eu_8065F3F0 min / lbl_eu_8065F3FC max) against the node's
// split plane on its axis; if the box lies entirely on one side only that
// side's child is visited, otherwise the near child is visited first and the
// far child visited only when the near side reported no hit.
// Retail one-level-inlines each direct self-call; the child visits below are
// therefore written out (deeper levels stay truly recursive).
extern "C" bool func_804BBAB0(ColiObj* self, ColiNode* node);

bool func_804BBAB0(ColiObj* self, ColiNode* node) {
    if (node->field_0x8 == 3) {
        return func_804B7B6C(self, &self->indices2[node->field_0x0 + 1],
                             self->indices2[node->field_0x0]);
    }
    u16 axis = node->field_0x8;
    f32 dmin = node->field_0x4 - (&lbl_eu_8065F3F0.x)[axis];
    f32 dmax = node->field_0x4 - (&lbl_eu_8065F3FC.x)[axis];
    // Written in retail layout order: straddle path falls through first with
    // plain forward branches; each direct self-call site is written out once
    // (retail one-level-inlines them); deeper levels stay truly recursive.
    bool hit;
    if (dmin * dmax < lbl_eu_8066AEFC) {
        // plane straddles the box: near child first (inlined), far child on miss
        ColiNode* nearChild = &self->tris2[self->indices2[node->field_0x0]];
        if (nearChild->field_0x8 == 3) {
            hit = func_804B7B6C(self, &self->indices2[nearChild->field_0x0 + 1],
                                self->indices2[nearChild->field_0x0]);
        } else {
            u16 nAxis = nearChild->field_0x8;
            f32 ndmin = nearChild->field_0x4 - (&lbl_eu_8065F3F0.x)[nAxis];
            f32 ndmax = nearChild->field_0x4 - (&lbl_eu_8065F3FC.x)[nAxis];
            if (ndmin * ndmax < lbl_eu_8066AEFC) {
                // gchild straddles its plane too
                if (func_804BBAB0(self, &self->tris2[self->indices2[nearChild->field_0x0]])) {
                    hit = true;
                } else {
                    hit = func_804BBAB0(self,
                                        &self->tris2[self->indices2[nearChild->field_0x0 + 1]]);
                }
            } else if (ndmin < lbl_eu_8066AEFC) {
                hit = func_804BBAB0(self, &self->tris2[self->indices2[nearChild->field_0x0]]);
            } else {
                hit = func_804BBAB0(self, &self->tris2[self->indices2[nearChild->field_0x0 + 1]]);
            }
        }
        if (hit) {
            return true;
        }
        // far child visit (inlined), result returned directly
        ColiNode* farChild = &self->tris2[self->indices2[node->field_0x0 + 1]];
        bool hit2;
        if (farChild->field_0x8 == 3) {
            hit2 = func_804B7B6C(self, &self->indices2[farChild->field_0x0 + 1],
                                 self->indices2[farChild->field_0x0]);
        } else {
            u16 fAxis = farChild->field_0x8;
            f32 fdmin = farChild->field_0x4 - (&lbl_eu_8065F3F0.x)[fAxis];
            f32 fdmax = farChild->field_0x4 - (&lbl_eu_8065F3FC.x)[fAxis];
            if (fdmin * fdmax < lbl_eu_8066AEFC) {
                // gchild straddles its plane too
                if (func_804BBAB0(self, &self->tris2[self->indices2[farChild->field_0x0]])) {
                    hit2 = true;
                } else {
                    hit2 = func_804BBAB0(self,
                                         &self->tris2[self->indices2[farChild->field_0x0 + 1]]);
                }
            } else if (fdmin < lbl_eu_8066AEFC) {
                hit2 = func_804BBAB0(self, &self->tris2[self->indices2[farChild->field_0x0]]);
            } else {
                hit2 = func_804BBAB0(self, &self->tris2[self->indices2[farChild->field_0x0 + 1]]);
            }
        }
        return hit2;
    }

    // box entirely on one side of the plane: descend into that side only
    // (each side written out - retail keeps two inline visit copies here)
    if (dmin >= lbl_eu_8066AEFC) {
        ColiNode* child = &self->tris2[self->indices2[node->field_0x0 + 1]];
        if (child->field_0x8 == 3) {
            hit = func_804B7B6C(self, &self->indices2[child->field_0x0 + 1],
                                self->indices2[child->field_0x0]);
        } else {
            u16 cAxis = child->field_0x8;
            f32 cdmin = child->field_0x4 - (&lbl_eu_8065F3F0.x)[cAxis];
            f32 cdmax = child->field_0x4 - (&lbl_eu_8065F3FC.x)[cAxis];
            if (cdmin * cdmax < lbl_eu_8066AEFC) {
                if (func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0]])) {
                    hit = true;
                } else {
                    hit = func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0 + 1]]);
                }
            } else if (cdmin < lbl_eu_8066AEFC) {
                hit = func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0]]);
            } else {
                hit = func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0 + 1]]);
            }
        }
        return hit;
    }
    ColiNode* child = &self->tris2[self->indices2[node->field_0x0]];
    if (child->field_0x8 == 3) {
        hit = func_804B7B6C(self, &self->indices2[child->field_0x0 + 1],
                            self->indices2[child->field_0x0]);
    } else {
        u16 cAxis = child->field_0x8;
        f32 cdmin = child->field_0x4 - (&lbl_eu_8065F3F0.x)[cAxis];
        f32 cdmax = child->field_0x4 - (&lbl_eu_8065F3FC.x)[cAxis];
        if (cdmin * cdmax < lbl_eu_8066AEFC) {
            if (func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0]])) {
                hit = true;
            } else {
                hit = func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0 + 1]]);
            }
        } else if (cdmin < lbl_eu_8066AEFC) {
            hit = func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0]]);
        } else {
            hit = func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0 + 1]]);
        }
    }
    return hit;
}

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

// Segment-sweep query setup: shrink the segment [a,b] toward its midpoint by
// the per-index scale factor, record the shrunken endpoints as the shared
// query min/max corners, expand the box extents by |max| + the per-index
// base, then descend into the segment-vs-tree walker.
void func_804BBFD4(ColiObj* self, const Vec3* a, const Vec3* b, int idx) {
    // paired-single vector kernels (VEC3Add/Sub/Scale) reproduce retail PS ops
    QueryState& qs = *reinterpret_cast<QueryState*>(&lbl_eu_8065F3F0);
    const nw4r::math::VEC3& va = *reinterpret_cast<const nw4r::math::VEC3*>(a);
    const nw4r::math::VEC3& vb = *reinterpret_cast<const nw4r::math::VEC3*>(b);

    nw4r::math::VEC3 t;
    nw4r::math::VEC3Add(&t, &va, &vb);
    // copy-constructed intermediate (retail keeps a float-wise copy here)
    Vec3 m(*reinterpret_cast<const Vec3*>(&t));

    f32 scale = lbl_eu_8066AEF0;
    f32 base = lbl_eu_8066AF00;

    nw4r::math::VEC3 n;
    nw4r::math::VEC3Scale(&n, reinterpret_cast<const nw4r::math::VEC3*>(&m), scale);
    // box corners are recorded as raw word copies (retail lwz/stw)
    qs.min = *reinterpret_cast<const nw4r::math::VEC3*>(&n);

    nw4r::math::VEC3 dt;
    nw4r::math::VEC3Sub(&dt, &vb, &qs.min);
    Vec3 d(*reinterpret_cast<const Vec3*>(&dt));
    qs.max = *reinterpret_cast<const nw4r::math::VEC3*>(&d);

    qs.ext.x = base + __fabsf(qs.max.x);
    qs.ext.y = base + __fabsf(qs.max.y);
    qs.ext.z = base + __fabsf(qs.max.z);
    func_804BB4EC(self, &self->tris[idx]);
}

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

// Oriented-box query setup: pts holds the six signed extent axes
// (+X,-X,+Y,-Y,+Z,-Z); each corner bit picks one vector from every pair,
// scales it and offsets by base. All 8 corners are materialized before the
// shared query AABB (seeded from the sdata2 slab constants) is folded,
// matching retail's schedule.
void func_804BC164(ColiObj* self, const Vec3* pts, const Vec3* base, f32 s0, f32 s1, f32 s2,
                   int idx) {
    // scale the three signed axis pairs (+X,-X / +Y,-Y / +Z,-Z) once, then
    // enumerate the 8 corners by accumulating one axis from each pair onto
    // the base point (partial sums shared across inner iterations)
    const nw4r::math::VEC3* p = reinterpret_cast<const nw4r::math::VEC3*>(pts);
    const nw4r::math::VEC3& b = *reinterpret_cast<const nw4r::math::VEC3*>(base);
    nw4r::math::VEC3 ax0, ax1, ay0, ay1, az0, az1;
    nw4r::math::VEC3 ta, tb;
    nw4r::math::VEC3 s00, s01, s10, s11;
    // scaled axes are formed lazily, interleaved with the partial sums
    nw4r::math::VEC3Scale(&ax0, &p[0], s1);
    nw4r::math::VEC3Add(&ta, &b, &ax0);
    nw4r::math::VEC3Scale(&ay0, &p[2], s0);
    nw4r::math::VEC3Add(&s00, &ta, &ay0);
    nw4r::math::VEC3Scale(&az0, &p[4], s2);
    nw4r::math::VEC3Scale(&az0, &p[4], s2);
    Vec3 corners[8];
    nw4r::math::VEC3* c = reinterpret_cast<nw4r::math::VEC3*>(corners);
    nw4r::math::VEC3Add(&c[0], &s00, &az0);
    nw4r::math::VEC3Scale(&ax1, &p[1], s1);
    nw4r::math::VEC3Add(&tb, &b, &ax1);
    nw4r::math::VEC3Add(&s10, &tb, &ay0);
    nw4r::math::VEC3Add(&c[1], &s10, &az0);
    nw4r::math::VEC3Scale(&ay1, &p[3], s0);
    nw4r::math::VEC3Add(&s01, &ta, &ay1);
    nw4r::math::VEC3Add(&c[2], &s01, &az0);
    nw4r::math::VEC3Add(&s11, &tb, &ay1);
    nw4r::math::VEC3Add(&c[3], &s11, &az0);
    nw4r::math::VEC3Scale(&az1, &p[5], s2);
    nw4r::math::VEC3Add(&c[4], &s00, &az1);
    nw4r::math::VEC3Add(&c[5], &s10, &az1);
    nw4r::math::VEC3Add(&c[6], &s01, &az1);
    nw4r::math::VEC3Add(&c[7], &s11, &az1);
    Vec3* mn = &lbl_eu_8065F3F0;
    Vec3* mx = &lbl_eu_8065F3FC;
    mn->x = mn->y = mn->z = lbl_eu_8066AF04;
    mx->x = mx->y = mx->z = lbl_eu_8066AF08;
    const volatile Vec3* cc = (const volatile Vec3*)corners;
    for (int i = 0; i < 8; i++) {
        if (cc->x < mn->x) mn->x = cc->x;
        if (cc->x > mx->x) mx->x = cc->x;
        if (cc->y < mn->y) mn->y = cc->y;
        if (cc->y > mx->y) mx->y = cc->y;
        if (cc->z < mn->z) mn->z = cc->z;
        if (cc->z > mx->z) mx->z = cc->z;
        ++cc;
    }
    func_804BB904(self, &self->tris[idx]);
}
// Box query entry: record the query box (arg2 -> min corner lbl_eu_8065F3F0,
// arg3 -> max corner lbl_eu_8065F3FC, copied as raw u32 words) and walk the
// secondary BVH rooted at self->tris2. The root level of the walk is written
// out (retail one-level-inlines func_804BBAB0); deeper levels recurse
// through func_804BBAB0.
bool func_804BC494(ColiObj* self, const Vec3* minSrc, const Vec3* maxSrc) {
    u32* dstMin = (u32*)&lbl_eu_8065F3F0;
    u32* dstMax = (u32*)&lbl_eu_8065F3FC;
    const u32* sMin = (const u32*)minSrc;
    const u32* sMax = (const u32*)maxSrc;
    *dstMin++ = *sMin++;
    *dstMax++ = *sMax++;
    *dstMin++ = *sMin++;
    *dstMin++ = *sMin++;
    *dstMax++ = *sMax++;
    *dstMax = *sMax;

    ColiNode* node = self->tris2;
    if (node->field_0x8 == 3) {
        u32 idx = node->field_0x0;
        return func_804B7B6C(self, &self->indices2[idx + 1], self->indices2[idx]);
    }
    f32 dmin = node->field_0x4 - (&lbl_eu_8065F3F0.x)[node->field_0x8];
    f32 dmax = node->field_0x4 - (&lbl_eu_8065F3FC.x)[node->field_0x8];
    bool hit;
    if (dmin * dmax < lbl_eu_8066AEFC) {
        // plane straddles the box: visit the near child first (inlined), then
        // the far child only when the near side reported no hit.
        ColiNode* nearChild = &self->tris2[self->indices2[node->field_0x0]];
        if (nearChild->field_0x8 == 3) {
            u32 idx = nearChild->field_0x0;
            return func_804B7B6C(self, &self->indices2[idx + 1], self->indices2[idx]);
        }
        f32 ndmin = nearChild->field_0x4 - (&lbl_eu_8065F3F0.x)[nearChild->field_0x8];
        f32 ndmax = nearChild->field_0x4 - (&lbl_eu_8065F3FC.x)[nearChild->field_0x8];
        if (ndmin * ndmax < lbl_eu_8066AEFC) {
            if (func_804BBAB0(self, &self->tris2[self->indices2[nearChild->field_0x0]])) {
                hit = true;
            } else {
                hit = func_804BBAB0(self,
                                    &self->tris2[self->indices2[nearChild->field_0x0 + 1]]);
            }
        } else if (ndmin < lbl_eu_8066AEFC) {
            return func_804BBAB0(self, &self->tris2[self->indices2[nearChild->field_0x0]]);
        } else {
            return func_804BBAB0(self, &self->tris2[self->indices2[nearChild->field_0x0 + 1]]);
        }
        if (hit) return true;

        ColiNode* farChild = &self->tris2[self->indices2[node->field_0x0 + 1]];
        if (farChild->field_0x8 == 3) {
            u32 idx = farChild->field_0x0;
            return func_804B7B6C(self, &self->indices2[idx + 1], self->indices2[idx]);
        }
        f32 fdmin = farChild->field_0x4 - (&lbl_eu_8065F3F0.x)[farChild->field_0x8];
        f32 fdmax = farChild->field_0x4 - (&lbl_eu_8065F3FC.x)[farChild->field_0x8];
        if (fdmin * fdmax < lbl_eu_8066AEFC) {
            if (func_804BBAB0(self, &self->tris2[self->indices2[farChild->field_0x0]])) {
                return true;
            }
            return func_804BBAB0(self, &self->tris2[self->indices2[farChild->field_0x0 + 1]]);
        }
        if (fdmin < lbl_eu_8066AEFC) {
            return func_804BBAB0(self, &self->tris2[self->indices2[farChild->field_0x0]]);
        }
        return func_804BBAB0(self, &self->tris2[self->indices2[farChild->field_0x0 + 1]]);
    }

    // box entirely on one side of the plane: descend into that side only.
    // Each side is written out with an inlined child visit (deeper levels
    // recurse through func_804BBAB0).
    ColiNode* child;
    if (dmin >= lbl_eu_8066AEFC) {
        child = &self->tris2[self->indices2[node->field_0x0 + 1]];
        if (child->field_0x8 == 3) {
            u32 idx = child->field_0x0;
            return func_804B7B6C(self, &self->indices2[idx + 1], self->indices2[idx]);
        }
        f32 cdmin = child->field_0x4 - (&lbl_eu_8065F3F0.x)[child->field_0x8];
        f32 cdmax = child->field_0x4 - (&lbl_eu_8065F3FC.x)[child->field_0x8];
        if (cdmin * cdmax < lbl_eu_8066AEFC) {
            if (func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0]])) {
                return true;
            }
            return func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0 + 1]]);
        }
        if (cdmin < lbl_eu_8066AEFC) {
            return func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0]]);
        }
        return func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0 + 1]]);
    }
    child = &self->tris2[self->indices2[node->field_0x0]];
    if (child->field_0x8 == 3) {
        u32 idx = child->field_0x0;
        return func_804B7B6C(self, &self->indices2[idx + 1], self->indices2[idx]);
    }
    f32 cdmin = child->field_0x4 - (&lbl_eu_8065F3F0.x)[child->field_0x8];
    f32 cdmax = child->field_0x4 - (&lbl_eu_8065F3FC.x)[child->field_0x8];
    if (cdmin * cdmax < lbl_eu_8066AEFC) {
        if (func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0]])) {
            return true;
        }
        return func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0 + 1]]);
    }
    if (cdmin < lbl_eu_8066AEFC) {
        return func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0]]);
    }
    return func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0 + 1]]);
}

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
