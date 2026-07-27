#pragma once

#include <types.h>

class CMenuBattleChain {
public:
    CMenuBattleChain();
    virtual ~CMenuBattleChain();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

