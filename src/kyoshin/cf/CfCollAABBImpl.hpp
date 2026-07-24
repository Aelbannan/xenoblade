#pragma once

#include <types.h>
#include "monolib/math/CVec3.hpp"

namespace cf {

// Collision AABB shape implementation.
// Layout offsets derived from CfCollAABBImpl.s.
struct CfCollAABBImpl {
    u8 pad_00[0xD8];        // 0x00: vtable + unknown fields
    ml::CVec3 mMin;         // 0xD8: AABB minimum corner
    ml::CVec3 mMax;         // 0xE4: AABB maximum corner
    ml::CVec3 mCenter;      // 0xF0: AABB center
    u8 pad_FC[0x14C - 0xFC]; // padding / other collision fields
    float mAngle;           // 0x14C: rotation angle (used by debug render)
};

} // namespace cf
