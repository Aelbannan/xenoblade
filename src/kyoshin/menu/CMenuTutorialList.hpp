#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

#include "kyoshin/IUICf.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CTutorial.hpp"
#include "kyoshin/CTutorialList.hpp"

// Forward declaration: the owning scene (CScn.hpp pulls broken monolib
// umbrella headers; keep the pointer opaque, same scheme as CMenuCollepedia).
class CScn;

/**
 * Tutorial list menu widget.
 *
 * Retail layout (ctor 802AE4E8): IUICf base (CProcess + 0x54/0x55 flags) at 0x00,
 * IScnRender subobject at 0x58, members from 0x5C.
 */
class CMenuTutorialList : public IUICf, public IScnRender {
public:
    CMenuTutorialList(CScn* scene, u16 choice);
    virtual ~CMenuTutorialList();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore(CScn* scene);

    static CMenuTutorialList* create(CProcess* parent, CScn* scene, u32 choice);
    static CMenuTutorialList* sInstance;

    /* 0x5C */ CScn* mScene;
    /* 0x60 */ CBgTex mBgTex;                            // size 0x20
    /* 0x80 */ CTitleAHelp mTitleAHelp;                  // size 0x38
    /* 0xB8 */ CTutorialList mTutorialList;                      // size 0x288
    /* 0x340 */ CTutorial mTutorial;                     // size 0x54
    /* 0x394 */ f32 mTimer;
    /* 0x398 */ u8 mFlag;   // phase state machine byte (Move)
    /* 0x39A */ u16 mChoice;
}; // size: 0x39C
