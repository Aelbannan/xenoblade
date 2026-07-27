#pragma once

#include <types.h>
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"

// Layout for MI subobject offset computation (IWorkEvent at +0x58, IScnRender at +0x5c)
struct CMenuBattleDamageLayout {
    u8 _00[0x58];
    IWorkEvent mIWorkEvent;
    IScnRender mIScnRender;
};

class CMenuBattleDamage {
public:
    CMenuBattleDamage();
    virtual ~CMenuBattleDamage();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    void thunk_IWorkEvent_dtor();
    void thunk_IScnRender_cbRenderBefore();
    void thunk_IScnRender_dtor();

    // TODO: add fields
};

class CPcSelectCursor01 {
public:
    virtual ~CPcSelectCursor01();

    void cursorInit();
    u8 cursorGetAnimFrame();

    u8 mState;
    u8 mAnimFrame;
    // TODO: add fields
};

