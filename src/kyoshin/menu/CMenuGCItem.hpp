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
 *  0x4AC4: u8 mState       -- phase/state indicator (0..4)
 *  0x4AC5: u8 mArg         -- constructor argument
 */
class CMenuGCItem : public CProcess, public IScnRender {
public:
    CMenuGCItem(CProcess* parent, u32 arg);
    virtual ~CMenuGCItem();

    // CProcess overrides
    virtual void Init();
    virtual void Term();
    virtual void Move();

    // IScnRender override
    void cbRenderBefore();

    // IScnRender vtable this-adjusting thunks
    void func_802B0F08();
    void func_802B0F10();

    // Fields (partial -- enough for known accessors)
    // Inherited: CProcess (0x00-0x57)
    // IScnRender at 0x58
    u8 _pad58[8];                                          // 0x58-0x5F (IScnRender is empty)
    CBgTex mBgTex;                                         // 0x60
    CTitleAHelp mTitleAHelp;                               // 0x80
    u8 _padB8to4AC4[0x4AC4 - 0xB8];                         // 0xB8-0x4AC3 (CItemBoxGrid + gap)
    u8 mState;                                             // 0x4AC4
    u8 mArg;                                               // 0x4AC5
    CMenuGCItem();
};
