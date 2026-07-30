#pragma once

#include <types.h>

class CMenuPause {
public:
    virtual ~CMenuPause();
    CMenuPause();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
    unsigned long isInitialized();

    // TODO: add fields
};

