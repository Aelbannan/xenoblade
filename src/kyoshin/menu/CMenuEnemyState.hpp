#pragma once

#include <types.h>
#include <nw4r/lyt.h>

// Minimal layout for batch-14f/14h matching (cbRenderBefore / Move).
// Panel slots: this + idx * 0x4c, fields at +0xa8..+0xcd (see cbRenderBefore)
// and +0xa4..+0xd8 (see Move; panelData = entry + 0xa4).
class CMenuEnemyState {
public:
    void cbRenderBefore();
    void Move();

    u8 unk00[0x60];
    void* unk60; // 0x60 - battle-manager-ish object; func_80496264(unk60,-1) -> pose data
    u8 unk64[0x74 - 0x64];
    nw4r::lyt::Layout* unk74; // 0x74 - shared layout draw target
    nw4r::lyt::AnimTransform* unk78; // 0x78
    u8 unk7C[0x778 - 0x7C];
    u32 unk778; // 0x778 - last-cleared panel actor id
    u8 unk77C[0x78D - 0x77C];
    u8 unk78D; // 0x78D - gate for the stale-flag search loop
    u8 unk78E[0x7E4 - 0x78E];
    u8 unk7E4[0x800 - 0x7E4]; // embedded sub-object passed by address to func_8010EE40
    nw4r::lyt::Layout* unk800; // 0x800
    nw4r::lyt::AnimTransform* unk804; // 0x804
    nw4r::lyt::AnimTransform* unk808; // 0x808
    u8 unk80C[0x824 - 0x80C];
    u8 unk824; // 0x824
    u8 unk825[0x828 - 0x825];
    s32 unk828; // 0x828 - draw/anim state (0..4)
    u8 unk82C[0x830 - 0x82C];
    u32 unk830; // 0x830 - last-selected target id
    u8 unk834; // 0x834 - cooldown direction latch
    u8 unk835[0x838 - 0x835];
    f32 unk838; // 0x838 - target-select pulse timer
};
