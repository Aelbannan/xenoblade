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

    // IScnRender vtable this-adjusting thunks
    void func_802B0F08();
    void func_802B0F10();

    // Fields (partial)
    // Inherited: CProcess (0x00-0x57)
    // IScnRender at 0x58
    u8 _pad58[8];                                    // 0x58-0x5F (IScnRender is empty)
    void* mParentProcess;                             // 0x5C
    u8 _pad60to4AC4[0x4AC4 - 0x60];                  // 0x60-0x4AC3
    u8 mState;                                        // 0x4AC4
    u8 mArg;                                          // 0x4AC5
    CMenuGCItem();
};
