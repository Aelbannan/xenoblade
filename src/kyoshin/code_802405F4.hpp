#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

class CMenuMapSelect : public CProcess, public IScnRender {
public:
    CMenuMapSelect();
    virtual ~CMenuMapSelect();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

    // TODO: add fields
};

