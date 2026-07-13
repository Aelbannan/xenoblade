#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn.hpp"
#include <nw4r/lyt/lyt_layout.h>

// Minimal layout for batch-14f matching (Term only).
// Retail: CTTask<IUIBattle>, IWorkEvent@0x58, IScnRender@0x5c, IObjectInfo@0x60.

// Object pointed by lbl_eu_80663F24 (SDA); Term clears +0xb0 and ORs +0x68.
struct UnkArtsSelectRef {
    u8 unk00[0x68];
    u32 unk68; // 0x68
    u8 unk6C[0xB0 - 0x6C];
    void* unkB0; // 0xB0
};

// Element of unk200[]; cbRenderBefore only reads the low bit of +0xBB.
struct UnkArtsSelectEntry {
    u8 unk00[0xBB];
    u8 unkBB; // 0xBB - bit0 gates per-slot draw in cbRenderBefore
};

class CMenuArtsSelect {
public:
    void Term();
    void cbRenderBefore();

    u8 unk00[0x64];
    CScn* mScn; // 0x64
    u32 unk68; // 0x68
    UnkClass_8045F564 unk6C; // 0x6C
    u8 unk7C[0x80 - 0x7C];
    nw4r::lyt::Layout* unk80; // 0x80
    u8 unk84[0x8C - 0x84];
    nw4r::lyt::Layout* unk8C; // 0x8C
    u8 unk90[0x98 - 0x90];
    nw4r::lyt::Layout* unk98; // 0x98
    u8 unk9C[0xA4 - 0x9C];
    nw4r::lyt::Layout* unkA4[8]; // 0xA4
    u8 unkC4[0x104 - 0xC4];
    nw4r::lyt::Layout* unk104[9]; // 0x104
    u8 unk128[0x170 - 0x128];
    nw4r::lyt::Layout* unk170[9]; // 0x170
    u8 unk194[0x1B8 - 0x194];
    nw4r::lyt::Layout* unk1B8[9]; // 0x1B8 - cbRenderBefore loop keyed by unk30C bits
    u8 unk1DC[0x200 - 0x1DC];
    UnkArtsSelectEntry* unk200[9]; // 0x200 - indexed (i+1)%9 in cbRenderBefore
    u8 unk224[0x294 - 0x224];
    nw4r::lyt::Layout* unk294; // 0x294
    s32 unk298; // 0x298 - cbRenderBefore loop-1 gate (drawn when > 1)
    u8 unk29C[0x2C0 - 0x29C];
    s32 unk2C0[9]; // 0x2C0 - per-slot mode, compared against 0xC
    u8 unk2E4[0x308 - 0x2E4];
    u32 unk308; // 0x308 - render flag bits (cbRenderBefore)
    u16 unk30C; // 0x30C - per-slot (unk1B8) visibility bits
    u8 unk30E[0x310 - 0x30E];
    u32 unk310; // 0x310 - per-slot (unk104) latch bits 0..8
    u32 unk314; // 0x314 - per-slot (unk170) latch bits 0..8
    u32 unk318; // 0x318 - per-slot (unkA4) latch bits 0..7
    u8 unk31C[0x328 - 0x31C];
    s32 unk328; // 0x328 - render mode (unk98 draw gate == 4)
    u8 unk32C[0x334 - 0x32C];
    u8 unk334; // 0x334
    u8 unk335[0x340 - 0x335];
    u16 unk340; // 0x340 - gate for per-slot (unkA4) draw call
};

extern "C" {
extern CMenuArtsSelect* lbl_eu_80663F20;
extern UnkArtsSelectRef* lbl_eu_80663F24;
}
