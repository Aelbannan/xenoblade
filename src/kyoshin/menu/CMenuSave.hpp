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

    // IScnRender vtable this-adjusting thunks
    void func_8028E7B8();
    void func_8028E7C0();

    // TODO: add fields
};

