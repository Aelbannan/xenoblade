#pragma once

#include <types.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

class CScn;

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_8049034C(void* a, void* b, int c);
extern "C" void func_800A50AC(void* a, void* b, float c, void* d);
// Global CScn instance (defined CScn* in kyoshin/cf/CfGameManager.cpp)

namespace cf {

// Collision circle shape implementation.
// Layout offsets derived from CfCollCircleImpl.s. The debug-draw functions
// in this TU take the shape as cf::CfObjectColl (radius at 0xB8, position
// via CfObject_UnkVirtualFunc23 at +0xAC, cf. CfCollSphereImpl), so this
// struct only documents the layout; the pad vtable view is deleted.
struct CfCollCircleImpl {
    u8 pad_00[0xB8];    // 0x00: vtable + unknown fields
    float field_B8;     // 0xB8: circle radius/scale field
};

} // namespace cf
