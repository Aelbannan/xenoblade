#pragma once

#include <types.h>
#include <monolib/scn/IScnRender.hpp>
#include <monolib/work/CProcess.hpp>

#include "kyoshin/IUICf.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/COption.hpp"

/*
 * Option (settings) menu screen process (IUICf task + IScnRender render callback).
 *
 * Offsets:
 *   0x00: CTTask<IUICf> base   -- task header, null move/draw slots
 *   0x54: u8 mField54          -- phase/state flag (IUICf)
 *   0x55: u8 mField55          (IUICf)
 *   0x58: IScnRender           -- render-callback subobject (vptr)
 *   0x5C: CProcess* mParentRef -- parent process reference
 *   0x60: CBgTex mBgTex        -- background layout widget
 *   0x80: CTitleAHelp          -- title/help bar
 *   0xB8: COption mOption      -- core option menu logic
 *  0x1BC: u8 mState            -- phase/state indicator (0..4)
 *  0x1BD: u8 mArg              -- constructor argument
 */
class CMenuOption : public IUICf, public IScnRender {
public:
    CMenuOption(CProcess* parent, u32 arg);
    virtual ~CMenuOption();

    // CProcess overrides
    virtual void Init();
    virtual void Term();
    virtual void Move();

    // Render callback (dispatched through the IScnRender subobject)
    virtual void cbRenderBefore(CScn* scene);

    CProcess* mParentRef;         // 0x5C: parent process reference
    CBgTex mBgTex;                // 0x60
    CTitleAHelp mTitleAHelp;      // 0x80
    COption mOption;              // 0xB8
    u8 mState;                    // 0x1BC
    u8 mArg;                      // 0x1BD
};

// Singleton factory (retail unmangled symbol; called from CUICfManager).
extern "C" CMenuOption* createOptionMenu(CProcess* registParent, CProcess* parent, u32 arg);
// Live-instance check (flat retail name; defined in this TU).
extern "C" u32 hasOptionMenu();
