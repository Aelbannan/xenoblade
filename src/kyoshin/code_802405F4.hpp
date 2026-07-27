#pragma once

#include <types.h>

class CMenuMapSelect {
public:
    virtual ~CMenuMapSelect();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

