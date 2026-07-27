#pragma once

#include <types.h>

class CTaskGamePic {
public:
    CTaskGamePic();
    virtual ~CTaskGamePic();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
    void create();

    // TODO: add fields
    void Draw();
    void OnFileEvent();
};

