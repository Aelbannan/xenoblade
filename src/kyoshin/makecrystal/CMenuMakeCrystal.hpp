#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

class CBgTex;
class CMakeCrystalWin;

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
};
