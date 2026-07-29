#pragma once

#include <types.h>

class CMainMenu {
public:
    CMainMenu();
    virtual ~CMainMenu();
    void Init();
    void Term();
    void cbRenderBefore();
    int func_800FF778();

    // vtable pointer at 0x00 (implicit)
    u8 _04[0x3C - 0x04];
    u32 field_0x3C;
    u32 field_0x40;
    u32 field_0x44;
    u8 _48[0x74 - 0x48];
    u32 field_0x74;

    void OnFileEvent();
};

