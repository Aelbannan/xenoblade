#pragma once

#include <types.h>
#include <nw4r/lyt.h>

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn.hpp"

// Minimal layout for batch-14e/14l matching (cbRenderBefore / Move / Init).
class CMenuPTGauge {
public:
    void Init();
    void Move();
    void cbRenderBefore();

    // 0x00: base / unknown
    u8 unk00[0x60];
    CScn* mScn; // 0x60 -- owning scene; addRenderCB target in Init
    UnkClass_8045F564 unk64; // 0x64 -- layout memory region (Init createRegion)
    nw4r::lyt::Layout* unk74; // 0x74 -- layout draw target
    nw4r::lyt::AnimTransform* unk78; // 0x78
    nw4r::lyt::AnimTransform* unk7C; // 0x7C
    nw4r::lyt::AnimTransform* unk80; // 0x80
    nw4r::lyt::AnimTransform* unk84; // 0x84
    nw4r::lyt::AnimTransform* unk88; // 0x88
    s32 unk8C; // 0x8C -- render/move state
    s32 unk90; // 0x90 -- PTMF / substate index
    s32 unk94; // 0x94
    s32 unk98; // 0x98
    s32 unk9C; // 0x9C
    s32 unkA0; // 0xA0 -- gauge value latch
};
