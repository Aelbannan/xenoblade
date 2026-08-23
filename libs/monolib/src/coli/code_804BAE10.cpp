// Auto-scaffolded catalog TU for monolib/src/coli/code_804BAE10
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
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
};

extern Vec3 lbl_eu_8065F3F0;
extern Vec3 lbl_eu_8065F3FC;
extern f32 lbl_eu_80665960;
extern const f32 lbl_eu_8066AEF4;  // sdata2: slab max init
extern const f32 lbl_eu_8066AEF8;  // sdata2: slab min init
extern const f32 lbl_eu_8066AEFC;  // sdata2: degenerate-axis epsilon
extern const f32 lbl_eu_8066AEF0;  // sdata2: segment direction scale
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
extern "C" void func_804B791C(ColiObj* self, const u16* list);
extern "C" void func_804B7ACC(ColiObj* self, const u16* list);
extern "C" bool func_804B7B6C(ColiObj* self, const u16* list);

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

// Segment-vs-triangle-tree walker. Project a scaled segment from vertex A
// toward vertex B, reject the node when the point escapes the box expanded by
// lbl_eu_8065F408 or fails one of the three edge plane tests, otherwise
// dispatch into children/leaves like func_804BB904.
void func_804BB4EC(ColiObj* self, ColiTri* tri) {
    const Vec3* vB = &self->verts[tri->field_0x6];
    const Vec3* vA = &self->verts[tri->field_0x4];
    f32 dx = (vB->x - vA->x) * lbl_eu_8066AEF0;
    f32 dy = (vB->y - vA->y) * lbl_eu_8066AEF0;
    f32 dz = (vB->z - vA->z) * lbl_eu_8066AEF0;
    f32 px = vA->x + dx;
    f32 py = vA->y + dy;
    f32 pz = vA->z + dz;
    // vector from the moving point to the box min corner
    f32 nx = lbl_eu_8065F3F0.x - px;
    f32 ny = lbl_eu_8065F3F0.y - py;
    f32 nz = lbl_eu_8065F3F0.z - pz;
    if (__fabsf(nx) > dx + lbl_eu_8065F408.x) return;
    if (__fabsf(ny) > dy + lbl_eu_8065F408.y) return;
    if (__fabsf(nz) > dz + lbl_eu_8065F408.z) return;
    // edge-plane tests against the max corner (lbl_eu_8065F3FC)
    if (__fabsf(nz * lbl_eu_8065F3FC.y - ny * lbl_eu_8065F3FC.z) >
        dy * lbl_eu_8065F3FC.z + dz * lbl_eu_8065F408.y)
        return;
    if (__fabsf(nx * lbl_eu_8065F3FC.z - nz * lbl_eu_8065F3F0.y) >
        dx * lbl_eu_8065F3FC.z + dz * lbl_eu_8065F408.z)
        return;
    if (__fabsf(ny * lbl_eu_8065F3F0.y - nx * lbl_eu_8065F3FC.y) >
        dx * lbl_eu_8065F408.y + dy * lbl_eu_8065F408.z)
        return;
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
            func_804BB4EC(self, &self->tris[self->indices[i]]);
        }
    }
}

void func_804BB768(ColiObj* self, ColiTri* tri);

// Guard the not-yet-recovered stub so MWCC does not inline the empty body and
// drop the call sites (func_804BC134 tail-calls it; see MWCC_CASES
// empty-stub pattern).
#pragma push
#pragma auto_inline off
void func_804BB768(ColiObj* self, ColiTri* tri) {}
#pragma pop

