#include "types.h"
#include "kyoshin/cf/CfCollSphereImpl.hpp"

// cf::CfDebugDrawManager::renderSphere(float radius)
// Renders a debug sphere with the given radius.
void renderSphere__Q22cf18CfDebugDrawManagerFv(cf::CfDebugDrawManager* mgr, float radius);

// func_800A5738: collision query helper defined in kyoshin/code_800A3B24.
void func_800A5738(void* query, cf::CfDebugDrawManager* mgr, float radius, void* result);

// Debug-draw the sphere collision shape.
// r3 is unused; r4 is the sphere shape.
void func_800AAD28(void* /*unused*/, cf::CfCollSphereImpl* shape) {
    // Convert radius to unsigned int and back to float (MWCC __cvt_fp2unsigned pattern).
    float radius = (float)(unsigned int)shape->mRadius;
    cf::CfDebugDrawManager* mgr = shape->getDebugDrawManager();
    renderSphere__Q22cf18CfDebugDrawManagerFv(mgr, radius);
}

// Pass sphere collision data to the collision query system.
// r3 is unused; r4 is the sphere shape; r5/r6 pass through to func_800A5738.
void func_800AAD94(void* /*unused*/, cf::CfCollSphereImpl* shape, void* query, void* result) {
    // Convert radius to unsigned int and back to float (MWCC __cvt_fp2unsigned pattern).
    float radius = (float)(unsigned int)shape->mRadius;
    cf::CfDebugDrawManager* mgr = shape->getDebugDrawManager();
    func_800A5738(query, mgr, radius, result);
}
