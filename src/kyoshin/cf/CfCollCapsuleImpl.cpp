#include "types.h"
#include "kyoshin/cf/CfCollCapsuleImpl.hpp"

// Shared float constants used for capsule debug colors.
extern const float lbl_eu_80666910; // 0.0f
extern const float lbl_eu_80666914; // 1.0f
extern const float lbl_eu_8066692C; // 0.1f
extern const float lbl_eu_80666930; // 0.5f
extern const float lbl_eu_80666934; // 0.15f
extern const float lbl_eu_80666948; // 0.75f

// Collision query helper: process capsule against collision context.
// Retail names this symbol UNMANGLED (plain C linkage).
extern "C" void func_800A4C48(void* query, const ml::CVec3* start,
                               const ml::CVec3* end, float radius);

// Render the capsule for debug visualization.
// r3 is unused (this of CfDebugDrawManager).
void func_800AB2E4(void* /*unused*/, cf::CfCollCapsuleImpl* capsule){
    cf::CfDebugCol col;
    col.f[0] = lbl_eu_80666910;
    col.f[1] = lbl_eu_80666930;
    col.f[2] = lbl_eu_80666914;
    col.f[3] = lbl_eu_80666934;

    if ((int)capsule->mKind == 1) {
        // Build the branch color as floats, then copy word-by-word (lwz/stw).
        // Mirrors retail's stfs-build-then-block-copy codegen.
        cf::CfDebugCol tmp;
        tmp.f[0] = lbl_eu_80666910;
        tmp.f[1] = lbl_eu_80666914;
        tmp.f[2] = lbl_eu_80666914;
        tmp.f[3] = lbl_eu_8066692C;
        col.u[0] = tmp.u[0];
        col.u[1] = tmp.u[1];
        col.u[2] = tmp.u[2];
        col.u[3] = tmp.u[3];
    } else if ((int)capsule->mKind == 6) {
        cf::CfDebugCol tmp;
        tmp.f[0] = lbl_eu_80666910;
        tmp.f[1] = lbl_eu_80666948;
        tmp.f[2] = lbl_eu_80666948;
        tmp.f[3] = lbl_eu_8066692C;
        col.u[0] = tmp.u[0];
        col.u[1] = tmp.u[1];
        col.u[2] = tmp.u[2];
        col.u[3] = tmp.u[3];
    }

    renderCapsule__Q22cf18CfDebugDrawManagerFv(
        &capsule->mStart, &capsule->mEnd, col.f, capsule->mRadius);
}

// Forward capsule collision data to the collision query system.
// r3 is unused.
extern "C" void func_800AB3B8(void* /*unused*/, cf::CfCollCapsuleImpl* capsule,
                               void* query, float offsetRadius) {
    func_800A4C48(query, &capsule->mStart, &capsule->mEnd,
                  capsule->mRadius + offsetRadius);
}
