#pragma once

#include <types.h>

class CMenuPTChangeNotice {
public:
    CMenuPTChangeNotice();
    virtual ~CMenuPTChangeNotice();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
    void OnFileEvent();
};

