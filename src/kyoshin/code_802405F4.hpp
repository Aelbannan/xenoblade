#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CMapSel.hpp"
#include "kyoshin/CFade.hpp"
#include "kyoshin/CFloorMap.hpp"

class CScn;

class CMenuMapSelect : public CProcess, public IScnRender {
public:
    CMenuMapSelect();
    virtual ~CMenuMapSelect();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    // Retail layout, recovered from the Init__14CMenuMapSelectFv /
    // __dt__14CMenuMapSelectFv / Move__14CMenuMapSelectFv disassembly:
    //   0x00-0x3C: CProcess base
    //   0x3C-0x40: IScnRender base (vtable ptr)
    //   0x40-0x53: unknown
    //   0x54:      state byte
    //   0x5C:      CScn* (loaded at Init's addRenderCB call)
    //   0x60:      CBgTex      (0x20)
    //   0x80:      CTitleAHelp (0x38)
    //   0xB8:      CMapSel     (0xB4)
    //   0x16C:     CFade       (0x28)
    //   0x194:     CFloorMap   (0x3348)
    //   0x34DC:    state byte (Move state machine)
    u8 mField40[0x14];       // 0x40
    u8 mField54;             // 0x54
    u8 _pad55[7];            // 0x55-0x5B
    CScn* mScn;              // 0x5C
    CBgTex mBgTex;           // 0x60
    CTitleAHelp mTitleHelp;  // 0x80
    CMapSel mMapSel;         // 0xB8
    CFade mFade;             // 0x16C
    CFloorMap mFloorMap;     // 0x194
    u8 mState;               // 0x34DC
};
