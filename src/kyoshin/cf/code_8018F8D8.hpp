#pragma once

#include <types.h>

class CMenuPTState {
public:
    virtual ~CMenuPTState();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

