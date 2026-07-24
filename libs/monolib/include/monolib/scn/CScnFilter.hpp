#pragma once

#include <types.h>

// Scene filter base class (size: 0x18)
// Manages a visual filter effect with configurable parameters.
class CScnFilter {
public:
    CScnFilter();
    virtual ~CScnFilter();

    // vtable at 0x00
    u8 mEnabled;      // 0x04
    u8 pad_05[3];
    u32 mFlags;       // 0x08
    u32 mUnk0C;       // 0x0C
    f32 mIntensity;   // 0x10
    u32 mUnk14;       // 0x14
}; // size = 0x18
