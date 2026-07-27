#pragma once

#include <types.h>

class CSysWinSave {
public:
    virtual ~CSysWinSave();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

