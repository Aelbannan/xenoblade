#pragma once

#include "kyoshin/IUICf.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/work/IWorkEvent.hpp"

class CProcess;
class CScn;
namespace nw4r { namespace lyt { class Layout; class AnimTransform; } }

// Battle-mode menu (IUICf task + IWorkEvent + IScnRender render callback).
class CMenuBattleMode : public IUICf, public IWorkEvent, public IScnRender {
public:
    CMenuBattleMode(CScn* scene);
    virtual ~CMenuBattleMode();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    // Only one battle-mode menu exists at a time (subsequent calls return NULL).
    static CMenuBattleMode* create(CProcess* parent, CScn* scene);

    static CMenuBattleMode* sInstance;

    /* 0x60 */ UnkClass_8045F564 mLayoutMem;   // layout memory region (MEM2 alloc, scoped region guard)
    /* 0x70 */ CScn* mScn;                     // owning scene; addRenderCB target
    /* 0x74 */ nw4r::lyt::Layout* mLayout;     // layout draw target (hosts animations)
    /* 0x78 */ nw4r::lyt::AnimTransform* mAnimDefault; // default animation (label text, enabled at init)
    /* 0x7C */ nw4r::lyt::AnimTransform* mAnimLabel;   // label animation (disabled at init)
    /* 0x80 */ u8 mState;                      // battle-mode phase/state machine index
}; // size: 0x84
