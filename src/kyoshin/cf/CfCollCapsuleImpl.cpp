#include "types.h"
#include "kyoshin/cf/CfCollCapsuleImpl.hpp"

struct CfCollCapsuleImpl_Col {
    struct { float x, y, z, w; };
};

// cf::CfDebugDrawManager::renderCapsule(Vec3* start, Vec3* end, float* color, float radius)
void renderCapsule__Q22cf18CfDebugDrawManagerFv(
    const ml::CVec3* start, const ml::CVec3* end, const float* color, float radius);

// Collision query helper: process capsule against collision context.
void func_800A4C48(void* query, const ml::CVec3* start,
                               const ml::CVec3* end, float radius);

// Render the capsule for debug visualization.
// r3 is unused (this of CfDebugDrawManager).
void func_800AB2E4(void* /*unused*/, cf::CfCollCapsuleImpl* capsule){
    CfCollCapsuleImpl_Col col;
    col.x = 0.0f;
    col.y = 0.5f;
    col.z = 1.0f;
    col.w = 0.15f;

    if (capsule->mKind == 1) {
        col.x = 0.0f;
        col.y = 1.0f;
        col.z = 1.0f;
        col.w = 0.1f;
    } else if (capsule->mKind == 6) {
        col.x = 0.0f;
        col.y = 0.75f;
        col.z = 0.75f;
        col.w = 0.1f;
    }

    renderCapsule__Q22cf18CfDebugDrawManagerFv(
        &capsule->mStart, &capsule->mEnd, (const float*)&col, capsule->mRadius);
}

// Forward capsule collision data to the collision query system.
// r3 is unused.
extern "C" void func_800AB3B8(void* /*unused*/, cf::CfCollCapsuleImpl* capsule,
                               void* query, float offsetRadius) {
    func_800A4C48(query, &capsule->mStart, &capsule->mEnd,
                  capsule->mRadius + offsetRadius);
}
