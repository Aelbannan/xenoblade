#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectColl.hpp"

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// renderSphere: member of cf::CfDebugDrawManager; retail passes (manager,
// float_radius) despite the Fv mangling. func_800A5738: defined in
// kyoshin/code_800A3B24.
extern "C" void renderSphere__Q22cf18CfDebugDrawManagerFv(void* self, float val);
extern "C" void func_800A5738(void* a, void* b, float val, void* c);

namespace cf {

// Real class tree recovered from retail vtables:
//
// - Retail vtable for cf::CfCollSphereImpl is lbl_eu_80528800 in
//   build/us/asm/split1.s (US) : { &RTTI_CfCollSphereImpl (80661C98), 0,
//   func_800AAD28, func_800AAD94 }  -- 0x10 bytes, two virtuals at +0x08/+0x0C
//   (CfCollImpl base).  The TU has no .data of its own (see
//   build/us/asm/kyoshin/cf/CfCollSphereImpl.s), so the class is
//   __declspec(novtable) and the retail label is used at runtime.
// - The virtual at +0xAC used by the two debug helpers is NOT a member of
//   CfCollSphereImpl; it is cf::CfObject::CfObject_UnkVirtualFunc23
//   (ml::CVec3* at +0xAC, see lbl_eu_80528600 / CfObjectColl.hpp, 0x178 bytes,
//   RTTI lbl_eu_80661C78 "cf::CfObjectColl").  The "shape" object passed to
//   func_800AAD28/94 is a CfObject-family game object whose coll fields live
//   at the same offsets as CfCollSphereImpl's (radius at 0xB8).  That is why
//   CfCollSphereImpl inherits CfObject here: it gives the correct vtable slot
//   offset (172) and the same field layout for hexdiff, without emitting a
//   compiler __vt__.
//
struct __declspec(novtable) CfCollSphereImpl : public CfObject {
    // CfObject is 0x70 bytes (vptr + pad04[0x68-0x04] + field_0x68/0x6C).
    // Pad to keep mRadius at the retail offset 0xB8.
    u8 _pad70[0xB8 - 0x70];
    float mRadius; // 0xB8: sphere radius (aliases CfObjectColl::field_0xB8)
};

} // namespace cf
