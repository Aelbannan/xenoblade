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
    UnkClass_8045F564 mLayoutMem;        // 0x60 -- layout memory region (MEM2 alloc, scoped region guard)
    CScn* mScn;                          // 0x70 -- owning scene; addRenderCB target
    nw4r::lyt::Layout* mLayout;          // 0x74 -- layout draw target (hosts animations)
    nw4r::lyt::AnimTransform* mAnimDefault; // 0x78 -- default animation (label text, enabled at init)
    nw4r::lyt::AnimTransform* mAnimLabel;   // 0x7C -- label animation (disabled at init)
};
