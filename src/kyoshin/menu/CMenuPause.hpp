#pragma once

#include <types.h>

#include "kyoshin/IUICf.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"

namespace nw4r { namespace lyt { class Layout; class AnimTransform; } }
class CScn;

// Pause menu screen.
class CMenuPause : public IUICf, public IWorkEvent, public IScnRender {
public:
    CMenuPause(CScn* scene) : mScene(scene), mLayout(nullptr), mAnim(nullptr), mState(0) {}
    virtual ~CMenuPause();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    static CMenuPause* create(CProcess* parent, CScn* scene);
    static unsigned long isInitialized();

    static CMenuPause* sInstance;

    /* 0x60 */ CScn* mScene;
    /* 0x64 */ UnkClass_8045F564 mMemRegion;
    /* 0x74 */ u32 mField74;
    /* 0x78 */ u32 mField78;
    /* 0x7C */ nw4r::lyt::Layout* mLayout;
    /* 0x80 */ nw4r::lyt::AnimTransform* mAnim;
    /* 0x84 */ u8 mState;
}; // size: 0x88
