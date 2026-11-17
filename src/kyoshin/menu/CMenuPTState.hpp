#pragma once

#include <types.h>

// CPartyStateWin.hpp references cf::CfObjectMove before its own forward decl;
// pre-declare it here (same pattern as other includers).
namespace cf { class CfObjectMove; }
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>
#include "kyoshin/IUICf.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CPartyStateWin.hpp"

// PT-state menu task: IUICf task base (0x00-0x57), the IScnRender render-callback
// subobject at +0x58, then the embedded widgets.
class CMenuPTState : public IUICf, public IScnRender {
public:
    CMenuPTState(CProcess* storedParent);
    virtual ~CMenuPTState();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    static CMenuPTState* create(CProcess* parent, CProcess* storedParent);
    static CMenuPTState* sInstance;

    // --- member fields ---
    CProcess* mStoredParent;       // 0x5C
    CBgTex mBgTex;                 // 0x60
    CPartyStateWin mPartyStateWin; // 0x80
    u8 mPhase;                // 0x6C6C
};
