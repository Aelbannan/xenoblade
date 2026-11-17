#pragma once

#include <types.h>
#include "kyoshin/IUICf.hpp"
#include "kyoshin/CTutorial.hpp"
#include "kyoshin/CTitleAHelp.hpp"

// IScnRender keeps its default virtual dtor here (CMenuTutorial.cpp does not
// define ISCN_RENDER_NO_DTOR): the IScnRender base cleanup is part of the
// retail extab for the ctor/dtor.
#include "monolib/scn/IScnRender.hpp"

/*
 * Tutorial menu screen process (IUICf + IScnRender MI).
 *
 * Layout:
 *   0x00: IUICf             -- CTTask<IUICf> base (0x54 bytes), mField54/mField55 at 0x54/0x55
 *   0x58: IScnRender        -- render-callback subobject (sub-vtable)
 *   0x5C: CProcess*         -- parent process reference
 *   0x60: CTutorial         -- tutorial widget member (0x54 bytes -> 0xB4)
 *   0xB4: CTitleAHelp       -- title/help bar member (0x38 bytes -> 0xEC)
 *   0xEC: u8 mIsInitialised, mType, mSomething; 0xEF pad
 *   0xF0: u32               -- saved input flags
 */
class CMenuTutorial : public IUICf, public IScnRender {
public:
    CMenuTutorial(CProcess* parent, u32 arg2);
    virtual ~CMenuTutorial();

    // Lazily constructs the single tutorial menu instance and registers it as a
    // child of `self`. Returns nullptr when one already exists.
    static CMenuTutorial* create(CProcess* self, CProcess* parent, u32 arg2);

    // CProcess overrides (Draw is inherited from CTTask<IUICf>)
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

    CProcess* mParentRef;            // 0x5C: owning process (cast to CScn* at call sites)
    CTutorial mTutorial;             // 0x60: tutorial widget
    CTitleAHelp mTitleAHelp;         // 0xB4: title/help bar
    u8 mIsInitialised;               // 0xEC: initialisation flag
    u8 mType;                        // 0xED: tutorial type
    u8 mSomething;                   // 0xEE
    u8 _padEF;                       // 0xEF
    u32 mSavedInputFlags;            // 0xF0: input flags snapshot at construction

    static CMenuTutorial* sInstance; // singleton instance
};
