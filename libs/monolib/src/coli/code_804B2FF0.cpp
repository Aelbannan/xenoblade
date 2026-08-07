// Auto-scaffolded catalog TU for monolib/src/coli/code_804B2FF0
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>
#include "monolib/coli/CTaskColiManager.hpp"
#include "monolib/work/CProcess.hpp"
#include <revolution/MTX.h>
#include <nw4r/math.h>

void func_804B2FF0(){}

void func_804B30CC(){}

void func_804B31EC(){}

void func_804B33C8(){}

void func_804B34F4(){}

void func_804B3B18(){}

void func_804B4BDC(){}

void func_804B4854(){}

void func_804B4E10(){}

void func_804B526C(){}

// Global axis-aligned bounding box shared by the coli object-space
// bounds-expansion helpers. Layout: max x/y/z at +0x0/+0x4/+0x8,
// min x/y/z at +0xC/+0x10/+0x14.
struct CColiBounds {
    f32 max[3]; //0x0 max x/y/z
    f32 min[3]; //0xC min x/y/z
};

extern "C" CColiBounds* lbl_eu_80665944;

// Position (x/y/z at +0x4/+0x8/+0xC) and radius at +0x10 used for
// sphere-like bounds expansion.
struct CColiSphere {
    f32 pad0;     //0x0
    f32 x;        //0x4
    f32 y;        //0x8
    f32 z;        //0xC
    f32 radius;   //0x10
};

void func_804B43B4(CColiSphere* p) {
    // Expand the box's max corner by the sphere's max extent and the
    // min corner by its min extent, per axis.
    if (lbl_eu_80665944->max[0] < p->x + p->radius) {
        lbl_eu_80665944->max[0] = p->x + p->radius;
    }
    if (lbl_eu_80665944->min[0] > p->x - p->radius) {
        lbl_eu_80665944->min[0] = p->x - p->radius;
    }
    if (lbl_eu_80665944->max[1] < p->y + p->radius) {
        lbl_eu_80665944->max[1] = p->y + p->radius;
    }
    if (lbl_eu_80665944->min[1] > p->y - p->radius) {
        lbl_eu_80665944->min[1] = p->y - p->radius;
    }
    if (lbl_eu_80665944->max[2] < p->z + p->radius) {
        lbl_eu_80665944->max[2] = p->z + p->radius;
    }
    if (lbl_eu_80665944->min[2] > p->z - p->radius) {
        lbl_eu_80665944->min[2] = p->z - p->radius;
    }
}

// Variant with separate horizontal (x/z) and vertical (y) radii.
// Position x/y/z at +0x4/+0x8/+0xC, radiusXZ at +0x10, radiusY at +0x14.
struct CColiEllipsoid {
    f32 pad0;      //0x0
    f32 x;         //0x4
    f32 y;         //0x8
    f32 z;         //0xC
    f32 radiusXZ;  //0x10
    f32 radiusY;   //0x14
};

void func_804B4478(CColiEllipsoid* p) {
    if (lbl_eu_80665944->max[0] < p->x + p->radiusXZ) {
        lbl_eu_80665944->max[0] = p->x + p->radiusXZ;
    }
    if (lbl_eu_80665944->min[0] > p->x - p->radiusXZ) {
        lbl_eu_80665944->min[0] = p->x - p->radiusXZ;
    }
    if (lbl_eu_80665944->max[2] < p->z + p->radiusXZ) {
        lbl_eu_80665944->max[2] = p->z + p->radiusXZ;
    }
    if (lbl_eu_80665944->min[2] > p->z - p->radiusXZ) {
        lbl_eu_80665944->min[2] = p->z - p->radiusXZ;
    }
    if (lbl_eu_80665944->max[1] < p->y + p->radiusY) {
        lbl_eu_80665944->max[1] = p->y + p->radiusY;
    }
    if (lbl_eu_80665944->min[1] > p->y - p->radiusY) {
        lbl_eu_80665944->min[1] = p->y - p->radiusY;
    }
}

// Capsule (segment swept by a radius) bounds expansion: two endpoints
// (+0x4..+0xC and +0x10..+0x18) and a uniform radius at +0x28, expanded
// into the global box one axis at a time.
struct CColiCapsule {
    f32 field_0x0;         //0x0
    f32 endPos[3];         //0x4  (x/y/z of first end)
    f32 endPosB[3];        //0x10 (x/y/z of second end)
    f32 pad_0x1C;          //0x1C
    f32 field_0x20;        //0x20
    f32 field_0x24;        //0x24
    f32 radius;            //0x28
};

void func_804B453C(CColiCapsule* p) {
    for (int i = 0; i < 3; i++) {
        if (lbl_eu_80665944->max[i] < p->endPos[i] + p->radius) {
            lbl_eu_80665944->max[i] = p->endPos[i] + p->radius;
        }
        if (lbl_eu_80665944->min[i] > p->endPos[i] - p->radius) {
            lbl_eu_80665944->min[i] = p->endPos[i] - p->radius;
        }
        if (lbl_eu_80665944->max[i] < p->endPosB[i] + p->radius) {
            lbl_eu_80665944->max[i] = p->endPosB[i] + p->radius;
        }
        if (lbl_eu_80665944->min[i] > p->endPosB[i] - p->radius) {
            lbl_eu_80665944->min[i] = p->endPosB[i] - p->radius;
        }
    }
}

