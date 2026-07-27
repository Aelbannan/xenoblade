#pragma once

#include <types.h>

class CSimpleEveTalkWin {
public:
    CSimpleEveTalkWin();
    virtual ~CSimpleEveTalkWin();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

