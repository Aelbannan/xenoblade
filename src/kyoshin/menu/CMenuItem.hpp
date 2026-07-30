#pragma once

#include <types.h>

class CMenuItem {
public:
    CMenuItem();
    virtual ~CMenuItem();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