// Another bounds-expansion source: position x/y/z at +0x10/+0x20/+0x30
// and a single uniform radius at +0x70.
struct CColiBoundsPoint {
    f32 field_0x00[4];  //0x0-0xF
    f32 x;              //0x10
    f32 field_0x14[3];  //0x14-0x1F
    f32 y;              //0x20
    f32 field_0x24[3];  //0x24-0x2F
    f32 z;              //0x30
    f32 field_0x34[15]; //0x34-0x6F
    f32 radius;         //0x70
};

void func_804B45E4(CColiBoundsPoint* p) {
    if (lbl_eu_80665944->max[0] < p->x + p->radius) {
        lbl_eu_80665944->max[0] = p->x + p->radius;
    }
    if (lbl_eu_80665944->min[0] > p->x - p->radius) {
        lbl_eu_80665944->min[0] = p->x - p->radius;
    }
    if (lbl_eu_80665944->max[1] < p->y + p->radius) {
        lbl_eu_80665944->max[1] = p->y + p->radius;
    }
    if (lbl_eu_80665944->min[1] > p->y - p->radius) {
        lbl_eu_80665944->min[1] = p->y - p->radius;
    }
    if (lbl_eu_80665944->max[2] < p->z + p->radius) {
        lbl_eu_80665944->max[2] = p->z + p->radius;
    }
    if (lbl_eu_80665944->min[2] > p->z - p->radius) {
        lbl_eu_80665944->min[2] = p->z - p->radius;
    }
}

// Bounds-expansion source sharing the +0x10/+0x20/+0x30 position layout
// but with the uniform radius at +0x64 instead of +0x70.
struct CColiBoundsPoint2 {
    f32 field_0x00[4];  //0x0-0xF
    f32 x;              //0x10
    f32 field_0x14[3];  //0x14-0x1F
    f32 y;              //0x20
    f32 field_0x24[3];  //0x24-0x2F
    f32 z;              //0x30
    f32 field_0x34[12]; //0x34-0x63
    f32 radius;         //0x64
};

void func_804B46A8(CColiBoundsPoint2* p) {
    if (lbl_eu_80665944->max[0] < p->x + p->radius) {
        lbl_eu_80665944->max[0] = p->x + p->radius;
    }
    if (lbl_eu_80665944->min[0] > p->x - p->radius) {
        lbl_eu_80665944->min[0] = p->x - p->radius;
    }
    if (lbl_eu_80665944->max[1] < p->y + p->radius) {
        lbl_eu_80665944->max[1] = p->y + p->radius;
    }
    if (lbl_eu_80665944->min[1] > p->y - p->radius) {
        lbl_eu_80665944->min[1] = p->y - p->radius;
    }
    if (lbl_eu_80665944->max[2] < p->z + p->radius) {
        lbl_eu_80665944->max[2] = p->z + p->radius;
    }
    if (lbl_eu_80665944->min[2] > p->z - p->radius) {
        lbl_eu_80665944->min[2] = p->z - p->radius;
    }
}

// Half-position (x/y) and quarter-extent (x/y/z) bound components from
// two additional globals combined into the global box.
struct CColiHx { f32 ax; f32 ay; f32 f8; };        //0x0/0x4/0x8
struct CColiQx { f32 qx; f32 qy; f32 qz; }; //0x0/0x4/0x8

extern "C" CColiHx* lbl_eu_8066594C;
extern "C" CColiQx* lbl_eu_80665948;

// sdata2 literals and the shared "master node index" used by the
// bounds/transform helpers.
extern "C" s16 lbl_eu_80665950; // master node index (lha)
extern "C" f32 lbl_eu_8066AEB0; // 0.0f
extern "C" f32 lbl_eu_8066AEB4; // 1.0f

// --- Shared collision-node / matrix-source types ---

// Node header shared by the coli objects: signed bone index at +0x0 and
// flags word at +0x2 (bit 1 selects the root matrix instead of the index
// lookup; bit 0 marks the object dirty after a bounds update).
struct CColiHead {
    s16 index; //0x0
    u16 flags; //0x2
};

// Matrix source owned by the collision manager: the root (identity/bind)
// matrix at +0x3C and a bone-matrix table pointer at +0xEC (each entry
// 0x30 bytes, selected by node index).
struct CColiMesh {
    char pad0[0x3C];
    Mtx rootMtx;     //0x3C
    char pad1[0x80]; //0x6C..0xEC
    Mtx* boneTable;  //0xEC
};

// Collision manager: node-matrix source pointer at +0x147C.
struct CColiMgr {
    char pad[0x147C];
    CColiMesh* mesh; //0x147C
};

// Full node (func_804B3EA8): world matrix, inverse world matrix, a local
// vector at +0x64 and a scalar bounds radius at +0x70.
struct CColiNode {
    CColiHead hd; //0x0
    Mtx mtxW;     //0x4
    Mtx invW;     //0x34
    f32 vec[3];   //0x64
    f32 radius;   //0x70
};

