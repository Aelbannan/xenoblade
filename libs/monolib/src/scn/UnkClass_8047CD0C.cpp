// Translation unit: monolib/src/scn/UnkClass_8047CD0C
//
// Node-pool manager shared by the scene path/walker classes (UnkClass_8047D2AC,
// UnkClass_8047E064 -- see code_8047D2AC.cpp).  It tracks a single allocation
// in the global sWalkNodeStorage slot, exposes a set of "get/clear" helpers on
// the pool object at sWalkTableLocator, and is initialised by sinit_8047D290.
//
// Retail main.dol is stripped; symbols.txt annotates every method as Fv even
// though getNodeBlock really takes a register index argument.  That one
// function is therefore defined with extern "C" linkage using the exact retail
// mangled name (PLAN.md §17.6 relocation-name pattern), mirroring how
// code_8047D2AC.cpp already references it.

#include <harness_catalog.h>
#include <types.h>
#include <monolib/util/MemManager.hpp>
#include <monolib/scn/UnkClass_8047E110.hpp>
#include <monolib/scn/UnkClass_8047CD0C.hpp>

// ------------------------------------------------------------------
// Node object type (0x28-byte elements in the pool's node array).  The
// methods initScnDefaults / resetScnState / attachScnNode live in another
// TU (code_8047D2AC.cpp) and are declared there with extern "C" linkage
// because retail annotates them as Fv while they take register args
// (PLAN.md §17.6 relocation-name pattern).  Keep the same layout here.
// ------------------------------------------------------------------

struct UnkClass_8047E064 {
    void initScnDefaults();
    void resetScnState();

    u32 mField0;   // 0x00
    f32 mField4;   // 0x04
    u16 mField8;   // 0x08
    u16 mFieldA;   // 0x0A
    u16 mFieldC;   // 0x0C
    u16 mFieldE;   // 0x0E
    u16 mFlags;    // 0x10
    u16 mField12;  // 0x12
    u16 mField14;  // 0x14
    u16 mField16;  // 0x16
    u16 mField18;  // 0x18
    u16 mField1A;  // 0x1A
    u16 mField1C;  // 0x1C
    u16 mField1E;  // 0x1E
    u16 mField20;  // 0x20
    u16 mField22;  // 0x22
    u16 mField24;  // 0x24
};

// Header of the scene-config blob: a magic word then the node array
// proper begins at offset 0x0C (after the magic + 8 pad bytes).
struct UnkCfgHead {
    u32 magic;   // 0x00
    u32 field4;  // 0x04
    u32 field8;  // 0x08
    u8 nodes[];  // 0x0C - node array follows the header (sizeof == 0x0C)
};
// ------------------------------------------------------------------
// Node-pool layout
// ------------------------------------------------------------------

struct UnkClass_8047CD0C {
    void configurePoolLayout(UnkClass_8047CD0C* src);
    u8* allocateNodeStorage(mtl::ALLOC_HANDLE handle, u32 size, u32 nodeCount);
    UnkClass_8047E064* allocFreeNode();
    void clearPoolData();
    void resetPoolState();
    void refreshPoolMemory();
    void releasePoolMemory();
    void clearPoolFlags();
    void freePoolAllocation();
    void createGlobalHeap();
    void freeGlobalHeap();

    u8* mData;        // 0x00 - owned allocation (freed/deallocated)
    u32 mField04;     // 0x04 - base/geometry (node-array base or a count)
    u32 mField08;     // 0x08 - count/geometry
    u16* mNodes;      // 0x0C - base of node index array
    u32 mNodeCount;   // 0x10
    u32 mStride;      // 0x14
    u32 mFlags;       // 0x18
    u32 mPad1C;       // 0x1C (retail pool symbol spans 0x20; tail unused)
};

// ------------------------------------------------------------------
// Extern declarations for the retail-linked node/scene helpers used by
// this TU.  All are Fv-annotated but really take register arguments, so
// they keep their exact retail mangled names via extern "C" (§17.6).
// ------------------------------------------------------------------

extern u8 lbl_eu_80658560[]; // scene manager (bss)

extern "C" void attachScnNode__17UnkClass_8047E064Fv(UnkClass_8047E064* self, u32 val);

// UnkClass_8047CD0C::configurePoolLayout / allocateNodeStorage are declared
// on the real class above; the compiler emits their C++ names.

