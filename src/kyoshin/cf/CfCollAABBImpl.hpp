#pragma once

#include <types.h>
#include "monolib/math/CVec3.hpp"

namespace ml {
    struct CCol4;
    struct CMat34;
}

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void setCol__7CDrawGXFRCQ22ml5CCol4(void* self, const ml::CCol4* col);
extern "C" void setMatrix__7CDrawGXFRCQ22ml6CMat34(void* self, const ml::CMat34* mat);
extern "C" void renderCube__7CDrawGXFRCQ22ml5CVec3RCQ22ml5CVec3(void* self, const ml::CVec3* min, const ml::CVec3* max);
extern "C" void func_8049034C(void* a, void* b, int c);
extern "C" void func_800A5FE8(void* a, void* b, void* c, void* d, void* e);

// Debug color / scale constants from .sdata2.
extern const float lbl_eu_80666910; // 0.0
extern const float lbl_eu_80666914; // 1.0
extern const float lbl_eu_80666918; // 0.3
extern const float lbl_eu_80666928; // sin/cos index scale
extern const float lbl_eu_8066692C; // 0.1
extern const float lbl_eu_80666930; // 0.5
extern const float lbl_eu_80666934; // 0.15

namespace cf {

// Collision AABB shape implementation.
// Retail __vt__ is lbl_eu_805287E0 (US) / __vt__Q22cf14CfCollAABBImpl (JP)
//   0x10 bytes: RTTI lbl_eu_80661C90 "cf::CfCollAABBImpl", 0, func_800AAE24, func_800AAFF4
//   Two virtuals at +0x08/+0x0C (CfCollImpl base). TU has no .data
//   (build/us/asm/kyoshin/cf/CfCollAABBImpl.s), so no compiler __vt__ is emitted.
//   The world-position getter at +0xAC is cf::CfObject::GetPosition
//   (ml::CVec3* at +0xAC, lbl_eu_80528600), called on the CfObjectColl that
//   carries the AABB data at 0x94/0xD8/0xE4/0xF0/0x14C.
// Layout offsets derived from CfCollAABBImpl.s (view onto CfObjectColl union).
struct CfCollAABBImpl {
    void* vtable;           // 0x00 (retail singleton vptr at lbl_eu_805287E0 when used as CfCollImpl)
    u8 pad_04[0x94 - 0x04];
    int field_94;           // 0x94: color-mode selection flag (compared signed)
    u8 pad_98[0xD8 - 0x98];
    ml::CVec3 mMin;         // 0xD8: AABB minimum corner
    ml::CVec3 mMax;         // 0xE4: AABB maximum corner
    ml::CVec3 mCenter;      // 0xF0: AABB center
    u8 pad_FC[0x14C - 0xFC];
    float mAngle;           // 0x14C: rotation angle (used by debug render)
};

} // namespace cf
