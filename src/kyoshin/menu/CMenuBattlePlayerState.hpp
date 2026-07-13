#pragma once

#include "kyoshin/IUICf.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include <nw4r/lyt.h>

// Per-party slot (stride 0x270). Exception table: member array at 0x74, count 3.
struct CMenuBattlePlayerStateSlot {
    nw4r::lyt::Layout* unk00; // +0x00 → this+0x74
    u8 pad04[0x08 - 0x04];
    nw4r::lyt::Layout* unk08; // +0x08 → this+0x7c
    u8 pad0C[0x18 - 0x0C];
    nw4r::lyt::Layout* unk18; // +0x18 → this+0x8c
    u8 pad1C[0x20 - 0x1C];
    nw4r::lyt::Layout* unk20; // +0x20 → this+0x94
    u8 pad24[0x28 - 0x24];
    nw4r::lyt::Layout* unk28; // +0x28 → this+0x9c
    u8 pad2C[0x78 - 0x2C];
    nw4r::lyt::Layout* unk78; // +0x78 → this+0xec
    u8 pad7C[0x80 - 0x7C];
    u8 unk80; // +0x80 → this+0xf4: gates the extra unk78 draw in cbRenderBefore
    u8 pad81[0x244 - 0x81];
    u32 unk244; // +0x244 → this+0x2b8: per-slot render/active gate
    u8 pad248[0x25C - 0x248];
    u32 unk25C; // +0x25C → this+0x2d0: per-layout draw flag bits
    u8 pad260[0x270 - 0x260];
};

// IUICf/CTTask is 0x54; retail places IWorkEvent at 0x58 (extab), so pad 4.
class CMenuBattlePlayerStateBase : public IUICf {
    u8 pad54[4];
};

class CMenuBattlePlayerState : public CMenuBattlePlayerStateBase,
                               public IWorkEvent,
                               public IScnRender {
public:
    void Term();
    void cbRenderBefore();

    // IWorkEvent @0x58; IScnRender @0x5c (extab)
    CScn* mScn; // 0x60
    UnkClass_8045F564 unk64; // 0x64
    CMenuBattlePlayerStateSlot mSlots[3]; // 0x74
    u8 pad7C4[0x7C9 - 0x7C4];
    u8 unk7C9; // 0x7c9: cbRenderBefore early-out gate (skip render when nonzero)
    u8 pad7CA[0x7D0 - 0x7CA];
    UnkClass_8045F564 unk7D0; // 0x7d0
    u8 pad7E0[0x7E4 - 0x7E0];
    nw4r::lyt::Layout* unk7E4; // 0x7e4
    u8 pad7E8[0x7F8 - 0x7E8];
    u32 unk7F8; // 0x7f8: gates the trailing unk7E4 draw in cbRenderBefore
};
