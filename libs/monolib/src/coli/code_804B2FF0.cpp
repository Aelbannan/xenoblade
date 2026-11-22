// Auto-scaffolded catalog TU for monolib/src/coli/code_804B2FF0
// Replace stubs with high-level C/C++ during decomp.

// Suppress MWCC auto-vtable/typeinfo emissions: retail ships no standard RTTI
// for this class (monolib type descriptors are hand-built below, CDesktop
// novtable pattern).
class __declspec(novtable) CTaskColiManager;

#include <harness_catalog.h>
#include <string.h>
#include "monolib/coli/CTaskColiManager.hpp"
#include <monolib/coli/CColiProc.hpp> // CColiResCall (the mover object this TU updates)
#include "monolib/coli/code_804B2FF0.hpp"
#include "monolib/work/CTTask.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include <revolution/MTX.h>
#include <nw4r/math.h>
#include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/g3d/res/g3d_resnode.h>
#include <nw4r/db/db_assert.h>

// sColiWalkState / sColiTargetTable are declared with their full types in
// code_804B2FF0.hpp and defined in the dissolved-data block at the TU tail.
// C-linkage retail import (declared at global scope so MWCC emits the name
// unmangled; the definition lives in the sibling coli unit).
extern "C" void ColiTranslatePoints(void* self);
// Segment-query helpers imported from the sibling coli unit (stubs there).
extern "C" void ColiInitPointSpec(void*, const void*, int);
extern "C" void ColiCopyPointPair(void*, const void*);
extern "C" int ColiQueryNodePoint(const void*, const void*, const void*);
extern "C" void ColiInitPointRadius(void*, const Vec*, void*, f32);
extern "C" void ColiMakeAabbRadius(void*, const Vec*, f32);
extern "C" int ColiProcessSegmentPair(const CColiQueryNode*, const void*, int);
// CProcess is abstract (pure virtuals), so the base ctor is invoked via its
// pre-mangled retail symbol like the sibling sinit helpers do.
extern "C" void __ct__8CProcessFv(CProcess* self);

// node-lookup helpers below
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


// Node header shared by the coli objects: signed bone index at +0x0 and
// flags word at +0x2 (bit 1 selects the root matrix instead of the index
// lookup; bit 0 marks the object dirty after a bounds update).
struct CColiHead {
    s16 index; //0x0
    u16 flags; //0x2
};

// Capsule collision node: primary index + flags at 0x0/0x2 and a secondary
// index at 0x30 (both written by ColiFindNodePair). This is the real node
// type used by the capsule dispatch family (ColiSetCapsulePos etc.).
struct CColiCapsuleNode {
    CColiHead hd;    //0x0 (index + flags)
    f32 posA[3];     //0x4
    f32 posB[3];     //0x10
    f32 dir[3];      //0x1C
    f32 radius;      //0x28
    f32 lenSq;       //0x2C
    s16 index2;      //0x30
};

// Second-name reference: a byte offset (+0x4) into the shared node-name table.
struct CNameRef {
    char pad[0x4];
    s32 offset; //0x4
};

// Shared node-name table base (retail loads the pointer via sda21).
extern char* g_coliSpecTable;

// C-linkage spelling for the dispatch-table participants (retail symbol
// names; the definitions below adopt this linkage).
extern "C" void ColiFindNodePair(CColiCapsuleNode*, const char*, const CModelRes*,
                              const CNameRef*);

