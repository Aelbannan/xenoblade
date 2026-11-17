#pragma once

#include <types.h>

// Virtual light base object (size 0x34). The retail vtable is an unnamed
// data symbol, so the vptr slot at +0x00 is a plain member that each ctor
// stores explicitly (CVirtualLightObj::CVirtualLightObj in
// CVirtualLightObj.cpp, then the derived Amb/Dir ctors).
class CVirtualLightObj {
public:
    CVirtualLightObj();

    u8* mVtable;      // +0x00: vtable pointer (stored manually)
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
