#pragma once

#include "kyoshin/IUICf.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include <nw4r/lyt.h>

// Batch 2026-07-14l: menu-battlemode-init owns Init exclusively.
// IUICf/CTTask is 0x54; retail places IWorkEvent at 0x58 (extab), so pad 4
// (same shape proven for CMenuBattlePlayerState -- see MWCC_CASES section 15).
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
    u8 mState;                       // 0x80 -- battle-mode phase/state machine index
    virtual ~CMenuBattleMode();
    void Term();
    void Move();
    void cbRenderBefore();
    CMenuBattleMode();
};

// --- Imports used by this unit (data/globals referenced at their retail names) ---
// Singleton instance pointer for the battle-mode menu (shared .sbss 4-byte ptr).
extern u32 lbl_eu_80664318;
// Secondary-mode flags bitfield (bit 21 = 0x200000 gated in Move/cbRenderBefore).
extern u32 lbl_eu_80663E28;
// sdata2 animation frame constants.
extern const f32 lbl_eu_80667C80;
extern const f32 lbl_eu_80667C84;
// ARC resource name string table base.
extern char lbl_eu_80503D80[];
// CProcess primary vtable + CMenuBattleMode primary vtable (written by ctor).
extern char lbl_eu_8052BF70[];
extern char lbl_eu_80532DD0[];
// Null pointer-to-member-function constant used for the PTMF callback slots.
extern u32 __ptmf_null[3];
// Shared battle ARC resource accessor -- retail calls the unmangled name.
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();
// CProcess ctor import: CProcess is abstract (pure virtuals) so it cannot be
// placement-new'd from the factory; retail emits its mangled C-ABI name.
extern "C" void __ct__8CProcessFv(CProcess* self);
