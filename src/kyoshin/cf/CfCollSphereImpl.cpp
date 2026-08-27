#include "types.h"
#include "kyoshin/cf/CfCollSphereImpl.hpp"

// Retail helpers now use the real class tree: the position getter at vtable
// slot +0xAC is cf::CfObject::CfObject_UnkVirtualFunc23 (ml::CVec3*), whose
// retail entry lives in lbl_eu_80528600 (0x178-byte vtable, RTTI
// lbl_eu_80661C78 "cf::CfObjectColl").  CfCollSphereImpl inherits CfObject
// (novtable) so this->CfObject_UnkVirtualFunc23() emits the same
//   lwz r12,0(rX) / lwz r12,172(r12) / bctrl
// sequence as the old reinterpret_cast<If*> pad, and the radius at 0xB8 is
// the inherited field_0xB8 / mRadius.

// func_800AAD28: debug draw for sphere collision shape (single call).
extern "C" void func_800AAD28(void* context, cf::CfCollSphereImpl* shape) {
    u32 uval = static_cast<u32>(shape->mRadius);
    ml::CVec3* pos = shape->CfObject_UnkVirtualFunc23();
    // renderSphere's "manager" arg is layout-compatible with a CVec3* position
    // (CfDebugDrawManager::mPos at offset 0).  Retail passes the position
    // vector as the manager pointer.
    renderSphere__Q22cf18CfDebugDrawManagerFv(reinterpret_cast<void*>(pos),
                                              static_cast<float>(uval));
}

// func_800AAD94: debug draw for sphere with extra transform params.
extern "C" void func_800AAD94(void* context, cf::CfCollSphereImpl* shape, void* a, void* b) {
    u32 uval = static_cast<u32>(shape->mRadius);
    ml::CVec3* pos = shape->CfObject_UnkVirtualFunc23();
    func_800A5738(a, pos, static_cast<float>(uval), b);
}
