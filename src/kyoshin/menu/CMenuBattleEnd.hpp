#pragma once

#include <types.h>

class CMenuBattleEnd {
public:
    CMenuBattleEnd();
    virtual ~CMenuBattleEnd();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

