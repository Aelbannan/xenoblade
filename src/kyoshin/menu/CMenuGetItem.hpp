#pragma once

#include <types.h>

class CMenuGetItem {
public:
    CMenuGetItem();
    virtual ~CMenuGetItem();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

