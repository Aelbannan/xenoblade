// Auto-scaffolded catalog TU for monolib/src/coli/code_804B2FF0
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>
#include "monolib/coli/CTaskColiManager.hpp"
// The shared header declares lbl_eu_8065D138 / lbl_eu_8056F4F0 as SDA-sized
// externs (4-byte pointer / 1-byte u8), but retail addresses both as full
// objects via lis/addi (ADDR16_HA/LO relocs). Rename the header's declarations
// aside and re-declare them at their full size below so MWCC keeps the 32-bit
// addressing the retail relocs require instead of SDA21. lbl_eu_8066AEC0 is
// re-declared const so MWCC can hoist its load past the aliasing stores (like
// a pooled literal) while still emitting the retail reloc name.
#define lbl_eu_8056F4F0 lbl_eu_8056F4F0_hdr
#define lbl_eu_8066AEC0 lbl_eu_8066AEC0_hdr
#define lbl_eu_8066AEB0 lbl_eu_8066AEB0_hdr
#define lbl_eu_8066AEC4 lbl_eu_8066AEC4_hdr
#define lbl_eu_8066AEC8 lbl_eu_8066AEC8_hdr
#include "monolib/coli/code_804B2FF0.hpp"
#undef lbl_eu_8056F4F0
#undef lbl_eu_8066AEC0
#undef lbl_eu_8066AEB0
#undef lbl_eu_8066AEC4
#undef lbl_eu_8066AEC8
#include "monolib/work/CTTask.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include <revolution/MTX.h>
#include <nw4r/math.h>
#include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/g3d/res/g3d_resnode.h>
#include <nw4r/db/db_assert.h>

// Full-size externs (retail: walk-state object at 0x8065D138, target table at
// 0x8056F4F0). Declared at global scope so MWCC emits the names unmangled.
extern u32 lbl_eu_8056F4F0[4];
extern f32 lbl_eu_8066AEC0;
extern const f32 lbl_eu_8066AEA8;
extern const f32 lbl_eu_8066AEB0;
extern const f32 lbl_eu_8066AEC4;
extern const f32 lbl_eu_8066AEC8;
extern const f32 lbl_eu_8066AEB8;

// C-linkage retail import (declared at global scope so MWCC emits the name
// unmangled; the definition lives in the sibling coli unit).
extern "C" void func_804A7ED0(void* self);
// Segment-query helpers imported from the sibling coli unit (stubs there).
extern "C" void func_804A7878(void*, const void*, int);
extern "C" void func_804B07F0(void*, const void*);
extern "C" int func_804B236C(const void*, const void*, const void*);
extern "C" void func_804A790C(void*, const Vec*, void*, f32);
extern "C" void func_804B06FC(void*, const Vec*, f32);
extern "C" int func_804B204C(const CColiQueryNode*, const void*, int);
// CProcess is abstract (pure virtuals), so the base ctor is invoked via its
// pre-mangled retail symbol like the sibling sinit helpers do.
extern "C" void __ct__8CProcessFv(CProcess* self);

// Panic file/format strings and va-arg node-name needles used by the nw4r
// node-lookup helpers below (global-scope C-linkage data names; the unsized
// arrays get 32-bit lis/addi addressing, the small ones sda21).
extern char lbl_eu_8056E194[];   // Panic file (node-null assert, line 0x2c)
extern char lbl_eu_8056E178[];   // Panic fmt
extern char lbl_eu_80529678[];   // Panic file (line 0x53 asserts)
extern char lbl_eu_80529658[];   // Panic fmt
extern char lbl_eu_80663910[8];  // assert va arg 1 (sda string)
extern char lbl_eu_80663AA8[8];  // assert va arg 2 (sda string)

// Model resource holder: nw4r ResMdl handle at +0x146C (node-table source).
struct CModelRes {
    char pad[0x146C];
    void* resMdl; //0x146C
};

// ---------------------------------------------------------------------------
// func_804B2FF0: move a collision-source position through a bone matrix twice,
// fold the delta into the object's accumulated offset, then add the linear
// offset component. Both stages end by notifying the target object. The
// delta/accumulate stages use the nw4r SDK inline ASM helpers so MWCC emits
// the retail psq_l/ps_sub/ps_add paired-single sequences.
// ---------------------------------------------------------------------------
struct CColiMtxSrc {
    Mtx mtx0;       //0x0
    char pad[0x60]; //0x30
    Mtx mtx1;       //0x90
};

struct CColiMover {
    char pad0[0xC];
    Vec pos;           //0xC
    char pad1[0xC];    //0x18
    Vec off;           //0x24
    char pad2[0x8];    //0x30
    Vec acc;           //0x38
    CColiMtxSrc* src;  //0x44
    void* target;      //0x48
};

void func_804B2FF0(CColiMover* self) {
    if (self->src != 0 && self->off.y > lbl_eu_8066AEA8) {
        Vec tmp;
        // First pass uses the +0x90 matrix, second pass the base matrix.
        PSMTXMultVec((const f32(*)[4])&self->src->mtx1[0], &self->pos, &tmp);
        PSMTXMultVec((const f32(*)[4])&self->src->mtx0[0], &tmp, &tmp);

        // Paired-single delta + accumulate: the nw4r SDK inline ASM helpers
        // emit the retail psq_l/ps_sub/ps_add sequences (MWCC reschedules the
        // loads first, matching the retail load-all-first shape).
        nw4r::math::VEC3Sub((nw4r::math::VEC3*)&tmp, (nw4r::math::VEC3*)&tmp,
                            (nw4r::math::VEC3*)&self->pos);
        nw4r::math::VEC3Add((nw4r::math::VEC3*)&self->acc,
                            (nw4r::math::VEC3*)&self->acc,
                            (nw4r::math::VEC3*)&tmp);
        func_804A7ED0(self->target);
    }

    // Block 2: retail materialises the off base into r4 (addi r31,0x24) for
    // the two off reads, so pass it through a named pointer local.
    const nw4r::math::VEC3* offp = (const nw4r::math::VEC3*)&self->off;
    nw4r::math::VEC3Add((nw4r::math::VEC3*)&self->acc,
                        (nw4r::math::VEC3*)&self->acc, offp);
    func_804A7ED0(self->target);
}

