#pragma once

#include <types.h>
#include "monolib/math/CVec3.hpp"

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_800A5B18(void* a, void* b, void* c, float f1, float f2);
extern "C" float lbl_eu_80666940; // 0.01 threshold
// 0.4 truncation factor
extern "C" float lbl_eu_80666944;

namespace cf {

// Collision cylinder shape implementation.
// Layout offsets derived from CfCollCylinderImpl.s.
struct CfCollCylinderImpl {
    u8 pad_00[0x94];            // 0x00: vtable + unknown fields
    int mKind;                  // 0x94: collision kind (0,1,2,5,6); compared as signed
    u8 pad_98[4];               // 0x98: padding
    u32 mSubKind;               // 0x9C: sub-kind discriminator (used when mKind=5)
    ml::CVec3 mStart;           // 0xA0: cylinder start point
    ml::CVec3 mEnd;             // 0xAC: cylinder end point
    float mRadius;              // 0xB8: cylinder radius
};

} // namespace cf
