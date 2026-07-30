#pragma once

#include <types.h>

class CVirtualLightObj {
public:
    CVirtualLightObj();
    virtual ~CVirtualLightObj();

    // Fields initialized by constructor
    // +0x00: vtable pointer (auto)
    f32 mField04;     // +0x04 = 0.0f
    f32 mField08;     // +0x08 = 0.0f
    f32 mField0C;     // +0x0C = 0.0f
    f32 mField10;     // +0x10 = 0.0f
    f32 mField14;     // +0x14 = 0.0f
    u8 mByte18;       // +0x18 = 1
    u8 mByte19;       // +0x19 = 0
    f32 mField1C;     // +0x1C = 0.0f
    f32 mField20;     // +0x20 = 0.0f
    f32 mField24;     // +0x24 = 0.0f
    f32 mField28;     // +0x28 = 0.0f
    s32 mField2C;     // +0x2C = 0 (overridden by CVirtualLightAmb)
    s32 mField30;     // +0x30 = -1
    u8 _34[0x1140];   // remaining padding up to 0x1174
    u8 mUnk1174;      // at 0x1174
};
