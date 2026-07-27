#pragma once

#include <types.h>

class CMainMenu {
public:
    CMainMenu();
    virtual ~CMainMenu();
    void Init();
    void Term();
    void cbRenderBefore();
    void func_800FF778();

    // TODO: add fields
    void OnFileEvent();
};

