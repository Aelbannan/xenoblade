#pragma once

#include "monolib/scn/CScnFilter.hpp"

// Forward declaration for the primary base class (vtable at 0x804997D0)
// Initialized by func_804996E8, destroyed by __dt__804997D0.
class CScnBloomBase {
public:
    virtual ~CScnBloomBase();

    // Bloom effect parameters (initialized by func_80499718)
    f32 mColorR;         // 0x00
    f32 mColorG;         // 0x04
    f32 mColorB;         // 0x08
    f32 mColorA;         // 0x0C
    u8 mBlendMode;       // 0x10
    u8 mFilterType;      // 0x11
    u8 pad_12[2];
    u32 mPassCount;      // 0x14
    u32 mUnk18;          // 0x18
    u32 mUnk1C;          // 0x1C
    u8 mEnabled;         // 0x20
    u8 mRenderMode;      // 0x21
    u8 pad_22[2];
    f32 mThreshold;      // 0x24
    u32 mBlurSamples;    // 0x28
    u32 mUnk2C;          // 0x2C
    u32 mUnk30;          // 0x30
    u32 mUnk34;          // 0x34
    u32 mUnk38;          // 0x38
    u32 mUnk3C;          // 0x3C
    u32 mUnk40;          // 0x40
    u32 mUnk44;          // 0x44
}; // size = 0x48

// Bloom post-processing effect.
// Inherits from CScnBloomBase (primary, at offset 0x00) and CScnFilter (at offset 0x48).
class CScnBloom : public CScnBloomBase, public CScnFilter {
public:
    CScnBloom();
    virtual ~CScnBloom();

    // CScnFilter virtual overrides (thunks adjust this from CScnFilter subobject at 0x48)
    // Implemented as extern "C" thunks: func_804996D8, func_804996E0

private:
    // Main vtable at offset 0x80
};
