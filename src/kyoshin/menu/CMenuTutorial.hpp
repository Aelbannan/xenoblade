#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "kyoshin/CTitleAHelp.hpp"

// Forward declaration
class CTutorial;

/*
 * Tutorial menu screen process.
 *
 * Known layout (constructor / destructor / extab):
 *   0x00: CProcess          -- task-system base
 *   0x3C: vtable PMF data   -- compiler-generated virtual dispatch entries
 *   0x58: IScnRender        -- render-callback base (non-primary, needs adjuster thunks)
 *   0x5C: CProcess*         -- parent process reference (constructor param 2)
 *   0x60: CTutorial         -- tutorial widget member (0x54 bytes, up to 0xB3)
 *   0xB4: CTitleAHelp       -- title/help bar member (0x38 bytes, up to 0xEB)
 *   0xEC: u8[3]             -- flags (mIsInitialised, mType, mSomething)
 *   0xF0: u32               -- saved input flags
 *
 * Not yet decompiled: Init, Term, Move, cbRenderBefore, CTutorial partner
 */
class CMenuTutorial : public CProcess, public IScnRender {
public:
    CMenuTutorial(CProcess* parent, u32 arg2);
    virtual ~CMenuTutorial();

    // CProcess overrides
    virtual void Init();
    virtual void Term();
    virtual void Move();

    // IScnRender override
    void cbRenderBefore();

private:
    // 0x00-0x3B: CProcess (sizeof = 0x3C per header fields)
    u8 _pad3C[0x1C];                         // 0x3C-0x57: vtable PMF data + pre-IScnRender gap
    // 0x58: IScnRender base (inherited, 4 bytes vtable ptr, occupies 0x58-0x5B)
    CProcess* mParentRef;                    // 0x5C: parent process reference
    u8 _pad60[0xEC - 0x60];                  // 0x60-0xEB: CTutorial(0x60-0xB3) + CTitleAHelp(0xB4-0xEB)
    u8 mIsInitialised;                       // 0xEC: initialisation flag
    u8 mType;                                // 0xED: tutorial type (constructor param 3)
    u8 mSomething;                           // 0xEE
    u8 _padEF[0xF0 - 0xEF];
    u32 mSavedInputFlags;                    // 0xF0: input flags snapshot at construction
};
