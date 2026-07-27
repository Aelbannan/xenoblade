#pragma once

#include <types.h>

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

