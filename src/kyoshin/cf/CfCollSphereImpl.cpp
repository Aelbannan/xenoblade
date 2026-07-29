#include "types.h"
#include "kyoshin/cf/CfCollSphereImpl.hpp"

// cf::CfDebugDrawManager::renderSphere(float radius)
void renderSphere__Q22cf18CfDebugDrawManagerFv(cf::CfDebugDrawManager* mgr, float radius);

// func_800A5738: collision query helper defined in kyoshin/code_800A3B24.
void func_800A5738(void* query, cf::CfDebugDrawManager* mgr, float radius, void* result);

// Debug-draw the sphere collision shape.
// r3 is unused; r4 is the sphere shape.
void func_800AAD28(void* /*unused*/, cf::CfCollSphereImpl* shape) {
    // Convert radius to unsigned int (MWCC __cvt_fp2unsigned pattern).
    unsigned int uval = (unsigned int)shape->mRadius;
    // Call vtable entry at offset 0xAC to get the debug draw manager.
    cf::CfDebugDrawManager* mgr = (*(cf::CfCollSphereImplVTable**)shape)
        ->getDebugDrawManager(shape);
    // Reinterpret the uint32 bits as a float.
    renderSphere__Q22cf18CfDebugDrawManagerFv(mgr, (float)uval);
}

// Pass sphere collision data to the collision query system.
// r3 is unused; r4 is the sphere shape; r5/r6 pass through to func_800A5738.
void func_800AAD94(void* /*unused*/, cf::CfCollSphereImpl* shape, void* query, void* result) {
    // Convert radius to unsigned int (MWCC __cvt_fp2unsigned pattern).
    unsigned int uval = (unsigned int)shape->mRadius;
    // Call vtable entry at offset 0xAC to get the debug draw manager.
    cf::CfDebugDrawManager* mgr = (*(cf::CfCollSphereImplVTable**)shape)
        ->getDebugDrawManager(shape);
    // Reinterpret the uint32 bits as a float.
    func_800A5738(query, mgr, (float)uval, result);
}
