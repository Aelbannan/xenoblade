#pragma once

#include <types.h>
#include <monolib/scn/IScnRender.hpp>
#include <monolib/work/CProcess.hpp>

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"

// Forward declaration for member type (not yet decompiled)
class CCollepedia;

/*
 * Collepedia (encyclopedia) menu screen process.
 *
 * Known layout (constructor/destructor + Init):
 *   0x00: CProcess        -- task-system base
 *   0x58: IScnRender      -- render-callback base (non-primary, needs adjuster thunks)
 *   0x60: CBgTex          -- background layout widget
 *   0x80: CTitleAHelp     -- title/help bar
 *   0xB8: CCollepedia     -- core collepedia data/logic
 *  0x29B4: u8 state       -- phase/state indicator
 *  0x29B8: f32 timer      -- animation/open timer
 *  0x29BC: total size     -- sizeof(CMenuCollepedia) = 0x29BC
 */
class CMenuCollepedia : public CProcess, public IScnRender {
public:
    // CProcess overrides
    virtual ~CMenuCollepedia();
    virtual void Init();
    virtual void Term();
    virtual void Move();

    // IScnRender override
    void cbRenderBefore();

    // Fields (partial -- enough for known accessors)
    // Inherited: CProcess (0x00-0x57)
    // IScnRender at 0x58
    u8 _pad58[8];                                        // 0x58-0x5F padding (IScnRender is empty)
    CBgTex mBgTex;                                       // 0x60
    CTitleAHelp mTitleAHelp;                             // 0x80
    u8 _padB8to29B4[0x29B4 - 0xB8];                     // 0xB8-0x29B3 (collepedia + other fields)
    u8 mState;                                           // 0x29B4
    f32 mTimer;                                          // 0x29B8
};
