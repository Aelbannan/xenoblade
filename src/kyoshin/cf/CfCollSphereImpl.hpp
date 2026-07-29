#pragma once

#include <types.h>

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