// Node whose world matrix is copied from the bone/root source (upper 3x4),
// inverse derived, and a scalar radius at +0x64 used to expand the global
// AABB around the copied matrix's translation column.
// CObjMtx is a POD forcing lwz/stw word copies in struct assignment.
struct CObjMtx {
    f32 m[3][4];
};

struct CColiMtxNode {
    CColiHead hd;    //0x0
    CObjMtx mtxW;    //0x4
    CObjMtx invW;    //0x34
    f32 radius;      //0x64
};

// Rigid reference node (func_804B3EA8 'other'): matrix at +0x0, position
// vector at +0x30, used for PSMTXConcat.
struct CColiRigid {
    Mtx mtx;    //0x0
    f32 pos[3]; //0x30
};

// Sphere-shaped collision object (func_804B3658): position + radius.
struct CColiSphereOb {
    CColiHead hd; //0x0
    f32 pos[3];   //0x4
    f32 radius;   //0x10
};

void func_804B4020(CColiMtxNode* self, const CColiMgr* mgr,
                   const CColiSphereOb* other) {
    const f32 (*src)[4];
    if (self->hd.flags & 2) {
        src = mgr->mesh->rootMtx;
    } else {
        src = mgr->mesh->boneTable[self->hd.index];
    }

    CObjMtx tmp;
    PSMTXCopy(src, tmp.m);
    self->mtxW = tmp;
    PSMTXInverse(src, self->invW.m);

    Vec n;
    n.x = other->radius;
    n.y = lbl_eu_8066AEB0;
    n.z = lbl_eu_8066AEB0;
    nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&n,
                                    (const nw4r::math::MTX34*)&self->mtxW,
                                    (const nw4r::math::VEC3*)&n);
    f32 mag = PSVECMag(&n);

    self->radius = mag;
    self->hd.flags |= 1;

    CColiBounds* b = lbl_eu_80665944;
    if (b->max[0] < self->mtxW.m[0][3] + self->radius)
        b->max[0] = self->mtxW.m[0][3] + self->radius;
    if (b->min[0] > self->mtxW.m[0][3] - self->radius)
        b->min[0] = self->mtxW.m[0][3] - self->radius;
    if (b->max[1] < self->mtxW.m[1][3] + self->radius)
        b->max[1] = self->mtxW.m[1][3] + self->radius;
    if (b->min[1] > self->mtxW.m[1][3] - self->radius)
        b->min[1] = self->mtxW.m[1][3] - self->radius;
    if (b->max[2] < self->mtxW.m[2][3] + self->radius)
        b->max[2] = self->mtxW.m[2][3] + self->radius;
    if (b->min[2] > self->mtxW.m[2][3] - self->radius)
        b->min[2] = self->mtxW.m[2][3] - self->radius;
}

// Ellipsoid-shaped collision object (func_804B37E4 / func_804B3970).
struct CColiEllipsoidOb {
    CColiHead hd;     //0x0
    f32 pos[3];       //0x4
    f32 radiusXZ;     //0x10
    f32 radiusY;      //0x14
    f32 f18;          //0x18
};

// Object carrying both a position Vec (used as the transform source) and a
// secondary Vec at +0xC (func_804B3970 'other').
struct CColiPosVec {
    Vec pos; //0x0
    Vec v;   //0xC
};

// Matrix output object (func_804B49B8): world + inverse matrix, then the
// same two matrices shifted up by 0x60.
struct CColiMtxOut {
    Mtx m0; //0x0
    Mtx m1; //0x30
    Mtx m2; //0x60
    Mtx m3; //0x90
};

// Capsule collision node (func_804B3D1C): two endpoint positions, the
// segment direction, a uniform radius and the squared direction length.
struct CColiCapsuleNode {
    CColiHead hd;    //0x0 (index + flags)
    f32 posA[3];     //0x4
    f32 posB[3];     //0x10
    f32 dir[3];      //0x1C
    f32 radius;      //0x28
    f32 lenSq;       //0x2C
    s16 index2;      //0x30
};

// ---------------------------------------------------------------------------
// func_804B41FC: transform a position by the fixed bone matrix, derive an
// ellipsoid-ish half-extent into the shared Qx/Hx globals, and fold them
// into the global AABB.
// ---------------------------------------------------------------------------
void func_804B41FC(const CColiNode* unused, const CColiMgr* mgr,
                   const CColiPosVec* other) {
    const Mtx* table = mgr->mesh->boneTable;
    const f32 (*src)[4] = table[lbl_eu_80665950];

    PSMTXMultVec(src, &other->pos, (Vec*)lbl_eu_80665948);

    nw4r::math::VEC3 n;
    n.x = lbl_eu_8066AEB4;
    n.y = lbl_eu_8066AEB0;
    n.z = lbl_eu_8066AEB0;
    nw4r::math::VEC3TransformNormal(&n, (const nw4r::math::MTX34*)src, &n);
    f32 mag = PSVECMag((const Vec*)&n);

    lbl_eu_8066594C->ax = other->v.x * mag;
    lbl_eu_8066594C->ay = other->v.y * mag;
    lbl_eu_8066594C->f8 = other->v.z * mag;
    lbl_eu_80665948->qy += lbl_eu_8066594C->f8;
    lbl_eu_8066594C->ay -= lbl_eu_8066594C->f8;

    if (lbl_eu_80665944->max[0] < lbl_eu_80665948->qx + lbl_eu_8066594C->ax)
        lbl_eu_80665944->max[0] = lbl_eu_80665948->qx + lbl_eu_8066594C->ax;
    if (lbl_eu_80665944->min[0] > lbl_eu_80665948->qx - lbl_eu_8066594C->ax)
        lbl_eu_80665944->min[0] = lbl_eu_80665948->qx - lbl_eu_8066594C->ax;
    if (lbl_eu_80665944->max[2] < lbl_eu_80665948->qz + lbl_eu_8066594C->ax)
        lbl_eu_80665944->max[2] = lbl_eu_80665948->qz + lbl_eu_8066594C->ax;
    if (lbl_eu_80665944->min[2] > lbl_eu_80665948->qz - lbl_eu_8066594C->ax)
        lbl_eu_80665944->min[2] = lbl_eu_80665948->qz - lbl_eu_8066594C->ax;
    if (lbl_eu_80665944->max[1] < lbl_eu_80665948->qy + lbl_eu_8066594C->ay)
        lbl_eu_80665944->max[1] = lbl_eu_80665948->qy + lbl_eu_8066594C->ay;
    if (lbl_eu_80665944->min[1] > lbl_eu_80665948->qy)
        lbl_eu_80665944->min[1] = lbl_eu_80665948->qy;
}

