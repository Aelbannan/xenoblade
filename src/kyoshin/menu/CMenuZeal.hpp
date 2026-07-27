#pragma once

#include <types.h>

class CMenuZeal {
public:
    CMenuZeal();
    virtual ~CMenuZeal();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

