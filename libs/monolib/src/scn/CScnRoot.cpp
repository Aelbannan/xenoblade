// Translation unit: monolib/src/scn/CScnRoot
// Contains CScnRoot class and related helpers.

#include <types.h>
#include <stdlib.h>                    // labs
#include <revolution/MEM.h>
#include "monolib/scn/CScnRoot.hpp"       // CScnRoot class (include path)
#include "monolib/scn/CScnRootNw4r.hpp"   // __dt__12CScnRootNw4rFv import
#include "monolib/util/MemManager.hpp"    // mtl::MemManager
#include "libs/monolib/src/scn/CScnRoot.hpp"       // flat layouts + retail data symbols
#include "libs/monolib/src/scn/CScnRootNw4r.hpp"   // CScnRootNw4r class (vf9, field_0xC)

// --- CScnRoot class ---

// Retail C-linkage constructor names (forward decls; defined below).
CScnRootNw4r* __ct__CScnRootNw4r(CScnRootNw4r* obj, void* mgr, void* param);

// Constructor (retail C-linkage name __ct__CScnRoot__FP8CScnRootPv):
// installs the vtable, stores the mgr pointer, zeroes the reserved word.
extern "C" void __ct__CScnRoot__FP8CScnRootPv(CScnRoot* root, void* mgr);
void __ct__CScnRoot__FP8CScnRootPv(CScnRoot* root, void* mgr) {
    CScnRootLayout* s = (CScnRootLayout*)root;
    s->vtable = (void*)lbl_eu_8056E730;
    s->mpMgr = mgr;
    s->mReserved = 0;
}

// Destructor (extern "C" so the vtable reloc carries the un-mangled retail
// name; the strong definition is the C++ member dtor CScnRoot::~CScnRoot
// below, which MWCC mangles to the same symbol).
extern "C" void __dt__8CScnRootFv(u8* self, int deleteFlag);

// Shared-pool s32->f32 conversion magic (retail loads the named constant
// lbl_eu_8066A9F8 instead of pooling a local .sdata2 entry). The helper
// reproduces the retail xoris/stw/lfd/lfd/fsubs sequence so this TU emits
// no .sdata2 of its own (retail .sdata2 is empty for CScnRoot).
extern const double lbl_eu_8066A9F8;  // 0x4330000080000000 = 2^52 + 2^31

static inline f32 s32ToF32_8066A9F8(s32 v) {
    union { u32 w[2]; double d; } c;
    c.w[0] = 0x43300000u;
    c.w[1] = (u32)v ^ 0x80000000u;
    return (f32)(c.d - lbl_eu_8066A9F8);
}
// --- CScnRoot helpers ---

// Allocates a 0x4EC-byte CScnRootNw4r and constructs it in place.
CScnRootNw4r* func_8048ED80(void* mgr, u32 allocHandle, void* param) {
    CScnRootNw4r* obj =
        (CScnRootNw4r*)mtl::MemManager::allocate(0x4EC, allocHandle);
    if (obj != NULL) {
        return __ct__CScnRootNw4r(obj, mgr, param);
    }
    return obj;
}

// --- FULL_MATCH virtual callback stubs ---
// These are C-linkage functions referenced from the CScnRoot vtable.
// They unconditionally return -1 (no-op stubs for unused virtual slots).

extern "C" int func_8048EDD0()
{
    return -1;
}

extern "C" int func_8048EDD8()
{
    return -1;
}

extern "C" int func_8048EDE0()
{
    return -1;
}

// --- CScnRootNw4r class ---