// ------------------------------------------------------------------
// Externals: global node-pool object + owned-allocation slot (retail
// linker names; plain globals are not mangled so no extern "C" needed).
// ------------------------------------------------------------------

extern UnkClass_8047CD0C sWalkTableLocator; // node pool (bss)
u8* sWalkNodeStorage;    // name-guess: walk-node storage pointer; owned allocation slot (sbss)

// ------------------------------------------------------------------
// initFromConfig -- constructor from a scene-config blob.
// Config magic 0x57504F49: adopt node array (config+0x0C), resize the pool,
// attach nodes. Retail calls init__17UnkClass_8047E110Fv(self, config+0x0C).
extern "C" void init__17UnkClass_8047E110Fv(UnkClass_8047E110* self, void* desc);
// ------------------------------------------------------------------

extern "C" void initFromConfig__17UnkClass_8047CD0CFv(UnkClass_8047CD0C* self,
                                                      u8* config) {
    s32 i;
    s32 off;
    self->mData = 0;
    if (((UnkCfgHead*)config)->magic != 0x57504F49) {
        return;
    }
    config += 0xC;
    self->mData = config;
    init__17UnkClass_8047E110Fv((UnkClass_8047E110*)lbl_eu_80658560, config);
    sWalkTableLocator.configurePoolLayout((UnkClass_8047CD0C*)self->mData);
    if (self->mField08 != 0) {
        i = 0;
        off = 0;
        for (; i < (s32)self->mField08; i++) {
            attachScnNode__17UnkClass_8047E064Fv(
                (UnkClass_8047E064*)(self->mField04 + off), i);
            off += 0x28;
        }
    }
}
// ------------------------------------------------------------------
// initNodePool -- full pool setup.  Call D038 to (re)allocate the node
// storage, store the count, clear every node object, then attach them.
// Fv-annotated but really takes (handle, size, nodeCount) in r4-r6.
// ------------------------------------------------------------------

extern "C" void initNodePool__17UnkClass_8047CD0CFv(UnkClass_8047CD0C* self,
                                                      mtl::ALLOC_HANDLE handle,
                                                      u32 size, u32 nodeCount) {
    self->mField04 = (u32)sWalkTableLocator.allocateNodeStorage(handle, size, nodeCount);
    self->mField08 = nodeCount;
    s32 i = 0;
    s32 off = 0;
    while (i < (s32)self->mField08) {
        ((UnkClass_8047E064*)(self->mField04 + off))->resetScnState();
        off += 0x28;
        i++;
    }
    if (self->mData) {
        s32 o;
        s32 k = 0;
        o = 0;
        while (k < (s32)self->mField08) {
            attachScnNode__17UnkClass_8047E064Fv(
                (UnkClass_8047E064*)(self->mField04 + o), k);
            o += 0x28;
            k++;
        }
    }
}

// ------------------------------------------------------------------
// allocFreeNode -- find the first unowned node element, initialise it via
// initScnDefaults, clear its ownership flag, and return it (or NULL).
// ------------------------------------------------------------------

UnkClass_8047E064* UnkClass_8047CD0C::allocFreeNode() {
    if (mData) {
        for (s32 i = 0; i < (s32)mField08; i++) {
            // Check via the strength-reduced (accumulated) pointer, but pass a
            // freshly i*0x28-computed pointer to the call so MWCC materialses
            // a mulli like retail (keeps i*0x28 in a callee-saved reg).
            if (!((((UnkClass_8047E064*)(mField04 + i * 0x28))->mFlags) & 1)) {
                ((UnkClass_8047E064*)(mField04 + i * 0x28))->initScnDefaults();
                ((UnkClass_8047E064*)(mField04 + i * 0x28))->mFlags &= 0xff83;
                return (UnkClass_8047E064*)(mField04 + i * 0x28);
            }
        }
    }
    return 0;
}

// ------------------------------------------------------------------
// releaseNode -- clear the ownership flag of one node element by pointer
// (or of every element when target is NULL).  Fv-annotated, real arg in r4.
// ------------------------------------------------------------------

