#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CKizunaTalkList.hpp"

// Shared string pool (.rodata) used by Init.
extern char lbl_eu_8050E970[];

/*
 * Kizuna (Friendship) Talk List screen process.
 *
 * Layout (constructor/Init):
 *   0x00: CProcess          -- task-system base (0x3C) + vtable PMF data
 *   0x58: IScnRender        -- render-callback interface
 *   0x5C: CProcess*         -- parent process reference
 *   0x60: CBgTex            -- background layout widget (0x20 bytes)
 *   0x80: CTitleAHelp       -- title/help bar (0x38 bytes)
 *   0xB8: CKizunaTalkList   -- core talk-list data/logic (0x1494 bytes)
 *  0x154C: u8               -- phase/state byte
 */
class CMenuKizunaTalkList : public CProcess {
public:
    CMenuKizunaTalkList();
    virtual ~CMenuKizunaTalkList();

    // CProcess overrides
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

    // IScnRender vtable this-adjusting thunks
    void func_802726E4();
    void func_802726EC();

private:
    friend void func_80272560(CMenuKizunaTalkList* self);

    u8 _pad3C[0x18];            // 0x3C-0x53: vtable PMF data
    u8 _pad54[0x58 - 0x54];     // 0x54-0x57: padding
    IScnRender mIScnRender;     // 0x58
    CProcess* mParentRef;       // 0x5C
    CBgTex mBgTex;              // 0x60
    CTitleAHelp mTitleAHelp;    // 0x80
    CKizunaTalkList mKizunaTalkList; // 0xB8
    u8 mState;                  // 0x154C
};