#pragma once

#include <types.h>

class CMenuKizunaTalk {
public:
    CMenuKizunaTalk();
    virtual ~CMenuKizunaTalk();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
    void OnFileEvent();
};

