// Translation unit: monolib/src/scn/UnkClass_8047CD0C
//
// Node-pool manager shared by the scene path/walker classes (UnkClass_8047D2AC,
// UnkClass_8047E064 -- see code_8047D2AC.cpp).  It tracks a single allocation
// in the global lbl_eu_806658C8 slot, exposes a set of "get/clear" helpers on
// the pool object at lbl_eu_80658540, and is initialised by sinit_8047D290.
//
// Retail main.dol is stripped; symbols.txt annotates every method as Fv even
// though func_8047D178 really takes a register index argument.  That one
// function is therefore defined with extern "C" linkage using the exact retail
// mangled name (PLAN.md §17.6 relocation-name pattern), mirroring how
// code_8047D2AC.cpp already references it.

#include <harness_catalog.h>
#include <types.h>
#include <monolib/util/MemManager.hpp>
#include <monolib/scn/UnkClass_8047E110.hpp>

// ------------------------------------------------------------------
// Node object type (0x28-byte elements in the pool's node array).  The
// methods func_8047E0B8 / func_8047E100 / func_8047E064 live in another
// TU (code_8047D2AC.cpp) and are declared there with extern "C" linkage
// because retail annotates them as Fv while they take register args
// (PLAN.md §17.6 relocation-name pattern).  Keep the same layout here.
// ------------------------------------------------------------------

struct UnkClass_8047E064 {
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
};

// ------------------------------------------------------------------
// Node-pool layout
// ------------------------------------------------------------------

struct UnkClass_8047CD0C {
    UnkClass_8047E064* func_8047CE7C();
    void func_8047CFBC();
    void func_8047CFD0();
    void func_8047D024();
    void func_8047D028();
    void func_8047D02C();
    void func_8047D1B0();
    void func_8047D208();
    void func_8047D258();

    u8* mData;        // 0x00 - owned allocation (freed/deallocated)
    u32 mField04;     // 0x04 - base/geometry (node-array base or a count)
    u32 mField08;     // 0x08 - count/geometry
    u16* mNodes;      // 0x0C - base of node index array
    u32 mNodeCount;   // 0x10
    u32 mStride;      // 0x14
    u32 mFlags;       // 0x18
};

// ------------------------------------------------------------------
// Extern declarations for the retail-linked node/scene helpers used by
// this TU.  All are Fv-annotated but really take register arguments, so
// they keep their exact retail mangled names via extern "C" (§17.6).
// ------------------------------------------------------------------

extern u8 lbl_eu_80658560[]; // scene manager (bss)

extern "C" void func_8047E110__17UnkClass_8047E110Fv(u8* self);

extern "C" void func_8047E0B8__17UnkClass_8047E064Fv(UnkClass_8047E064* self);
extern "C" void func_8047E100__17UnkClass_8047E064Fv(UnkClass_8047E064* self);
extern "C" void func_8047E064__17UnkClass_8047E064Fv(UnkClass_8047E064* self, u32 val);

extern "C" void func_8047D0F0__17UnkClass_8047CD0CFv(UnkClass_8047CD0C* self,
                                                      UnkClass_8047CD0C* src);
extern "C" u8* func_8047D038__17UnkClass_8047CD0CFv(UnkClass_8047CD0C* self,
                                                      mtl::ALLOC_HANDLE handle,
                                                      u32 size, u32 nodeCount);

// ------------------------------------------------------------------
// Externals: global node-pool object + owned-allocation slot (retail
// linker names; plain globals are not mangled so no extern "C" needed).
// ------------------------------------------------------------------

extern UnkClass_8047CD0C lbl_eu_80658540; // node pool (bss)
extern void* lbl_eu_806658C8;             // owned allocation slot (sbss)

// ------------------------------------------------------------------
// Pool "clear/reset" helpers
// ------------------------------------------------------------------

// func_8047D024 -- tail-call to func_8047D208 (reallocate pool storage)
void UnkClass_8047CD0C::func_8047D024() {
    func_8047D208();
}

// func_8047D028 -- tail-call to func_8047D258 (release pool storage)
void UnkClass_8047CD0C::func_8047D028() {
    func_8047D258();
}

// func_8047D02C -- clear flags word. noinline so the CFBC/CFD0 callers emit a
// tail-call `b` instead of inlining (retail keeps them as separate functions).
void __declspec(noinline) UnkClass_8047CD0C::func_8047D02C() {
    mFlags = 0;
}

// func_8047CFBC -- clear own data pointer, then clear pool flags
void UnkClass_8047CD0C::func_8047CFBC() {
    mData = 0;
    lbl_eu_80658540.func_8047D02C();
}

