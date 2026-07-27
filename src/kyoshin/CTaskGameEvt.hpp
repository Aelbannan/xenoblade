#pragma once

#include <types.h>

class CTaskGameEvt {
public:
    CTaskGameEvt();
    virtual ~CTaskGameEvt();
    void Init();
    void Term();
    void Move();
    void create();

    // TODO: add fields
    void cbRenderBefore();
    void Draw();
    void OnFileEvent();
};

