#pragma once

#include <types.h>

#include "kyoshin/IUICf.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"

namespace nw4r { namespace lyt { class Layout; class AnimTransform; } }
class CProcess;
class CScn;

// One slot of the 3-entry array at +0x74 (0x38 stride).
struct CMenuLvUpEntry {
    nw4r::lyt::Layout* field_0x00;        // +0x00 first layout
    nw4r::lyt::AnimTransform* field_0x04; // +0x04 first anim
    nw4r::lyt::Layout* field_0x08;        // +0x08 second layout
    nw4r::lyt::AnimTransform* field_0x0C; // +0x0C second anim
    u32 field_0x10;                       // +0x10 actor id (findObjectById)
    u8 field_0x14;                        // +0x14 state (0/1/2/3)
    u8 field_0x15;                        // +0x15 last-processed queue state
    u8 field_0x16[0x20];                  // +0x16..0x35 queued-state list (0 = empty)
    u8 field_0x36;                        // +0x36
    u8 field_0x37;                        // +0x37
};

// Level-up menu task.
class CMenuLvUp : public IUICf, public IWorkEvent, public IScnRender {
public:
    CMenuLvUp(CScn* scene);
    virtual ~CMenuLvUp();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    static CMenuLvUp* create(CProcess* parent, CScn* scene);

    static CMenuLvUp* sInstance;

    /* 0x60 */ UnkClass_8045F564 mMemRegion;
    /* 0x70 */ CScn* mScene;
    /* 0x74 */ CMenuLvUpEntry mEntries[3];
    /* 0x11C */ u8 mField11C; // menu-active flag
}; // size: 0x120