// func_8047CFD0 -- hand pool storage over to D1B0/D02C, clear own state
void UnkClass_8047CD0C::func_8047CFD0() {
    lbl_eu_80658540.func_8047D1B0();
    mField04 = 0;
    mField08 = 0;
    mData = 0;
    lbl_eu_80658540.func_8047D02C();
}

// func_8047D1B0 -- release owned allocation and reset pool fields
void __declspec(noinline) UnkClass_8047CD0C::func_8047D1B0() {
    if (!lbl_eu_806658C8) {
        if (mData) {
            mtl::MemManager::deallocate(mData);
            mData = 0;
        }
    }
    mData = 0;
    mFlags = 0;
}

// func_8047D208 -- (re)allocate the pool storage slot. noinline so func_8047D024
// emits a tail-call `b` rather than inlining this body. The duplicated `lbl || lbl`
// makes MWCC CSE the null-test into one cmp + two beq (dead second branch).
void __declspec(noinline) UnkClass_8047CD0C::func_8047D208() {
    // Nested identical null-tests so MWCC emits two beq-skip branches (dead second
    // branch) before the deallocate, matching retail's double-beq shape.
    if (lbl_eu_806658C8) {
        if (lbl_eu_806658C8) {
            mtl::MemManager::deallocate(lbl_eu_806658C8);
            lbl_eu_806658C8 = 0;
        }
    }
    lbl_eu_806658C8 =
        mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM1(), 0xAF000, 0x20);
}

// func_8047D258 -- release the pool storage slot. noinline so func_8047D028
// emits a tail-call `b` rather than inlining this body. Same duplicate-null-test
// trick as D208 for the retail dead-beq shape.
void __declspec(noinline) UnkClass_8047CD0C::func_8047D258() {
    // Nested identical null-tests -> two beq-skip branches (dead second branch),
    // matching retail's double-beq before deallocate.
    if (lbl_eu_806658C8) {
        if (lbl_eu_806658C8) {
            mtl::MemManager::deallocate(lbl_eu_806658C8);
        }
    }
    lbl_eu_806658C8 = 0;
}

// ------------------------------------------------------------------
// func_8047CD0C -- constructor from a scene-config blob.
// If the config begins with the magic 0x57504F49, adopt the node array
// (config+0x0C), (re)size the global pool via func_8047D0F0, and attach
// every node element to the pool.  Fv-annotated but really takes config.
// ------------------------------------------------------------------

extern "C" void func_8047CD0C__17UnkClass_8047CD0CFv(UnkClass_8047CD0C* self,
                                                      u8* config) {
    self->mData = 0;
    if (*(u32*)config == 0x57504F49) {
        config += 0xc;
        self->mData = config;
        func_8047E110__17UnkClass_8047E110Fv(lbl_eu_80658560);
        func_8047D0F0__17UnkClass_8047CD0CFv(&lbl_eu_80658540,
                                             (UnkClass_8047CD0C*)self->mData);
        if (self->mField08 != 0) {
            s32 i = 0;
            s32 off = 0;
            for (; i < (s32)self->mField08; i++) {
                func_8047E064__17UnkClass_8047E064Fv(
                    (UnkClass_8047E064*)(self->mField04 + off), i);
                off += 0x28;
            }
        }
    }
}

// ------------------------------------------------------------------
// func_8047CDBC -- full pool setup.  Call D038 to (re)allocate the node
// storage, store the count, clear every node object, then attach them.
// Fv-annotated but really takes (handle, size, nodeCount) in r4-r6.
// ------------------------------------------------------------------

extern "C" void func_8047CDBC__17UnkClass_8047CD0CFv(UnkClass_8047CD0C* self,
                                                      mtl::ALLOC_HANDLE handle,
                                                      u32 size, u32 nodeCount) {
    self->mField04 = (u32)func_8047D038__17UnkClass_8047CD0CFv(
        &lbl_eu_80658540, handle, size, nodeCount);
    self->mField08 = nodeCount;
    s32 i = 0;
    s32 off = 0;
    while (i < (s32)self->mField08) {
        func_8047E100__17UnkClass_8047E064Fv(
            (UnkClass_8047E064*)(self->mField04 + off));
        off += 0x28;
        i++;
    }
    if (self->mData) {
        s32 o;
        s32 k = 0;
        o = 0;
        while (k < (s32)self->mField08) {
            func_8047E064__17UnkClass_8047E064Fv(
                (UnkClass_8047E064*)(self->mField04 + o), k);
            o += 0x28;
            k++;
        }
    }
}

// ------------------------------------------------------------------
// func_8047CE7C -- find the first unowned node element, initialise it via
// func_8047E0B8, clear its ownership flag, and return it (or NULL).
// ------------------------------------------------------------------

