#pragma once

#include <types.h>

// Forward declarations needed for the C-linkage import signatures
struct CETrail;
struct CETrailNode;

namespace ml {
struct CVec3;
struct CVec4;
}

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void TrailNode_Init(CETrailNode* node);
extern "C" void Trail_AppendPoint(CETrail* t, const ml::CVec3* posA, const ml::CVec3* posB,
                              const ml::CVec4* color, const ml::CVec4* scale);
