#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

// World map screen UI process.
// Inherits from CProcess (task system) and IScnRender (render callback).
// IScnRender subobject is at offset 0x58, requiring this-adjusting thunks.
class CMenuMapSelect : public CProcess, public IScnRender {
public:
    // Implemented in code_802405F4.cpp
    virtual ~CMenuMapSelect();
    void cbRenderBefore();

    // Stub declarations for functions not yet decompiled
};
