#pragma once

#include <types.h>
#include <nw4r/lyt.h>

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn.hpp"

// PTGauge widget state machine:
//   mPhase 0 = entry/appear, 1 = appearing anim, 2 = active, 3 = disappearing anim
//   mActionIdx indexes into the PTMF dispatch table lbl_eu_805323F8

class CMenuPTGauge {
public:
    void Init();
    void Move();
    void cbRenderBefore();

    // 0x00: base classes (CTTask<IUICf>, IWorkEvent, IScnRender at +0x5c)
    u8 unk00[0x60];
    CScn* mScn; // 0x60 -- owning scene; addRenderCB target in Init
    UnkClass_8045F564 mLayoutMem; // 0x64 -- layout memory region
    nw4r::lyt::Layout* mLayout; // 0x74 -- layout draw target
    nw4r::lyt::AnimTransform* mAnimDefault; // 0x78 -- main animation (PTGauge_00)
    nw4r::lyt::AnimTransform* mAnimOpen; // 0x7C -- open animation (PTGauge_01)
    nw4r::lyt::AnimTransform* mAnimClose; // 0x80 -- close animation (PTGauge_02)
    nw4r::lyt::AnimTransform* mAnimIdle; // 0x84 -- idle animation (PTGauge_03)
    nw4r::lyt::AnimTransform* mAnimSpecial; // 0x88 -- special animation (PTGauge_04)
    s32 mPhase; // 0x8C -- FSM phase (0=entry, 1=appearing, 2=active, 3=disappearing)
    s32 mActionIdx; // 0x90 -- substate / PTMF dispatch index (0, 1, 2)
    s32 mGaugeBase; // 0x94 -- gauge starting value
    s32 mGaugePrev; // 0x98 -- gauge display / previous value
    s32 unk9C; // 0x9C -- unused
    s32 mGaugeInit; // 0xA0 -- gauge panel initialised flag (non-zero = needs layout fixup)
};