void func_804B476C() {
    if (lbl_eu_80665944->max[0] < lbl_eu_80665948->qx + lbl_eu_8066594C->ax) {
        lbl_eu_80665944->max[0] = lbl_eu_8066594C->ax + lbl_eu_80665948->qx;
    }
    if (lbl_eu_80665944->min[0] > lbl_eu_80665948->qx - lbl_eu_8066594C->ax) {
        lbl_eu_80665944->min[0] = lbl_eu_80665948->qx - lbl_eu_8066594C->ax;
    }
    if (lbl_eu_80665944->max[2] < lbl_eu_80665948->qz + lbl_eu_8066594C->ax) {
        lbl_eu_80665944->max[2] = lbl_eu_8066594C->ax + lbl_eu_80665948->qz;
    }
    if (lbl_eu_80665944->min[2] > lbl_eu_80665948->qz - lbl_eu_8066594C->ax) {
        lbl_eu_80665944->min[2] = lbl_eu_80665948->qz - lbl_eu_8066594C->ax;
    }
    if (lbl_eu_80665944->max[1] < lbl_eu_80665948->qy + lbl_eu_8066594C->ay) {
        lbl_eu_80665944->max[1] = lbl_eu_8066594C->ay + lbl_eu_80665948->qy;
    }
    if (lbl_eu_80665944->min[1] > lbl_eu_80665948->qy) {
        lbl_eu_80665944->min[1] = lbl_eu_80665948->qy;
    }
}

// ---------------------------------------------------------------------------
// func_804B3EA8: transform a rigid reference node into self space, invert,
// then expand the global AABB using the transformed position and magnitude.
// ---------------------------------------------------------------------------
void func_804B3EA8(CColiNode* self, const CColiMgr* mgr, const CColiRigid* other) {
    const f32 (*src)[4];
    if (self->hd.flags & 2) {
        src = mgr->mesh->rootMtx;
    } else {
        src = mgr->mesh->boneTable[self->hd.index];
    }

    PSMTXConcat(src, other->mtx, self->mtxW);
    PSMTXInverse(self->mtxW, self->invW);

    self->vec[0] = other->pos[0];
    self->vec[1] = other->pos[1];
    self->vec[2] = other->pos[2];

    nw4r::math::VEC3 n;
    nw4r::math::VEC3TransformNormal(&n, (const nw4r::math::MTX34*)self->mtxW,
                                    (const nw4r::math::VEC3*)self->vec);
    f32 mag = PSVECMag((const Vec*)&n);

    self->radius = mag;
    self->hd.flags |= 1;

    CColiBounds* b = lbl_eu_80665944;
    if (b->max[0] < self->mtxW[0][3] + self->radius) b->max[0] = self->mtxW[0][3] + self->radius;
    if (b->min[0] > self->mtxW[0][3] - self->radius) b->min[0] = self->mtxW[0][3] - self->radius;
    if (b->max[1] < self->mtxW[1][3] + self->radius) b->max[1] = self->mtxW[1][3] + self->radius;
    if (b->min[1] > self->mtxW[1][3] - self->radius) b->min[1] = self->mtxW[1][3] - self->radius;
    if (b->max[2] < self->mtxW[2][3] + self->radius) b->max[2] = self->mtxW[2][3] + self->radius;
    if (b->min[2] > self->mtxW[2][3] - self->radius) b->min[2] = self->mtxW[2][3] - self->radius;
}

