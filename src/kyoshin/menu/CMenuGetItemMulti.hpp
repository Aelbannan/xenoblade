#pragma once

#include <types.h>

class CMenuGetItemMulti {
public:
    CMenuGetItemMulti();
    virtual ~CMenuGetItemMulti();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
    void OnFileEvent();
};

