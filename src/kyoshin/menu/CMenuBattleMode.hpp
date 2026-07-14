#pragma once

#include "kyoshin/IUICf.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include <nw4r/lyt.h>

// Batch 2026-07-14l: menu-battlemode-init owns Init exclusively.
// IUICf/CTTask is 0x54; retail places IWorkEvent at 0x58 (extab), so pad 4
// (same shape proven for CMenuBattlePlayerState -- see MWCC_REFERENCE section 15).
class CMenuBattleModeBase : public IUICf {
public:
    u8 pad54[4];
};

class CMenuBattleMode : public CMenuBattleModeBase,
                         public IWorkEvent,
                         public IScnRender {
public:
    void Init();

    // IWorkEvent @0x58; IScnRender @0x5c (extab)
    UnkClass_8045F564 unk60; // 0x60 -- layout memory region
    CScn* unk70;              // 0x70 -- owning scene (addRenderCB target)
    nw4r::lyt::Layout* unk74; // 0x74 -- layout draw target
    nw4r::lyt::AnimTransform* unk78; // 0x78
    nw4r::lyt::AnimTransform* unk7C; // 0x7c
};
