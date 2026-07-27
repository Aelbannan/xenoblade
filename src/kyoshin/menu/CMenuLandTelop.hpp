#pragma once

#include <types.h>

class CMenuLandTelop {
public:
    CMenuLandTelop();
    virtual ~CMenuLandTelop();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

