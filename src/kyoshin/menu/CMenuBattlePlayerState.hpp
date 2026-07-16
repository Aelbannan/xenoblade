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
    nw4r::lyt::AnimTransform* unk04; // +0x04 → this+0x78
    nw4r::lyt::Layout* unk08; // +0x08 → this+0x7c
    nw4r::lyt::AnimTransform* unk0C; // +0x0C → this+0x80
    nw4r::lyt::AnimTransform* unk10; // +0x10 → this+0x84
    nw4r::lyt::AnimTransform* unk14; // +0x14 → this+0x88
    nw4r::lyt::Layout* unk18; // +0x18 → this+0x8c
    nw4r::lyt::AnimTransform* unk1C; // +0x1C → this+0x90
    nw4r::lyt::Layout* unk20; // +0x20 → this+0x94
    nw4r::lyt::AnimTransform* unk24; // +0x24 → this+0x98
    nw4r::lyt::Layout* unk28; // +0x28 → this+0x9c
    nw4r::lyt::AnimTransform* unk2C; // +0x2C → this+0xA0
    nw4r::lyt::AnimTransform* unk30; // +0x30 → this+0xA4
    nw4r::lyt::AnimTransform* unk34; // +0x34 → this+0xA8
    nw4r::lyt::AnimTransform* unk38; // +0x38 → this+0xAC
    nw4r::lyt::AnimTransform* unk3C; // +0x3C → this+0xB0
    nw4r::lyt::AnimTransform* unk40; // +0x40 → this+0xB4
    nw4r::lyt::Pane* unk44; // +0x44: "Txt_Dr_Lv"
    nw4r::lyt::Pane* unk48; // +0x48: "Txt_Dr"
    nw4r::lyt::Pane* unk4C; // +0x4C: "Txt_LvNum"
    nw4r::lyt::Pane* unk50; // +0x50: "N_HpGauge"
    nw4r::lyt::Pane* unk54; // +0x54: "N_HpGauge_Max"
    nw4r::lyt::Pane* unk58; // +0x58: "N_Tension"
    nw4r::lyt::Pane* unk5C; // +0x5C: "N_Tension_Gauge"
    nw4r::lyt::Pane* unk60; // +0x60: "N_PartyOn"
    nw4r::lyt::Pane* unk64; // +0x64: "N_ArtsEn"
    nw4r::lyt::Pane* unk68; // +0x68: "N_Tension_Star"
    nw4r::lyt::Pane* unk6C; // +0x6C: "N_Level_Star"
    nw4r::lyt::Pane* unk70; // +0x70: "N_AutoHeal"
    void* unk74; // +0x74: saved ArcResourceAccessor* (sub-layout)
    nw4r::lyt::Layout* unk78; // +0x78 → this+0xec
    nw4r::lyt::AnimTransform* unk7C; // +0x7c
    u8 unk80; // +0x80 → this+0xf4: gates the extra unk78 draw in cbRenderBefore
    u8 pad81[0x84 - 0x81];
    void* unk84; // +0x84
    void* unk88; // +0x88
    void* unk8C; // +0x8c
    u8 pad90[0x204 - 0x90]; // cleared each ctor pass (retail 0x90..0x204)
    u8 unk204; // +0x204: snapshot of actor+0x3f28 (low byte)
    u8 pad205[0x208 - 0x205];
    u32 unk208; // +0x208
    u32 unk20C; // +0x20C: previous vt+0x108 value
    u32 unk210; // +0x210: current HP
    u32 unk214; // +0x214: current max HP
    u32 unk218; // +0x218
    u32 unk21C; // +0x21C: previous unk218
    f32 unk220; // +0x220: HP ratio (0..100)
    f32 unk224; // +0x224: tension-like A
    f32 unk228; // +0x228: tension-like B
    f32 unk22C; // +0x22C: displayed tension A
    u32 unk230; // +0x230
    u8 pad234[0x238 - 0x234];
    u32 unk238; // +0x238
    u8 pad23C[0x240 - 0x23C];
    u8 unk240; // +0x240: dirty / in-combat gate
    u8 pad241[0x244 - 0x241];
    u32 unk244; // +0x244 → this+0x2b8: per-slot anim state / active gate
    u32 unk248; // +0x248: ctor default 4
    u8 pad24C[0x250 - 0x24C];
    u32 unk250; // +0x250: ctor default 6
    u32 unk254; // +0x254: ctor default 0xb
    u32 unk258; // +0x258: slot index
    u32 unk25C; // +0x25C → this+0x2d0: per-layout draw flag bits
    u8 pad260[0x264 - 0x260];
    f32 unk264; // +0x264
    u8 pad268[0x270 - 0x268];
};

// IUICf/CTTask is 0x54; retail places IWorkEvent at 0x58 (extab), so pad 4.
class CMenuBattlePlayerStateBase : public IUICf {
public:
    u8 unk54; // 0x54
    u8 unk55; // 0x55
    u8 pad56[2];
};

class CMenuBattlePlayerState : public CMenuBattlePlayerStateBase,
                               public IWorkEvent,
                               public IScnRender {
public:
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // IWorkEvent @0x58; IScnRender @0x5c (extab)
    CScn* mScn; // 0x60
    UnkClass_8045F564 unk64; // 0x64
    CMenuBattlePlayerStateSlot mSlots[3]; // 0x74
    f32 unk7C4; // 0x7c4: full-HP hold timer
    u8 unk7C8; // 0x7c8: once-guard for func_80138078(0x9a)
    u8 unk7C9; // 0x7c9: cbRenderBefore early-out gate (skip render when nonzero)
    u8 pad7CA[0x7CC - 0x7CA];
    char* unk7CC; // 0x7cc: retail embeds lbl_eu_8052C42C
    UnkClass_8045F564 unk7D0; // 0x7d0
    void* unk7E0; // 0x7e0
    nw4r::lyt::Layout* unk7E4; // 0x7e4
    nw4r::lyt::AnimTransform* unk7E8; // 0x7e8
    nw4r::lyt::AnimTransform* unk7EC; // 0x7ec
    nw4r::lyt::AnimTransform* unk7F0; // 0x7f0
    u8 unk7F4; // 0x7f4
    u8 unk7F5; // 0x7f5: feeds pane translate Y scaling
    u8 pad7F6[0x7F8 - 0x7F6];
    u32 unk7F8; // 0x7f8: gates the trailing unk7E4 draw / shared anim state
};
