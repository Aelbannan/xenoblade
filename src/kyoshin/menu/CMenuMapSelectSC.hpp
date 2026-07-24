#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

// World map screen UI process (split-screen version).
// Inherits from CProcess (task system) and IScnRender (render callback).
// IScnRender subobject is at offset 0x58, requiring this-adjusting thunks.
class CMenuMapSelectSC : public CProcess, public IScnRender {
public:
    virtual ~CMenuMapSelectSC();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();
};
