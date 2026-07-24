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

    // Fields (partial -- enough for known accessors)
    // Inherited: CProcess (0x00-0x57)
    // IScnRender at 0x58
    u8 _pad58[8];                                          // 0x58-0x5F (IScnRender is empty)
    CBgTex mBgTex;                                         // 0x60
    CTitleAHelp mTitleAHelp;                               // 0x80
    u8 _padB8to1BC[0x1BC - 0xB8];                         // 0xB8-0x1BB (COption + other fields)
    u8 mState;                                             // 0x1BC
    u8 mArg;                                               // 0x1BD
};

// IScnRender vtable this-adjusting thunk for cbRenderBefore.
//
// IScnRender is a non-primary base at offset 0x58 within CMenuOption.
// The thunk converts the subobject pointer (IScnRender*) back to the enclosing
// CMenuOption* via offset adjustment before forwarding to the real override.
// The function-pointer cast prevents MWCC from null-checking the static_cast
// chain (which would add a cmpwi/beq that the retail thunk does not have).
//
// Retail: subi r3, r3, 0x58; b cbRenderBefore__11CMenuOptionFv
extern "C" void func_8029BECC(IScnRender* self);

// IScnRender vtable this-adjusting thunk for ~CMenuOption.
//
// Same adjustment as func_8029BECC but forwards to the destructor.
// r4 (the MWCC deletion flag) is preserved from the caller because the cast
// to void(*)(CMenuOption*) declares only one parameter, leaving r4 unmodified
// in the tail call.
//
// Retail: subi r3, r3, 0x58; b __dt__11CMenuOptionFv
extern "C" void func_8029BED4(IScnRender* self);
