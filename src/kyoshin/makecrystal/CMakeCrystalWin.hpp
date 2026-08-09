#pragma once

#include <types.h>

class CMakeCrystalWin {
public:
    CMakeCrystalWin();
    virtual ~CMakeCrystalWin();
    void cbRenderBefore();

    // TODO: add fields
    u8 _pad4361[0x435D];
    u8 field_4361; // 0x4361 (after vtable)
};

