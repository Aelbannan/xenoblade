#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

class CBgTex;
class CMakeCrystalWin;

// Data block copied by func_80211CB8 (fields +0x4..+0x14 only; +0x0 is not
// part of the copy). Two bytes, three words, one byte - meaning unknown.
struct CMakeCrystalCopyBlock {
    u32 field_0x0;   // +0x00 (not copied)
    u8  field_0x4;   // +0x04
    u8  field_0x5;   // +0x05
    u32 field_0x8;   // +0x08
    u32 field_0xC;   // +0x0C
    u32 field_0x10;  // +0x10
    u8  field_0x14;  // +0x14
};

// Main menu UI for the crystal crafting system.
// Inherits from CProcess (task system) and IScnRender (render callback interface).
// IScnRender subobject is at offset 0x58, requiring this-adjusting thunks.
class CMenuMakeCrystal : public CProcess, public IScnRender {
public:
    CMenuMakeCrystal();
    virtual ~CMenuMakeCrystal();

    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // IScnRender vtable this-adjusting thunks
    void func_802124F4();
    void func_802124FC();
};
