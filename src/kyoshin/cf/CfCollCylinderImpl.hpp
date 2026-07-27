#pragma once

#include <types.h>
#include "monolib/math/CVec3.hpp"

namespace cf {

// Collision cylinder shape implementation.
// Layout offsets derived from CfCollCylinderImpl.s.
struct CfCollCylinderImpl {
    u8 pad_00[0x94];            // 0x00: vtable + unknown fields
    u32 mKind;                  // 0x94: collision kind (0,1,2,5,6)
    u8 pad_98[4];               // 0x98: padding
    u16 mSubKind;               // 0x9C: sub-kind discriminator (used when mKind=5)
    u8 pad_9E[2];              // 0x9E: padding to 0xA0
    ml::CVec3 mStart;           // 0xA0: cylinder start point
    ml::CVec3 mEnd;             // 0xAC: cylinder end point
    float mRadius;              // 0xB8: cylinder radius
};

} // namespace cf
