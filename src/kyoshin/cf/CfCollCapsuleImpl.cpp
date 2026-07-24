#include "types.h"
#include "kyoshin/cf/CfCollCapsuleImpl.hpp"

// cf::CfDebugDrawManager::renderCapsule(Vec3* start, Vec3* end, float* color, float radius)
extern "C" void renderCapsule__Q22cf18CfDebugDrawManagerFv(
    const ml::CVec3* start, const ml::CVec3* end, const float* color, float radius);

// Collision query helper: process capsule against collision context.
extern "C" void func_800A4C48(void* query, const ml::CVec3* start,
                               const ml::CVec3* end, float radius);

// Render the capsule for debug visualization.
// r3 is unused (this of CfDebugDrawManager).
extern "C" void func_800AB2E4(void* /*unused*/, cf::CfCollCapsuleImpl* capsule) {
    float col[4] = {0.0f, 0.5f, 1.0f, 0.15f};

    if (capsule->mKind == 1) {
        col[0] = 0.0f;
        col[1] = 1.0f;
        col[2] = 1.0f;
        col[3] = 0.1f;
    } else if (capsule->mKind == 6) {
        col[0] = 0.0f;
        col[1] = 0.75f;
        col[2] = 0.75f;
        col[3] = 0.1f;
    }

    renderCapsule__Q22cf18CfDebugDrawManagerFv(
        &capsule->mStart, &capsule->mEnd, col, capsule->mRadius);
}

// Forward capsule collision data to the collision query system.
// r3 is unused.
extern "C" void func_800AB3B8(void* /*unused*/, cf::CfCollCapsuleImpl* capsule,
                               void* query, float offsetRadius) {
    func_800A4C48(query, &capsule->mStart, &capsule->mEnd,
                  capsule->mRadius + offsetRadius);
}
