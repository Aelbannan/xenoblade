#pragma once

#include <types.h>

class CMenuUpdate {
public:
    CMenuUpdate();
    virtual ~CMenuUpdate();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

class IUIWindow {
public:
    IUIWindow();

    // TODO: add fields
};

class CTTask_IUIWindow {
public:
    CTTask_IUIWindow();

    // TODO: add fields
};

