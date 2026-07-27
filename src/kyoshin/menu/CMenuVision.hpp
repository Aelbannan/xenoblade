#pragma once

#include <types.h>

class CMenuVision {
public:
    CMenuVision();
    virtual ~CMenuVision();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

