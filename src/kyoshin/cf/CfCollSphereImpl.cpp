#include "types.h"
#include "kyoshin/cf/CfCollSphereImpl.hpp"
#include "kyoshin/cf/object/CfObjectColl.hpp"

// The position getter at vtable slot +0xAC is cf::CfObject::GetPosition
// (ml::CVec3* at +0xAC, lbl_eu_80528600, RTTI 80661C78 "cf::CfObjectColl").
// The sphere helpers operate on the CfObject-family coll object that
// carries the radius at 0xB8 (CfObjectColl::field_0xB8).  Calling via
// CfObject ensures the compiler emits the same
//   lwz r12,0(rX) / lwz r12,172(r12) / bctrl
// as the old CfCollSphereVTableIf pad.

// func_800AAD28: debug draw for sphere collision shape (single call).
extern "C" void func_800AAD28(void* context, cf::CfObjectColl* shape) {
    u32 uval = static_cast<u32>(shape->field_0xB8);
    ml::CVec3* pos = shape->CfObject_UnkVirtualFunc23();
    renderSphere__Q22cf18CfDebugDrawManagerFv(reinterpret_cast<void*>(pos),
                                              static_cast<float>(uval));
}

// func_800AAD94: debug draw for sphere with extra transform params.
extern "C" void func_800AAD94(void* context, cf::CfObjectColl* shape, void* a, void* b) {
    u32 uval = static_cast<u32>(shape->field_0xB8);
    ml::CVec3* pos = shape->CfObject_UnkVirtualFunc23();
    func_800A5738(a, pos, static_cast<float>(uval), b);
}
