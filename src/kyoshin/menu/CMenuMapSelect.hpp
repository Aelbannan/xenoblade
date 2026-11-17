#pragma once

#include <types.h>
#include <monolib/scn/IScnRender.hpp>

#include "kyoshin/IUICf.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CFade.hpp"
#include "kyoshin/CFloorMap.hpp"
#include "kyoshin/CMapSel.hpp"

class CProcess;

// World map screen UI process (IUICf task + IScnRender render callback).
class CMenuMapSelect : public IUICf, public IScnRender {
public:
    CMenuMapSelect(CProcess* parentRef);
    virtual ~CMenuMapSelect();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    // Only one world map menu exists at a time (subsequent calls return NULL).
    static CMenuMapSelect* create(CProcess* parent, CProcess* parentRef);

    void handleWorldMapInput();

    static CMenuMapSelect* sInstance;

    /* 0x5C */ CProcess* mParent;
    /* 0x60 */ CBgTex mBgTex;
    /* 0x80 */ CTitleAHelp mTitleAHelp;
    /* 0xB8 */ CMapSel mMapSel;
    /* 0x16C */ CFade mFade;
    /* 0x194 */ CFloorMap mFloorMap;
    /* 0x34DC */ u8 mState;
    /* 0x34E0 */ f32 mTimer;
}; // size: 0x34E4