// Dual-index collision node: primary index + flags at 0x0/0x2 and a secondary
// index at 0x30 (both written by func_804B31EC).
struct CDualIndexNode {
    s16 index;      //0x0
    u16 flags;      //0x2
    char pad[0x2C]; //0x4
    s16 index2;     //0x30
};

// Second-name reference: a byte offset (+0x4) into the shared node-name table.
struct CNameRef {
    char pad[0x4];
    s32 offset; //0x4
};

// Shared node-name table base (retail loads the pointer via sda21).
extern char* lbl_eu_80665940;

// ---------------------------------------------------------------------------
// func_804B31EC: scan the model's node table for up to two node names - the
// caller's `name` and a second name taken from the shared name table - and
// record each match's matrix id into self->index / self->index2. Both found
// (flags == 3) exits early; unmatched indices fall back to 0 / 1.
// ---------------------------------------------------------------------------
void func_804B31EC(CDualIndexNode* self, const char* name,
                   const CModelRes* model, const CNameRef* ref) {
    const char* name2 = lbl_eu_80665940 + ref->offset;
    int num = (int)nw4r::g3d::ResMdl(model->resMdl).GetResNodeNumEntries();
    u32 flags = 0;
    self->flags = (u16)flags;
    for (int i = 0; i < num; i++) {
        nw4r::g3d::ResNode node =
            nw4r::g3d::ResMdl(model->resMdl).GetResNode(i);
        if (!(flags & 1)) {
            if (!node.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                                lbl_eu_80663910, lbl_eu_80663AA8);
            }
            if (strcmp(name, node.ofs_to_ptr<char>(node.ref().name)) == 0) {
                if (!node.IsValid()) {
                    nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
                }
                self->index = (s16)node.GetMtxID();
                flags |= 1;
                if (flags == 3) return;
            }
        }
        if (!(flags & 2)) {
            if (!node.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                                lbl_eu_80663910, lbl_eu_80663AA8);
            }
            if (strcmp(name2, node.ofs_to_ptr<char>(node.ref().name)) == 0) {
                if (!node.IsValid()) {
                    nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
                }
                self->index2 = (s16)node.GetMtxID();
                flags |= 2;
                if (flags == 3) return;
            }
        }
    }
    if (!(flags & 1)) self->index = 0;
    if (!(flags & 2)) self->index2 = 1;
}

void func_804B4E10(){}

void func_804B526C(){}

