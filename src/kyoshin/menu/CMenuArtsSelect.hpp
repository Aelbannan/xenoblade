#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn.hpp"
#include <nw4r/lyt/lyt_layout.h>

// Minimal layout for batch-14f/14g/14j matching (Term / cbRenderBefore / Move).
// Retail: CTTask<IUIBattle>, IWorkEvent@0x58, IScnRender@0x5c, IObjectInfo@0x60.

// Object pointed by lbl_eu_80663F24 (SDA); Term clears +0xb0 and ORs +0x68.
struct UnkArtsSelectRef {
    u8 unk00[0x68];
    u32 unk68; // 0x68
    u8 unk6C[0xB0 - 0x6C];
    void* unkB0; // 0xB0
};

// Element of unk200[]; flag byte at +0xBB (Pane::mFlag / bit0).
struct UnkArtsSelectEntry {
    u8 unk00[0xBB];
    u8 unkBB; // 0xBB - bit0 gates per-slot draw in cbRenderBefore
};

class CMenuArtsSelect {
public:
    void Term();
    void Move();
    void cbRenderBefore();

    // Process base (CProcess): CDoubleListNode + vtable@0x10 + child list + flags@0x38.
    u8 unk00[0x10];
    void* vtable; // 0x10
    u8 unk14[0x3C - 0x14];
    u32 ptmfMove[3]; // 0x3C - __ptmf_null copy
    u32 ptmfDraw[3]; // 0x48 - __ptmf_null copy
    u8 unk54; // 0x54 - set when case-3 hide anim completes
    u8 unk55; // 0x55
    u8 unk56[0x58 - 0x56];
    void* vtWorkEvent; // 0x58 - lbl_eu_8052C084+0x24
    void* vtScnRender; // 0x5C - lbl_eu_8052C084+0xAC
    void* vtObjectInfo; // 0x60 - lbl_eu_8052C084+0xBC
    CScn* mScn; // 0x64
    u32 unk68; // 0x68
    UnkClass_8045F564 unk6C; // 0x6C
    s8 unk7C; // 0x7C
    s8 unk7D; // 0x7D
    s8 unk7E; // 0x7E
    u8 unk7F;
    nw4r::lyt::Layout* unk80; // 0x80
    nw4r::lyt::AnimTransform* unk84; // 0x84
    nw4r::lyt::AnimTransform* unk88; // 0x88
    nw4r::lyt::Layout* unk8C; // 0x8C
    u8 unk90[0x98 - 0x90];
    nw4r::lyt::Layout* unk98; // 0x98
    u8 unk9C[0xA0 - 0x9C];
    nw4r::lyt::AnimTransform* unkA0; // 0xA0
    nw4r::lyt::Layout* unkA4[8]; // 0xA4
    nw4r::lyt::AnimTransform* unkC4[8]; // 0xC4
    nw4r::lyt::AnimTransform* unkE4[8]; // 0xE4
    nw4r::lyt::Layout* unk104[9]; // 0x104
    nw4r::lyt::AnimTransform* unk128[9]; // 0x128
    nw4r::lyt::AnimTransform* unk14C[9]; // 0x14C
    nw4r::lyt::Layout* unk170[9]; // 0x170
    nw4r::lyt::AnimTransform* unk194[9]; // 0x194
    nw4r::lyt::Layout* unk1B8[9]; // 0x1B8 - cbRenderBefore loop keyed by unk30C bits
    nw4r::lyt::AnimTransform* unk1DC[9]; // 0x1DC
    UnkArtsSelectEntry* unk200[9]; // 0x200 - indexed (i+1)%9 in cbRenderBefore
    UnkArtsSelectEntry* unk224[9]; // 0x224
    UnkArtsSelectEntry* unk248[9]; // 0x248
    UnkArtsSelectEntry* unk26C[7]; // 0x26C..0x284 - visibility flag group
    u8 unk288; // 0x288
    u8 unk289[8]; // 0x289..0x290 - ctor clears per-slot bytes
    u8 unk291; // 0x291
    u8 unk292[0x294 - 0x292];
    nw4r::lyt::Layout* unk294; // 0x294
    s32 unk298; // 0x298 - main FSM / cbRenderBefore loop-1 gate (drawn when > 1)
    s32 unk29C; // 0x29C - secondary FSM when unk328 == 4
    s32 unk2A0[8]; // 0x2A0 - per-art slot substate (9/10/11)
    s32 unk2C0[9]; // 0x2C0 - per-slot mode, compared against 0xC
    s32 unk2E4[9]; // 0x2E4 - per-slot alt mode (0x10..0x13)
    u32 unk308; // 0x308 - render flag bits (cbRenderBefore)
    u16 unk30C; // 0x30C - per-slot (unk1B8) visibility bits
    u8 unk30E[0x310 - 0x30E];
    u32 unk310; // 0x310 - per-slot (unk104) latch bits 0..8
    u32 unk314; // 0x314 - per-slot (unk170) latch bits 0..8
    u32 unk318; // 0x318 - per-slot (unkA4) latch bits 0..7
    u32 unk31C; // 0x31C
    u8 unk320; // 0x320
    u8 unk321[0x324 - 0x321];
    s32 unk324; // 0x324 - compared to 4 after func_80107580
    s32 unk328; // 0x328 - render mode (unk98 draw gate == 4)
    u32 unk32C; // 0x32C
    u32 unk330; // 0x330
    u8 unk334; // 0x334
    u8 unk335; // 0x335
    u8 unk336; // 0x336
    u8 unk337[0x340 - 0x337];
    u16 unk340; // 0x340 - gate for per-slot (unkA4) draw call
    u8 unk342[0x344 - 0x342];
    f32 unk344; // 0x344
    u8 unk348; // 0x348 - party-target latch
};

extern "C" {
extern CMenuArtsSelect* lbl_eu_80663F20;
extern UnkArtsSelectRef* lbl_eu_80663F24;
}
