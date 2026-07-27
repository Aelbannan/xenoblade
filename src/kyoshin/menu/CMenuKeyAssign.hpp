#pragma once

#include <types.h>

class CMenuKeyAssign {
public:
    virtual ~CMenuKeyAssign();
    CMenuKeyAssign();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
    void Draw();
};

