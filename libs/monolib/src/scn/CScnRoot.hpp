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


// .sdata2 constants used by the CScnRootNw4r constructor.
extern const f32 lbl_eu_8066A9E8;  // 20.0f (default viewport scale)
extern const f32 lbl_eu_8066A9EC;  // 0.01f (scale multiplier)
extern const f32 lbl_eu_8066A9F0;  // 0.08f (region overhead ratio)
extern const f32 lbl_eu_8066A9F4;  // 0.0f  (allocator-selection threshold)
extern const f64 lbl_eu_8066A9F8;  // 0x4330000080000000 (s32 -> f64 magic)




// Alloc-region object behind CScn::unk88.
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



// nw4r::g3d::ScnGroup tail field written by the constructor (mpFuncObjExec).
struct CScnGroupTailLayout {
    u8 pad_0x0[0xD4];
    void* field_0xD4;               // +0xD4 (IScnObjCallback*)
};

// Custom allocator callbacks for the nw4r MEMAllocator instances (defined in
// CScnRootNw4r.cpp; plain global-scope names).
extern "C" void* RootNw4r_AllocBuf(MEMAllocator* alloc, u32 size);
extern "C" void RootNw4r_FreeBuf(MEMAllocator* alloc, void* block);

// Second-stage bloom constructor (defined in CScnBloom.cpp).
#include "libs/monolib/src/scn/CScnBloom.hpp"
CScnBloom* CScnBloom_InitWithView(CScnBloom* self, CView* view);
