#include "types.h"
#include "kyoshin/cf/CfCollSphereImpl.hpp"

// renderSphere: member of cf::CfDebugDrawManager.
// Retail passes (manager, float_radius) despite the Fv mangling.
void renderSphere__Q22cf18CfDebugDrawManagerFv(void* self, float val);

// func_800A5738: defined in kyoshin/code_800A3B24.
void func_800A5738(void* a, void* b, float val, void* c);

// func_800AAD28: debug draw for sphere collision shape.
// Reads mRadius, converts float->unsigned->float (preserving bit pattern),
// calls vfunc at vtable offset 0xAC to get draw data, then calls renderSphere.
// The vtable load colors as r4 (MWCC) vs retail r12 -- a known Chaitin
// allocation difference that does not affect EQUIVALENT_MATCH.
void func_800AAD28(void* /*unused*/, cf::CfCollSphereImpl* shape) {
    u32 uval = static_cast<u32>(shape->mRadius);
    void** vtbl = *reinterpret_cast<void***>(shape);
    void* data = reinterpret_cast<void*(*)(void*)>(vtbl[0xAC / 4])(shape);
    renderSphere__Q22cf18CfDebugDrawManagerFv(data, static_cast<float>(uval));
}

// func_800AAD94: same sphere debug draw but passes extra parameters through
// to func_800A5738.
void func_800AAD94(void* /*unused*/, cf::CfCollSphereImpl* shape, void* a, void* b) {
    u32 uval = static_cast<u32>(shape->mRadius);
    void** vtbl = *reinterpret_cast<void***>(shape);
    void* data = reinterpret_cast<void*(*)(void*)>(vtbl[0xAC / 4])(shape);
    func_800A5738(a, data, static_cast<float>(uval), b);
}
