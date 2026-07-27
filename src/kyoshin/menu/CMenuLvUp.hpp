#pragma once

#include <types.h>

class CMenuLvUp {
public:
    virtual ~CMenuLvUp();
    CMenuLvUp();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

