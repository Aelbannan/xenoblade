#pragma once

#include <types.h>

class CTaskGameEff {
public:
    CTaskGameEff();
    virtual ~CTaskGameEff();
    void Init();
    void Term();

    // TODO: add fields
    void Move();
    void cbRenderBefore();
    void Draw();
};

