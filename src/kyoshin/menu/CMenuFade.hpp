#pragma once

#include "kyoshin/IUICf.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/CScn.hpp"

#include <nw4r/lyt.h>

// IScnRender without a virtual destructor (CMenuFade.cpp defines
// ISCN_RENDER_NO_DTOR): the inline `virtual ~IScnRender(){}` makes MWCC emit a
// null-check branch in derived-class dtors that retail does not have.
#include "monolib/scn/IScnRender.hpp"

// MWCC_CASES §190: Do not inherit from IWorkEvent (out-of-line dtor
// pulls weak stubs). Intermediate base holds the 0x54-0x5b fields so that
// IScnRender lands at retail offset 0x5c, giving correct non-primary
// base-cast null-check codegen in Term().
class CMenuFadeBase : public IUICf {
public:


    u32 mIWorkEventVtbl;                        // 0x58 - IWorkEvent vtable (raw)
};

class CMenuFade : public CMenuFadeBase, public IScnRender {
public:
    CMenuFade();
    virtual ~CMenuFade();
    void Init();
    void Term();
    void Move();
    void Draw();
    void cbRenderBefore();

    // IScnRender at 0x5c (public non-primary base)
    CScn* mScn;                                 // 0x60
    UnkClass_8045F564 mLayoutMem;               // 0x64
    nw4r::lyt::Layout* mLayout;                 // 0x74
    nw4r::lyt::AnimTransform* mAnimDefault;     // 0x78
    float field_0x7c;                           // 0x7c - current animation frame (initial position set by state)
    float field_0x80;                           // 0x80 - fade-in duration (frame_size / field_0x80 if >= 0, else 1.0)
    float field_0x84;                           // 0x84 - fade-out duration (clamped to 30.0 if negative)
    float field_0x88;                           // 0x88 - third speed param (frame_size / field_0x88 if >= 0, else 1.0)
    u8 pad8C[4];                                // 0x8c
    s32 field_0x90;                             // 0x90 - initial frame state (0/1 = start at frame 0; 2 = start at frame_size-1)
    u8 field_0x94;                              // 0x94 - fade flag (read by isFadeActive)
    u8 pad95[3];                                // 0x95-0x97
    s32 field_0x98;                             // 0x98 - fade direction (0 = one-shot 0xff, 1 = -1)
};