// Global axis-aligned bounding box shared by the coli object-space
// bounds-expansion helpers. Layout: max x/y/z at +0x0/+0x4/+0x8,
// min x/y/z at +0xC/+0x10/+0x14.
struct CColiBounds {
    f32 max[3]; //0x0 max x/y/z
    f32 min[3]; //0xC min x/y/z
};

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
    // min corner by its min extent, per axis. Each check reloads the radius
    // temp first (retail does the same - the bound load reuses the FPR, so
    // the radius is re-fetched at the top of every check).
    {
        f32 r = p->radius;
        if (lbl_eu_80665944->max[0] < p->x + r) {
            lbl_eu_80665944->max[0] = p->x + r;
        }
    }
    {
        f32 r = p->radius;
        if (lbl_eu_80665944->min[0] > p->x - r) {
            lbl_eu_80665944->min[0] = p->x - r;
        }
    }
    {
        f32 r = p->radius;
        if (lbl_eu_80665944->max[1] < p->y + r) {
            lbl_eu_80665944->max[1] = p->y + r;
        }
    }
    {
        f32 r = p->radius;
        if (lbl_eu_80665944->min[1] > p->y - r) {
            lbl_eu_80665944->min[1] = p->y - r;
        }
    }
    {
        f32 r = p->radius;
        if (lbl_eu_80665944->max[2] < p->z + r) {
            lbl_eu_80665944->max[2] = p->z + r;
        }
    }
    {
        f32 r = p->radius;
        if (lbl_eu_80665944->min[2] > p->z - r) {
            lbl_eu_80665944->min[2] = p->z - r;
        }
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
        // Per-sub-block radius temp: retail reloads it at the top of every
        // sub-block (the bound loads reuse the FPR, alternating f1/f2).
        {
            f32 r = p->radius;
            if (lbl_eu_80665944->max[i] < p->radius + p->endPos[i]) {
                lbl_eu_80665944->max[i] = p->radius + p->endPos[i];
            }
        }
        {
            f32 r = p->radius;
            if (lbl_eu_80665944->min[i] > p->endPos[i] - r) {
                lbl_eu_80665944->min[i] = p->endPos[i] - r;
            }
        }
        {
            f32 r = p->radius;
            if (lbl_eu_80665944->max[i] < p->radius + p->endPosB[i]) {
                lbl_eu_80665944->max[i] = p->radius + p->endPosB[i];
            }
        }
        {
            f32 r = p->radius;
            if (lbl_eu_80665944->min[i] > p->endPosB[i] - r) {
                lbl_eu_80665944->min[i] = p->endPosB[i] - r;
            }
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
    // Per-sub-block radius temp: retail reloads it at the top of every check
    // (the bound loads reuse the FPR, so the radius is re-fetched per block).
    {
        f32 r = p->radius;
        if (lbl_eu_80665944->max[0] < p->x + r) {
            lbl_eu_80665944->max[0] = p->x + r;
        }
    }
    {
        f32 r = p->radius;
        if (lbl_eu_80665944->min[0] > p->x - r) {
            lbl_eu_80665944->min[0] = p->x - r;
        }
    }
    {
        f32 r = p->radius;
        if (lbl_eu_80665944->max[1] < p->y + r) {
            lbl_eu_80665944->max[1] = p->y + r;
        }
    }
    {
        f32 r = p->radius;
        if (lbl_eu_80665944->min[1] > p->y - r) {
            lbl_eu_80665944->min[1] = p->y - r;
        }
    }
    {
        f32 r = p->radius;
        if (lbl_eu_80665944->max[2] < p->z + r) {
            lbl_eu_80665944->max[2] = p->z + r;
        }
    }
    {
        f32 r = p->radius;
        if (lbl_eu_80665944->min[2] > p->z - r) {
            lbl_eu_80665944->min[2] = p->z - r;
        }
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
    // Per-sub-block radius temp: retail reloads it at the top of every check
    // (the bound loads reuse the FPR, so the radius is re-fetched per block).
    {
        f32 r = p->radius;
        if (lbl_eu_80665944->max[0] < p->x + r) {
            lbl_eu_80665944->max[0] = p->x + r;
        }
    }
    {
        f32 r = p->radius;
        if (lbl_eu_80665944->min[0] > p->x - r) {
            lbl_eu_80665944->min[0] = p->x - r;
        }
    }
    {
        f32 r = p->radius;
        if (lbl_eu_80665944->max[1] < p->y + r) {
            lbl_eu_80665944->max[1] = p->y + r;
        }
    }
    {
        f32 r = p->radius;
        if (lbl_eu_80665944->min[1] > p->y - r) {
            lbl_eu_80665944->min[1] = p->y - r;
        }
    }
    {
        f32 r = p->radius;
        if (lbl_eu_80665944->max[2] < p->z + r) {
            lbl_eu_80665944->max[2] = p->z + r;
        }
    }
    {
        f32 r = p->radius;
        if (lbl_eu_80665944->min[2] > p->z - r) {
            lbl_eu_80665944->min[2] = p->z - r;
        }
    }
}

// Half-position (x/y) and quarter-extent (x/y/z) bound components from
// two additional globals combined into the global box.
struct CColiHx { f32 ax; f32 ay; f32 f8; };        //0x0/0x4/0x8
struct CColiQx { f32 qx; f32 qy; f32 qz; }; //0x0/0x4/0x8

// sdata2 literals and the shared "master node index" used by the
// bounds/transform helpers.

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

// ---------------------------------------------------------------------------
// func_804B30CC: scan the model's node table for the node named `name` and
// store its matrix id as the collision node's bone index. On failure, clear
// the index and set flag bit 1 so the matrix-source selectors fall back to
// the root matrix.
// ---------------------------------------------------------------------------
void func_804B30CC(CColiHead* self, const char* name, const CModelRes* model) {
    self->flags = 0;
    int num = (int)nw4r::g3d::ResMdl(model->resMdl).GetResNodeNumEntries();
    for (int i = 0; i < num; i++) {
        nw4r::g3d::ResNode node =
            nw4r::g3d::ResMdl(model->resMdl).GetResNode(i);
        if (!node.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_80663AA8);
        }
        if (strcmp(name, node.ofs_to_ptr<char>(node.ref().name)) == 0) {
            if (!node.IsValid()) {
                nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
            }
            self->index = (s16)node.GetMtxID();
            return;
        }
    }
    self->index = 0;
    self->flags |= 2;
}

// ---------------------------------------------------------------------------
// func_804B34F4: copy the source matrix's translation column into the
// sphere's position, derive the sphere radius from the transformed
// (x, 0, 0) normal, and expand the global AABB around the new position.
// ---------------------------------------------------------------------------
void func_804B34F4(CColiSphereOb* self, const CColiMgr* mgr, const Vec* other) {
    f32 (*src)[4];
    if (self->hd.flags & 2) {
        src = mgr->mesh->rootMtx;
    } else {
        src = mgr->mesh->boneTable[self->hd.index];
    }

    const f32 pz = src[2][3];
    const f32 py = src[1][3];
    const f32 px = src[0][3];
    self->pos[0] = px;
    self->pos[1] = py;
    self->pos[2] = pz;

    nw4r::math::VEC3 n;
    n.x = other->x;
    n.y = lbl_eu_8066AEB0;
    n.z = lbl_eu_8066AEB0;
    nw4r::math::VEC3TransformNormal(&n, (const nw4r::math::MTX34*)src, &n);
    f32 mag = PSVECMag((const Vec*)&n);

    self->radius = mag;
    self->hd.flags |= 1;

    if (lbl_eu_80665944->max[0] < self->pos[0] + self->radius)
        lbl_eu_80665944->max[0] = self->pos[0] + self->radius;
    if (lbl_eu_80665944->min[0] > self->pos[0] - self->radius)
        lbl_eu_80665944->min[0] = self->pos[0] - self->radius;
    if (lbl_eu_80665944->max[1] < self->pos[1] + self->radius)
        lbl_eu_80665944->max[1] = self->pos[1] + self->radius;
    if (lbl_eu_80665944->min[1] > self->pos[1] - self->radius)
        lbl_eu_80665944->min[1] = self->pos[1] - self->radius;
    if (lbl_eu_80665944->max[2] < self->pos[2] + self->radius)
        lbl_eu_80665944->max[2] = self->pos[2] + self->radius;
    if (lbl_eu_80665944->min[2] > self->pos[2] - self->radius)
        lbl_eu_80665944->min[2] = self->pos[2] - self->radius;
}

void func_804B4020(CColiMtxNode* self, const CColiMgr* mgr,
                   const CColiSphereOb* other) {
    // Select the matrix source: the flagged root (identity/bind) matrix, or
    // the per-node bone table entry addressed by the signed bone index.
    const f32 (*src)[4];
    if (self->hd.flags & 2) {
        src = mgr->mesh->rootMtx;
    } else {
        src = mgr->mesh->boneTable[self->hd.index];
    }

    // Copy the source matrix word-by-word into self-space (retail keeps the
    // temp on the stack), then derive inverse world from the same source.
    CObjMtx tmp;
    PSMTXCopy(src, tmp.m);
    self->mtxW = tmp;
    PSMTXInverse(src, self->invW.m);

    // Build a (radius, 0, 0) normal in world space and record its magnitude
    // as the node's scalar radius.
    Vec n;
    n.x = other->radius;
    n.y = lbl_eu_8066AEB0;
    n.z = lbl_eu_8066AEB0;
    nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&n,
                                    (const nw4r::math::MTX34*)&self->mtxW,
                                    (const nw4r::math::VEC3*)&n);
    f32 mag = PSVECMag((const Vec*)&n);

    self->radius = mag;
    self->hd.flags |= 1;

    // Expand the global box around each translation-column axis by the radius;
    // every check re-reads the shared bounds pointer and the radius through a
    // per-check local (retail reloads both per check; the first check's local
    // is store-forwarded to the live magnitude via frsp).
    {
        f32 r = self->radius;
        if (lbl_eu_80665944->max[0] < self->mtxW.m[0][3] + r)
            lbl_eu_80665944->max[0] = self->mtxW.m[0][3] + r;
    }
    {
        f32 r = self->radius;
        if (lbl_eu_80665944->min[0] > self->mtxW.m[0][3] - r)
            lbl_eu_80665944->min[0] = self->mtxW.m[0][3] - r;
    }
    {
        f32 r = self->radius;
        if (lbl_eu_80665944->max[1] < self->mtxW.m[1][3] + r)
            lbl_eu_80665944->max[1] = self->mtxW.m[1][3] + r;
    }
    {
        f32 r = self->radius;
        if (lbl_eu_80665944->min[1] > self->mtxW.m[1][3] - r)
            lbl_eu_80665944->min[1] = self->mtxW.m[1][3] - r;
    }
    {
        f32 r = self->radius;
        if (lbl_eu_80665944->max[2] < self->mtxW.m[2][3] + r)
            lbl_eu_80665944->max[2] = self->mtxW.m[2][3] + r;
    }
    {
        f32 r = self->radius;
        if (lbl_eu_80665944->min[2] > self->mtxW.m[2][3] - r)
            lbl_eu_80665944->min[2] = self->mtxW.m[2][3] - r;
    }
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

// ---------------------------------------------------------------------------
// func_804B33C8: node-lookup sibling of func_804B30CC - records the matched
// node's matrix id into the shared master-node index and folds a source
// vector's x/y/z into the shared half-extent (ay = y - z).
// ---------------------------------------------------------------------------
void func_804B33C8(CColiHead* self, const char* name, const CModelRes* model,
                   const CColiPosVec* v) {
    int num = (int)nw4r::g3d::ResMdl(model->resMdl).GetResNodeNumEntries();
    for (int i = 0; i < num; i++) {
        nw4r::g3d::ResNode node =
            nw4r::g3d::ResMdl(model->resMdl).GetResNode(i);
        if (!node.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_80663AA8);
        }
        if (strcmp(name, node.ofs_to_ptr<char>(node.ref().name)) == 0) {
            if (!node.IsValid()) {
                nw4r::db::Panic(lbl_eu_80529678, 0x53, lbl_eu_80529658);
            }
            lbl_eu_80665950 = (s16)node.GetMtxID();
            lbl_eu_8066594C->ax = v->v.x;
            lbl_eu_8066594C->ay = v->v.y - v->v.z;
            return;
        }
    }
}