// ---------------------------------------------------------------------------
// ColiFindNodePair: scan the model's node table for up to two node names - the
// caller's `name` and a second name taken from the shared name table - and
// record each match's matrix id into self->index / self->index2. Both found
// (flags == 3) exits early; unmatched indices fall back to 0 / 1.
// ---------------------------------------------------------------------------
void ColiFindNodePair(CColiCapsuleNode* self, const char* name,
                   const CModelRes* model, const CNameRef* ref) {
    const char* name2 = g_coliSpecTable + ref->offset;
    int num = (int)nw4r::g3d::ResMdl(model->resMdl).GetResNodeNumEntries();
    u32 flags = 0;
    self->hd.flags = (u16)flags;
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
                self->hd.index = (s16)node.GetMtxID();
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
    if (!(flags & 1)) self->hd.index = 0;
    if (!(flags & 2)) self->index2 = 1;
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

// CRTP task base -- canonical monolib template (declared-only members so the
// explicit out-of-line specializations below emit the retail Move/Draw/dtor
// symbols). The full class definition lives here (single-TU scope) because
// CTaskColiManager's full layout is only needed in this TU.
class CTaskColiManager : public CTTask<CTaskColiManager> {
public:
    // Overrides from CProcess (pure virtual)
    void Init() override;
    void Term() override;
    void Move() override;
    void Draw() override;

    virtual ~CTaskColiManager();

    // name-guess: create() stores the scene/model arguments at 0x54/0x58,
    // zeroes the six-word AABB at 0x60..0x6F, then clears the walk flag.
    void* mpScene;              //0x54
    void* mpModel;              //0x58
    CColiQueryNode* mHead;      //0x5C
    f32 mAabbMin[3];            //0x60
    f32 mAabbMax[3];            //0x6C
    u32 mFlags;                 //0x78
};


// Shared walk state (sColiWalkState): accumulated AABB (bbMin +0x38,
// bbMax +0x44), the queried source segment (+0x50), a scalar at +0x5C and
// a flag at +0x60.
// 3-word raw snapshot (u32 members force lwz/stw word copies like retail).
struct CColiSrcVec {
    u32 x, y, z;
};

// Pair+trailing-word snapshot: same 12 bytes, but the leading u32[2] makes
// MWCC use its pipelined two-word copy expansion.
struct CColiPairCopy {
    u32 xy[2];
    u32 z;
};

struct CColiWalkState {
    char pad0[0x38];
    f32 bbMin[3];  //0x38
    f32 bbMax[3];  //0x44
    CColiSrcVec src; //0x50
    f32 f5c;       //0x5C
    u8 flag;       //0x60
};

// Retail .bss object sColiWalkState spans 0x68 bytes: the walk state plus a
// 4-byte retail tail pad. Defined (zero-initialised) in the dissolved block
// at the bottom of this TU so the object owns its retail .bss slice.
struct CColiWalkStateObj : CColiWalkState {
    u8 tail[4]; //0x64..0x67 retail padding
};

// Query scratch filled by ColiMakeAabbMinMax/ColiMakeAabbRadius: component-wise min x
// at +0x0 and max x at +0xC (trailing pad keeps the retail 0x18-byte slot).
struct CColiQuery {
    f32 lo;         //0x0 min x
    f32 pad0[2];    //0x4
    f32 hi;         //0xC max x
    char pad1[0x8]; //0x10
};

extern "C" int Coli_WalkQuery(CTaskColiManager* self, CColiQueryNode* owner, const Vec* a,
                  u32 mask, int arg, int isFirst, f32 val) {
    // val survives every helper call in an FP saved register.
    const f32 v = val;

    // Both direction bits must be set in the owner's flags.
    u32 oflags = owner->flags;
    if (!(oflags & 4) || !(oflags & 2)) return 0;

    // Walk state arg: retail reuses the incoming isFirst register for the
    // pointer, so a non-first call passes the raw isFirst value through.
    CColiWalkState* st = (CColiWalkState*)(u32)isFirst;
    int init = 0;
    if (isFirst == 0) {
        // All walk-state accesses are volatile so MWCC keeps the whole
        // sequence in retail order: AEC4 seed load, flag clear, init mark,
        // AEC8 seed, x/y snapshot with y stored first, the zero literal
        // load wedged between the snapshot stores, then z.
        volatile CColiWalkState* vs = &sColiWalkState;
        st = &sColiWalkState;
        f32 v5c;
        f32 v4c = lbl_eu_8066AEC4;
        vs->flag = 0;
        init = 1;
        v5c = *(const volatile f32*)&lbl_eu_8066AEC8;
        const volatile CColiSrcVec* av = (const volatile CColiSrcVec*)a;
        u32 hi;
        u32 lo = av->x;
        hi = av->y;
        vs->src.y = hi;
        f32 zero = lbl_eu_8066AEC0;
        vs->src.x = lo;
        u32 zv = av->z;
        vs->src.z = zv;
        vs->bbMax[2] = v4c;
        vs->f5c = v5c;
        vs->bbMin[0] = zero;
        vs->bbMin[1] = zero;
        vs->bbMin[2] = zero;
        vs->bbMax[0] = zero;
        vs->bbMax[1] = zero;
        self->mAabbMin[0] = zero;
        self->mAabbMin[1] = zero;
        self->mAabbMin[2] = zero;
        self->mAabbMax[0] = zero;
        self->mAabbMax[1] = zero;
        self->mAabbMax[2] = zero;
    }

    char seg[0x318];
    CColiQuery query;
    ColiInitPointRadius(&seg, a, st, v);
    int found = 0;
    ColiMakeAabbRadius(&query, a, v);

    // Forward pass along the next chain: nodes whose +0x18 bound is below
    // the query low bound are skipped.
    CColiQueryNode* n = owner->next;
    while (n != 0) {
        if (!(mask & n->filter)) {
            if (query.lo >= n->maxX) {
                if ((int)owner->level <= (int)n->level &&
                    ColiTestAabbContains(&query, n) != 0 &&
                    ColiProcessSegmentPair(n, &seg, arg) != 0) {
                    found = 1;
                }
            }
        }
        n = n->next;
    }

    // Reverse pass along the prev chain (ascending f_a4): once the query
    // high bound passes a node's +0xA4 key the walk can stop early.
    for (n = owner->prev; n != 0; n = n->prev) {
        if (!(mask & n->filter)) {
            if (query.hi <= n->minX) {
                if ((int)owner->level <= (int)n->level &&
                    ColiTestAabbContains(&query, n) != 0 &&
                    ColiProcessSegmentPair(n, &seg, arg) != 0) {
                    found = 1;
                }
            } else if (query.hi > n->f_a4) {
                break;
            }
        }
    }

    if (init != 0) {
        if (found == 0) {
            return 0;
        }
        if (sColiWalkState.flag != 0) {
            // Word copies of the accumulated AABB via the pipelined
            // two-word struct-copy expansion (y stored before x, trailing z).
            *(CColiPairCopy*)&self->mAabbMin[0] =
                *(const CColiPairCopy*)&sColiWalkState.bbMin[0];
            *(CColiPairCopy*)&self->mAabbMax[0] =
                *(const CColiPairCopy*)&sColiWalkState.bbMax[0];
            return 1;
        }
        return 0;
    }
    return found;
}

extern "C" int ColiWalkSegment(CTaskColiManager* self, CColiQueryNode* owner, const Vec* a,
                  const Vec* b, u32 mask, int arg, int isFirst) {
    // Distinct web for `b`: retail materializes it into a callee-saved
    // register at entry (it must survive the first helper call).
    const Vec* bv;

    // Both direction bits must be set in the owner's flags.
    u32 oflags = owner->flags;
    if (!(oflags & 4) || !(oflags & 2)) return 0;

    // Walk state arg: retail reuses the incoming isFirst register for the
    // pointer, so a non-first call passes the raw isFirst value through.
    CColiWalkState* st = (CColiWalkState*)(u32)isFirst;
    int init = 0;
    if (isFirst == 0) {
        st = &sColiWalkState;
        // All walk-state accesses are volatile so MWCC keeps the whole
        // sequence in retail order (seed load, flag clear, init mark, f5c
        // seed, x/y snapshot with y stored first, z, zero-fill seed).
        volatile CColiWalkState* vs = &sColiWalkState;
        f32 v76 = 1.0f;
        vs->flag = 0;
        init = 1;
        f32 v92 = 1000000.0f;
        const volatile CColiSrcVec* av = (const volatile CColiSrcVec*)a;
        u32 lo = av->x;
        u32 hi = av->y;
        vs->src.y = hi;
        vs->src.x = lo;
        u32 zv = av->z;
        vs->src.z = zv;
        f32 zero = 0.0f;
        vs->bbMax[2] = v76;
        vs->f5c = v92;
        vs->bbMin[0] = zero;
        vs->bbMin[1] = zero;
        vs->bbMin[2] = zero;
        vs->bbMax[0] = zero;
        vs->bbMax[1] = zero;
        self->mAabbMin[0] = zero;
        self->mAabbMin[1] = zero;
        self->mAabbMin[2] = zero;
        self->mAabbMax[0] = zero;
        self->mAabbMax[1] = zero;
        self->mAabbMax[2] = zero;
    }

    char seg[0x318];
    CColiQuery query;
    bv = b;
    ColiInitSegment(&seg, a, bv, st);
    int found = 0;
    ColiMakeAabbMinMax(&query, a, bv);

    // Forward pass along the next chain: nodes whose +0x18 bound is below
    // the query low bound are skipped.
    CColiQueryNode* n = owner->next;
    while (n != 0) {
        // Nested ifs keep each test's loads at its retail position (the
        // level lhz must stay below the bound fcmpo).
        if (!(mask & n->filter)) {
            if (query.lo >= n->maxX) {
                if ((s16)owner->level <= (s16)n->level &&
                    ColiTestAabbContains(&query, n) != 0 &&
                    ColiProcessSegmentSelect(n, &seg, arg) != 0) {
                    found = 1;
                }
            }
        }
        n = n->next;
    }

    // Reverse pass along the prev chain (ascending f_a4): once the query
    // high bound passes a node's +0xA4 key the walk can stop early.
    for (n = owner->prev; n != 0; n = n->prev) {
        if (!(mask & n->filter)) {
            if (query.hi <= n->minX) {
                if ((int)owner->level <= (int)n->level &&
                    ColiTestAabbContains(&query, n) != 0 &&
                    ColiProcessSegmentSelect(n, &seg, arg) != 0) {
                    found = 1;
                }
            } else if (query.hi > n->f_a4) {
                break;
            }
        }
    }

    if (!init) return found;
    if (found == 0) return 0;
    if (sColiWalkState.flag == 0) return 0;
    // Word copies of the accumulated AABB: pipelined pairs (load x/y, store
    // y/x) plus a trailing z word.
    {
        const CColiSrcVec* smin =
            (const CColiSrcVec*)&sColiWalkState.bbMin[0];
        CColiSrcVec* dmin = (CColiSrcVec*)&self->mAabbMin[0];
        u32 mlo = smin->x;
        u32 mhi = smin->y;
        dmin->y = mhi;
        dmin->x = mlo;
        dmin->z = smin->z;
    }
    {
        const CColiSrcVec* smax =
            (const CColiSrcVec*)&sColiWalkState.bbMax[0];
        CColiSrcVec* dmax = (CColiSrcVec*)&self->mAabbMax[0];
        u32 xlo = smax->x;
        u32 xhi = smax->y;
        dmax->y = xhi;
        dmax->x = xlo;
        dmax->z = smax->z;
    }
    return 1;
}

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

// Dispatch-table participants (bounds expansion family).
extern "C" void ColiExpandSphere(CColiSphere*);

void ColiExpandSphere(CColiSphere* p) {
    // Expand the box's max corner by the sphere's max extent and the
    // min corner by its min extent, per axis. Each check reloads the radius
    // temp first (retail does the same - the bound load reuses the FPR, so
    // the radius is re-fetched at the top of every check).
    {
        f32 r = p->radius;
        if (g_coliBounds->max[0] < p->x + r) {
            g_coliBounds->max[0] = p->x + r;
        }
    }
    {
        f32 r = p->radius;
        if (g_coliBounds->min[0] > p->x - r) {
            g_coliBounds->min[0] = p->x - r;
        }
    }
    {
        f32 r = p->radius;
        if (g_coliBounds->max[1] < p->y + r) {
            g_coliBounds->max[1] = p->y + r;
        }
    }
    {
        f32 r = p->radius;
        if (g_coliBounds->min[1] > p->y - r) {
            g_coliBounds->min[1] = p->y - r;
        }
    }
    {
        f32 r = p->radius;
        if (g_coliBounds->max[2] < p->z + r) {
            g_coliBounds->max[2] = p->z + r;
        }
    }
    // Final check keeps the scoped radius temp so retail's radius-first
    // load order (f1 <- p->radius before f0 <- p->z) is reproduced.
    {
        f32 r = p->radius;
        if (g_coliBounds->min[2] > p->z - r) {
            g_coliBounds->min[2] = p->z - r;
        }
    }
}

// Variant with separate horizontal (x/z) and vertical (y) radii.
struct CColiEllipsoid {
    f32 pad0;      //0x0
    f32 x;         //0x4
    f32 y;         //0x8
    f32 z;         //0xC
    f32 radiusXZ;  //0x10
    f32 radiusY;   //0x14
};

extern "C" void Coli_ExpandEllipsoid(CColiEllipsoid*);

void Coli_ExpandEllipsoid(CColiEllipsoid* p) {
    if (g_coliBounds->max[0] < p->x + p->radiusXZ) {
        g_coliBounds->max[0] = p->x + p->radiusXZ;
    }
    if (g_coliBounds->min[0] > p->x - p->radiusXZ) {
        g_coliBounds->min[0] = p->x - p->radiusXZ;
    }
    if (g_coliBounds->max[2] < p->z + p->radiusXZ) {
        g_coliBounds->max[2] = p->z + p->radiusXZ;
    }
    if (g_coliBounds->min[2] > p->z - p->radiusXZ) {
        g_coliBounds->min[2] = p->z - p->radiusXZ;
    }
    if (g_coliBounds->max[1] < p->y + p->radiusY) {
        g_coliBounds->max[1] = p->y + p->radiusY;
    }
    if (g_coliBounds->min[1] > p->y - p->radiusY) {
        g_coliBounds->min[1] = p->y - p->radiusY;
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

extern "C" void ColiExpandCapsule(CColiCapsule*);

void ColiExpandCapsule(CColiCapsule* p) {
    for (int i = 0; i < 3; i++) {
        {
            f32 v;
            f32 r = p->radius;
            v = p->endPos[i];
            if (g_coliBounds->max[i] < v + r) {
                g_coliBounds->max[i] = v + r;
            }
        }
        {
            f32 r = p->radius;
            if (g_coliBounds->min[i] > p->endPos[i] - r) {
                g_coliBounds->min[i] = p->endPos[i] - r;
            }
        }
        {
            f32 v;
            f32 r = p->radius;
            v = p->endPosB[i];
            if (g_coliBounds->max[i] < v + r) {
                g_coliBounds->max[i] = v + r;
            }
        }
        {
            f32 r = p->radius;
            if (g_coliBounds->min[i] > p->endPosB[i] - r) {
                g_coliBounds->min[i] = p->endPosB[i] - r;
            }
        }
    }
}

// Another bounds-expansion source: position x/y/z at +0x10/+0x20/+0x30
// and a single uniform radius at +0x70.
struct CColiBoundsPoint {
    struct Axis {
        f32 value;    // axis value
        f32 pad[3];
    };
    f32 field_0x00[4]; //0x0-0xF
    Axis pos[3];       //0x10, 0x20, 0x30
    f32 field_0x34[12]; //0x40-0x6F
    f32 radius;         //0x70
};

extern "C" void Coli_ExpandPoint(CColiBoundsPoint*);

void Coli_ExpandPoint(CColiBoundsPoint* p) {
    // Per-check radius temp: retail reloads it at the top of every check.
    // Retail unrolls a 3-axis loop (max check then min check per axis).
    for (int i = 0; i < 3; i++) {
        f32 v;
        f32 r = p->radius;
        v = p->pos[i].value;
        if (g_coliBounds->max[i] < v + r) {
            g_coliBounds->max[i] = v + r;
        }
        r = p->radius;
        if (g_coliBounds->min[i] > p->pos[i].value - r) {
            g_coliBounds->min[i] = p->pos[i].value - r;
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

extern "C" void ColiExpandPoint(CColiBoundsPoint2*);
extern "C" void Coli_ExpandGlobalBox();

void ColiExpandPoint(CColiBoundsPoint2* p) {
    // Per-sub-block radius temp: retail reloads it at the top of every check
    // (the bound loads reuse the FPR, so the radius is re-fetched per block).
    // NOTE residual: the final block's bounds-pointer reload colors r3 in our
    // build vs retail r4 (see docs/MWCC_CASES.md bounds-expansion family).
    {
        f32 r = p->radius;
        if (g_coliBounds->max[0] < p->x + r) {
            g_coliBounds->max[0] = p->x + r;
        }
    }
    {
        f32 r = p->radius;
        if (g_coliBounds->min[0] > p->x - r) {
            g_coliBounds->min[0] = p->x - r;
        }
    }
    {
        f32 r = p->radius;
        if (g_coliBounds->max[1] < p->y + r) {
            g_coliBounds->max[1] = p->y + r;
        }
    }
    {
        f32 r = p->radius;
        if (g_coliBounds->min[1] > p->y - r) {
            g_coliBounds->min[1] = p->y - r;
        }
    }
    {
        f32 r = p->radius;
        if (g_coliBounds->max[2] < p->z + r) {
            g_coliBounds->max[2] = p->z + r;
        }
    }
    {
        f32 r = p->radius;
        if (g_coliBounds->min[2] > p->z - r) {
            g_coliBounds->min[2] = p->z - r;
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

// Matrix source owned by the collision manager:
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

// Full node (ColiXformRigid): world matrix, inverse world matrix, a local
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
struct CColiMtxNode {
    CColiHead hd;    //0x0
    CObjMtx mtxW;    //0x4
    CObjMtx invW;    //0x34
    f32 radius;      //0x64
};

// Rigid reference node (ColiXformRigid 'other'): matrix at +0x0, position
// vector at +0x30, used for PSMTXConcat.
struct CColiRigid {
    Mtx mtx;    //0x0
    f32 pos[3]; //0x30
};

// Sphere-shaped collision object (ColiXformSphere): position + radius.
struct CColiSphereOb {
    CColiHead hd; //0x0
    f32 pos[3];   //0x4
    f32 radius;   //0x10
};

// Dispatch-table participants (node lookup + transform/bounds family).
extern "C" void Coli_FindNodeByName(CColiHead*, const char*, const CModelRes*);
extern "C" void ColiSetSphereMtx(CColiSphereOb*, const CColiMgr*, const Vec*);
extern "C" void ColiUpdateMtxNode(CColiMtxNode*, const CColiMgr*,
                              const CColiSphereOb*);
extern "C" void ColiXformRigid(CColiNode*, const CColiMgr*, const CColiRigid*);

// ---------------------------------------------------------------------------
// Coli_FindNodeByName: scan the model's node table for the node named `name` and
// store its matrix id as the collision node's bone index. On failure, clear
// the index and set flag bit 1 so the matrix-source selectors fall back to
// the root matrix.
// ---------------------------------------------------------------------------
void Coli_FindNodeByName(CColiHead* self, const char* name, const CModelRes* model) {
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
// ColiSetSphereMtx: copy the source matrix's translation column into the
// sphere's position, derive the sphere radius from the transformed
// (x, 0, 0) normal, and expand the global AABB around the new position.
// ---------------------------------------------------------------------------
void ColiSetSphereMtx(CColiSphereOb* self, const CColiMgr* mgr, const Vec* other) {
    // Struct-typed matrix source (vs array-pointer) to steer scheduling.
    const CObjMtx* src;
    if (self->hd.flags & 2) {
        src = (const CObjMtx*)mgr->mesh->rootMtx;
    } else {
        src = (const CObjMtx*)mgr->mesh->boneTable[self->hd.index];
    }

    f32 pz;
    f32 px;
    f32 py;
    pz = src->m[2][3];
    py = src->m[1][3];
    px = src->m[0][3];
    self->pos[0] = px;
    self->pos[1] = py;
    self->pos[2] = pz;

    nw4r::math::VEC3 n;
    n.x = other->x;
    n.y = 0.0f;
    n.z = 0.0f;
    nw4r::math::VEC3TransformNormal(&n, (const nw4r::math::MTX34*)&src->m,
                                    &n);

    // Direct field uses: the first read is store-forwarded (frsp) from the
    // call result; later reads (across aliasing stores) reload from memory.
    self->radius = PSVECMag((const Vec*)&n);
    self->hd.flags |= 1;

    {
        f32 r = self->radius;
        if (g_coliBounds->max[0] < r + self->pos[0])
            g_coliBounds->max[0] = r + self->pos[0];
    }
    {
        f32 r = self->radius;
        if (g_coliBounds->min[0] > self->pos[0] - r)
            g_coliBounds->min[0] = self->pos[0] - r;
    }
    {
        f32 r = self->radius;
        if (g_coliBounds->max[1] < r + self->pos[1])
            g_coliBounds->max[1] = r + self->pos[1];
    }
    {
        f32 r = self->radius;
        if (g_coliBounds->min[1] > self->pos[1] - r)
            g_coliBounds->min[1] = self->pos[1] - r;
    }
    {
        f32 r = self->radius;
        if (g_coliBounds->max[2] < r + self->pos[2])
            g_coliBounds->max[2] = r + self->pos[2];
    }
    {
        f32 r = self->radius;
        if (g_coliBounds->min[2] > self->pos[2] - r)
            g_coliBounds->min[2] = self->pos[2] - r;
    }
}

void ColiUpdateMtxNode(CColiMtxNode* self, const CColiMgr* mgr,
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
    n.y = 0.0f;
    n.z = 0.0f;
    nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&n,
                                    (const nw4r::math::MTX34*)&self->mtxW,
                                    (const nw4r::math::VEC3*)&n);
    self->radius = PSVECMag((const Vec*)&n);
    self->hd.flags |= 1;

    // Expand the global box around each translation-column axis by the
    // radius; every check reloads the stored radius, the matrix element and
    // the shared bounds pointer.
    f32 r = self->radius;
    if (g_coliBounds->max[0] < self->mtxW.m[0][3] + r)
        g_coliBounds->max[0] = self->mtxW.m[0][3] + r;
    r = self->radius;
    if (g_coliBounds->min[0] > self->mtxW.m[0][3] - r)
        g_coliBounds->min[0] = self->mtxW.m[0][3] - r;
    r = self->radius;
    if (g_coliBounds->max[1] < self->mtxW.m[1][3] + r)
        g_coliBounds->max[1] = self->mtxW.m[1][3] + r;
    r = self->radius;
    if (g_coliBounds->min[1] > self->mtxW.m[1][3] - r)
        g_coliBounds->min[1] = self->mtxW.m[1][3] - r;
    r = self->radius;
    if (g_coliBounds->max[2] < self->mtxW.m[2][3] + r)
        g_coliBounds->max[2] = self->mtxW.m[2][3] + r;
    r = self->radius;
    if (g_coliBounds->min[2] > self->mtxW.m[2][3] - r)
        g_coliBounds->min[2] = self->mtxW.m[2][3] - r;
}

// Ellipsoid-shaped collision object (ColiSetEllipsoid / ColiXformEllipsoid).
struct CColiEllipsoidOb {
    CColiHead hd;     //0x0
    f32 pos[3];       //0x4
    f32 radiusXZ;     //0x10
    f32 radiusY;      //0x14
    f32 f18;          //0x18
};

// Object carrying both a position Vec (used as the transform source) and a
// secondary Vec at +0xC (ColiXformEllipsoid 'other').
struct CColiPosVec {
    Vec pos; //0x0
    Vec v;   //0xC
};

// Dispatch-table participants (ellipsoid family).
extern "C" void Coli_FindNodeRecord(CColiHead*, const char*, const CModelRes*,
                              const CColiPosVec*);
extern "C" void ColiSetEllipsoid(CColiEllipsoidOb*, const CColiMgr*, const Vec*);
extern "C" void ColiXformEllipsoid(CColiEllipsoidOb*, const CColiMgr*,
                              const CColiPosVec*);

// ---------------------------------------------------------------------------
// Coli_FindNodeRecord: node-lookup sibling of Coli_FindNodeByName - records the matched
// node's matrix id into the shared master-node index and folds a source
// vector's x/y/z into the shared half-extent (ay = y - z).
// ---------------------------------------------------------------------------
void Coli_FindNodeRecord(CColiHead* self, const char* name, const CModelRes* model,
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
            g_coliNodeIndex = (s16)node.GetMtxID();
            g_coliHalfExtent->ax = v->v.x;
            g_coliHalfExtent->ay = v->v.y - v->v.z;
            return;
        }
    }
}

// ---------------------------------------------------------------------------
// Coli_ShiftMtxHist: shift the previous matrix pair up into the history slots,
// then copy the bone matrix into m0 and derive its inverse into m1.
// ---------------------------------------------------------------------------
extern "C" void Coli_ShiftMtxHist(CColiHead* hd, CColiMtxOut* dst, CColiMgr* mgr) {
    dst->old = dst->cur;
    const CObjMtx* src = (const CObjMtx*)mgr->mesh->boneTable[hd->index];
    dst->cur.m0 = *src;
    PSMTXInverse(dst->cur.m0.m, dst->cur.m1.m);
}


extern "C" void ColiUpdateCapsule(CColiCapsuleNode*, const CColiMgr*,
                              const CColiRigid*);
extern "C" void ColiSetCapsulePos(CColiCapsuleNode*, const CColiMgr*, const Vec*);
extern "C" void ColiXformFixed(const CColiNode*, const CColiMgr*,
                              const CColiPosVec*);

// ---------------------------------------------------------------------------
// ColiUpdateCapsule: update a capsule collision node. Concat the source (root or
// bone) matrix with the rigid reference, transform the (0,1,0) axis by the
// result to get the endpoints A = T + v / B = T - v, derive the segment
// direction + squared length via the nw4r PS helpers, scale the transformed
// (1,0,0) axis magnitude into the radius, then expand the global AABB around
// both endpoints swept by the radius.
// ---------------------------------------------------------------------------
void ColiUpdateCapsule(CColiCapsuleNode* self, const CColiMgr* mgr,
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
    v.x = 0.0f;
    v.y = 0.5f;
    v.z = 0.0f;

    nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&v,
                                    (const nw4r::math::MTX34*)tmp,
                                    (const nw4r::math::VEC3*)&v);

    const f32 az = tmp[2][3] + v.z;
    const f32 ay = tmp[1][3] + v.y;
    const f32 ax = tmp[0][3] + v.x;

    // Radius-axis normal seed held as scalars: retail keeps these two
    // literals live in registers across the endpoint math and stores them
    // into the reused vector slot just before the second transform.
    const f32 seedX = 1.0f;
    const f32 seedYZ = 0.0f;

    self->posA[1] = ay;
    self->posA[0] = ax;
    self->posA[2] = az;

    const f32 by = tmp[1][3] - v.y;
    const f32 bx = tmp[0][3] - v.x;
    const f32 bz = tmp[2][3] - v.z;
    self->posB[0] = bx;
    self->posB[1] = by;
    self->posB[2] = bz;

    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&self->dir,
                        (const nw4r::math::VEC3*)&self->posB,
                        (const nw4r::math::VEC3*)&self->posA);
    self->lenSq = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&self->dir,
                                      (const nw4r::math::VEC3*)&self->dir);

    v.x = seedX;
    v.y = seedYZ;
    v.z = seedYZ;
    nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&v,
                                    (const nw4r::math::MTX34*)tmp,
                                    (const nw4r::math::VEC3*)&v);
    self->radius = 0.5f * PSVECMag((const Vec*)&v);
    self->hd.flags |= 1;

    // Expand the global AABB around both endpoints swept by the radius, one
    // axis per counted-loop iteration (retail reloads the bounds pointer for
    // every check, so dereference the global directly).
    for (int i = 0; i < 3; i++) {
        if (g_coliBounds->max[i] < self->posA[i] + self->radius)
            g_coliBounds->max[i] = self->posA[i] + self->radius;
        if (g_coliBounds->min[i] > self->posA[i] - self->radius)
            g_coliBounds->min[i] = self->posA[i] - self->radius;
        if (g_coliBounds->max[i] < self->posB[i] + self->radius)
            g_coliBounds->max[i] = self->posB[i] + self->radius;
        if (g_coliBounds->min[i] > self->posB[i] - self->radius)
            g_coliBounds->min[i] = self->posB[i] - self->radius;
    }
}

// ---------------------------------------------------------------------------
// ColiXformFixed: transform a position by the fixed bone matrix, derive an
// ellipsoid-ish half-extent into the shared Qx/Hx globals, and fold them
// into the global AABB.
// ---------------------------------------------------------------------------
void ColiXformFixed(const CColiNode* unused, const CColiMgr* mgr,
                   const CColiPosVec* other) {
    const s16 idx = g_coliNodeIndex;
    const f32 (*src)[4] = mgr->mesh->boneTable[idx];

    PSMTXMultVec(src, &other->pos, (Vec*)g_coliXformPos);

    nw4r::math::VEC3 n;
    n.x = 1.0f;
    n.y = 0.0f;
    n.z = 0.0f;
    nw4r::math::VEC3TransformNormal(&n, (const nw4r::math::MTX34*)src, &n);
    f32 mag = PSVECMag((const Vec*)&n);

    CColiHx* hx = g_coliHalfExtent;
    hx->ax = other->v.x * mag;
    hx->ay = other->v.y * mag;
    hx->f8 = other->v.z * mag;
    g_coliXformPos->qy += g_coliHalfExtent->f8;
    g_coliHalfExtent->ay -= g_coliHalfExtent->f8;

    if (g_coliBounds->max[0] < g_coliXformPos->qx + g_coliHalfExtent->ax)
        g_coliBounds->max[0] = g_coliXformPos->qx + g_coliHalfExtent->ax;
    if (g_coliBounds->min[0] > g_coliXformPos->qx - g_coliHalfExtent->ax)
        g_coliBounds->min[0] = g_coliXformPos->qx - g_coliHalfExtent->ax;
    if (g_coliBounds->max[2] < g_coliXformPos->qz + g_coliHalfExtent->ax)
        g_coliBounds->max[2] = g_coliXformPos->qz + g_coliHalfExtent->ax;
    if (g_coliBounds->min[2] > g_coliXformPos->qz - g_coliHalfExtent->ax)
        g_coliBounds->min[2] = g_coliXformPos->qz - g_coliHalfExtent->ax;
    if (g_coliBounds->max[1] < g_coliXformPos->qy + g_coliHalfExtent->ay)
        g_coliBounds->max[1] = g_coliXformPos->qy + g_coliHalfExtent->ay;
    if (g_coliBounds->min[1] > g_coliXformPos->qy)
        g_coliBounds->min[1] = g_coliXformPos->qy;
}

void Coli_ExpandGlobalBox() {
    if (g_coliBounds->max[0] < g_coliXformPos->qx + g_coliHalfExtent->ax) {
        g_coliBounds->max[0] = g_coliHalfExtent->ax + g_coliXformPos->qx;
    }
    if (g_coliBounds->min[0] > g_coliXformPos->qx - g_coliHalfExtent->ax) {
        g_coliBounds->min[0] = g_coliXformPos->qx - g_coliHalfExtent->ax;
    }
    if (g_coliBounds->max[2] < g_coliXformPos->qz + g_coliHalfExtent->ax) {
        g_coliBounds->max[2] = g_coliHalfExtent->ax + g_coliXformPos->qz;
    }
    if (g_coliBounds->min[2] > g_coliXformPos->qz - g_coliHalfExtent->ax) {
        g_coliBounds->min[2] = g_coliXformPos->qz - g_coliHalfExtent->ax;
    }
    if (g_coliBounds->max[1] < g_coliXformPos->qy + g_coliHalfExtent->ay) {
        g_coliBounds->max[1] = g_coliHalfExtent->ay + g_coliXformPos->qy;
    }
    if (g_coliBounds->min[1] > g_coliXformPos->qy) {
        g_coliBounds->min[1] = g_coliXformPos->qy;
    }
}

// ---------------------------------------------------------------------------
// ColiXformRigid: transform a rigid reference node into self space, invert,
// then expand the global AABB using the transformed position and magnitude.
// ---------------------------------------------------------------------------
void ColiXformRigid(CColiNode* self, const CColiMgr* mgr, const CColiRigid* other) {
    const f32 (*src)[4];
    if (self->hd.flags & 2) {
        src = mgr->mesh->rootMtx;
    } else {
        src = mgr->mesh->boneTable[self->hd.index];
    }

    PSMTXConcat(src, other->mtx, self->mtxW);
    PSMTXInverse(self->mtxW, self->invW);

    nw4r::math::VEC3 n;

    // Forward locals give retail's fp-reg allocation (x->f0..z->f2) and
    // ascending stores.
    const f32 px = other->pos[0];
    const f32 py = other->pos[1];
    const f32 pz = other->pos[2];

    self->vec[0] = px;
    self->vec[1] = py;
    self->vec[2] = pz;
    nw4r::math::VEC3TransformNormal(&n, (const nw4r::math::MTX34*)self->mtxW,
                                    (const nw4r::math::VEC3*)self->vec);

    // First bounds check store-forwards the live magnitude (frsp); later
    // checks re-read self->radius after the possible-aliasing AABB stores.
    self->radius = PSVECMag((const Vec*)&n);
    self->hd.flags |= 1;

    // Expand the shared AABB around the world matrix translation column;
    // every check re-dereferences the bounds pointer and re-reads radius.
    {
        f32 r = self->radius;
        if (g_coliBounds->max[0] < self->mtxW[0][3] + r)
            g_coliBounds->max[0] = self->mtxW[0][3] + r;
    }
    {
        f32 r = self->radius;
        if (g_coliBounds->min[0] > self->mtxW[0][3] - r)
            g_coliBounds->min[0] = self->mtxW[0][3] - r;
    }
    {
        f32 r = self->radius;
        if (g_coliBounds->max[1] < self->mtxW[1][3] + r)
            g_coliBounds->max[1] = self->mtxW[1][3] + r;
    }
    {
        f32 r = self->radius;
        if (g_coliBounds->min[1] > self->mtxW[1][3] - r)
            g_coliBounds->min[1] = self->mtxW[1][3] - r;
    }
    {
        f32 r = self->radius;
        if (g_coliBounds->max[2] < self->mtxW[2][3] + r)
            g_coliBounds->max[2] = self->mtxW[2][3] + r;
    }
    {
        f32 r = self->radius;
        if (g_coliBounds->min[2] > self->mtxW[2][3] - r)
            g_coliBounds->min[2] = self->mtxW[2][3] - r;
    }
}

// ---------------------------------------------------------------------------
// ColiXformSphere: transform a sphere collision object's center into parent
// space, derive its radius from the transformed (z,0,0) normal, and expand
// the global AABB.
// ---------------------------------------------------------------------------
// Third argument of ColiXformSphere: the transformed normal seed lives at
// offset 0xC (three leading words).
struct CColiSphereSeed {
    f32 pad[3]; //0x0
    f32 val;    //0xC
};

extern "C" void ColiXformSphere(CColiSphereOb*, const CColiMgr*,
                              const CColiSphereSeed*);

void ColiXformSphere(CColiSphereOb* self, const CColiMgr* mgr,
                   const CColiSphereSeed* other) {
    // Two-statement typed copy (void* intermediate): keeps `other` as a
    // distinct local web born before `src`, so locals color r31-down
    // (o -> r31, src -> r30) and only self stays a saved param (r29).
    const void* ov = other;
    const CColiSphereSeed* o = (const CColiSphereSeed*)ov;
    const f32 (*src)[4] = (self->hd.flags & 2)
                              ? mgr->mesh->rootMtx
                              : mgr->mesh->boneTable[self->hd.index];

    Vec outv;
    PSMTXMultVec(src, (const Vec*)o, &outv);
    // Declared back-to-front: MWCC schedules the outv loads z, y, x like
    // retail.
    const f32 pz = outv.z;
    const f32 py = outv.y;
    const f32 px = outv.x;
    self->pos[0] = px;
    self->pos[1] = py;
    self->pos[2] = pz;

    nw4r::math::VEC3 n;
    n.x = o->val;
    n.y = 0.0f;
    n.z = 0.0f;
    nw4r::math::VEC3TransformNormal(&n, (const nw4r::math::MTX34*)src, &n);

    // Radius stored straight from the magnitude; the first bounds check's
    // radius local is store-forwarded (frsp).
    self->radius = PSVECMag((const Vec*)&n);
    self->hd.flags |= 1;

    {
        f32 r = self->radius;
        if (g_coliBounds->max[0] < self->pos[0] + r)
            g_coliBounds->max[0] = self->pos[0] + r;
    }
    {
        f32 r = self->radius;
        if (g_coliBounds->min[0] > self->pos[0] - r)
            g_coliBounds->min[0] = self->pos[0] - r;
    }
    {
        f32 r = self->radius;
        if (g_coliBounds->max[1] < self->pos[1] + r)
            g_coliBounds->max[1] = self->pos[1] + r;
    }
    {
        f32 r = self->radius;
        if (g_coliBounds->min[1] > self->pos[1] - r)
            g_coliBounds->min[1] = self->pos[1] - r;
    }
    {
        f32 r = self->radius;
        if (g_coliBounds->max[2] < self->pos[2] + r)
            g_coliBounds->max[2] = self->pos[2] + r;
    }
    {
        f32 r = self->radius;
        if (g_coliBounds->min[2] > self->pos[2] - r)
            g_coliBounds->min[2] = self->pos[2] - r;
    }
}

// ---------------------------------------------------------------------------
// ColiSetEllipsoid: read the transform's translation into an ellipsoid object,
// scale an (x/y/z) extent by the (1,0,0)-transformed normal magnitude, and
// expand the global AABB with horizontal (XZ) and vertical (Y) radii.
// ---------------------------------------------------------------------------
void ColiSetEllipsoid(CColiEllipsoidOb* self, const CColiMgr* mgr, const Vec* other) {
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
    n.x = 1.0f;
    n.y = 0.0f;
    n.z = 0.0f;
    nw4r::math::VEC3TransformNormal(&n, (const nw4r::math::MTX34*)src, &n);

    f32 mag = PSVECMag((const Vec*)&n);
    // Single-precision throughout: retail multiplies the extents straight
    // into the returned magnitude (fmuls chain, no frsp).
    self->f18 = other->z * mag;
    self->radiusY = other->y * mag;
    self->radiusXZ = other->x * mag;
    self->hd.flags |= 1;

    // Expand the shared AABB with the horizontal (XZ) and vertical (Y)
    // radii; plain field expressions keep pos as the leading operand
    // (retail loads pos -> f1 before the radius -> f0).
    if (g_coliBounds->max[0] < self->pos[0] + self->radiusXZ)
        g_coliBounds->max[0] = self->pos[0] + self->radiusXZ;
    if (g_coliBounds->min[0] > self->pos[0] - self->radiusXZ)
        g_coliBounds->min[0] = self->pos[0] - self->radiusXZ;
    if (g_coliBounds->max[2] < self->pos[2] + self->radiusXZ)
        g_coliBounds->max[2] = self->pos[2] + self->radiusXZ;
    if (g_coliBounds->min[2] > self->pos[2] - self->radiusXZ)
        g_coliBounds->min[2] = self->pos[2] - self->radiusXZ;
    if (g_coliBounds->max[1] < self->pos[1] + self->radiusY)
        g_coliBounds->max[1] = self->pos[1] + self->radiusY;
    if (g_coliBounds->min[1] > self->pos[1] - self->radiusY)
        g_coliBounds->min[1] = self->pos[1] - self->radiusY;
}

// ---------------------------------------------------------------------------
// ColiXformEllipsoid: transform a position into ellipsoid space, scale an offset
// into XZ/Y radii, and expand the global AABB (same layout as ColiSetEllipsoid).
// ---------------------------------------------------------------------------
void ColiXformEllipsoid(CColiEllipsoidOb* self, const CColiMgr* mgr,
                   const CColiPosVec* other) {
    // Distinct-local web for `other` so its local colors the first saved
    // register (r31), matching retail; the matrix source then colors r30.
    const void* ov = other;
    const CColiPosVec* o = (const CColiPosVec*)ov;

    const f32 (*src)[4];
    if (self->hd.flags & 2) {
        src = mgr->mesh->rootMtx;
    } else {
        src = mgr->mesh->boneTable[self->hd.index];
    }

    Vec outv;
    PSMTXMultVec(src, &o->pos, &outv);
    // Retail emits the loads back-to-front (z, y, x) and interleaves the
    // normal-seed stores between the position stores, which fixes both the
    // FPR allocation and the sdata2 literal-pool order.
    const f32 pz = outv.z;
    const f32 py = outv.y;
    const f32 px = outv.x;
    self->pos[0] = px;
    nw4r::math::VEC3 n;
    n.x = 1.0f;
    self->pos[1] = py;
    n.y = 0.0f;
    self->pos[2] = pz;
    n.z = 0.0f;
    nw4r::math::VEC3TransformNormal(&n, (const nw4r::math::MTX34*)src, &n);
    f32 mag = PSVECMag((const Vec*)&n);

    self->f18 = o->v.z * mag;
    self->radiusY = o->v.y * mag;
    self->radiusXZ = o->v.x * mag;
    self->hd.flags |= 1;

    // Every check re-reads the shared bounds pointer and the self fields
    // (retail reloads both per check).
    if (g_coliBounds->max[0] < self->pos[0] + self->radiusXZ)
        g_coliBounds->max[0] = self->pos[0] + self->radiusXZ;
    if (g_coliBounds->min[0] > self->pos[0] - self->radiusXZ)
        g_coliBounds->min[0] = self->pos[0] - self->radiusXZ;
    if (g_coliBounds->max[2] < self->pos[2] + self->radiusXZ)
        g_coliBounds->max[2] = self->pos[2] + self->radiusXZ;
    if (g_coliBounds->min[2] > self->pos[2] - self->radiusXZ)
        g_coliBounds->min[2] = self->pos[2] - self->radiusXZ;
    if (g_coliBounds->max[1] < self->pos[1] + self->radiusY)
        g_coliBounds->max[1] = self->pos[1] + self->radiusY;
    if (g_coliBounds->min[1] > self->pos[1] - self->radiusY)
        g_coliBounds->min[1] = self->pos[1] - self->radiusY;
}

// ---------------------------------------------------------------------------
// ColiSetCapsulePos: build a capsule from two bone matrices and expand the global
// AABB by both endpoints swept by the (transformed) radius magnitude.
// ---------------------------------------------------------------------------
// Bone-table entry for capsule nodes (stride 0x30): positions at
// +0xC/+0x1C/+0x2C.
struct CColiBoneEntry {
    char pad0[0xC]; //0x0
    f32 p0;         //0xC
    char pad1[0xC]; //0x10
    f32 p1;         //0x1C
    char pad2[0xC]; //0x20
    f32 p2;         //0x2C
}; //0x30

void ColiSetCapsulePos(CColiCapsuleNode* self, const CColiMgr* mgr, const Vec* other) {
    // Seed x-component read first so its load hoists above the table walks.
    const f32 ox = other->x;
    const CColiBoneEntry* srcA =
        &((const CColiBoneEntry*)mgr->mesh->boneTable)[self->hd.index];
    const CColiBoneEntry* srcB =
        &((const CColiBoneEntry*)mgr->mesh->boneTable)[self->index2];

    // Entries load back-to-front (p2,p1,p0), store forward.
    const f32 az = srcA->p2;
    const f32 ay = srcA->p1;
    const f32 ax = srcA->p0;
    self->posA[0] = ax;
    self->posA[1] = ay;
    self->posA[2] = az;

    const f32 bz = srcB->p2;
    const f32 by = srcB->p1;
    const f32 bx = srcB->p0;
    self->posB[0] = bx;
    self->posB[1] = by;
    self->posB[2] = bz;

    // Segment direction + squared length via the PS-inlined SDK helpers.
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&self->dir,
                        (const nw4r::math::VEC3*)&self->posB,
                        (const nw4r::math::VEC3*)&self->posA);
    self->lenSq = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&self->dir,
                                      (const nw4r::math::VEC3*)&self->dir);

    nw4r::math::VEC3 n;
    n.x = ox;
    n.y = 0.0f;
    n.z = 0.0f;
    nw4r::math::VEC3TransformNormal(&n, (const nw4r::math::MTX34*)srcA, &n);
    self->radius = PSVECMag((const Vec*)&n);
    self->hd.flags |= 1;

    // Every check re-dereferences the shared bounds pointer (aliasing).
    for (int i = 0; i < 3; i++) {
        if (g_coliBounds->max[i] < self->posA[i] + self->radius)
            g_coliBounds->max[i] = self->posA[i] + self->radius;
        if (g_coliBounds->min[i] > self->posA[i] - self->radius)
            g_coliBounds->min[i] = self->posA[i] - self->radius;
        if (g_coliBounds->max[i] < self->posB[i] + self->radius)
            g_coliBounds->max[i] = self->posB[i] + self->radius;
        if (g_coliBounds->min[i] > self->posB[i] - self->radius)
            g_coliBounds->min[i] = self->posB[i] - self->radius;
    }
}

// ---------------------------------------------------------------------------
// Coli_CopyBoneMtx: copy the bone matrix from the manager table into m0,
// derive its inverse into m1, then duplicate both up by one pair into the
// history slots.
// ---------------------------------------------------------------------------
extern "C" void Coli_CopyBoneMtx(CColiHead* hd, CColiMtxOut* dst, CColiMgr* mgr) {
    const s16 idx = hd->index;
    const CObjMtx* src = (const CObjMtx*)mgr->mesh->boneTable[idx];
    dst->cur.m0 = *src;
    PSMTXInverse(dst->cur.m0.m, dst->cur.m1.m);
    dst->old = dst->cur;
}

// The list walkers use the real CTaskColiManager owner and CColiQueryNode:
// mHead (+0x5C), next (+0x9C), prev (+0xA0), swept-distance (+0xA4) and
// status flags (+0xA8) are the fields these helpers touch.

// Append `node` to the back of the list owned by `self`, conditioning it with
// ColiSetMoveEnableFlag first. If the node is already present it is left untouched.
// Newly-linked nodes have their forward link cleared and their swept-distance
// accumulator (+0xA4) reset to zero.
extern "C" void ColiListAppend(CTaskColiManager* self, CColiQueryNode* node) {
    ColiSetMoveEnableFlag(node, 1);
    CColiQueryNode* cur = self->mHead;
    if (cur != 0) {
        CColiQueryNode* tail = cur;
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
        self->mHead = node;
        node->next = 0;
        node->prev = 0;
        node->f_a4 = lbl_eu_8066AEC0;
    }
}

// Remove `findNode` from the list owned by `self`, relinking neighbours and
// fixing the head when the removed node was the head. Goto-shaped to match
// the retail control flow: pre-check on the head value, entry jump to the
// tail condition, and the found path falling out to the common epilogue.
extern "C" void Coli_ListRemoveNode(CTaskColiManager* self, CColiQueryNode* findNode) {
    ColiSetMoveEnableFlag(findNode, 0);
    // Declared at function top so the allocator colours prev/next before the
    // loop variable: retail keeps cur in r4 with prev/next sharing r3.
    CColiQueryNode* prev;
    CColiQueryNode* next;
    CColiQueryNode* cur;
    // `head` stays loop-invariant (r0) for the final branch's comparison;
    // the middle branches re-read self->head because the relink stores may
    // alias it. `cur` is assigned after the pre-check like retail.
    CColiQueryNode* head = self->mHead;
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
            if (self->mHead == findNode) self->mHead = prev;
            goto done;
        }
        if (next != 0) {
            next->prev = 0;
            if (self->mHead == findNode) self->mHead = next;
            goto done;
        }
        if (head == findNode) self->mHead = 0;
        goto done;
    }
