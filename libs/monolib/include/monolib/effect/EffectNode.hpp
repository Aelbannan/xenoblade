#pragma once

#include <types.h>

// Effect node chain head / node record used by the effect system. The first
// 0x1C bytes are shared by the embedded head in CEffectObj and the pooled
// nodes fetched by SchedNode_FetchByIndex.
struct EffectNode {
    s16 field_0x00;
    s16 field_0x02;
    s16 field_0x04;
    s16 field_0x06;
    s16 field_0x08;
    s16 field_0x0a;
    s16 field_0x0c;
    u8 pad_0x0e[0x10 - 0x0e];
    f32 mTime; // 0x10: name-guess: per-node accumulated time read by EffObj_EmitUpdate
    u8 pad_0x14[0x18 - 0x14];
    u32 field_0x18;
    void EffFxReleaseNodeChain();
    EffectNode* EffFxStepNextNode();
    EffectNode* EffFxFetchHeadNode();
    void EffFxProcessNodeChain();
    void EffFxAppendChainNode(s32 index);
};
