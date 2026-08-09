#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CTutorial.hpp"

/*
 * Tutorial menu screen process.
 *
 * Known layout (constructor / destructor / extab):
 *   0x00: CProcess          -- task-system base
 *   0x3C: vtable PMF data   -- compiler-generated virtual dispatch entries (0x3C-0x53)
 *   0x54: u8                -- writable state byte (written by func_8029A8D4)
 *   0x58: IScnRender        -- render-callback interface (member, aligned to 0x58)
 *   0x5C: CProcess*         -- parent process reference (constructor param 2)
 *   0x60: CTutorial         -- tutorial widget member (0x54 bytes, up to 0xB3)
 *   0xB4: CTitleAHelp       -- title/help bar member (0x38 bytes, up to 0xEB)
 *   0xEC: u8[3]             -- flags (mIsInitialised, mType, mSomething)
 *   0xF0: u32               -- saved input flags
 *
 * Not yet decompiled: Init, Term, Move, cbRenderBefore, CTutorial partner.
 */
class CMenuTutorial : public CProcess {
public:
    virtual ~CMenuTutorial();

    // CProcess overrides
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void Draw();
    void cbRenderBefore();

private:
    friend void func_8029A764(CMenuTutorial* self);
    friend void func_8029A8D4(CMenuTutorial* self);
    friend void func_8029A668(CMenuTutorial* self);
    friend void func_8029A7B4(CMenuTutorial* self);

    u8 _pad3C[0x18];              // 0x3C-0x53: vtable PMF data
    u8 field_54;                  // 0x54: writable state byte
    u8 _pad55[0x58 - 0x55];       // 0x55-0x57
    IScnRender mIScnRender;       // 0x58-0x5B: render-callback interface
    CProcess* mParentRef;         // 0x5C: parent process reference
    CTutorial mTutorial;          // 0x60-0xB3: tutorial widget
    CTitleAHelp mTitleAHelp;      // 0xB4-0xEB: title/help bar
    u8 mIsInitialised;            // 0xEC: initialisation flag
    u8 mType;                     // 0xED: tutorial type
    u8 mSomething;                // 0xEE
    u8 _padEF;                    // 0xEF
    u32 mSavedInputFlags;         // 0xF0: input flags snapshot at construction
};

// IScnRender vtable this-adjusting thunks (retail: subi r3, r3, 0x58; b ...).
void func_8029A924(IScnRender* sub);
void func_8029A92C(IScnRender* sub);

// Global (unmangled retail) accessors for the CMenuTutorial object.
void func_8029A764(CMenuTutorial* self);
void func_8029A8D4(CMenuTutorial* self);
CMenuTutorial* func_8029A5DC(CProcess* self, CProcess* parent, u32 arg2);