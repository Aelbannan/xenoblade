#pragma once

#include <types.h>
#include <monolib/scn/IScnRender.hpp>
#include <monolib/work/CProcess.hpp>

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"

// Forward declaration for member type (not yet decompiled)
class COption;

/*
 * Option (settings) menu screen process.
 *
 * Known layout (constructor/destructor):
 *   0x00: CProcess        -- task-system base
 *   0x58: IScnRender      -- render-callback base (non-primary, needs adjuster thunks)
 *   0x5C: CProcess*       -- parent process reference
 *   0x60: CBgTex          -- background layout widget
 *   0x80: CTitleAHelp     -- title/help bar
 *   0xB8: COption         -- core option menu logic
 *  0x1BC: u8 mState       -- phase/state indicator (0..4)
 *  0x1BD: u8 mArg         -- constructor argument
 */
class CMenuOption : public CProcess, public IScnRender {
public:
    CMenuOption(CProcess* parent, u32 arg);
    virtual ~CMenuOption();

    // CProcess overrides
    virtual void Init();
    virtual void Term();
    virtual void Move();

    // IScnRender override
    void cbRenderBefore();

    // IScnRender vtable this-adjusting thunks
    void func_8029BECC();
    void func_8029BED4();

    // Fields (partial -- enough for known accessors)
    // Inherited: CProcess (0x00-0x57)
    // IScnRender at 0x58
    u8 _pad58[8];                                          // 0x58-0x5F (IScnRender is empty)
    CBgTex mBgTex;                                         // 0x60
    CTitleAHelp mTitleAHelp;                               // 0x80
    u8 _padB8to1BC[0x1BC - 0xB8];                         // 0xB8-0x1BB (COption + other fields)
    u8 mState;                                             // 0x1BC
    u8 mArg;                                               // 0x1BD
    CMenuOption();
};
