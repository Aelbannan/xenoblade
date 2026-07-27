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

// Full object layout for CMenuUpdate (used by C-linkage accessors)
struct CMenuUpdateFull {
    u8 _00[0x64];
    u8 field_64;
    u8 _65[0x67 - 0x65];
    u8 field_67;
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

