#pragma once

#include <types.h>

class CMenuBattleCommu {
public:
    CMenuBattleCommu();
    virtual ~CMenuBattleCommu();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

