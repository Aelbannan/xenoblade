#pragma once

#include <types.h>

class CUIErrMesWin {
public:
    virtual ~CUIErrMesWin();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

class CMenuTitle {
public:
    virtual ~CMenuTitle();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

