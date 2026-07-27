#pragma once

#include <types.h>

class CSysWinScenarioLog {
public:
    CSysWinScenarioLog();
    virtual ~CSysWinScenarioLog();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
    void OnFileEvent();
};