advance:
    cur = cur->next;
cond:
    if (cur != 0) goto body;
done:
    return;
}

// Static initializer (.ctors) for the shared collision walk object at
// sColiWalkState: run the CColiProc base constructor, then point its default
// target data at the alternate table sColiTargetTable.
struct CColiProcLocal {
    void* field_0x0; //0x0
    u32 field_0x4;   //0x4
};

extern "C" void sinit_804B598C() {
    // Placement-construct CColiProc over the walk-state object's storage,
    // then point its default target data at the alternate table. The address
    // is taken twice, so MWCC keeps only the HA half in r31 and folds the LO
    // half into each use (call arg / store displacement) like retail.
    __ct__9CColiProcFv((CColiProcLocal*)&sColiWalkState);
    ((CColiProcLocal*)&sColiWalkState)->field_0x0 = &sColiTargetTable;
}

// Walk/traverse helpers (C-linkage retail symbols in the sibling coli unit).

// Walk the segment query: reject degenerate (equal) vectors, otherwise run
// the segment build + node query and expand the result AABB.
extern "C" int ColiQuerySegment(CTaskColiManager* self, const Vec* a, const Vec* b,
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
        st = &sColiWalkState;
        st->flag = 0;
        // Local-pointer reads keep FPR coloring and load positions near
        // retail (plain const refs hoist to block start).
        f32 v92 = 1000000.0f;
        init = 1;
        f32 v76 = 1.0f;
        // Src snapshot: load x/y, store y (hi) before x (lo), then z.
        const CColiSrcVec* av = (const CColiSrcVec*)a;
        u32 lo = av->x;
        u32 hi = av->y;
        st->src.y = hi;
        f32 zero = 0.0f;
        st->src.x = lo;
        st->src.z = av->z;
        st->bbMax[2] = v76;
        st->f5c = v92;
        st->bbMin[0] = zero;
        st->bbMin[1] = zero;
        st->bbMin[2] = zero;
        st->bbMax[0] = zero;
        st->bbMax[1] = zero;
        self->mAabbMin[0] = zero;
        self->mAabbMin[1] = zero;
        self->mAabbMin[2] = zero;
        self->mAabbMax[0] = zero;
        self->mAabbMax[1] = zero;
        self->mAabbMax[2] = zero;
    }

    char seg[0x318];
    char query[0x18];
    ColiInitSegment(seg, a, b, st);
    int found = 0;
    ColiMakeAabbMinMax(query, a, b);

    CColiQueryNode* n = self->mHead;
    while (n != 0) {
        if (!(filter & n->filter)) {
            if (ColiTestAabbContains(query, n) != 0 &&
                ColiProcessSegmentSelect(n, seg, 1) != 0) {
                found = 1;
            }
        }
        n = n->next;
    }

    if (init != 0) {
        if (found == 0) return 0;
        if (sColiWalkState.flag == 0) return 0;
        // Word copies of the accumulated AABB: each 3-word group is a
        // pipelined pair (load x/y, store y/x) plus a trailing z word.
        const CColiSrcVec* smin =
            (const CColiSrcVec*)&sColiWalkState.bbMin[0];
        CColiSrcVec* dmin = (CColiSrcVec*)&self->mAabbMin[0];
        u32 mlo = smin->x;
        u32 mhi = smin->y;
        dmin->y = mhi;
        dmin->x = mlo;
        dmin->z = smin->z;
        const CColiSrcVec* smax =
            (const CColiSrcVec*)&sColiWalkState.bbMax[0];
        CColiSrcVec* dmax = (CColiSrcVec*)&self->mAabbMax[0];
        u32 xlo = smax->x;
        u32 xhi = smax->y;
        dmax->y = xhi;
        dmax->x = xlo;
        dmax->z = smax->z;
        return 1;
    }
    return found;
}