// ---------------------------------------------------------------------------
// func_804B3658: transform a sphere collision object's center into parent
// space, derive its radius from the transformed (z,0,0) normal, and expand
// the global AABB.
// ---------------------------------------------------------------------------
void func_804B3658(CColiSphereOb* self, const CColiMgr* mgr, const Vec* other) {
    const f32 (*src)[4];
    if (self->hd.flags & 2) {
        src = mgr->mesh->rootMtx;
    } else {
        src = mgr->mesh->boneTable[self->hd.index];
    }

    Vec outv;
    PSMTXMultVec(src, other, &outv);
    self->pos[0] = outv.x;
    self->pos[1] = outv.y;
    self->pos[2] = outv.z;

    nw4r::math::VEC3 n;
    n.x = other->z;
    n.y = lbl_eu_8066AEB0;
    n.z = lbl_eu_8066AEB0;
    nw4r::math::VEC3TransformNormal(&n, (const nw4r::math::MTX34*)src, &n);
    f32 mag = PSVECMag((const Vec*)&n);

    self->radius = mag;
    self->hd.flags |= 1;

    CColiBounds* b = lbl_eu_80665944;
    if (b->max[0] < self->pos[0] + self->radius) b->max[0] = self->pos[0] + self->radius;
    if (b->min[0] > self->pos[0] - self->radius) b->min[0] = self->pos[0] - self->radius;
    if (b->max[1] < self->pos[1] + self->radius) b->max[1] = self->pos[1] + self->radius;
    if (b->min[1] > self->pos[1] - self->radius) b->min[1] = self->pos[1] - self->radius;
    if (b->max[2] < self->pos[2] + self->radius) b->max[2] = self->pos[2] + self->radius;
    if (b->min[2] > self->pos[2] - self->radius) b->min[2] = self->pos[2] - self->radius;
}

// ---------------------------------------------------------------------------
// func_804B37E4: read the transform's translation into an ellipsoid object,
// scale an (x/y/z) extent by the (1,0,0)-transformed normal magnitude, and
// expand the global AABB with horizontal (XZ) and vertical (Y) radii.
// ---------------------------------------------------------------------------
void func_804B37E4(CColiEllipsoidOb* self, const CColiMgr* mgr, const Vec* other) {
    const f32 (*src)[4];
    if (self->hd.flags & 2) {
        src = mgr->mesh->rootMtx;
    } else {
        src = mgr->mesh->boneTable[self->hd.index];
    }

    self->pos[0] = src[0][3];
    self->pos[1] = src[1][3];
    self->pos[2] = src[2][3];

    nw4r::math::VEC3 n;
    n.x = lbl_eu_8066AEB4;
    n.y = lbl_eu_8066AEB0;
    n.z = lbl_eu_8066AEB0;
    nw4r::math::VEC3TransformNormal(&n, (const nw4r::math::MTX34*)src, &n);
    f32 mag = PSVECMag((const Vec*)&n);

    self->f18 = other->z * mag;
    self->radiusY = other->y * mag;
    self->radiusXZ = other->x * mag;
    self->hd.flags |= 1;

    CColiBounds* b = lbl_eu_80665944;
    if (b->max[0] < self->pos[0] + self->radiusXZ) b->max[0] = self->pos[0] + self->radiusXZ;
    if (b->min[0] > self->pos[0] - self->radiusXZ) b->min[0] = self->pos[0] - self->radiusXZ;
    if (b->max[2] < self->pos[2] + self->radiusXZ) b->max[2] = self->pos[2] + self->radiusXZ;
    if (b->min[2] > self->pos[2] - self->radiusXZ) b->min[2] = self->pos[2] - self->radiusXZ;
    if (b->max[1] < self->pos[1] + self->radiusY) b->max[1] = self->pos[1] + self->radiusY;
    if (b->min[1] > self->pos[1] - self->radiusY) b->min[1] = self->pos[1] - self->radiusY;
}

// ---------------------------------------------------------------------------
// func_804B3970: transform a position into ellipsoid space, scale an offset
// into XZ/Y radii, and expand the global AABB (same layout as func_804B37E4).
// ---------------------------------------------------------------------------
void func_804B3970(CColiEllipsoidOb* self, const CColiMgr* mgr, const CColiPosVec* other) {
    const f32 (*src)[4];
    if (self->hd.flags & 2) {
        src = mgr->mesh->rootMtx;
    } else {
        src = mgr->mesh->boneTable[self->hd.index];
    }

    Vec outv;
    PSMTXMultVec(src, &other->pos, &outv);
    self->pos[0] = outv.x;
    self->pos[1] = outv.y;
    self->pos[2] = outv.z;

    nw4r::math::VEC3 n;
    n.x = lbl_eu_8066AEB4;
    n.y = lbl_eu_8066AEB0;
    n.z = lbl_eu_8066AEB0;
    nw4r::math::VEC3TransformNormal(&n, (const nw4r::math::MTX34*)src, &n);
    f32 mag = PSVECMag((const Vec*)&n);

    self->f18 = other->v.z * mag;
    self->radiusY = other->v.y * mag;
    self->radiusXZ = other->v.x * mag;
    self->hd.flags |= 1;

    CColiBounds* b = lbl_eu_80665944;
    if (b->max[0] < self->pos[0] + self->radiusXZ) b->max[0] = self->pos[0] + self->radiusXZ;
    if (b->min[0] > self->pos[0] - self->radiusXZ) b->min[0] = self->pos[0] - self->radiusXZ;
    if (b->max[2] < self->pos[2] + self->radiusXZ) b->max[2] = self->pos[2] + self->radiusXZ;
    if (b->min[2] > self->pos[2] - self->radiusXZ) b->min[2] = self->pos[2] - self->radiusXZ;
    if (b->max[1] < self->pos[1] + self->radiusY) b->max[1] = self->pos[1] + self->radiusY;
    if (b->min[1] > self->pos[1] - self->radiusY) b->min[1] = self->pos[1] - self->radiusY;
}