// Box-vs-triangle-tree walker. Culls the subtree as soon as the query box
// (lbl_eu_8065F3F0 min / lbl_eu_8065F3FC max) lies entirely on the near side
// of one of the node's bounding planes (vertex A far side / vertex B near
// side); otherwise dispatches into branch children or the leaf triangle
// range, mirroring func_804BB2C0.
void func_804BB904(ColiObj* self, ColiTri* tri) {
    const Vec3* vB = &self->verts[tri->field_0x6];
    const Vec3* vA = &self->verts[tri->field_0x4];
    if (vB->x <= lbl_eu_8065F3F0.x && lbl_eu_8065F3FC.x <= vA->x &&
        vB->z <= lbl_eu_8065F3F0.z && lbl_eu_8065F3FC.z <= vA->z &&
        vB->y <= lbl_eu_8065F3F0.y && lbl_eu_8065F3FC.y <= vA->y) {
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
extern "C" bool func_804BBAB0(ColiObj* self, ColiNode* node) {
    if (node->field_0x8 == 3) {
        return func_804B7B6C(self, &self->indices2[node->field_0x0 + 1]);
    }
    const f32* minBox = &lbl_eu_8065F3F0.x;
    const f32* maxBox = &lbl_eu_8065F3FC.x;
    f32 dmin = node->field_0x4 - minBox[node->field_0x8];
    f32 dmax = node->field_0x4 - maxBox[node->field_0x8];
    if (dmin * dmax >= lbl_eu_8066AEFC) {
        // box entirely on one side of the plane: descend into that side only.
        // The child visit is written out inline (matches retail's one-level
        // self-inlining); its own children go through real recursive calls.
        ColiNode* child;
        if (dmin >= lbl_eu_8066AEFC) {
            child = &self->tris2[self->indices2[node->field_0x0 + 1]];
        } else {
            child = &self->tris2[self->indices2[node->field_0x0]];
        }
        bool hit;
        if (child->field_0x8 == 3) {
            hit = func_804B7B6C(self, &self->indices2[child->field_0x0 + 1]);
        } else {
            f32 cdmin = child->field_0x4 - minBox[child->field_0x8];
            f32 cdmax = child->field_0x4 - maxBox[child->field_0x8];
            if (cdmin * cdmax >= lbl_eu_8066AEFC) {
                if (cdmin >= lbl_eu_8066AEFC) {
                    hit = func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0 + 1]]);
                } else {
                    hit = func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0]]);
                }
            } else {
                hit = false;
                if (cdmax < lbl_eu_8066AEFC) {
                    hit = func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0]]);
                }
                if (!hit) {
                    hit = func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0 + 1]]);
                }
            }
        }
        return hit;
    }
    // near side: visit the near child first (inlined), then the far child
    ColiNode* nearChild = &self->tris2[self->indices2[node->field_0x0]];
    bool hit;
    if (nearChild->field_0x8 == 3) {
        hit = func_804B7B6C(self, &self->indices2[nearChild->field_0x0 + 1]);
    } else {
        f32 ndmin = nearChild->field_0x4 - minBox[nearChild->field_0x8];
        f32 ndmax = nearChild->field_0x4 - maxBox[nearChild->field_0x8];
        if (ndmin * ndmax >= lbl_eu_8066AEFC) {
            if (ndmin >= lbl_eu_8066AEFC) {
                hit = func_804BBAB0(self, &self->tris2[self->indices2[nearChild->field_0x0 + 1]]);
            } else {
                hit = func_804BBAB0(self, &self->tris2[self->indices2[nearChild->field_0x0]]);
            }
        } else {
            hit = false;
            if (ndmax < lbl_eu_8066AEFC) {
                hit = func_804BBAB0(self, &self->tris2[self->indices2[nearChild->field_0x0]]);
            }
            if (!hit) {
                hit = func_804BBAB0(self, &self->tris2[self->indices2[nearChild->field_0x0 + 1]]);
            }
        }
    }
    if (hit) return true;
    ColiNode* farChild = &self->tris2[self->indices2[node->field_0x0 + 1]];
    bool hit2;
    if (farChild->field_0x8 == 3) {
        hit2 = func_804B7B6C(self, &self->indices2[farChild->field_0x0 + 1]);
    } else {
        f32 fdmin = farChild->field_0x4 - minBox[farChild->field_0x8];
        f32 fdmax = farChild->field_0x4 - maxBox[farChild->field_0x8];
        if (fdmin * fdmax >= lbl_eu_8066AEFC) {
            if (fdmin >= lbl_eu_8066AEFC) {
                hit2 = func_804BBAB0(self, &self->tris2[self->indices2[farChild->field_0x0 + 1]]);
            } else {
                hit2 = func_804BBAB0(self, &self->tris2[self->indices2[farChild->field_0x0]]);
            }
        } else {
            hit2 = false;
            if (fdmax < lbl_eu_8066AEFC) {
                hit2 = func_804BBAB0(self, &self->tris2[self->indices2[farChild->field_0x0]]);
            }
            if (!hit2) {
                hit2 = func_804BBAB0(self, &self->tris2[self->indices2[farChild->field_0x0 + 1]]);
            }
        }
    }
    return hit2;
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