// Matrix output object (func_804B4854 / func_804B49B8): a matrix pair at
// +0x0 (m0 + inverse m1), duplicated up by one pair into the history slots
// at +0x60. CObjMtx members keep the copies as word loads/stores.
struct CColiMtxPair {
    CObjMtx m0; //0x0
    CObjMtx m1; //0x30
};

struct CColiMtxOut {
    CColiMtxPair cur; //0x0
    CColiMtxPair old; //0x60
};

// ---------------------------------------------------------------------------
// func_804B4854: shift the previous matrix pair up into the history slots,
// then copy the bone matrix into m0 and derive its inverse into m1.
// ---------------------------------------------------------------------------
void func_804B4854(CColiHead* hd, CColiMtxOut* dst, CColiMgr* mgr) {
    dst->old = dst->cur;
    const CObjMtx* src = (const CObjMtx*)mgr->mesh->boneTable[hd->index];
    dst->cur.m0 = *src;
    PSMTXInverse(dst->cur.m0.m, dst->cur.m1.m);
}

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
// func_804B3B18: update a capsule collision node. Concat the source (root or
// bone) matrix with the rigid reference, transform the (0,1,0) axis by the
// result to get the endpoints A = T + v / B = T - v, derive the segment
// direction + squared length via the nw4r PS helpers, scale the transformed
// (1,0,0) axis magnitude into the radius, then expand the global AABB around
// both endpoints swept by the radius.
// ---------------------------------------------------------------------------
void func_804B3B18(CColiCapsuleNode* self, const CColiMgr* mgr,
                   const CColiRigid* other) {
    const f32 (*src)[4];
    if (self->hd.flags & 2) {
        src = mgr->mesh->rootMtx;
    } else {
        src = mgr->mesh->boneTable[self->hd.index];
    }

    Mtx tmp;
    PSMTXConcat(src, other->mtx, tmp);

    Vec v;
    v.x = lbl_eu_8066AEB0;
    v.y = lbl_eu_8066AEB8;
    v.z = lbl_eu_8066AEB0;
    nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&v,
                                    (const nw4r::math::MTX34*)tmp,
                                    (const nw4r::math::VEC3*)&v);

    self->posA[2] = tmp[2][3] + v.z;
    self->posA[1] = tmp[1][3] + v.y;
    self->posA[0] = tmp[0][3] + v.x;
    self->posB[2] = tmp[2][3] - v.z;
    self->posB[1] = tmp[1][3] - v.y;
    self->posB[0] = tmp[0][3] - v.x;

    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&self->dir,
                        (const nw4r::math::VEC3*)&self->posB,
                        (const nw4r::math::VEC3*)&self->posA);
    self->lenSq = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&self->dir,
                                      (const nw4r::math::VEC3*)&self->dir);

    v.x = lbl_eu_8066AEB4;
    v.y = lbl_eu_8066AEB0;
    v.z = lbl_eu_8066AEB0;
    nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&v,
                                    (const nw4r::math::MTX34*)tmp,
                                    (const nw4r::math::VEC3*)&v);
    self->radius = lbl_eu_8066AEB8 * PSVECMag((const Vec*)&v);
    self->hd.flags |= 1;

    // Expand the global AABB around both endpoints swept by the radius, one
    // axis per counted-loop iteration (retail reloads the bounds pointer for
    // every check, so dereference the global directly).
    for (int i = 0; i < 3; i++) {
        if (lbl_eu_80665944->max[i] < self->posA[i] + self->radius)
            lbl_eu_80665944->max[i] = self->posA[i] + self->radius;
        if (lbl_eu_80665944->min[i] > self->posA[i] - self->radius)
            lbl_eu_80665944->min[i] = self->posA[i] - self->radius;
        if (lbl_eu_80665944->max[i] < self->posB[i] + self->radius)
            lbl_eu_80665944->max[i] = self->posB[i] + self->radius;
        if (lbl_eu_80665944->min[i] > self->posB[i] - self->radius)
            lbl_eu_80665944->min[i] = self->posB[i] - self->radius;
    }
}

