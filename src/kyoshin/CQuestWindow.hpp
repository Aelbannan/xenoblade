#pragma once

#include <types.h>

class CQuestWindow {
public:
    CQuestWindow();
    virtual ~CQuestWindow();
    void Init();
    void Term();
    void cbRenderBefore();

    // TODO: add fields
    void OnFileEvent();
};

class IUIWindow {
public:
    virtual ~IUIWindow();

    // TODO: add fields
};

