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

    // TODO: add fields
};

class CPcSelectCursor01 {
public:
    virtual ~CPcSelectCursor01();

    // TODO: add fields
};

