#pragma once

#include <types.h>

// NOTE: vtable pointer is at offset 0 (compiler-generated)
struct __declspec(novtable) CVirtualLightAmb {
    virtual ~CVirtualLightAmb();

    // Fields at CVirtualLightObj-compatible offsets
    // +0x00: vtable pointer (auto/compiler-managed)
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
    s32 mField2C;     // +0x2C
    s32 mField30;     // +0x30
    u8 _34[0x1140];   // remaining padding up to 0x1174
    u8 mUnk1174;      // at 0x1174
};
