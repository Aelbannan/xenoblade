#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

// Kizuna gram (affinity chart) screen shown on the World Map.
// Inherits from CProcess (task system) and IScnRender (render callback).
// IScnRender subobject is at offset +0x58, requiring this-adjusting thunks.
class CMenuKizunagram : public CProcess, public IScnRender {
public:
    CMenuKizunagram();
    virtual ~CMenuKizunagram();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
};
