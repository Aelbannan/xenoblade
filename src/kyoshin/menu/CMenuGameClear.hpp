#pragma once

#include <types.h>

#include "kyoshin/IUICf.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/cf/CtrlObjectParamInit.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"

class CProcess;
class CScn;

/*
 * Game-clear menu screen (singleton, built by create()).
 *
 * Layout: the IUICf task base (CTTask<IUICf>, 0x00-0x57), the IScnRender
 * subobject at 0x58, then the members:
 *   0x58  IScnRender subobject (sub-vtable)
 *   0x5C  CScn* mScene (owning scene)
 *   0x60  UnkClass_8045F564 mMemRegion (0x10)
 *   0x70  u8 mField70 (screen state / jump-table index)
 *   0x71  s8 mField71 (cursor row, wraps 0..1)
 *   0x74  CSysWin storage (0x3C)
 *   0xB0  CCur18 mCursor (0x18)
 *   0xC8  uninitialised region
 *   0x12338  cf::CtrlObjectParamInit mParams[14] (14 x 0xCC)
 *   size 0x12E60
 */
class CMenuGameClear : public IUICf, public IScnRender {
public:
    CMenuGameClear(CScn* scene);
    virtual ~CMenuGameClear();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore(CScn* scene);

    static CMenuGameClear* create(CProcess* parent, CScn* scene);

    static CMenuGameClear* sInstance;

    /* 0x5C */ CScn* mScene;
    /* 0x60 */ UnkClass_8045F564 mMemRegion;
    /* 0x70 */ u8 mField70;
    /* 0x71 */ s8 mField71;
    /* 0x72 */ u8 _72[0x74 - 0x72];
    /* 0x74 */ CSysWin mSysWin;
    /* 0xB0 */ CCur18 mCursor;
    /* 0xC8 */ u8 _C8[0x12338 - 0xC8];
    /* 0x12338 */ cf::CtrlObjectParamInit mParams[14];
}; // size: 0x12E60
