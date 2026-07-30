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
// r3 = context pointer (passed through to renderSphere as first arg),
// r4 = shape.
//
// Reads mRadius, converts float->unsigned->float (MWCC idiom for
// sanitising the fractional part), calls vfunc at vtable offset 0xAC
// to get draw data, then calls renderSphere.
extern "C" void func_800AAD28(void* context, cf::CfCollSphereImpl* shape) {
    u32 uval = static_cast<u32>(shape->mRadius);
    void** vtbl = *reinterpret_cast<void***>(shape);
    CfCollSphereVFunc_0xAC getData = (CfCollSphereVFunc_0xAC)(vtbl[0xAC / 4]);
    void* data = getData(shape);
    renderSphere__Q22cf18CfDebugDrawManagerFv(data, static_cast<float>(uval));
}

// func_800AAD94: debug draw for sphere with extra transform params.
// r3 = context, r4 = shape, r5 = a, r6 = b.
// Same radius/vtable pattern, then calls func_800A5738 with
// additional parameters.
extern "C" void func_800AAD94(void* context, cf::CfCollSphereImpl* shape, void* a, void* b) {
    u32 uval = static_cast<u32>(shape->mRadius);
    void** vtbl = *reinterpret_cast<void***>(shape);
    CfCollSphereVFunc_0xAC getData = (CfCollSphereVFunc_0xAC)(vtbl[0xAC / 4]);
    void* data = getData(shape);
    func_800A5738(a, data, static_cast<float>(uval), b);
}
