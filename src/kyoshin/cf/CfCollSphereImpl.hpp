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

// Vtable layout for CfCollSphereImpl.
// The vtable pointer is stored at offset 0 of the struct.
struct CfCollSphereImplVTable {
    u32 pad[0xAC / 4];  // padding up to offset 0xAC
    // Returns the debug draw manager for this shape.
    CfDebugDrawManager* (*getDebugDrawManager)(CfCollSphereImpl* self);
};

} // namespace cf
