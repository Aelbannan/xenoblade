#include "types.h"
#include "kyoshin/cf/CfCollSphereImpl.hpp"

// VTable function pointer at offset 0xAC (slot 43) on CfCollSphereImpl.
// Returns a CfDebugDrawManager* (or draw-context handle).
typedef void* (*CfCollSphereVFunc_0xAC)(cf::CfCollSphereImpl*);

// renderSphere: member of cf::CfDebugDrawManager.
// Retail passes (manager, float_radius) despite the Fv mangling.
extern "C" void renderSphere__Q22cf18CfDebugDrawManagerFv(void* self, float val);

// func_800A5738: defined in kyoshin/code_800A3B24.
extern "C" void func_800A5738(void* a, void* b, float val, void* c);

// func_800AAD28: debug draw for sphere collision shape (single call).
extern "C" void func_800AAD28(void* context, cf::CfCollSphereImpl* shape) {
    u32 uval = static_cast<u32>(shape->mRadius);
    void* data = ((CfCollSphereVFunc_0xAC)((*((void***)shape))[(0xAC / 4)]))(shape);
    renderSphere__Q22cf18CfDebugDrawManagerFv(data, static_cast<float>(uval));
}

// func_800AAD94: debug draw for sphere with extra transform params.
extern "C" void func_800AAD94(void* context, cf::CfCollSphereImpl* shape, void* a, void* b) {
    u32 uval = static_cast<u32>(shape->mRadius);
    void* data = ((CfCollSphereVFunc_0xAC)((*((void***)shape))[(0xAC / 4)]))(shape);
    func_800A5738(a, data, static_cast<float>(uval), b);
}