// ---------------------------------------------------------------------------
// func_804B41FC: transform a position by the fixed bone matrix, derive an
// ellipsoid-ish half-extent into the shared Qx/Hx globals, and fold them
// into the global AABB.
// ---------------------------------------------------------------------------
void func_804B41FC(const CColiNode* unused, const CColiMgr* mgr,
                   const CColiPosVec* other) {
    const s16 idx = lbl_eu_80665950;
    const f32 (*src)[4] = mgr->mesh->boneTable[idx];

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
    if (lbl_eu_80665944->max[2] < lbl_eu_80665948->qz + lbl_eu_8066594C->ay)
        lbl_eu_80665944->max[2] = lbl_eu_80665948->qz + lbl_eu_8066594C->ay;
    if (lbl_eu_80665944->min[2] > lbl_eu_80665948->qz - lbl_eu_8066594C->ay)
        lbl_eu_80665944->min[2] = lbl_eu_80665948->qz - lbl_eu_8066594C->ay;
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

    // Retail loads the other-space position back-to-front (z, y, x) while
    // allocating registers in forward declaration order, so declare the
    // components forward and let MWCC emit the loads reversed.
    const f32 px = other->pos[0];
    const f32 py = other->pos[1];
    const f32 pz = other->pos[2];
    self->vec[0] = px;
    self->vec[1] = py;
    self->vec[2] = pz;

    nw4r::math::VEC3 n;
    nw4r::math::VEC3TransformNormal(&n, (const nw4r::math::MTX34*)self->mtxW,
                                    (const nw4r::math::VEC3*)self->vec);

    self->radius = PSVECMag((const Vec*)&n);
    self->hd.flags |= 1;

    // Expand the shared AABB around the world matrix translation column;
    // every check re-dereferences the shared bounds pointer and re-reads the
    // radius through a per-check local (retail reloads both per check; the
    // first check's local is store-forwarded to the live magnitude via frsp).
    {
        f32 r = self->radius;
        if (lbl_eu_80665944->max[0] < self->mtxW[0][3] + r)
            lbl_eu_80665944->max[0] = self->mtxW[0][3] + r;
    }
    {
        f32 r = self->radius;
        if (lbl_eu_80665944->min[0] > self->mtxW[0][3] - r)
            lbl_eu_80665944->min[0] = self->mtxW[0][3] - r;
    }
    {
        f32 r = self->radius;
        if (lbl_eu_80665944->max[1] < self->mtxW[1][3] + r)
            lbl_eu_80665944->max[1] = self->mtxW[1][3] + r;
    }
    {
        f32 r = self->radius;
        if (lbl_eu_80665944->min[1] > self->mtxW[1][3] - r)
            lbl_eu_80665944->min[1] = self->mtxW[1][3] - r;
    }
    {
        f32 r = self->radius;
        if (lbl_eu_80665944->max[2] < self->mtxW[2][3] + r)
            lbl_eu_80665944->max[2] = self->mtxW[2][3] + r;
    }
    {
        f32 r = self->radius;
        if (lbl_eu_80665944->min[2] > self->mtxW[2][3] - r)
            lbl_eu_80665944->min[2] = self->mtxW[2][3] - r;
    }
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
    // Retail emits the loads back-to-front (z, y, x); declare them reversed
    // so MWCC emits them in that order.
    const f32 pz = outv.z;
    const f32 py = outv.y;
    const f32 px = outv.x;
    self->pos[0] = px;
    self->pos[1] = py;
    self->pos[2] = pz;

    nw4r::math::VEC3 n;
    n.x = other->z;
    n.y = lbl_eu_8066AEB0;
    n.z = lbl_eu_8066AEB0;
    nw4r::math::VEC3TransformNormal(&n, (const nw4r::math::MTX34*)src, &n);

    self->radius = PSVECMag((const Vec*)&n);
    self->hd.flags |= 1;

    // Expand the shared AABB around the new position; every check
    // re-dereferences the shared bounds pointer and re-reads the radius
    // through a per-check local (retail reloads both per check; the first
    // check's local is store-forwarded to the live magnitude via frsp).
    {
        f32 r = self->radius;
        if (lbl_eu_80665944->max[0] < self->pos[0] + r)
            lbl_eu_80665944->max[0] = self->pos[0] + r;
    }
    {
        f32 r = self->radius;
        if (lbl_eu_80665944->min[0] > self->pos[0] - r)
            lbl_eu_80665944->min[0] = self->pos[0] - r;
    }
    {
        f32 r = self->radius;
        if (lbl_eu_80665944->max[1] < self->pos[1] + r)
            lbl_eu_80665944->max[1] = self->pos[1] + r;
    }
    {
        f32 r = self->radius;
        if (lbl_eu_80665944->min[1] > self->pos[1] - r)
            lbl_eu_80665944->min[1] = self->pos[1] - r;
    }
    {
        f32 r = self->radius;
        if (lbl_eu_80665944->max[2] < self->pos[2] + r)
            lbl_eu_80665944->max[2] = self->pos[2] + r;
    }
    {
        f32 r = self->radius;
        if (lbl_eu_80665944->min[2] > self->pos[2] - r)
            lbl_eu_80665944->min[2] = self->pos[2] - r;
    }
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

    // Retail loads the source translation back-to-front (z, y, x) while
    // storing forward, so pass the components through z/y/x locals.
    const f32 pz = src[2][3];
    const f32 py = src[1][3];
    const f32 px = src[0][3];
    self->pos[0] = px;
    self->pos[1] = py;
    self->pos[2] = pz;

    nw4r::math::VEC3 n;
    n.x = lbl_eu_8066AEB4;
    n.y = lbl_eu_8066AEB0;
    n.z = lbl_eu_8066AEB0;
    nw4r::math::VEC3TransformNormal(&n, (const nw4r::math::MTX34*)src, &n);

    double mag = PSVECMag((const Vec*)&n);
    self->f18 = other->z * mag;
    self->radiusY = other->y * mag;
    self->radiusXZ = other->x * mag;
    self->hd.flags |= 1;

    // Expand the shared AABB with the horizontal (XZ) and vertical (Y)
    // radii; every check re-dereferences the shared bounds pointer and
    // re-reads the radius through a per-check local (retail reloads after
    // the first check, which reuses the live scaled value).
    {
        f32 r = self->radiusXZ;
        if (lbl_eu_80665944->max[0] < self->pos[0] + r)
            lbl_eu_80665944->max[0] = self->pos[0] + r;
    }
    {
        f32 r = self->radiusXZ;
        if (lbl_eu_80665944->min[0] > self->pos[0] - r)
            lbl_eu_80665944->min[0] = self->pos[0] - r;
    }
    {
        f32 r = self->radiusXZ;
        if (lbl_eu_80665944->max[2] < self->pos[2] + r)
            lbl_eu_80665944->max[2] = self->pos[2] + r;
    }
    {
        f32 r = self->radiusXZ;
        if (lbl_eu_80665944->min[2] > self->pos[2] - r)
            lbl_eu_80665944->min[2] = self->pos[2] - r;
    }
    {
        f32 r = self->radiusY;
        if (lbl_eu_80665944->max[1] < self->pos[1] + r)
            lbl_eu_80665944->max[1] = self->pos[1] + r;
    }
    {
        f32 r = self->radiusY;
        if (lbl_eu_80665944->min[1] > self->pos[1] - r)
            lbl_eu_80665944->min[1] = self->pos[1] - r;
    }
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
    // Retail emits the loads back-to-front (z, y, x); declare them reversed
    // so MWCC emits them in that order.
    const f32 pz = outv.z;
    const f32 py = outv.y;
    const f32 px = outv.x;
    self->pos[0] = px;
    self->pos[1] = py;
    self->pos[2] = pz;

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

    // Every check re-reads the shared bounds pointer and the self fields
    // (retail reloads both per check).
    if (lbl_eu_80665944->max[0] < self->pos[0] + self->radiusXZ)
        lbl_eu_80665944->max[0] = self->pos[0] + self->radiusXZ;
    if (lbl_eu_80665944->min[0] > self->pos[0] - self->radiusXZ)
        lbl_eu_80665944->min[0] = self->pos[0] - self->radiusXZ;
    if (lbl_eu_80665944->max[2] < self->pos[2] + self->radiusXZ)
        lbl_eu_80665944->max[2] = self->pos[2] + self->radiusXZ;
    if (lbl_eu_80665944->min[2] > self->pos[2] - self->radiusXZ)
        lbl_eu_80665944->min[2] = self->pos[2] - self->radiusXZ;
    if (lbl_eu_80665944->max[1] < self->pos[1] + self->radiusY)
        lbl_eu_80665944->max[1] = self->pos[1] + self->radiusY;
    if (lbl_eu_80665944->min[1] > self->pos[1] - self->radiusY)
        lbl_eu_80665944->min[1] = self->pos[1] - self->radiusY;
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
// func_804B49B8: copy the bone matrix from the manager table into m0,
// derive its inverse into m1, then duplicate both up by one pair into the
// history slots.
// ---------------------------------------------------------------------------
void func_804B49B8(CColiHead* hd, CColiMtxOut* dst, CColiMgr* mgr) {
    const s16 idx = hd->index;
    const CObjMtx* src = (const CObjMtx*)mgr->mesh->boneTable[idx];
    dst->cur.m0 = *src;
    PSMTXInverse(dst->cur.m0.m, dst->cur.m1.m);
    dst->old = dst->cur;
}

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
        node->f_a4 = lbl_eu_8066AEC0;
    } else {
        self->head = node;
        node->next = 0;
        node->prev = 0;
        node->f_a4 = lbl_eu_8066AEC0;
    }
}

