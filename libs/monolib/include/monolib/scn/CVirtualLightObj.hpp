#pragma once

#include <types.h>

#include "monolib/math/CCol4.hpp"

// Virtual light base object (size 0x34). Polymorphic: the compiler-emitted
// vtable (__vt__16CVirtualLightObj, CVirtualLightObj.cpp .data) holds
// [rtti, 0, dtor, getColor, getSentinel]. The dtor is inline, so its weak
// out-of-line copy lands in the first TU that needs it (CScnVirtualLight).
// getColor returns the struct by value (hidden result pointer in r3).
class CVirtualLightObj {
public:
    CVirtualLightObj();
    virtual ~CVirtualLightObj() {}
    virtual ml::CCol4 getColor();
    virtual f32 getSentinel();

    f32 mField04;     // +0x04
    f32 mField08;     // +0x08
    f32 mField0C;     // +0x0C
    f32 mField10;     // +0x10
    f32 mField14;     // +0x14
    u8 mByte18;       // +0x18
    u8 mByte19;       // +0x19
    f32 mField1C;     // +0x1C
    f32 mField20;     // +0x20
    f32 mField24;     // +0x24
    f32 mField28;     // +0x28
    s32 mField2C;     // +0x2C: light kind (0 base, 1 amb, 2 dir)
    s32 mField30;     // +0x30
};
