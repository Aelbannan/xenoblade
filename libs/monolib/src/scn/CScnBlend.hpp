#pragma once

#include <monolib/scn/CScnFilter.hpp>

// Region + tint as one anonymous aggregate (MWCC struct-copies via lwz/stw).
// x/y/w/h describe the blend quad in normalised screen space; tR/tG/tB are the
// per-channel tint factors.
struct RectRegion {
    struct {
        f32 x;      // 0x38
        f32 y;      // 0x3C
        f32 w;      // 0x40
        f32 h;      // 0x44
        f32 tR;     // 0x48
        f32 tG;     // 0x4C
        f32 tB;     // 0x50
    };
};

struct CScnFilterHost;

struct CScnBlend : public CScnFilter {
    CScnBlend();
    virtual ~CScnBlend();
    virtual void drawFilter(CScnFilterHost* host);

    u8 pad_18[0x38 - 0x18];
    RectRegion mRect;   // 0x38
    u8 mFlags;          // 0x54 bit0: render-to-texture, bit1: halve target size
    u8 pad_55[3];
    u32 field_0x58;     // texture format passed to the tex-work allocator
    s32 field_0x5C;     // blend mode (compared against 3 for the two-stage path)
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6C;
};
