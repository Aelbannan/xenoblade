#pragma once

#include <types.h>
#include <revolution/MEM.h>

// Forward declarations (full definitions live in CScnRootNw4r.hpp and the
// nw4r g3d headers).
class CScnRootNw4r;
namespace nw4r {
namespace g3d {
class ScnRoot;
}
} // namespace nw4r

// ---------------------------------------------------------------------------
// Retail data symbols referenced by this TU.
// Global-scope names are not C++-mangled, so plain extern decls emit the
// exact retail lbl_eu_* symbols (no extern "C" needed).
// ---------------------------------------------------------------------------

// CScnRoot / CScnRootNw4r vtables (.data). Sized arrays force lis/addi
// (absolute) addressing - a small extern type would pick @sda21 instead.
// lbl_eu_8056E730 is DEFINED in CScnRoot.cpp (dissolved monolibdata2 data);
// lbl_eu_8056E768 stays extern (owned by CScnRootNw4r.cpp).
extern u32 lbl_eu_8056E730[0x38 / 4];
extern u8 lbl_eu_8056E768[0x5C];

// .sdata2 constants used by the CScnRootNw4r constructor.
extern const f32 lbl_eu_8066A9E8;  // 20.0f (default viewport scale)
extern const f32 lbl_eu_8066A9EC;  // 0.01f (scale multiplier)
extern const f32 lbl_eu_8066A9F0;  // 0.08f (region overhead ratio)
extern const f32 lbl_eu_8066A9F4;  // 0.0f  (allocator-selection threshold)
extern const f64 lbl_eu_8066A9F8;  // 0x4330000080000000 (s32 -> f64 magic)

// .rodata: memory-region name strings ("NW4R Mem", "NW4R Mem(P)", ...).
extern const char lbl_eu_80523FE0[];

// ---------------------------------------------------------------------------
// CScnRoot layout (retail: vtable at +0x00, mpMgr at +0x04, mReserved at
// +0x08). The canonical class lives in monolib/scn/CScnRoot.hpp (include
// path); the constructor is a plain C-linkage symbol (__ct__CScnRoot), so
// this TU installs the vtable through the flat layout instead.
// ---------------------------------------------------------------------------
struct CScnRootLayout {
    void* vtable;     // +0x00
    void* mpMgr;      // +0x04
    u32 mReserved;    // +0x08
};

// Owning scene object (retail CScn) - only the alloc-region slot is read.
struct CScnRootMgr {
    u8 pad_0x0[0x88];
    void* field_0x88;  // +0x88 alloc-region object
};

// Alloc-region object behind CScnRootMgr::field_0x88.
struct CScnRootMgrRegion {
    u8 pad_0x0[0x8];
    u32 field_0x8;     // +0x8 alloc handle
    u32 field_0xC;     // +0xC region size (labs input)
};

// Optional constructor parameter blob (viewport overrides).
struct CScnRootNw4rParam {
    u8 pad_0x0[0x8];
    f32 field_0x8;      // +0x8 scale (default 20.0f)
    u16 field_0xC;      // +0xC scene-root maxScnObj
    u16 rect[10];       // +0xE scn-group capacities
};

// Retail CScnRootNw4r vtable shape (lbl_eu_8056E768); only the slots this TU
// dispatches through are typed.
struct CScnRootNw4rVtbl {
    void* mSlots0[0x28 / 4];        // +0x00..+0x24
    u32 (*mVf8)(CScnRootNw4r*);     // +0x28 (func_8048F2F0)
    u32 (*mVf9)(CScnRootNw4r*);     // +0x2C (func_804871A8)
};

// Full flat layout of the retail CScnRootNw4r object (0x4EC bytes). The
// CScnRootNw4r class in CScnRootNw4r.hpp only covers the +0x00..+0x4BC
// range; the allocator/handle tail is described here.
struct CScnRootNw4rLayout {
    CScnRootNw4rVtbl* vtable;       // +0x00 (lbl_eu_8056E768)
    void* mpMgr;                    // +0x04
    void* mpBuffer;                 // +0x08
    void* mpCallback;               // +0x0C (IScnObjCallback interface)
    nw4r::g3d::ScnRoot* mScnRoot;   // +0x10
    void* mBloom;                   // +0x14
    u8 field_0x18;                  // +0x18
    u8 pad_0x19;                    // +0x19
    u8 field_0x1A;                  // +0x1A
    u8 pad_0x1B;                    // +0x1B
    u32 field_0x1C;                 // +0x1C
    u32 mArrA[4];                   // +0x20
    s32 mArrB[4];                   // +0x30
    u32 mPairCount;                 // +0x40
    u8 pad_0x44[0x400];             // +0x44
    u32 mIdCount;                   // +0x444
    u8 field_0x448;                 // +0x448
    u8 pad_0x449[3];                // +0x449
    MEMAllocator mAlloc[6];         // +0x44C (6 * 0x10)
    struct CScnGroupAlloc {
        u32 mHandle;                // +0x0
        MEMAllocatorFuncs mFuncs;   // +0x4
    } mGroup[4];                    // +0x4AC (4 * 0xC)
    MEMAllocatorFuncs mFuncs[2];    // +0x4DC (2 * 0x8)
};

// nw4r::g3d::ScnGroup tail field written by the constructor (mpFuncObjExec).
struct CScnGroupTailLayout {
    u8 pad_0x0[0xD4];
    void* field_0xD4;               // +0xD4 (IScnObjCallback*)
};

// Custom allocator callbacks for the nw4r MEMAllocator instances (defined in
// CScnRootNw4r.cpp; plain global-scope names).
void* func_8048FE34(CScnRootNw4r* allocator, u32 size);
void func_8048FEC4(CScnRootNw4r* allocator, void* block);

// Second-stage bloom constructor (defined in CScnBloom.cpp).
#include "libs/monolib/src/scn/CScnBloom.hpp"
CScnBloom* func_80499FD0(CScnBloom* self, CView* view);
