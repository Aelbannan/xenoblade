#pragma once

#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

/*
 * Menu save/load screen process.
 * IScnRender subobject is at offset 0x58.
 */
class CMenuSave : public CProcess, public IScnRender {
public:
    CMenuSave();
    virtual ~CMenuSave();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

    // TODO: add fields
};

