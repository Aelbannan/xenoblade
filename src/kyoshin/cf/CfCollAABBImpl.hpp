#pragma once

#include <types.h>
#include "monolib/math/CVec3.hpp"

namespace cf {

// Collision AABB shape implementation.
// Layout offsets derived from CfCollAABBImpl.s.
struct CfCollAABBImpl {
    void* vtable;           // 0x00
    u8 pad_04[0x94 - 0x04];
    u32 field_94;           // 0x94: color-mode selection flag
    u8 pad_98[0xD8 - 0x98];
    ml::CVec3 mMin;         // 0xD8: AABB minimum corner
    ml::CVec3 mMax;         // 0xE4: AABB maximum corner
    ml::CVec3 mCenter;      // 0xF0: AABB center
    u8 pad_FC[0x14C - 0xFC]; // padding / other collision fields
    float mAngle;           // 0x14C: rotation angle (used by debug render)

    // Fetch world position through the vtable slot at 0xAC.
    ml::CVec3* GetPos() {
        void** vtbl = reinterpret_cast<void**>(vtable);
        return reinterpret_cast<ml::CVec3* (*)(void*)>(vtbl[0xAC / 4])(this);
    }
};

typedef ml::CVec3* (*CfCollAABBImplGetPosFn)(void*);

} // namespace cf