extern "C" void releaseNode__17UnkClass_8047CD0CFv(UnkClass_8047CD0C* self,
                                                      UnkClass_8047E064* target) {
    if (!self->mData) return;
    if (target) {
        // Compare via i*0x28, which MWCC strength-reduces into an advancing
        // offset; the match path recomputes via mulli (retail's dual cursor).
        for (s32 i = 0; i < (s32)self->mField08; i++) {
            if (target == (UnkClass_8047E064*)(self->mField04 + i * 0x28)) {
                ((UnkClass_8047E064*)self->mField04)[i].mFlags &= ~1;
                return;
            }
        }
    } else {
        for (s32 i = 0; i < (s32)self->mField08; i++) {
            ((UnkClass_8047E064*)self->mField04)[i].mFlags &= ~1;
        }
    }
}

// ------------------------------------------------------------------
// Pool "clear/reset" helpers
// ------------------------------------------------------------------

// clearPoolData -- clear own data pointer, then clear pool flags
void UnkClass_8047CD0C::clearPoolData() {
    mData = 0;
    sWalkTableLocator.clearPoolFlags();
}

// resetPoolState -- hand pool storage over to D1B0/D02C, clear own state
void UnkClass_8047CD0C::resetPoolState() {
    sWalkTableLocator.freePoolAllocation();
    mField04 = 0;
    mField08 = 0;
    mData = 0;
    sWalkTableLocator.clearPoolFlags();
}

// refreshPoolMemory -- tail-call to createGlobalHeap (reallocate pool storage)
void UnkClass_8047CD0C::refreshPoolMemory() {
    createGlobalHeap();
}

// releasePoolMemory -- tail-call to freeGlobalHeap (release pool storage)
void UnkClass_8047CD0C::releasePoolMemory() {
    freeGlobalHeap();
}

// clearPoolFlags -- clear flags word. noinline so the CFBC/CFD0 callers emit a
// tail-call `b` instead of inlining (retail keeps them as separate functions).
void UnkClass_8047CD0C::clearPoolFlags() {
    mFlags = 0;
}

// ------------------------------------------------------------------
// allocateNodeStorage -- (re)allocate the pool node storage.  If the global
// owned slot is already populated, reuse it; otherwise free the current
// allocation and allocate a fresh block sized for `nodeCount` 0x28-byte
// elements.  Returns the storage pointer in r3.  Fv-annotated (another
// TU calls it via CDBC), real args in r4-r6.  noinline so CDBC emits a
// real `bl` to it (retail keeps it a separate function).
// ------------------------------------------------------------------

u8* UnkClass_8047CD0C::allocateNodeStorage(mtl::ALLOC_HANDLE handle,
                                           u32 size, u32 nodeCount) {
    if (!sWalkNodeStorage) {
        // Nested identical null-tests -> one cmp + two beq (dead second
        // branch), matching retail's double-beq shape.
        if (mData) {
            if (mData) {
                mtl::MemManager::deallocate(mData);
                mData = 0;
            }
        }
        mData =
            (u8*)mtl::MemManager::allocate_head(handle, size, 0x20);
    } else {
        mData = sWalkNodeStorage;
    }
    mNodeCount = nodeCount;
    mField04 = size - nodeCount * 0x28;
    mField08 = (u32)mData + nodeCount * 0x28;
    mFlags |= 1;
    return mData;
}

// ------------------------------------------------------------------
// configurePoolLayout -- (re)size the pool geometry from a node header's counts:
// align a computed capacity, store it as the node-array base, and derive
// the element stride from the leftover space.  Fv-annotated, real arg in r4.
// noinline so the CD0C constructor emits a real `bl` to it.
// ------------------------------------------------------------------

void UnkClass_8047CD0C::configurePoolLayout(UnkClass_8047CD0C* src) {
    if (!(mFlags & 1)) return;

    u32 cap = (u32)src->mData;
    // count*4 + 0x28 must stay unfolded as slwi+add+addi (retail shape): any
    // single-expression multiply form (count*4, (count*2)*2, count*2+count*2,
    // (count+count)*2) is folded by MWCC into one rlwinm count,2.  Only a
    // compound self-add survives; keeping the doubled value in short-lived t
    // (dead after the +0x28) lets the add reuse t's register in place.
    u32 t = src->mNodeCount * 2;
    t += t;
    u32 base = t + 0x28;
    // Retail defaults sel to cap>>1 and only overrides with cap when cap < 0x1f4
    // (bge skips the copy); the select is evaluated inline so shr is born late.
    u32 grow = (cap + ((cap < 0x1f4) ? cap : (cap >> 1)) * 2) * 2;
    if (grow > base) {
        base = grow;
    }
    base += 0x1f - (base & 0x1f);

    mNodes = (u16*)(mField08 + base);
    mStride = ((mField04 - base) / mNodeCount) >> 1;
    // Written LAST (with the plain `= |` form) so the mFlags web is born last and
    // colored first (r0) like retail; the scheduler keeps the mFlags store first
    // because the field offsets don't alias.
    mFlags = mFlags | 2;
}

