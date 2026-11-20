#pragma once

#include <types.h>

// C-ABI retail symbols. functions.hpp (pulled in via monolib/scn.hpp if
// CTaskGame.hpp is included) declares the C++-mangled bool f(int) for
// CtrlRemote_TouchBitByArg, which would win the C-name resolution and emit a byte-masked
// return; CTaskGame.hpp is therefore not included here and the two CTaskGame
// statics are addressed by their literal retail names instead.
#include "kyoshin/cf/CtrlRemoteApi.hpp"
class CTaskGame;

#include "kyoshin/IUICf.hpp"
#include <monolib/scn/IScnRender.hpp>
#include <monolib/core/CPadManager.hpp>

#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CFade.hpp"
#include "kyoshin/CFloorMap.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

extern const f32 lbl_eu_806687C0;  // 0.0f - world-map timer reset value

// World map screen UI process (split-screen version).
// IUICf task base (CTTask<IUICf>, whose mMoveFunc/mDrawFunc PMF slots sit at
// 0x3C/0x48 and whose IUICf flag bytes sit at 0x54/0x55), plus the IScnRender
// render-callback subobject at 0x58.
//
// Layout:
//   0x00: IUICf base (CTTask<IUICf>, flags at 0x54/0x55)
//   0x58: IScnRender (sub-vtable)
//   0x5C: CProcess* parent reference
//   0x60: CTitleAHelp
//   0x98: CFade
//   0xC0: CFloorMap (0x3348 bytes)
//  0x3408: u8 phase/state (Move()'s jumptable selector)
//  0x340C: f32 timer
class CMenuMapSelectSC : public IUICf, public IScnRender {
public:
    CMenuMapSelectSC(CProcess* parent);
    virtual ~CMenuMapSelectSC();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    static CMenuMapSelectSC* sInstance;

    CProcess* mParentRef;      // 0x5C
    CTitleAHelp mTitleAHelp;   // 0x60
    CFade mFade;               // 0x98
    CFloorMap mFloorMap;       // 0xC0
    u8 mState;                 // 0x3408
    f32 mTimer;                // 0x340C
}; // size: 0x3410