UnkClass_8047E064* UnkClass_8047CD0C::func_8047CE7C() {
    if (mData) {
        for (s32 i = 0; i < (s32)mField08; i++) {
            // Check via the strength-reduced (accumulated) pointer, but pass a
            // freshly i*0x28-computed pointer to the call so MWCC materialses
            // a mulli like retail (keeps i*0x28 in a callee-saved reg).
            if (!((((UnkClass_8047E064*)(mField04 + i * 0x28))->mFlags) & 1)) {
                func_8047E0B8__17UnkClass_8047E064Fv(
                    (UnkClass_8047E064*)(mField04 + i * 0x28));
                ((UnkClass_8047E064*)(mField04 + i * 0x28))->mFlags &= 0xff83;
                return (UnkClass_8047E064*)(mField04 + i * 0x28);
            }
        }
    }
    return 0;
}

// ------------------------------------------------------------------
// func_8047CF20 -- clear the ownership flag of one node element by pointer
// (or of every element when target is NULL).  Fv-annotated, real arg in r4.
// ------------------------------------------------------------------

extern "C" void func_8047CF20__17UnkClass_8047CD0CFv(UnkClass_8047CD0C* self,
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
// func_8047D038 -- (re)allocate the pool node storage.  If the global
// owned slot is already populated, reuse it; otherwise free the current
// allocation and allocate a fresh block sized for `nodeCount` 0x28-byte
// elements.  Returns the storage pointer in r3.  Fv-annotated (another
// TU calls it via CDBC), real args in r4-r6.  noinline so CDBC emits a
// real `bl` to it (retail keeps it a separate function).
// ------------------------------------------------------------------

extern "C" __declspec(noinline) u8* func_8047D038__17UnkClass_8047CD0CFv(UnkClass_8047CD0C* self,
                                                      mtl::ALLOC_HANDLE handle,
                                                      u32 size, u32 nodeCount) {
    if (!lbl_eu_806658C8) {
        // Nested identical null-tests -> one cmp + two beq (dead second
        // branch), matching retail's double-beq shape.
        if (self->mData) {
            if (self->mData) {
                mtl::MemManager::deallocate(self->mData);
                self->mData = 0;
            }
        }
        self->mData =
            (u8*)mtl::MemManager::allocate_head(handle, size, 0x20);
    } else {
        self->mData = (u8*)lbl_eu_806658C8;
    }
    self->mNodeCount = nodeCount;
    self->mField04 = size - nodeCount * 0x28;
    self->mField08 = (u32)self->mData + nodeCount * 0x28;
    self->mFlags |= 1;
    return self->mData;
}

// ------------------------------------------------------------------
// func_8047D0F0 -- (re)size the pool geometry from a node header's counts:
// align a computed capacity, store it as the node-array base, and derive
// the element stride from the leftover space.  Fv-annotated, real arg in r4.
// noinline so the CD0C constructor emits a real `bl` to it.
// ------------------------------------------------------------------

extern "C" __declspec(noinline) void func_8047D0F0__17UnkClass_8047CD0CFv(UnkClass_8047CD0C* self,
                                                      UnkClass_8047CD0C* src) {
    if (!(self->mFlags & 1)) return;

    u32 cap = (u32)src->mData;
    u32 shr = cap >> 1;
    u32 base = ((src->mNodeCount * 2) * 2) + 0x28;
    u32 sel = (cap >= 0x1f4) ? shr : cap;
    u32 grow = (cap + sel * 2) * 2;
    if (grow > base) {
        base = grow;
    }
    base += 0x1f - (base & 0x1f);

    self->mFlags |= 2;
    self->mNodes = (u16*)(self->mField08 + base);
    self->mStride = ((self->mField04 - base) / self->mNodeCount) >> 1;
}

// ------------------------------------------------------------------
// func_8047D178 -- resolve node subset pointer for an index.
// Declared Fv in symbols.txt, but the real code reads a register
// index argument (index in r4), so it must keep the exact retail
// mangled name via extern "C" (§17.6).
// ------------------------------------------------------------------

extern "C" u16* func_8047D178__17UnkClass_8047CD0CFv(UnkClass_8047CD0C* self, u32 index) {
    // Success block falls through into the shared return-0 tail, matching retail's
    // compute-in-the-middle / li r3,0-at-the-end layout.
    if ((self->mFlags & 2) && self->mNodeCount > index) {
        return self->mNodes + (index * self->mStride);
    }
    return 0;
}

// ------------------------------------------------------------------
// Static init: zero the pool object (node-pool lifetime guard).
// ------------------------------------------------------------------

extern "C" void sinit_8047D290() {
    lbl_eu_80658540.mData = 0;
    lbl_eu_80658540.mField04 = 0;
    lbl_eu_80658540.mFlags = 0;
}