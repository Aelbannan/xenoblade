#pragma once

#include "kyoshin/IUICf.hpp"
#include "monolib/scn/CScn.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CSkipTimer.hpp"
#include "monolib/scn/IScnRender.hpp"
#include <types.h>


// Skip-timer widget shown during cutscene skipping.
// IUICf task base (0x00-0x57), the IScnRender render-callback subobject at
// +0x58, then the embedded widgets.
class CMenuSkipTimer : public IUICf, public IScnRender {
public:
    CMenuSkipTimer(CProcess* parent);
    virtual ~CMenuSkipTimer();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore(CScn* scene);

    static CMenuSkipTimer* create(CProcess* self, CProcess* parent);
    static CMenuSkipTimer* sInstance;

    // --- member fields ---
    CProcess* mParentRef;          // 0x5C - scn/parent reference (removeRenderCB target)
    CBgTex mBgTex;                 // 0x60
    CTitleAHelp mTitleAHelp;       // 0x80
    CSkipTimer mTimerData;         // 0xB8
    u8 mFlag3;                     // 0x150
};

// Global mode bitfield defined in kyoshin/CUICfManager.cpp (.sbss).
extern u32 gmPauseFlags;

// Free functions defined in this TU (retail keeps their bare C names).
extern "C" void SkipTimer_Phase0Init(CMenuSkipTimer* self);
extern "C" void SkipTimer_Phase1Mark2(CMenuSkipTimer* self);
extern "C" void SkipTimer_Phase2PollPad(CMenuSkipTimer* self);
extern "C" void SkipTimer_Phase3Handle(CMenuSkipTimer* self);
