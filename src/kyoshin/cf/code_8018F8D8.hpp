#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>
#include "kyoshin/cf/CfGameManager.hpp"

class CMenuPTState : public CProcess, public IScnRender {
public:
    CMenuPTState();
    virtual ~CMenuPTState();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

    // TODO: add fields
};

// Container used by the catalog helper functions func_8019017C / func_80190034.
// The hosting object keeps a cf::CfGameManager* reference at offset 0x408.
struct CFuncHost {
    u8 _0[0x408];
    cf::CfGameManager* manager;  // 0x408
};
