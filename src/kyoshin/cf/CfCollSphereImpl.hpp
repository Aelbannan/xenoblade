#pragma once

#include <types.h>

namespace cf {

// Forward declaration
class CfDebugDrawManager;
class CfCollSphereImpl;

// Vtable for CfCollSphereImpl. Only the entry at offset 0xAC is known.
struct CfCollSphereImplVtable {
    void* pad[0xAC / 4];
    void* (*func_0xAC)(CfCollSphereImpl* self);
};

// Collision sphere shape implementation.
// Layout offsets derived from CfCollSphereImpl.s.
struct CfCollSphereImpl {
    u8 pad_00[0xB8];    // 0x00: vtable + unknown fields
    float mRadius;      // 0xB8: sphere radius
};

} // namespace cf
