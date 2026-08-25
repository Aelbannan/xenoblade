#pragma once

#include <types.h>
#include "monolib/math/CVec3.hpp"
#include "monolib/math/CCol4.hpp"

// C-linkage imports: both carry unmangled verbatim retail symbol names, so
// they must keep C linkage.
// - func_800A5B18: cylinder push-out helper (retail symbol is unmangled).
extern "C" void func_800A5B18(void* a, void* b, void* c, float f1, float f2);
// - renderCylinder__...Fv: real renderer is the Fb overload; this call site
//   branches into the Fv-labelled entry point, whose parameter list cannot be
//   expressed as a C++ declaration.
extern "C" void renderCylinder__Q22cf18CfDebugDrawManagerFv(void* start, void* end, ml::CCol4* color, float radius);

// Debug color constants for the collision-cylinder render (loaded from .sdata2).
// Global-scope variables are not mangled by MWCC, so plain extern declarations
// emit the retail label names directly.
extern const float lbl_eu_80666910; // 0.0
extern const float lbl_eu_80666914; // 1.0
extern const float lbl_eu_80666918; // 0.3
extern const float lbl_eu_8066692C; // 0.1
extern const float lbl_eu_80666930; // 0.5
extern const float lbl_eu_80666934; // 0.15
extern const float lbl_eu_80666938; // 0.7
extern const float lbl_eu_8066693C; // 0.2
// Threshold below which the whole cylinder is submitted. const qualifies it as
// compile-time-invariant so MWCC hoists its load to function top, letting the
// threshold compare emit before the height-diff loads (retail shape).
extern const float lbl_eu_80666940; // 0.01
// 0.4 truncation factor - NOT const: retail loads this lazily in the else arm.
extern float lbl_eu_80666944;

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