// ---------------------------------------------------------------------------
// func_804B3D1C: build a capsule from two bone matrices and expand the global
// AABB by both endpoints swept by the (transformed) radius magnitude.
// ---------------------------------------------------------------------------
void func_804B3D1C(CColiCapsuleNode* self, const CColiMgr* mgr, const Vec* other) {
    const Mtx* table = mgr->mesh->boneTable;
    const f32 (*srcA)[4] = table[self->hd.index];
    const f32 (*srcB)[4] = table[self->index2];

    self->posA[0] = srcA[0][3];
    self->posA[1] = srcA[1][3];
    self->posA[2] = srcA[2][3];
    self->posB[0] = srcB[0][3];
    self->posB[1] = srcB[1][3];
    self->posB[2] = srcB[2][3];

    self->dir[0] = self->posB[0] - self->posA[0];
    self->dir[1] = self->posB[1] - self->posA[1];
    self->dir[2] = self->posB[2] - self->posA[2];
    self->lenSq = self->dir[0] * self->dir[0] +
                  self->dir[1] * self->dir[1] +
                  self->dir[2] * self->dir[2];

    nw4r::math::VEC3 n;
    n.x = other->x;
    n.y = lbl_eu_8066AEB0;
    n.z = lbl_eu_8066AEB0;
    nw4r::math::VEC3TransformNormal(&n, (const nw4r::math::MTX34*)srcA, &n);
    f32 mag = PSVECMag((const Vec*)&n);

    self->radius = mag;
    self->hd.flags |= 1;

    CColiBounds* b = lbl_eu_80665944;
    for (int i = 0; i < 3; i++) {
        if (b->max[i] < self->posA[i] + self->radius) b->max[i] = self->posA[i] + self->radius;
        if (b->min[i] > self->posA[i] - self->radius) b->min[i] = self->posA[i] - self->radius;
        if (b->max[i] < self->posB[i] + self->radius) b->max[i] = self->posB[i] + self->radius;
        if (b->min[i] > self->posB[i] - self->radius) b->min[i] = self->posB[i] - self->radius;
    }
}

// ---------------------------------------------------------------------------
// func_804B49B8: copy a bone matrix from the manager table, store its
// inverse, then duplicate both matrices up by one slot in the output node.
// ---------------------------------------------------------------------------
void func_804B49B8(const CColiHead* hd, CColiMtxOut* dst, const CColiMgr* mgr) {
    const Mtx* table = mgr->mesh->boneTable;
    const f32* f = table[hd->index][0];
    dst->m0[0][0] = f[0];
    dst->m0[0][1] = f[1];
    dst->m0[0][2] = f[2];
    dst->m0[0][3] = f[3];
    dst->m0[1][0] = f[4];
    dst->m0[1][1] = f[5];
    dst->m0[1][2] = f[6];
    dst->m0[1][3] = f[7];
    dst->m0[2][0] = f[8];
    dst->m0[2][1] = f[9];
    dst->m0[2][2] = f[10];
    dst->m0[2][3] = f[11];

    PSMTXInverse(dst->m0, dst->m1);

    dst->m2[0][0] = dst->m0[0][0];
    dst->m2[0][1] = dst->m0[0][1];
    dst->m2[0][2] = dst->m0[0][2];
    dst->m2[0][3] = dst->m0[0][3];
    dst->m2[1][0] = dst->m0[1][0];
    dst->m2[1][1] = dst->m0[1][1];
    dst->m2[1][2] = dst->m0[1][2];
    dst->m2[1][3] = dst->m0[1][3];
    dst->m2[2][0] = dst->m0[2][0];
    dst->m2[2][1] = dst->m0[2][1];
    dst->m2[2][2] = dst->m0[2][2];
    dst->m2[2][3] = dst->m0[2][3];

    dst->m3[0][0] = dst->m1[0][0];
    dst->m3[0][1] = dst->m1[0][1];
    dst->m3[0][2] = dst->m1[0][2];
    dst->m3[0][3] = dst->m1[0][3];
    dst->m3[1][0] = dst->m1[1][0];
    dst->m3[1][1] = dst->m1[1][1];
    dst->m3[1][2] = dst->m1[1][2];
    dst->m3[1][3] = dst->m1[1][3];
    dst->m3[2][0] = dst->m1[2][0];
    dst->m3[2][1] = dst->m1[2][1];
    dst->m3[2][2] = dst->m1[2][2];
    dst->m3[2][3] = dst->m1[2][3];
}

extern "C" void func_804B1DC0(void*, int);

// Doubly-linked list used by the coli tasks: head pointer stored on the
// owner at +0x5C, each node links forward via next (+0x9C) and backward
// via prev (+0xA0).
struct CColiListItem {
    char pad0[0x9C];
    CColiListItem* next; //0x9C
    CColiListItem* prev; //0xA0
    f32 f_a4;            //0xA4
};

