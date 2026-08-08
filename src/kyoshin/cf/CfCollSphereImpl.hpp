#pragma once

#include <types.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// renderSphere: member of cf::CfDebugDrawManager; retail passes (manager,
// float_radius) despite the Fv mangling. func_800A5738: defined in
// kyoshin/code_800A3B24.
extern "C" void renderSphere__Q22cf18CfDebugDrawManagerFv(void* self, float val);
extern "C" void func_800A5738(void* a, void* b, float val, void* c);

namespace cf {

// Forward declaration
class CfDebugDrawManager;

// Collision sphere shape implementation.
// Layout offsets derived from CfCollSphereImpl.s.
struct CfCollSphereImpl {
    u8 pad_00[0xB8];    // 0x00: vtable + unknown fields
    float mRadius;      // 0xB8: sphere radius
};

} // namespace cf
