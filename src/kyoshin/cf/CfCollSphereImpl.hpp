#pragma once

#include <types.h>

namespace ml { struct CVec3; }

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// renderSphere: member of cf::CfDebugDrawManager; retail passes (manager,
// float_radius) despite the Fv mangling. func_800A5738: defined in
// kyoshin/code_800A3B24.
extern "C" void renderSphere__Q22cf18CfDebugDrawManagerFv(void* self, float val);
extern "C" void func_800A5738(void* a, void* b, float val, void* c);

namespace cf {

// Collision sphere shape implementation.
// Layout offsets derived from CfCollSphereImpl.s.
// Retail __vt__ for CfCollSphereImpl is lbl_eu_80528800 (0x10 bytes:
// RTTI 80661C98 "cf::CfCollSphereImpl", 0, func_800AAD28, func_800AAD94)
// — two virtuals at +0x08/+0x0C (CfCollImpl base).  TU has no .data
// (build/us/asm/kyoshin/cf/CfCollSphereImpl.s), so no compiler __vt__ is
// emitted.  The position getter at +0xAC is cf::CfObject::GetPosition
// (ml::CVec3* at +0xAC, lbl_eu_80528600), called on the CfObject-family
// coll object that carries the radius at 0xB8.
struct CfCollSphereImpl {
    u8 pad_00[0xB8];    // 0x00: vtable + unknown fields
    float mRadius;      // 0xB8: sphere radius
};

} // namespace cf