// Remove `findNode` from the list owned by `self`, relinking neighbours and
// fixing the head when the removed node was the head. Goto-shaped to match
// the retail control flow: pre-check on the head value, entry jump to the
// tail condition, and the found path falling out to the common epilogue.
void func_804B4C7C(CColiList* self, CColiListItem* findNode) {
    func_804B1DC0(findNode, 0);
    // Declared at function top so the allocator colours prev/next before the
    // loop variable: retail keeps cur in r4 with prev/next sharing r3.
    CColiListItem* prev;
    CColiListItem* next;
    CColiListItem* cur;
    // `head` stays loop-invariant (r0) for the final branch's comparison;
    // the middle branches re-read self->head because the relink stores may
    // alias it. `cur` is assigned after the pre-check like retail.
    CColiListItem* head = self->head;
    if (head == 0) goto done;
    cur = head;
    goto cond;
body:
    if (findNode == cur) {
        prev = cur->prev;
        next = cur->next;
        if (prev != 0) {
            prev->next = next;
            if (next != 0) next->prev = prev;
            if (self->head == findNode) self->head = prev;
            goto done;
        }
        if (next != 0) {
            next->prev = 0;
            if (self->head == findNode) self->head = next;
            goto done;
        }
        if (head == findNode) self->head = 0;
        goto done;
    }
advance:
    cur = cur->next;
cond:
    if (cur != 0) goto body;
done:
    return;
}

