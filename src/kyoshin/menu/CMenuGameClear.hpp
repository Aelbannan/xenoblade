#pragma once

#include <types.h>

class CMenuGameClear {
public:
    virtual ~CMenuGameClear();
    CMenuGameClear();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