// Constructor for CScnRootNw4r (inherits CScnRoot).
CScnRootNw4r* __ct__CScnRootNw4r(CScnRootNw4r* obj, void* mgr, void* param) {
    __ct__CScnRoot__FP8CScnRootPv((CScnRoot*)obj, mgr);

    CScnRootNw4rLayout* s = (CScnRootNw4rLayout*)obj;

    // Base field init + vtable override.
    s->mScnRoot = NULL;
    s->mpCallback = (void*)(lbl_eu_8056E768 + 0x38);
    s->vtable = (CScnRootNw4rVtbl*)lbl_eu_8056E768;
    s->mBloom = NULL;
    s->field_0x18 = 0;
    s->field_0x1A = 0;
    s->field_0x1C = 0;
    s->mPairCount = 0;
    s->mIdCount = 0;
    s->field_0x448 = 0;
    s->mGroup[0].mHandle = 0xFFFFFFFF;
    s->mGroup[1].mHandle = 0xFFFFFFFF;
    s->mGroup[2].mHandle = 0xFFFFFFFF;
    s->mGroup[3].mHandle = 0xFFFFFFFF;

    // Viewport overrides (default: 20.0f scale, 200 height, 160-wide groups).
    f32 scale = lbl_eu_8066A9E8;
    u32 vpHeight = 0xC8;
    u16 rect[10];
    for (u32 i = 0; i < 10; i++) {
        rect[i] = 0xA0;
    }

    if (param != NULL) {
        CScnRootNw4rParam* p = (CScnRootNw4rParam*)param;
        scale = p->field_0x8;
        vpHeight = p->field_0xC;
        for (u32 i = 0; i < 10; i++) {
            rect[i] = p->rect[i];
        }
    }

    // Size the scene-root buffers from the owning region's size.
    CScnRootMgr* scene = (CScnRootMgr*)s->mpMgr;
    scale *= lbl_eu_8066A9EC;
    CScnRootMgrRegion* region = (CScnRootMgrRegion*)scene->field_0x88;
    s32 v = labs(region->field_0xC);

    s32 bufsz = (s32)(s32ToF32_8066A9F8(v) * scale);
    if (bufsz & 0x1F) {
        bufsz = (s32)(s32ToF32_8066A9F8(v) * scale) + 0x20 - ((s32)(s32ToF32_8066A9F8(v) * scale) & 0x1F);
    }
    s32 rem = v - bufsz;
    s32 rem2 = 0;
    if (rem >= 0x80000) {
        rem2 = (s32)(s32ToF32_8066A9F8(rem) * lbl_eu_8066A9F0);
        if (rem2 & 0xF) {
            rem2 = rem2 + 0x10 - (rem2 & 0xF);
        }
        rem -= rem2;
    }

    // Per-slot alloc regions: create from the region handle when the viewport
    // scale is positive, otherwise fall back to the MEM2 handle.
    if (scale > lbl_eu_8066A9F4) {
        s->mGroup[0].mHandle =
            mtl::MemManager::create(region->field_0x8, rem - 0x100, &lbl_eu_80523FE0[0]);
    } else {
        s->mGroup[0].mHandle = mtl::MemManager::getHandleMEM2();
    }
    s->mGroup[0].mFuncs.allocFunc = (MEMAllocatorAllocFunc)func_8048FE34;
    s->mGroup[0].mFuncs.freeFunc = (MEMAllocatorFreeFunc)func_8048FEC4;
    s->mAlloc[0].funcs = &s->mGroup[0].mFuncs;
    s->mAlloc[0].heap = NULL;
    s->mAlloc[0].heapParam1 = (u32)(uintptr_t)obj;
    s->mAlloc[0].heapParam2 = s->mGroup[0].mHandle;

    if (scale > lbl_eu_8066A9F4) {
        s->mGroup[1].mHandle =
            mtl::MemManager::create(region->field_0x8, rem2 - 0x100, &lbl_eu_80523FE0[9]);
    } else {
        s->mGroup[1].mHandle = mtl::MemManager::getHandleMEM2();
    }
    s->mGroup[1].mFuncs.allocFunc = (MEMAllocatorAllocFunc)func_8048FE34;
    s->mGroup[1].mFuncs.freeFunc = (MEMAllocatorFreeFunc)func_8048FEC4;
    s->mAlloc[1].funcs = &s->mGroup[1].mFuncs;
    s->mAlloc[1].heap = NULL;
    s->mAlloc[1].heapParam1 = (u32)(uintptr_t)obj;
    s->mAlloc[1].heapParam2 = s->mGroup[1].mHandle;

    if (scale > lbl_eu_8066A9F4) {
        s->mGroup[2].mHandle =
            mtl::MemManager::create(region->field_0x8, bufsz - 0x100, &lbl_eu_80523FE0[0x15]);
    } else {
        s->mGroup[2].mHandle = mtl::MemManager::getHandleMEM2();
    }
    s->mGroup[2].mFuncs.allocFunc = (MEMAllocatorAllocFunc)func_8048FE34;
    s->mGroup[2].mFuncs.freeFunc = (MEMAllocatorFreeFunc)func_8048FEC4;
    s->mAlloc[2].funcs = &s->mGroup[2].mFuncs;
    s->mAlloc[2].heap = NULL;
    s->mAlloc[2].heapParam1 = (u32)(uintptr_t)obj;
    s->mAlloc[2].heapParam2 = s->mGroup[2].mHandle;

    if (scale > lbl_eu_8066A9F4) {
        s->mGroup[3].mHandle =
            mtl::MemManager::create(region->field_0x8, bufsz / 2, &lbl_eu_80523FE0[0x21]);
    } else {
        s->mGroup[3].mHandle = mtl::MemManager::getHandleMEM2();
    }
    s->mGroup[3].mFuncs.allocFunc = (MEMAllocatorAllocFunc)func_8048FE34;
    s->mGroup[3].mFuncs.freeFunc = (MEMAllocatorFreeFunc)func_8048FEC4;
    s->mAlloc[3].funcs = &s->mGroup[3].mFuncs;
    s->mAlloc[3].heap = NULL;
    s->mAlloc[3].heapParam1 = (u32)(uintptr_t)obj;
    s->mAlloc[3].heapParam2 = s->mGroup[3].mHandle;

    // MEM1 / MEM2 allocators (no per-slot handle).
    s->mFuncs[0].allocFunc = (MEMAllocatorAllocFunc)func_8048FE34;
    s->mFuncs[0].freeFunc = (MEMAllocatorFreeFunc)func_8048FEC4;
    s->mAlloc[4].funcs = &s->mFuncs[0];
    s->mAlloc[4].heap = NULL;
    s->mAlloc[4].heapParam1 = (u32)(uintptr_t)obj;
    s->mAlloc[4].heapParam2 = mtl::MemManager::getHandleMEM1();

    s->mFuncs[1].allocFunc = (MEMAllocatorAllocFunc)func_8048FE34;
    s->mFuncs[1].freeFunc = (MEMAllocatorFreeFunc)func_8048FEC4;
    s->mAlloc[5].funcs = &s->mFuncs[1];
    s->mAlloc[5].heap = NULL;
    s->mAlloc[5].heapParam1 = (u32)(uintptr_t)obj;
    s->mAlloc[5].heapParam2 = mtl::MemManager::getHandleMEM2();

    // Build the nw4r scene root from the second allocator slot.
    s->mScnRoot = nw4r::g3d::ScnRoot::Construct(&s->mAlloc[1], NULL, 10,
                                                vpHeight, 0x20, 4);

    // Bloom object (handled by the scene-root allocator).
    void* mem = mtl::MemManager::allocate(0x94, s->vtable->mVf8(obj));
    if (mem != NULL) {
        mem = func_80499FD0((CScnBloom*)mem, (CView*)mgr);
    }
    s->mBloom = mem;

    // Ten scn groups, each sized from the viewport rect and attached to the
    // scene root; the object itself registers as the group callback target.
    for (u32 i = 0; i < 10; i++) {
        MEMAllocator* alloc = (s->mGroup[2].mHandle == 0xFFFFFFFF)
                                  ? &s->mAlloc[0]
                                  : &s->mAlloc[2];
        nw4r::g3d::ScnGroup* group =
            nw4r::g3d::ScnGroup::Construct(alloc, NULL, (u32)rect[i]);
        s->mScnRoot->PushBack(group);
        ((CScnGroupTailLayout*)group)->field_0xD4 =
            obj ? (void*)&obj->field_0xC : NULL;
        group->EnableScnObjCallbackTiming(nw4r::g3d::ScnObj::CALLBACK_TIMING_A);
        group->EnableScnObjCallbackExecOp(nw4r::g3d::ScnObj::EXECOP_CALC_WORLD);
    }

    return obj;
}

// --- CScnRootNw4r helpers ---

// Returns the scene-group alloc handle, resolving it lazily through vf9 when
// the stored handle is still invalid (-1).
u32 func_8048F2F0(CScnRootNw4r* self) {
    CScnRootNw4rLayout* s = (CScnRootNw4rLayout*)self;
    u32 result = s->mGroup[2].mHandle;
    if (result == 0xFFFFFFFF) {
        result = self->vf9();
    }
    return result;
}

CScnRoot::~CScnRoot() {}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// Foreign .sdata RTTI locator referenced by the CScnRoot vtable (owned by
// the sdata pool unit).
extern u32 lbl_eu_80663998;

// [.data] 0x8056E730-0x8056E768 (0x38 = 56B): CScnRoot vtable.
// word0 = RTTI locator, +8 = dtor, +0x28/+0x2C/+0x30 = the three -1
// virtual stubs, rest zero.
extern "C" u32 lbl_eu_8056E730[14] = {
    (u32)&lbl_eu_80663998, 0x00000000,
    (u32)&__dt__8CScnRootFv, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    (u32)&func_8048EDE0, (u32)&func_8048EDD8, (u32)&func_8048EDD0, 0x00000000,
};
