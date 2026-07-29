// CfCollSphereImpl: debug-draw functions for sphere collision shapes.
// Matches CfCollSphereImpl.s (us).

#include "types.h"
#include "kyoshin/cf/CfCollSphereImpl.hpp"

// renderSphere: global function wrapping CfDebugDrawManager::renderSphere.
// Retail passes (draw_data, radius) despite the Fv mangling.
void renderSphere__Q22cf18CfDebugDrawManagerFv(u8* drawData, float radius);

// func_800A5738: defined in kyoshin/code_800A3B24.
void func_800A5738(u8* a, u8* drawData, float radius, u8* b);

// func_800AAD28: debug draw for sphere collision shape.
// Reads mRadius, converts float->unsigned->float (preserving bit pattern),
// calls vfunc at vtable offset 0xAC to get draw data, then calls renderSphere.
void func_800AAD28(void* /*unused*/, cf::CfCollSphereImpl* shape) {
    u32 uval = static_cast<u32>(shape->mRadius);
    u8* drawData = reinterpret_cast<u8* (*)(cf::CfCollSphereImpl*)>(
        reinterpret_cast<uptr_t*>(shape)[0xAC / 4])(shape);
    float radius = static_cast<float>(uval);
    renderSphere__Q22cf18CfDebugDrawManagerFv(drawData, radius);
}

// func_800AAD94: same sphere debug draw but passes extra parameters through
// to func_800A5738.
void func_800AAD94(void* /*unused*/, cf::CfCollSphereImpl* shape, u8* a, u8* b) {
    u32 uval = static_cast<u32>(shape->mRadius);
    u8* drawData = reinterpret_cast<u8* (*)(cf::CfCollSphereImpl*)>(
        reinterpret_cast<uptr_t*>(shape)[0xAC / 4])(shape);
    float radius = static_cast<float>(uval);
    func_800A5738(a, drawData, radius, b);
}
