#pragma once

#include <types.h>
#include "monolib/math/CVec3.hpp"

namespace cf {

// Collision capsule shape implementation.
// Layout offsets derived from CfCollCapsuleImpl.s.
struct CfCollCapsuleImpl {
    u8 pad_00[0x94];            // 0x00: vtable + unknown fields
    u32 mKind;                  // 0x94: collision kind (1=player, 6=enemy)
    u8 pad_98[8];               // 0x98: padding
    ml::CVec3 mStart;           // 0xA0: capsule start point
    ml::CVec3 mEnd;             // 0xAC: capsule end point
    float mRadius;              // 0xB8: capsule radius
};

} // namespace cf
