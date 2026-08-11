#pragma once

#include <types.h>
#include <revolution/GX.h>
#include "monolib/math/CCol4.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/core/CViewFrame.hpp"

class CView;

// Opaque render-work object (CScnTexWorkMan instance) that the bloom code only
// forwards to the texture-work helpers; no fields are accessed here.
struct CTexWorkObj;

// Shared .sdata2 float constants used by this unit (retail pool defined in
// monolibdata2.s). Declared at global scope: plain names, no mangling.
extern const f32 lbl_eu_8066AB00;  // 1.0f
extern const f32 lbl_eu_8066AB04;  // 0.5f
extern const f32 lbl_eu_8066AB08;  // 1.5f
extern const f32 lbl_eu_8066AB28;  // 1.0f
extern const f32 lbl_eu_8066AB2C;  // 0.5f
extern const f32 lbl_eu_8066AB30;  // 0.0f
extern const f32 lbl_eu_8066AB40;  // 255.0f

// Bloom base parameters (0x00-0x47), initialized by func_80499718.
// NOTE: this base is not polymorphic in retail (no vptr at 0x00); the
// destructor __dt__804997D0 is a separate fragment function.
struct CScnBloomBase {
    ml::CCol4 mColor;       // 0x00
    u8 mBlendMode;          // 0x10
    u8 mFilterType;         // 0x11
    u8 pad_12[2];
    u32 mTexFormat;         // 0x14 - texture format id passed to func_80490208
    u32 mUnk18;             // 0x18
    u32 mUnk1C;             // 0x1C
    u8 mEnabled;            // 0x20
    u8 mRenderMode;         // 0x21
    u8 pad_22[2];
    f32 mThreshold;         // 0x24
    u32 mBlurSamples;       // 0x28
    u32 mUnk2C;             // 0x2C
    s32 mUnk30;             // 0x30
    s32 mUnk34;             // 0x34 (signed: compared with cmpwi against 3)
    u32 mUnk38;             // 0x38
    u32 mUnk3C;             // 0x3C
    u32 mUnk40;             // 0x40
    u32 mUnk44;             // 0x44
}; // size = 0x48

// CView as seen by the bloom code: only field_0x6C (the render-work object
// pointer) is read.
struct CScnBloomView {
    u8 field_0x00[0x6C];
    CTexWorkObj* mTexWork;  // 0x6C
};

// Bloom post-processing object created by CScnRoot (0x94 bytes). func_80499FD0
// is its second-stage constructor (view stored at 0x48).
class CScnBloom : public CScnBloomBase {
public:
    CView* mView;           // 0x48 - owning view
    GXTexObj* mTexture;     // 0x4C - current screen texture
    u8 mUnk50;              // 0x50
    u8 pad_51[3];
    ml::CCol4 mClearColor;  // 0x54 - clear/background color
    u8 mFlags;              // 0x64 - flags byte (bit 2: extra bloom pass)
    u8 pad_65[3];
    ml::CCol4 mBlurColor;   // 0x68 - blur tint color
    ml::CVec3 mBlurScale;   // 0x78 - blur offset scale (x/y/z)
    u32 mViewportX;         // 0x84
    u32 mViewportY;         // 0x88
    u32 mViewportW;         // 0x8C
    u32 mViewportH;         // 0x90
}; // size = 0x94