void CTaskColiManager::Init() {
    // Empty override - no initialization required.
}

void CTaskColiManager::Term() {
    // Empty override - no termination required.
}

// Walk the collision-node list every tick: condition the nodes (ColiNodeRefreshAxes),
// fold any node requesting a pass into mFlags bit 1, re-walk for post-pass
// cleanup (ColiNodeResortKeyed), then re-anchor at the front of the list.
void CTaskColiManager::Move() {
    CColiQueryNode* node = mHead;
    mFlags &= ~0x2u;
    if (node != 0) {
        while (node != 0) {
            ColiNodeRefreshAxes(node);
            if (node->flags & 0x2000) {
                mFlags |= 0x2u;
            }
            node = node->next;
        }
        node = mHead;
        while (node != 0) {
            ColiNodeResortKeyed(node);
            node = node->next;
        }
        CColiQueryNode* front = mHead;
        CColiQueryNode* p = mHead->prev;
        while (p != 0) {
            front = p;
            p = p->prev;
        }
        mHead = front;
    }
}

void CTaskColiManager::Draw() {
    // Empty override - no draw-time work required.
}

// Reset the shared walk flag then invoke the base (CProcess) destructor path.
CTaskColiManager::~CTaskColiManager() {
    sEneColiProbe = 0;
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
extern "C" int Coli_SweepSegNodes(u32 unused, CColiQueryNode* self, const CColiSeg* src,
                  u32 mask, void* arg) {
    if (!(self->flags & 4) || !(self->flags & 2)) return 0;

    u8 big[0x320];
    Vec query[2];

    ColiInitPointSpec(big, src, 0);
    ColiCopyPointPair(query, src);

    for (CColiQueryNode* n = self->next; n != 0; n = n->next) {
        if (mask & n->filter) continue;
        // Only nodes whose max bound is at/below the segment start qualify.
        if (query[0].x >= n->maxX) {
            if ((int)self->level > (int)n->level) continue;
            if (ColiTestAabbContains((const void*)query, n) == 0) continue;
            if (ColiQueryNodePoint(n, big, arg) == 0) continue;
            return 1;
        }
    }

    for (CColiQueryNode* n = self->prev; n != 0; n = n->prev) {
        if (mask & n->filter) continue;
        if (query[1].x <= n->minX) {
            if ((int)self->level > (int)n->level) continue;
            if (ColiTestAabbContains((const void*)query, n) == 0) continue;
            if (ColiQueryNodePoint(n, big, arg) == 0) continue;
            return 1;
        }
        // Sweep stop: past the node's tail, abandon the search (falls out of
        // the loop to the shared return 0, like retail).
        if (query[1].x > n->f_a4) break;
    }
    return 0;
}


// Coli_WalkReport: gate on the owner's walk-enable flag, reject degenerate
// (equal) vectors, then run the shared segment query over the node list and
// report the accumulated AABB through out1 (min) / out2 (max).
// ---------------------------------------------------------------------------
extern "C" int Coli_WalkReport(CTaskColiManager* self, Vec* out1, Vec* out2,
                  Vec* a, Vec* b) {
    // Single-exit shape: every zero path breaks to one shared return block
    // (retail merges all li r3,0 sites), success falls out of the block.
    // Nested guards with one shared zero exit (retail's single li r3,0).
    if (self->mFlags & 2) {
        // Materialized boolean:
        // (li r0,0 / li r0,1 / cmpwi r0,0) instead of short-circuiting.
        int eq = a->x == b->x && a->y == b->y && a->z == b->z;
        if (eq) return 0;

        char seg[0x310];
        char query[0x18];
    // Each sdata2 literal is loaded once into a local (retail hoists
    // f2/f1/f0) and reused; the src snapshot is written field-by-field
    // interleaved with the literal loads like retail's schedule.
    // Declarations precede their (scheduled) assignments so MWCC colors
    // tail/big/zero -> f0/f1/f2 and sx/st -> r7/r8 like retail.
    f32 tail;
    f32 big;
    f32 zero;
    u32 sx;
    CColiWalkState* st = &sColiWalkState;
        st->flag = 0;
        zero = lbl_eu_8066AEC0;
        big = lbl_eu_8066AEC4;
        sx = ((const CColiSrcVec*)a)->x;
        st->src.y = ((const CColiSrcVec*)a)->y;
        tail = lbl_eu_8066AEC8;
        st->src.x = sx;
        st->src.z = ((const CColiSrcVec*)a)->z;
        st->bbMin[0] = zero;
        st->bbMin[1] = zero;
        st->bbMin[2] = zero;
        st->bbMax[0] = zero;
        st->bbMax[1] = zero;
        st->bbMax[2] = big;
        st->f5c = tail;
    ColiInitSegment(seg, a, b, st);
    ColiMakeAabbMinMax(query, a, b);

    for (CColiQueryNode* n = self->mHead; n != 0; n = n->next) {
        if (n->flags & 0x2000) {
            if (ColiTestAabbContains(query, n) != 0) {
                ColiProcessSegmentSelect(n, seg, 1);
            }
        }
    }

    if (sColiWalkState.flag != 0) {
        // Word-wise POD copies via the pair+word snapshot type: the
        // {u32[2], u32} layout makes MWCC emit its pipelined 12-byte
        // copy (load-pair / store-pair / trailing word), like retail.
        *(CColiPairCopy*)out1 = *(const CColiPairCopy*)&sColiWalkState.bbMin[0];
        *(CColiPairCopy*)out2 = *(const CColiPairCopy*)&sColiWalkState.bbMax[0];
        return 1;
    }
    }
    return 0;
}

// ---------------------------------------------------------------------------
// CTaskColiManager::create -- the retail symbol is Fv but the body reads three
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

extern u8 __ptmf_null[12];
extern u32 lbl_eu_8056F4B0[9];
extern u32 lbl_eu_8056F468[9];

extern "C" CTaskColiManager* create__16CTaskColiManagerFv(CProcess* parent,
                                                          void* a, void* b) {
    void* mem =
        mtl::MemManager::allocate(0x80, CWorkThreadSystem::getWorkMem());
    CTaskColiManager* obj = (CTaskColiManager*)mem;
    if (mem != 0) {
        __ct__8CProcessFv((CProcess*)mem);
        CTaskColiManagerInit* init = (CTaskColiManagerInit*)mem;
        // CTTask ctor body (inlined):
        // then the CTaskColiManager vtable. Each null-ptmf member is filled
        // by a 3-word copy from __ptmf_null; retail keeps one materialised
        // base pointer across both copies.
        init->vtable = (u32)lbl_eu_8056F4B0;
        {
            // Shared base pointer: retail materialises __ptmf_null once and
            // keeps it live across both member-func copies.
            const u32* src = (const u32*)__ptmf_null;
            u32 tmp[2];
            tmp[0] = src[0];
            tmp[1] = src[1];
            init->ptmf[0] = tmp[0];
            init->ptmf[1] = tmp[1];
            init->ptmf[2] = src[2];
            tmp[0] = src[0];
            tmp[1] = src[1];
            init->ptmf[3] = tmp[0];
            init->ptmf[4] = tmp[1];
            init->ptmf[5] = src[2];
        }
        init->vtable = (u32)lbl_eu_8056F468;
        init->field_0x54 = (u32)a;
        init->field_0x58 = (u32)b;
        init->field_0x5C = 0;
        init->field_0x78 = 0;
        init->field_0x7C = (u32)mtl::MemManager::getHandleMEM1();
        f32 z = 0.0f;
        init->f_0x60[0] = z;
        init->f_0x60[1] = z;
        init->f_0x60[2] = z;
        init->f_0x60[3] = z;
        init->f_0x60[4] = z;
        init->f_0x60[5] = z;
        sEneColiProbe = (ColiProbeObj*)mem;
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
}

// ===== Dissolved retail data (novtable TU) =====
// Retail bytes/relocs dumped from build/us/asm/monolib/src/coli/code_804B2FF0.s:
// .rodata 0x80524428 size 0x40 align 8, .data 0x8056F3FC size 0x10C align 4,
// .sdata 0x80663AA8 size 0x20 align 8, .bss 0x8065D138 size 0x68 align 8,
// .sbss 0x80665944 size 0x1C align 4. Emitted as extern "C"-named arrays with
// per-object alignment (CDesktop pattern).

// Forward decls for the dissolved blocks (definitions below, retail order).
extern u32 __RTTI__16CTaskColiManager[2];
extern u32 lbl_eu_80663AB8[2];
extern u32 __RTTI__12CColiManCall[2];
extern u32 lbl_eu_8056F48C[9];
extern u32 lbl_eu_8056F4D4[7];
extern u32 lbl_eu_8056F4FC[3];
// Own task vtable slots (defined in this TU under their retail mangled
// member spellings).
extern "C" void __dt__16CTaskColiManagerFv();
extern "C" void Init__16CTaskColiManagerFv();
extern "C" void Term__16CTaskColiManagerFv();
extern "C" void Move__16CTaskColiManagerFv();
extern "C" void Draw__16CTaskColiManagerFv();
extern "C" void Reset__14CChildListNodeFv();
extern "C" void Tail__8CProcessFv();
extern "C" void gateMoverMove();
// The retail CTask<CTaskColiManager> vtable slots spell the template
// specialization members with <> in the mangled names - not legal C++
// identifiers. Legal stand-ins; the §17.6 UNIT_RULES retarget those three
// slots (tools/postprocess_reloc_names.py "code_804B2FF0.o").
extern "C" void cttask_ctm_dtor();
extern "C" void cttask_ctm_move();
extern "C" void cttask_ctm_draw();
// Shared monolib RTTI hierarchy objects (other TUs / monolibdata splits).
extern u8 __RTTI__15CDoubleListNode[];
extern u8 __RTTI__14CChildListNode[];
extern u8 __RTTI__8CProcess[];
extern u8 __RTTI__9CColiProc[];

// .sdata 0x80663AA8..0x80663AC8: assert needle + monolib type descriptors
// {name string, hierarchy table}.
// "ref" needle: defined under a legal stand-in spelling (the retail-sized
// const char[4] type conflicts with the char[8] va-arg decl above); renamed
// to lbl_eu_80663AA8 by UNIT_RULES exact_renames.
enum { sdata_ref_needle_size = 4 };
extern __declspec(section ".sdata") __attribute__((aligned(8))) const char sdata_ref_needle[sdata_ref_needle_size] = "ref";
extern __attribute__((aligned(8))) u32 __RTTI__16CTaskColiManager[2] = {
    (u32)&"CTaskColiManager"[0], (u32)&lbl_eu_8056F48C,
};
extern __attribute__((aligned(8))) u32 lbl_eu_80663AB8[2] = {
    (u32)&"CTTask<CTaskColiManager>"[0], (u32)&lbl_eu_8056F4D4,
};
extern __attribute__((aligned(8))) u32 __RTTI__12CColiManCall[2] = {
    (u32)&"CColiManCall\0\0\0"[0], (u32)&lbl_eu_8056F4FC,
};

// .data 0x8056F3C0..0x8056F3FC: per-type spec walk tables (step size, payload
// size, dispatch class), indexed by entry type by the CColiProc unit.
s16 ColiSpecEntryStep[10] = { 4, 0x10, 0xC, 0x18, 0x30, 8, 0x3C, 0x14, 0x18, 0 };
s16 ColiSpecEntrySize[10] = { 0x14, 0x14, 0x1C, 0x1C, 0x34, 0x34, 0x74, 0x68, 0, 0 };
s16 ColiSpecDispatchClass[10] = { 0, 0, 1, 1, 2, 2, 3, 4, 0, 0 };

// .data 0x8056F3FC..0x8056F508: shape-dispatch tables, task vtables and
// monolib RTTI hierarchy tables.
u32 ColiSpecDataHandlers[9] = {
    (u32)&Coli_FindNodeByName, (u32)&Coli_FindNodeByName, (u32)&Coli_FindNodeByName,
    (u32)&Coli_FindNodeByName, (u32)&Coli_FindNodeByName, (u32)&ColiFindNodePair,
    (u32)&Coli_FindNodeByName, (u32)&Coli_FindNodeByName, (u32)&Coli_FindNodeRecord,
};
u32 ColiSpecMtxHandlers[9] = {
    (u32)&ColiSetSphereMtx, (u32)&ColiXformSphere, (u32)&ColiSetEllipsoid,
    (u32)&ColiXformEllipsoid, (u32)&ColiUpdateCapsule, (u32)&ColiSetCapsulePos,
    (u32)&ColiXformRigid, (u32)&ColiUpdateMtxNode, (u32)&ColiXformFixed,
};
u32 ColiSpecExpandHandlers[9] = {
    (u32)&ColiExpandSphere, (u32)&ColiExpandSphere, (u32)&Coli_ExpandEllipsoid,
    (u32)&Coli_ExpandEllipsoid, (u32)&ColiExpandCapsule, (u32)&ColiExpandCapsule,
    (u32)&Coli_ExpandPoint, (u32)&ColiExpandPoint, (u32)&Coli_ExpandGlobalBox,
};
u32 lbl_eu_8056F468[9] = {
    (u32)&__RTTI__16CTaskColiManager, 0,
    (u32)&__dt__16CTaskColiManagerFv, (u32)&Reset__14CChildListNodeFv,
    (u32)&Init__16CTaskColiManagerFv, (u32)&Term__16CTaskColiManagerFv,
    (u32)&Move__16CTaskColiManagerFv, (u32)&Draw__16CTaskColiManagerFv,
    (u32)&Tail__8CProcessFv,
};
u32 lbl_eu_8056F48C[9] = {
    (u32)&__RTTI__15CDoubleListNode, 0, (u32)&__RTTI__14CChildListNode, 0,
    (u32)&__RTTI__8CProcess, 0, (u32)&lbl_eu_80663AB8, 0, 0,
};
u32 lbl_eu_8056F4B0[9] = {
    (u32)&lbl_eu_80663AB8, 0,
    (u32)&cttask_ctm_dtor, (u32)&Reset__14CChildListNodeFv,
    0, 0,
    (u32)&cttask_ctm_move, (u32)&cttask_ctm_draw,
    (u32)&Tail__8CProcessFv,
};
u32 lbl_eu_8056F4D4[7] = {
    (u32)&__RTTI__15CDoubleListNode, 0, (u32)&__RTTI__14CChildListNode, 0,
    (u32)&__RTTI__8CProcess, 0, 0,
};
u32 sColiTargetTable[3] = {
    (u32)&__RTTI__12CColiManCall, 0, (u32)&gateMoverMove,
};
u32 lbl_eu_8056F4FC[3] = {
    (u32)&__RTTI__9CColiProc, 0, 0,
};

// .bss 0x8065D138..0x8065D1A0: shared collision walk state (+ tail pad).
CColiWalkStateObj sColiWalkState;

// .sbss 0x80665944..0x80665960: bounds/Hx/Qx holders, master-node index,
// task singleton; _pad objects reproduce the retail 8-byte slots.
CColiBounds* g_coliBounds;
CColiQx* g_coliXformPos;
CColiHx* g_coliHalfExtent;
s16 g_coliNodeIndex;
ColiProbeObj* sEneColiProbe;
u8 lbl_eu_80665950_pad[2];
u8 lbl_eu_80665958_pad[4];