// Nodes walked by CTaskColiManager::Move: forward/back links at +0x9C/+0xA0
// and a status flags word at +0xA8 checked for bit 13 (0x2000).
struct CColiMoveNode {
    char pad0[0x9C];
    CColiMoveNode* next; //0x9C
    CColiMoveNode* prev; //0xA0
    char padA4[0x4];     //0xA4
    u32 flags;           //0xA8
};

struct CColiList {
    char pad0[0x5C];
    CColiListItem* head; //0x5C
};

// Append `node` to the back of the list owned by `self`, conditioning it with
// func_804B1DC0 first. If the node is already present it is left untouched.
// Newly-linked nodes have their forward link cleared and their swept-distance
// accumulator (+0xA4) reset to zero.
void func_804B4BDC(CColiList* self, CColiListItem* node) {
    func_804B1DC0(node, 1);
    CColiListItem* cur = self->head;
    if (cur != 0) {
        CColiListItem* tail = cur;
        while (cur != 0) {
            if (node == cur) return; // already linked
            tail = cur;
            cur = cur->next;
        }
        tail->next = node;
        node->prev = tail;
        node->next = 0;
        node->f_a4 = 0.0f;
    } else {
        self->head = node;
        node->next = 0;
        node->prev = 0;
        node->f_a4 = 0.0f;
    }
}

// Remove `findNode` from the list owned by `self`, relinking neighbours and
// fixing the head when the removed node was the head.
void func_804B4C7C(CColiList* self, CColiListItem* findNode) {
    func_804B1DC0(findNode, 0);
    CColiListItem* cur = self->head;
    if (cur != 0) {
        for (;;) {
            if (cur == findNode) {
                CColiListItem* prev = cur->prev;
                CColiListItem* next = cur->next;
                if (prev != 0) {
                    prev->next = next;
                    if (next != 0) next->prev = prev;
                    if (self->head == findNode) self->head = prev;
                } else if (next != 0) {
                    next->prev = 0;
                    if (self->head == findNode) self->head = next;
                } else {
                    if (self->head == findNode) self->head = 0;
                }
                break;
            }
            cur = cur->next;
        }
    }
}

// Shared per-sector query structures used by the walk funcs below.
// Node in a spatially-split list: forward link at +0x9C, a 32-bit filter
// mask at +0xAC.
struct CColiQueryNode {
    char pad0[0x9C];
    CColiQueryNode* next; //0x9C
    char pad1[0x8];       //0xA0
    u32 filter;           //0xAC
};

// Output/Self for the walk funcs: an AABB pair (bbMin at +0x60, bbMax at
// +0x6C) fed from either the built segment or the shared global result.
struct CColiQueryResult {
    char pad0[0x5C];
    CColiQueryNode* head; //0x5C
    f32 bbMin[3];         //0x60
    f32 bbMax[3];         //0x6C
};

// Shared walk state (lbl_eu_8065D138): accumulated AABB (bbMin +0x38,
// bbMax +0x44), the queried source segment (+0x50), a scalar at +0x5C and
// a flag at +0x60.
struct CColiWalkState {
    char pad0[0x38];
    f32 bbMin[3];  //0x38
    f32 bbMax[3];  //0x44
    f32 src[3];    //0x50
    f32 f5c;       //0x5C
    u8 flag;       //0x60
};

extern "C" CColiWalkState* lbl_eu_8065D138;

// Static initializer (.ctors) for the shared collision walk object at
// lbl_eu_8065D138: run the CColiProc base constructor, then point its default
// target data at the alternate table lbl_eu_8056F4F0.
struct CColiProcLocal {
    void* field_0x0; //0x0
    u32 field_0x4;   //0x4
};

extern "C" u8 lbl_eu_8056F4F0;
extern "C" void __ct__CColiProc(CColiProcLocal* self);

extern "C" void sinit_804B598C() {
    CColiProcLocal* self = (CColiProcLocal*)&lbl_eu_8065D138;
    __ct__CColiProc(self);
    self->field_0x0 = &lbl_eu_8056F4F0;
}

extern "C" f32 lbl_eu_8066AEC0;
extern "C" f32 lbl_eu_8066AEC4;
extern "C" f32 lbl_eu_8066AEC8;

// Walk/traverse helpers (C-linkage retail symbols in the sibling coli unit).
extern "C" void func_804A7ACC(void*, const Vec*, const Vec*, void*);
extern "C" void func_804B077C(void*, const Vec*, const Vec*);
extern "C" int func_804B0818(const void*, const CColiQueryNode*);
extern "C" int func_804B21A8(const CColiQueryNode*, const void*, int);

