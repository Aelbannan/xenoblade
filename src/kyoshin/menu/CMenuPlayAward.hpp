#pragma once

#include <types.h>

class CMenuPlayAward {
public:
    CMenuPlayAward();
    virtual ~CMenuPlayAward();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

class CPlayAwardList {
public:
    CPlayAwardList();
    virtual ~CPlayAwardList();
    void OnFileEvent();

    // TODO: add fields
};