// Shared per-sector query structures used by the walk funcs below.
// Node in a spatially-split list: an x-sweep bound pair at +0xC/+0x18, a
// forward link at +0x9C, a backward link at +0xA0, a scalar at +0xA4,
// status flags at +0xA8, a 32-bit filter mask at +0xAC and a level index
// at +0xB2.
struct CColiQueryNode {
    char padA[0xC];        //0x0
    f32 minX;              //0xC
    char padB[0x8];        //0x10
    f32 maxX;              //0x18
    char padC[0x80];       //0x1C
    CColiQueryNode* next;  //0x9C
    CColiQueryNode* prev;  //0xA0
    f32 f_a4;              //0xA4
    u32 flags;             //0xA8
    u32 filter;            //0xAC
    u16 field_0xB0;        //0xB0
    u16 level;             //0xB2
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
// 3-word raw snapshot (u32 members force lwz/stw word copies like retail).
struct CColiSrcVec {
    u32 x, y, z;
};

struct CColiWalkState {
    char pad0[0x38];
    f32 bbMin[3];  //0x38
    f32 bbMax[3];  //0x44
    CColiSrcVec src; //0x50
    f32 f5c;       //0x5C
    u8 flag;       //0x60
};

// Static initializer (.ctors) for the shared collision walk object at
// lbl_eu_8065D138: run the CColiProc base constructor, then point its default
// target data at the alternate table lbl_eu_8056F4F0.
struct CColiProcLocal {
    void* field_0x0; //0x0
    u32 field_0x4;   //0x4
};

extern "C" void sinit_804B598C() {
    // Placement-construct CColiProc over the walk-state object's storage,
    // then point its default target data at the alternate table. The address
    // is taken twice, so MWCC keeps only the HA half in r31 and folds the LO
    // half into each use (call arg / store displacement) like retail.
    __ct__CColiProc((CColiProcLocal*)&lbl_eu_8065D138);
    ((CColiProcLocal*)&lbl_eu_8065D138)->field_0x0 = &lbl_eu_8056F4F0;
}

// Walk/traverse helpers (C-linkage retail symbols in the sibling coli unit).

// Walk the segment query: reject degenerate (equal) vectors, otherwise run
// the segment build + node query and expand the result AABB.
int func_804B5088(CColiQueryResult* self, const Vec* a, const Vec* b,
                   u32 filter, int isFirst) {
    // Materialized boolean: retail keeps the && chain result in a temp
    // (li r0,0 / li r0,1 / cmpwi r0,0) instead of short-circuiting straight
    // to the return.
    int eq = a->x == b->x && a->y == b->y && a->z == b->z;
    if (eq) return 0;

    // Walk state arg: retail reuses the incoming isFirst register (r7) for
    // the pointer, so a non-first call passes the raw isFirst value through.
    CColiWalkState* st = (CColiWalkState*)(u32)isFirst;
    int init = 0;
    if (isFirst == 0) {
        st = &lbl_eu_8065D138;
        st->flag = 0;
        init = 1;
        st->src = *(const CColiSrcVec*)a;
        st->bbMax[2] = lbl_eu_8066AEC4;
        st->f5c = lbl_eu_8066AEC8;
        f32 zero = lbl_eu_8066AEC0;
        st->bbMin[0] = zero;
        st->bbMin[1] = zero;
        st->bbMin[2] = zero;
        st->bbMax[0] = zero;
        st->bbMax[1] = zero;
        self->bbMin[0] = zero;
        self->bbMin[1] = zero;
        self->bbMin[2] = zero;
        self->bbMax[0] = zero;
        self->bbMax[1] = zero;
        self->bbMax[2] = zero;
    }

    char seg[0x310];
    char query[0x18];
    func_804A7ACC(seg, a, b, st);
    int found = 0;
    func_804B077C(query, a, b);

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
    if (!lbl_eu_8065D138.flag) return 0;
    // Word copies (lwz/stw) of the accumulated AABB, like the src snapshot:
    // 3-word struct copies keep the retail store order (y before x).
    *(CColiSrcVec*)&self->bbMin[0] =
        *(const CColiSrcVec*)&lbl_eu_8065D138.bbMin[0];
    *(CColiSrcVec*)&self->bbMax[0] =
        *(const CColiSrcVec*)&lbl_eu_8065D138.bbMax[0];
    return 1;
}
// CRTP task base — canonical monolib template (declared-only members so the
// explicit out-of-line specializations below emit the retail Move/Draw/dtor
// symbols). The full class definition lives here (single-TU scope) because
// CTaskColiManager's full layout is only needed in this TU.
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

private:
    char pad_0x54[0x8];      //0x54
    CColiMoveNode* mHead;    //0x5C
    char pad_0x60[0x18];     //0x60
    u32 mFlags;              //0x78
};

void CTaskColiManager::Init() {
    // Empty override - no initialization required.
}

void CTaskColiManager::Term() {
    // Empty override - no termination required.
}

// Walk the collision-node list every tick: condition the nodes (func_804B0CE8),
// fold any node requesting a pass into mFlags bit 1, re-walk for post-pass
// cleanup (func_804B0DF4), then re-anchor at the front of the list.
void CTaskColiManager::Move() {
    CColiMoveNode* node = mHead;
    mFlags &= ~0x2u;
    if (node != 0) {
        while (node != 0) {
            func_804B0CE8(node);
            if (node->flags & 0x2000) {
                mFlags |= 0x2u;
            }
            node = node->next;
        }
        node = mHead;
        while (node != 0) {
            func_804B0DF4(node);
            node = node->next;
        }
        CColiMoveNode* front = mHead;
        CColiMoveNode* p = mHead->prev;
        while (p != 0) {
            front = p;
            p = p->prev;
        }
        // Volatile final store: retail restores LR first in the epilogue;
        // MWCC only emits that order when the function's last store is
        // volatile (MWCC_CASES, CriWare adx_mwii pattern).
        CColiMoveNode* volatile* mhp = &mHead;
        *mhp = front;
    }
}

void CTaskColiManager::Draw() {
    // Empty override - no draw-time work required.
}

// Reset the shared walk flag then invoke the base (CProcess) destructor path.
CTaskColiManager::~CTaskColiManager() {
    lbl_eu_80665958 = 0;
}

// Segment source (two endpoints) fed to the query builders.
struct CColiSeg {
    Vec a; //0x0
    Vec b; //0xC
};

// Sweep a query segment over the node list: walk forward from self->next
// while the segment start is past each node's max bound, then backward from
// self->prev while the segment end is before each node's min bound. A node
// that survives the filter/bounds/level checks and both query tests wins.
int func_804B54D4(u32 unused, CColiQueryNode* self, const CColiSeg* src,
                  u32 mask, void* arg) {
    if (!(self->flags & 4) || !(self->flags & 2)) return 0;

    u8 big[0x320];
    Vec query[2];

    func_804A7878(big, src, 0);
    func_804B07F0(query, src);

    for (CColiQueryNode* n = self->next; n != 0; n = n->next) {
        if (mask & n->filter) continue;
        // Only nodes whose max bound is at/below the segment start qualify.
        if (query[0].x >= n->maxX) {
            if ((int)self->level > (int)n->level) continue;
            if (func_804B0818((const void*)query, n) == 0) continue;
            if (func_804B236C(n, big, arg) == 0) continue;
            return 1;
        }
    }

    for (CColiQueryNode* n = self->prev; n != 0; n = n->prev) {
        if (mask & n->filter) continue;
        if (query[1].x <= n->minX) {
            if ((int)self->level > (int)n->level) continue;
            if (func_804B0818((const void*)query, n) == 0) continue;
            if (func_804B236C(n, big, arg) == 0) continue;
            return 1;
        }
        // Sweep stop: past the node's tail, abandon the search (falls out of
        // the loop to the shared return 0, like retail).
        if (query[1].x > n->f_a4) break;
    }
    return 0;
}

