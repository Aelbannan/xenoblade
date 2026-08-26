#pragma once

#include <types.h>
#include "monolib/util/MemManager.hpp"
#include "monolib/util/reslist.hpp"

// Element type of the reslist managed by CTimeLightGrp.
// Retail instantiated the list over `unsigned long` (mangled <Ul>), which
// drives the mangled name of the implicitly referenced base-class vtable.
typedef unsigned long CVirtualLightObjPtr;

// Minimal layout of the per-node light object written by func_8005A374.
// The retail class (CVirtualLightObj, monolib/scn) is not on this TU's include
// path, so only the fields this TU touches are modelled. Offsets match the
// retail class: +0x19 byte flag, +0x1C..+0x28 four f32 scale components.
struct CVirtualLightObjFields {
    u8 pad00[0x19];   // +0x00..+0x18
    u8 mByte19;       // +0x19 - flag; 0 means "write the light transform"
    u8 pad1A[2];      // +0x1A..+0x1B
    f32 mField1C;     // +0x1C (sx)
    f32 mField20;     // +0x20 (sy)
    f32 mField24;     // +0x24 (sz)
    f32 mField28;     // +0x28 (sw)
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

extern "C" u8 lbl_eu_80526418[];
extern "C" u8 lbl_eu_80526430[];
extern "C" u8 lbl_eu_80526448[];
extern "C" const float lbl_eu_80666038;

extern "C" void __dl__FPv(void* ptr);
extern "C" void __dla__FPv(void* ptr);
extern "C" mtl::ALLOC_HANDLE func_80496004(void* ptr);
extern "C" void* allocate_array__Q23mtl10MemManagerFUlUl(u32 size, u32 handle);

// Time-light group: owns a reslist of virtual light object pointers.
// Layout: own vptr @0x00, parent @0x04, embedded reslist subobject @0x08
// (its vptr @0x08, sentinel ptr @0x0C, node array ptr @0x1C).
class CTimeLightGrp {
public:
    ~CTimeLightGrp();

    void* mVtable;                       // +0x00
    void* mParent;                       // +0x04
    reslist<CVirtualLightObjPtr> mList;  // +0x08
};
