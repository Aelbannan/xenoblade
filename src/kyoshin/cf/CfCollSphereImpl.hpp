#pragma once

#include <types.h>
#include "monolib/math/CVec3.hpp"

namespace cf { class CfObject; }

// C-linkage imports (retail symbol names - keep linkage verbatim).
// renderSphere: retail CfDebugDrawManager entry under the Fv linker name; the
// call passes the sphere centre in r3 and the radius in f1 (same convention
// as renderCapsule in CfCollCapsuleImpl.hpp).
// VecMath_PushOutSphere: defined in kyoshin/code_800A3B24.
extern "C" void renderSphere__Q22cf18CfDebugDrawManagerFv(const ml::CVec3* center, float radius);
extern "C" int VecMath_PushOutSphere(cf::CfObject* query, ml::CVec3* center, float radius, ml::CVec3* out);

// cf::CfCollSphereImpl has no data of its own: its retail vtable
// (lbl_eu_80528800: RTTI "cf::CfCollSphereImpl", 0, CfCollSphere_DebugDrawSimple,
// CfCollSphere_DebugDrawXform) is emitted as raw .data in CfCollSphereImpl.cpp,
// and both slots read the radius (+0xB8) and position of the owning
// cf::CfObjectColl passed as the second argument.
