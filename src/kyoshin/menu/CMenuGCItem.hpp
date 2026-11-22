#pragma once

#include <types.h>
#include <monolib/scn/IScnRender.hpp>

#include "kyoshin/IUICf.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CItemBoxGrid.hpp"

// GC item menu screen.
class CMenuGCItem : public IUICf, public IScnRender {
public:
    CMenuGCItem(CProcess* parent, u32 arg);
    virtual ~CMenuGCItem();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore(CScn* scene);

    static CMenuGCItem* create(CProcess* registParent, CProcess* parent, u32 arg);
    void HandlePhase2();

    static CMenuGCItem* sInstance;

    /* 0x5C */ CProcess* mParentRef;
    /* 0x60 */ CBgTex mBgTex;
    /* 0x80 */ CTitleAHelp mTitleAHelp;
    /* 0xB8 */ CItemBoxGrid mItemBoxGrid;
    /* 0x4AC4 */ u8 mState;                    // phase/state indicator (0..3)
    /* 0x4AC5 */ u8 mArg;
}; // size: 0x4AC6
