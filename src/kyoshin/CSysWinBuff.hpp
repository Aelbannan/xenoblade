#pragma once

#include <types.h>

class CSysWinBuff {
public:
    virtual ~CSysWinBuff();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
    void create();

    // TODO: add fields
    void getInstance();
    void OnFileEvent();
};

