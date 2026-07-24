#pragma once

#include <types.h>
#include <monolib/scn/IScnRender.hpp>
#include <monolib/work/CProcess.hpp>

/*
 * GameCube item menu screen process.
 *
 * Known layout (constructor/destructor):
 *   0x00: CProcess        -- task-system base
 *   0x58: IScnRender      -- render-callback base (non-primary, needs adjuster thunks)
 *   0x5C: CProcess*       -- parent process reference
 *   0x60: CBgTex          -- background layout widget
 *   0x80: CTitleAHelp     -- title/help bar
 *   0xB8: CItemBoxGrid    -- item grid widget
 * 0x4AC4: u8 mState       -- phase/state indicator (0..3)
 * 0x4AC5: u8 mArg         -- constructor argument
 *    Size: 0x4AC8
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

    // Fields (partial)
    // Inherited: CProcess (0x00-0x57)
    // IScnRender at 0x58
    u8 _pad58[8];                                    // 0x58-0x5F (IScnRender is empty)
    void* mParentProcess;                             // 0x5C
    u8 _pad60to4AC4[0x4AC4 - 0x60];                  // 0x60-0x4AC3
    u8 mState;                                        // 0x4AC4
    u8 mArg;                                          // 0x4AC5
};

// IScnRender vtable this-adjusting thunk for cbRenderBefore.
//
// IScnRender is a non-primary base at offset 0x58 within CMenuGCItem.
// The thunk converts the subobject pointer (IScnRender*) back to the enclosing
// CMenuGCItem* via offset adjustment before forwarding to the real override.
// The function-pointer cast prevents MWCC from null-checking the static_cast
// chain (which would add a cmpwi/beq that the retail thunk does not have).
//
// Retail: subi r3, r3, 0x58; b cbRenderBefore__11CMenuGCItemFv
extern "C" void func_802B0F08(IScnRender* self);

// IScnRender vtable this-adjusting thunk for ~CMenuGCItem.
//
// Same adjustment as func_802B0F08 but forwards to the destructor.
// r4 (the MWCC deletion flag) is preserved from the caller because the cast
// to void(*)(CMenuGCItem*) declares only one parameter, leaving r4 unmodified
// in the tail call.
//
// Retail: subi r3, r3, 0x58; b __dt__11CMenuGCItemFv
extern "C" void func_802B0F10(IScnRender* self);