// ------------------------------------------------------------------
// getNodeBlock -- resolve node subset pointer for an index.
// Declared Fv in symbols.txt, but the real code reads a register
// index argument (index in r4), so it must keep the exact retail
// mangled name via extern "C" (§17.6).
// ------------------------------------------------------------------

extern "C" u16* getNodeBlock__17UnkClass_8047CD0CFv(UnkClass_8047CD0C* self, u32 index) {
    // Success block falls through into the shared return-0 tail, matching retail's
    // compute-in-the-middle / li r3,0-at-the-end layout.
    if ((self->mFlags & 2) && self->mNodeCount > index) {
        return self->mNodes + (index * self->mStride);
    }
    return 0;
}

// freePoolAllocation -- release owned allocation and reset pool fields
void UnkClass_8047CD0C::freePoolAllocation() {
    if (!sWalkNodeStorage) {
        if (mData) {
            mtl::MemManager::deallocate(mData);
            mData = 0;
        }
    }
    mData = 0;
    mFlags = 0;
}

// createGlobalHeap -- (re)allocate the pool storage slot. noinline so refreshPoolMemory
// emits a tail-call `b` rather than inlining this body. The duplicated `lbl || lbl`
// makes MWCC CSE the null-test into one cmp + two beq (dead second branch).
void UnkClass_8047CD0C::createGlobalHeap() {
    // Nested identical null-tests so MWCC emits two beq-skip branches (dead second
    // branch) before the deallocate, matching retail's double-beq shape.
    if (sWalkNodeStorage) {
        if (sWalkNodeStorage) {
            mtl::MemManager::deallocate(sWalkNodeStorage);
            sWalkNodeStorage = 0;
        }
    }
    sWalkNodeStorage = static_cast<u8*>(
        mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM1(), 0xAF000, 0x20));
}

// freeGlobalHeap -- release the pool storage slot. noinline so releasePoolMemory
// emits a tail-call `b` rather than inlining this body. Same duplicate-null-test
// trick as D208 for the retail dead-beq shape.
void UnkClass_8047CD0C::freeGlobalHeap() {
    // Nested identical null-tests -> two beq-skip branches (dead second branch),
    // matching retail's double-beq before deallocate.
    if (sWalkNodeStorage) {
        if (sWalkNodeStorage) {
            mtl::MemManager::deallocate(sWalkNodeStorage);
        }
    }
    sWalkNodeStorage = 0;
}

// ------------------------------------------------------------------
// Static init: zero the pool object (node-pool lifetime guard).
// ------------------------------------------------------------------

extern "C" void sinit_8047D290() {
    sWalkTableLocator.mData = 0;
    sWalkTableLocator.mField04 = 0;
    sWalkTableLocator.mFlags = 0;
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.bss] 0x80658540-0x80658648 (0x108 = 264B): node-pool manager object
// (0x20) + scene-manager block (0xA8) + two tail objects (0x30 / 0x10).
UnkClass_8047CD0C sWalkTableLocator;  // node pool (0x20)
u8 lbl_eu_80658560[0xA8];           // scene manager (UnkClass_8047E110)
u8 sWalkWaypointTable[0x30];
f32 sMaruShadowCenter[4];
// NOTE: retail's split carries no FORCEACTIVE functions for this TU, and
// every bss symbol below is a non-static definition, so nothing can be
// dead-stripped.


// [.sbss] 0x806658C8-0x806658D8 (0x10 = 16B): owned-allocation slot (used
// as a 4-byte pointer) + pad to the retail 8-byte symbol + the 8-byte
// partner slot.
u32 lbl_eu_806658C8x;     // retail symbol tail (unused)
u8 sWalkWaypointInitFlags[8];
