#include "types.h"
#include "kyoshin/cf/CfCollSphereImpl.hpp"

// External symbols from other TUs.
// extern "C" prevents MWCC from appending parameter-type suffixes (__FPvf)
// to the relocation names, keeping them matching the retail symbol map.
extern "C" {

// renderSphere: debug sphere rendering. Despite the Fv mangled suffix,
// retail passes (void* data, float radius) via r3/f1.
void renderSphere__Q22cf18CfDebugDrawManagerFv(void* data, float radius);

// func_800A5738: defined in kyoshin/code_800A3B24.
// Call convention: (void* a, void* data, float radius, void* b) via r3/r4/f1/r5.
void func_800A5738(void* a, void* data, float radius, void* b);

} // extern "C"

// func_800AAD28: debug draw for sphere collision shape.
// Reads mRadius, converts float->unsigned->float (preserving bit pattern),
// calls vfunc at vtable offset 0xAC to get draw data, then calls renderSphere.
void func_800AAD28(void* /*unused*/, cf::CfCollSphereImpl* shape) {
    u32 uval = static_cast<u32>(shape->mRadius);
    cf::CfCollSphereImplVtable* vtbl = reinterpret_cast<cf::CfCollSphereImplVtable*>(
        *reinterpret_cast<void***>(shape));
    void* data = vtbl->func_0xAC(shape);
    renderSphere__Q22cf18CfDebugDrawManagerFv(data, static_cast<float>(uval));
}

// func_800AAD94: same sphere debug draw but passes extra parameters through
// to func_800A5738.
void func_800AAD94(void* /*unused*/, cf::CfCollSphereImpl* shape, void* a, void* b) {
    u32 uval = static_cast<u32>(shape->mRadius);
    cf::CfCollSphereImplVtable* vtbl = reinterpret_cast<cf::CfCollSphereImplVtable*>(
        *reinterpret_cast<void***>(shape));
    void* data = vtbl->func_0xAC(shape);
    func_800A5738(a, data, static_cast<float>(uval), b);
}
