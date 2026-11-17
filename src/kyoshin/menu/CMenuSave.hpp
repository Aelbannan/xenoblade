#pragma once

#include <types.h>

#include "kyoshin/IUICf.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CSaveLoad.hpp"

class CProcess;

// Menu save/load screen process (IUICf task + IScnRender render callback).
class CMenuSave : public IUICf, public IScnRender {
public:
    CMenuSave(CProcess* parent, u32 arg208, u32 arg209, u32 arg20A);
    virtual ~CMenuSave();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    // Only one save menu exists at a time (subsequent calls return NULL).
    static CMenuSave* create(CProcess* parent, CProcess* parentRef, u32 arg208,
                             u32 arg209, u32 arg20A);

    static CMenuSave* sInstance;

    /* 0x5C */ CProcess* mParentRef;
    /* 0x60 */ CBgTex mBgTex;
    /* 0x80 */ CTitleAHelp mTitleAHelp;
    /* 0xB8 */ CSaveLoad mSaveLoad;
    /* 0x208 */ u8 mField208;
    /* 0x209 */ u8 mField209;
    /* 0x20A */ u8 mField20A;
    /* 0x20B */ u8 mState;
}; // size: 0x20C