// Walk the segment query: reject degenerate (equal) vectors, otherwise run
// the segment build + node query and expand the result AABB.
int func_804B5088(CColiQueryResult* self, const Vec* a, const Vec* b,
                   u32 filter, int isFirst) {
    if (a->x == b->x && a->y == b->y && a->z == b->z) return 0;

    int init = 0;
    if (isFirst == 0) {
        lbl_eu_8065D138->flag = 0;
        init = 1;
        lbl_eu_8065D138->src[0] = a->x;
        lbl_eu_8065D138->src[1] = a->y;
        lbl_eu_8065D138->src[2] = a->z;
        lbl_eu_8065D138->bbMax[2] = lbl_eu_8066AEC4;
        lbl_eu_8065D138->f5c = lbl_eu_8066AEC8;
        lbl_eu_8065D138->bbMin[0] = lbl_eu_8066AEC0;
        lbl_eu_8065D138->bbMin[1] = lbl_eu_8066AEC0;
        lbl_eu_8065D138->bbMin[2] = lbl_eu_8066AEC0;
        lbl_eu_8065D138->bbMax[0] = lbl_eu_8066AEC0;
        lbl_eu_8065D138->bbMax[1] = lbl_eu_8066AEC0;
        self->bbMin[0] = lbl_eu_8066AEC0;
        self->bbMin[1] = lbl_eu_8066AEC0;
        self->bbMin[2] = lbl_eu_8066AEC0;
        self->bbMax[0] = lbl_eu_8066AEC0;
        self->bbMax[1] = lbl_eu_8066AEC0;
        self->bbMax[2] = lbl_eu_8066AEC0;
    }

    char seg[0x18];
    char query[0x18];
    func_804A7ACC(seg, a, b, lbl_eu_8065D138);
    func_804B077C(query, a, b);

    int found = 0;
    CColiQueryNode* n = self->head;
    while (n != 0) {
        if (!(filter & n->filter)) {
            if (func_804B0818(query, n) != 0 &&
                func_804B21A8(n, seg, 1) != 0) {
                found = 1;
            }
        }
        n = n->next;
    }

    if (!init) return found;
    if (!found) return 0;
    if (!lbl_eu_8065D138->flag) return 0;
    self->bbMin[0] = lbl_eu_8065D138->bbMin[0];
    self->bbMin[1] = lbl_eu_8065D138->bbMin[1];
    self->bbMin[2] = lbl_eu_8065D138->bbMin[2];
    self->bbMax[0] = lbl_eu_8065D138->bbMax[0];
    self->bbMax[1] = lbl_eu_8065D138->bbMax[1];
    self->bbMax[2] = lbl_eu_8065D138->bbMax[2];
    return 1;
}
// Local CTTask (out-of-line Move/Draw/dtor) for harness stubs.
// Do not include monolib/work/CTTask.hpp here — its inline methods collide
// with the explicit out-of-line instantiations below.
template <typename T>
class CTTask : public CProcess {
public:
    typedef void (T::*MoveFunc)();
    typedef void (T::*DrawFunc)();

    CTTask();
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();

protected:
    //0x0-10: CDoubleListNode
    //0x10: vtable
    //0x14-3C: CProcess
    MoveFunc mMoveFunc; //0x3C (12-byte __ptmf)
    DrawFunc mDrawFunc; //0x48
};

// --- CTaskColiManager class definition ---
// Full class definition lives here (single-TU scope) because CTTask must
// be defined locally to avoid inline-method codegen from CTTask.hpp.
class CTaskColiManager : public CTTask<CTaskColiManager> {
public:
    // Overrides from CProcess (pure virtual)
    void Init() override;
    void Term() override;
    void Move() override;
    void Draw() override;

    virtual ~CTaskColiManager();
    static CTaskColiManager* create();

private:
    char pad_0x54[0x8];      //0x54
    CColiMoveNode* mHead;    //0x5C
    char pad_0x60[0x18];     //0x60
    u32 mFlags;              //0x78
};

extern "C" void func_804B0CE8(CColiMoveNode* node);
extern "C" void func_804B0DF4(CColiMoveNode* node);

extern "C" int lbl_eu_80665958;

void CTaskColiManager::Init() {
    // Empty override — no initialization required.
}

void CTaskColiManager::Term() {
    // Empty override — no termination required.
}

// Walk the collision-node list every tick: condition the nodes (func_804B0CE8),
// fold any node requesting a pass into mFlags bit 1, re-walk for post-pass
// cleanup (func_804B0DF4), then re-anchor at the front of the list.
void CTaskColiManager::Move() {
    mFlags &= ~0x3u;
    CColiMoveNode* node = mHead;
    if (node != 0) {
        do {
            func_804B0CE8(node);
            if (node->flags & 0x2000) {
                mFlags |= 0x2u;
            }
            node = node->next;
        } while (node != 0);
    }
    node = mHead;
    if (node != 0) {
        do {
            func_804B0DF4(node);
            node = node->next;
        } while (node != 0);
    }
    CColiMoveNode* front = mHead;
    CColiMoveNode* p = mHead->prev;
    while (p != 0) {
        front = p;
        p = p->prev;
    }
    mHead = front;
}

void CTaskColiManager::Draw() {
    // Empty override — no draw-time work required.
}

// Reset the shared walk flag then invoke the base (CProcess) destructor path.
CTaskColiManager::~CTaskColiManager() {
    lbl_eu_80665958 = 0;
}

void func_804B54D4(){}

void func_804B5658(){}

CTaskColiManager* CTaskColiManager::create() { return 0; }

// --- Explicit template specializations for CTTask<CTaskColiManager> ---
template<> CTTask<CTaskColiManager>::~CTTask() {}

template<> void CTTask<CTaskColiManager>::Move() {
    if (mMoveFunc) {
        (static_cast<CTaskColiManager*>(this)->*mMoveFunc)();
    }
}

template<> void CTTask<CTaskColiManager>::Draw() {
    if (mDrawFunc) {
        (static_cast<CTaskColiManager*>(this)->*mDrawFunc)();
    }
}