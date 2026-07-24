#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

// Kizuna talk list screen.
// Inherits from CProcess (task system) and IScnRender (render callback).
// IScnRender subobject is at offset +0x58, requiring this-adjusting thunks.
class CMenuKizunaTalkList : public CProcess, public IScnRender {
public:
    CMenuKizunaTalkList();
    virtual ~CMenuKizunaTalkList();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
};