// Owner of a segment walk: node-list head at +0x5C and a control flags word
// at +0x78 (bit 1 gates the walk).
struct CColiWalkOwner {
    char pad0[0x5C];
    CColiQueryNode* head; //0x5C
    char pad1[0x18];      //0x60
    u32 flags;            //0x78
};

// ---------------------------------------------------------------------------
// func_804B5658: gate on the owner's walk-enable flag, reject degenerate
// (equal) vectors, then run the shared segment query over the node list and
// report the accumulated AABB through out1 (min) / out2 (max).
// ---------------------------------------------------------------------------
int func_804B5658(CColiWalkOwner* self, Vec* out1, Vec* out2,
                  Vec* a, Vec* b) {
    if (self->flags & 2) {
        int eq = a->x == b->x && a->y == b->y && a->z == b->z;
        if (eq) return 0;

        char seg[0x310];
        char query[0x18];
        CColiWalkState* st = &lbl_eu_8065D138;
        st->flag = 0;
        // Word-typed POD snapshot: the struct copy emits the retail lwz/stw
        // word copy for the source-vector snapshot.
        st->src = *(const CColiSrcVec*)a;
        st->bbMin[0] = lbl_eu_8066AEC0;
        st->bbMin[1] = lbl_eu_8066AEC0;
        st->bbMin[2] = lbl_eu_8066AEC0;
        st->bbMax[0] = lbl_eu_8066AEC0;
        st->bbMax[1] = lbl_eu_8066AEC0;
        st->bbMax[2] = lbl_eu_8066AEC4;
        st->f5c = lbl_eu_8066AEC8;
        func_804A7ACC(seg, a, b, st);
        func_804B077C(query, a, b);

        for (CColiQueryNode* n = self->head; n != 0; n = n->next) {
            if (n->flags & 0x2000) {
                if (func_804B0818(query, n) != 0) {
                    func_804B21A8(n, seg, 1);
                }
            }
        }

        if (!lbl_eu_8065D138.flag) return 0;
        out1->x = lbl_eu_8065D138.bbMin[0];
        out1->y = lbl_eu_8065D138.bbMin[1];
        out1->z = lbl_eu_8065D138.bbMin[2];
        out2->x = lbl_eu_8065D138.bbMax[0];
        out2->y = lbl_eu_8065D138.bbMax[1];
        out2->z = lbl_eu_8065D138.bbMax[2];
        return 1;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// CTaskColiManager::create — the retail symbol is Fv but the body reads three
// incoming registers (parent / a / b, per the CTaskGame call site: r3 = root
// proc, r4 = field_0x74, r5 = field_0x70). MWCC mangles a global function's
// name with its parameter list, so the exact retail symbol is emitted via
// extern "C" with the pre-mangled name (MWCC_CASES §1g Fv-with-hidden-
// params; the `bl` reloc name stays correct and the extra args pass normally).
// ---------------------------------------------------------------------------
// Raw first-0x80 mirror of the object the retail create() initialises: the
// vtable slot at 0x10, two null member-function-pointer slots (0x3C..0x50)
// filled from __ptmf_null, the caller args at 0x54/0x58, and the remaining
// state fields. The real class has compiler-managed virtuals, so the init
// stores go through this POD.
struct CTaskColiManagerInit {
    char pad0[0x10];
    u32 vtable;      //0x10
    char pad1[0x28]; //0x14
    u32 ptmf[6];     //0x3C
    u32 field_0x54;  //0x54
    u32 field_0x58;  //0x58
    u32 field_0x5C;  //0x5C
    f32 f_0x60[6];   //0x60
    u32 field_0x78;  //0x78
    u32 field_0x7C;  //0x7C
}; //0x80

extern u32 __ptmf_null[3];
extern u8 lbl_eu_8056F4B0[];
extern u8 lbl_eu_8056F468[];

extern "C" CTaskColiManager* create__16CTaskColiManagerFv(CProcess* parent,
                                                          void* a, void* b) {
    void* mem =
        mtl::MemManager::allocate(0x80, CWorkThreadSystem::getWorkMem());
    CTaskColiManager* obj = (CTaskColiManager*)mem;
    if (mem != 0) {
        __ct__8CProcessFv((CProcess*)mem);
        CTaskColiManagerInit* init = (CTaskColiManagerInit*)mem;
        // CTTask ctor body (inlined): CProcess vtable, null member funcs,
        // then the CTaskColiManager vtable. The retail loads each null-ptmf
        // word pair with word1 first, so the loads are written into locals in
        // that order and stored back word0-first.
        init->vtable = (u32)lbl_eu_8056F4B0;
        {
            u32 w1 = __ptmf_null[1];
            u32 w0 = __ptmf_null[0];
            init->ptmf[0] = w0;
            init->ptmf[1] = w1;
            init->ptmf[2] = __ptmf_null[2];
        }
        {
            u32 w1 = __ptmf_null[1];
            u32 w0 = __ptmf_null[0];
            init->ptmf[3] = w0;
            init->ptmf[4] = w1;
            init->ptmf[5] = __ptmf_null[2];
        }
        init->vtable = (u32)lbl_eu_8056F468;
        init->field_0x54 = (u32)a;
        init->field_0x58 = (u32)b;
        init->field_0x5C = 0;
        init->field_0x78 = 0;
        init->field_0x7C = (u32)mtl::MemManager::getHandleMEM1();
        f32 z = lbl_eu_8066AEC0;
        init->f_0x60[0] = z;
        init->f_0x60[1] = z;
        init->f_0x60[2] = z;
        init->f_0x60[3] = z;
        init->f_0x60[4] = z;
        init->f_0x60[5] = z;
        lbl_eu_80665958 = (int)(uintptr_t)mem;
    }
    ((CProcess*)obj)->Regist(parent, false);
    return obj;
}

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
}// data: retail sections verified via run.py data diff (no bypass)
