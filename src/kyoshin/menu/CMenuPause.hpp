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
    void isInitialized();

    // TODO: add fields
};