// Oriented-box query setup: accumulate the 8 transformed corner candidates
// (axis pairs scaled by the three float args plus base offsets), track the
// running candidate box initialized from the sdata2 constants, merge it into
// the shared query AABB (lbl_eu_8065F3F0 / lbl_eu_8065F3FC), then descend
// into the box-vs-tree walker at tri index idx.
void func_804BC164(ColiObj* self, const Vec3* pts, const Vec3* base, f32 s0, f32 s1, f32 s2,
                   int idx) {
    Vec3 lo;
    Vec3 hi;
    lo.x = lbl_eu_8066AF04;
    lo.y = lbl_eu_8066AF04;
    lo.z = lbl_eu_8066AF04;
    hi.x = lbl_eu_8066AF08;
    hi.y = lbl_eu_8066AF08;
    hi.z = lbl_eu_8066AF08;
    for (int i = 0; i < 8; i++) {
        f32 x = pts[i].x * s0 + base->x;
        f32 y = pts[i].y * s1 + base->y;
        f32 z = pts[i].z * s2 + base->z;
        if (x < lo.x) lo.x = x;
        if (y < lo.y) lo.y = y;
        if (z < lo.z) lo.z = z;
        if (x > hi.x) hi.x = x;
        if (y > hi.y) hi.y = y;
        if (z > hi.z) hi.z = z;
    }
    if (lo.x < lbl_eu_8065F3F0.x) lbl_eu_8065F3F0.x = lo.x;
    if (lo.y < lbl_eu_8065F3F0.y) lbl_eu_8065F3F0.y = lo.y;
    if (lo.z < lbl_eu_8065F3F0.z) lbl_eu_8065F3F0.z = lo.z;
    if (lbl_eu_8065F3FC.x > hi.x) lbl_eu_8065F3FC.x = hi.x;
    if (lbl_eu_8065F3FC.y > hi.y) lbl_eu_8065F3FC.y = hi.y;
    if (lbl_eu_8065F3FC.z > hi.z) lbl_eu_8065F3FC.z = hi.z;
    func_804BB904(self, &self->tris[idx]);
}
// Box query entry: record the query box (arg2 -> min corner lbl_eu_8065F3F0,
// arg3 -> max corner lbl_eu_8065F3FC, copied as raw u32 words) and walk the
// secondary BVH rooted at self->tris2. The root level of the walk is written
// out inline (retail one-level-inlines func_804BBAB0); deeper levels recurse
// through func_804BBAB0.
bool func_804BC494(ColiObj* self, const Vec3* minSrc, const Vec3* maxSrc) {
    u32* dstMin = (u32*)&lbl_eu_8065F3F0;
    u32* dstMax = (u32*)&lbl_eu_8065F3FC;
    const u32* sMin = (const u32*)minSrc;
    const u32* sMax = (const u32*)maxSrc;
    dstMin[0] = sMin[0];
    dstMax[0] = sMax[0];
    dstMin[1] = sMin[1];
    dstMin[2] = sMin[2];
    dstMax[1] = sMax[1];
    dstMax[2] = sMax[2];

    ColiNode* node = self->tris2;
    if (node->field_0x8 == 3) {
        return func_804B7B6C(self, &self->indices2[node->field_0x0 + 1]);
    }
    f32 dmin = node->field_0x4 - (&lbl_eu_8065F3F0.x)[node->field_0x8];
    f32 dmax = node->field_0x4 - (&lbl_eu_8065F3FC.x)[node->field_0x8];
    if (dmin * dmax >= lbl_eu_8066AEFC) {
        // box entirely on one side of the plane: descend into that side only.
        ColiNode* child;
        if (dmin >= lbl_eu_8066AEFC) {
            child = &self->tris2[self->indices2[node->field_0x0 + 1]];
        } else {
            child = &self->tris2[self->indices2[node->field_0x0]];
        }
        bool hit;
        if (child->field_0x8 == 3) {
            hit = func_804B7B6C(self, &self->indices2[child->field_0x0 + 1]);
        } else {
            f32 cdmin = child->field_0x4 - (&lbl_eu_8065F3F0.x)[child->field_0x8];
            f32 cdmax = child->field_0x4 - (&lbl_eu_8065F3FC.x)[child->field_0x8];
            if (cdmin * cdmax >= lbl_eu_8066AEFC) {
                if (cdmin >= lbl_eu_8066AEFC) {
                    hit = func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0 + 1]]);
                } else {
                    hit = func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0]]);
                }
            } else {
                hit = false;
                if (cdmax < lbl_eu_8066AEFC) {
                    hit = func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0]]);
                }
                if (!hit) {
                    hit = func_804BBAB0(self, &self->tris2[self->indices2[child->field_0x0 + 1]]);
                }
            }
        }
        return hit;
    }
    // plane straddles the box: visit the near child first (inlined), then the
    // far child only when the near side reported no hit.
    ColiNode* nearChild = &self->tris2[self->indices2[node->field_0x0]];
    bool hit;
    if (nearChild->field_0x8 == 3) {
        hit = func_804B7B6C(self, &self->indices2[nearChild->field_0x0 + 1]);
    } else {
        f32 ndmin = nearChild->field_0x4 - (&lbl_eu_8065F3F0.x)[nearChild->field_0x8];
        f32 ndmax = nearChild->field_0x4 - (&lbl_eu_8065F3FC.x)[nearChild->field_0x8];
        if (ndmin * ndmax >= lbl_eu_8066AEFC) {
            if (ndmin >= lbl_eu_8066AEFC) {
                hit = func_804BBAB0(self, &self->tris2[self->indices2[nearChild->field_0x0 + 1]]);
            } else {
                hit = func_804BBAB0(self, &self->tris2[self->indices2[nearChild->field_0x0]]);
            }
        } else {
            hit = false;
            if (ndmax < lbl_eu_8066AEFC) {
                hit = func_804BBAB0(self, &self->tris2[self->indices2[nearChild->field_0x0]]);
            }
            if (!hit) {
                hit = func_804BBAB0(self, &self->tris2[self->indices2[nearChild->field_0x0 + 1]]);
            }
        }
    }
    if (hit) return true;
    ColiNode* farChild = &self->tris2[self->indices2[node->field_0x0 + 1]];
    bool hit2;
    if (farChild->field_0x8 == 3) {
        hit2 = func_804B7B6C(self, &self->indices2[farChild->field_0x0 + 1]);
    } else {
        f32 fdmin = farChild->field_0x4 - (&lbl_eu_8065F3F0.x)[farChild->field_0x8];
        f32 fdmax = farChild->field_0x4 - (&lbl_eu_8065F3FC.x)[farChild->field_0x8];
        if (fdmin * fdmax >= lbl_eu_8066AEFC) {
            if (fdmin >= lbl_eu_8066AEFC) {
                hit2 = func_804BBAB0(self, &self->tris2[self->indices2[farChild->field_0x0 + 1]]);
            } else {
                hit2 = func_804BBAB0(self, &self->tris2[self->indices2[farChild->field_0x0]]);
            }
        } else {
            hit2 = false;
            if (fdmax < lbl_eu_8066AEFC) {
                hit2 = func_804BBAB0(self, &self->tris2[self->indices2[farChild->field_0x0]]);
            }
            if (!hit2) {
                hit2 = func_804BBAB0(self, &self->tris2[self->indices2[farChild->field_0x0 + 1]]);
            }
        }
    }
    return hit2;
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
